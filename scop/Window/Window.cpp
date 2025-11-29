#include "Window.hpp"
#include "../Draw/Draw.hpp"
#include "../Faces/Faces.hpp"
#include "../Material/Material.hpp"

Scop_window::Scop_window() {
    main_display = XOpenDisplay(0);
    root_window = XDefaultRootWindow(main_display);
    x = y = width = height = border_width = ui_class = depth = valuemask = 0;
    visual = CopyFromParent;
    attributes = {};
    toggle = false;
    created_window = false;
}

Scop_window::Scop_window(int x, int y, unsigned int width, unsigned int height, unsigned int border_width,
    int depth, unsigned int ui_class, Visual *visual, unsigned int valuemask, XSetWindowAttributes attributes) {
    
    main_display = XOpenDisplay(0);
    root_window = XDefaultRootWindow(main_display);
    created_window = false;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->border_width = border_width;
    this->depth = depth;
    this->ui_class = ui_class;
    if (visual == NULL)
        visual = CopyFromParent;
    this->visual = visual;
    this->valuemask = valuemask;
    this->attributes = attributes;
    if (this->attributes.background_pixel == 0)
        this->attributes.background_pixel = 0xffafe9af;
    create_window();
    toggle = false;
    created_window = true;
}

Scop_window& Scop_window::operator=(const Scop_window& copy) {
    if (this != &copy) {
        this->x = copy.x;
        this->y = copy.y;
        this->width = copy.width;
        this->height = copy.height;
        this->border_width = copy.border_width;
        this->depth = copy.depth;
        this->ui_class = copy.ui_class;
        visual = copy.visual;
        this->valuemask = copy.valuemask;
        this->attributes = copy.attributes;
    }
    return *this;
}

Scop_window::~Scop_window() {
    XUnmapWindow(main_display, main_window);
    XDestroyWindow(main_display, main_window);
    XCloseDisplay(main_display);
}

void Scop_window::create_window() {
    main_window = XCreateWindow(main_display, root_window, x, y, width, height, border_width, depth, ui_class, visual, valuemask, &attributes);
    if (main_window == 0)
        throw WindowCreationFailed();
    XStoreName(main_display, main_window, "Scop");
    XSelectInput(main_display, main_window, ExposureMask | KeyPressMask);
    XMapWindow(main_display, main_window);
}

void Scop_window::hold_open() {
    std::vector<inner_elements> node = faces->get_list();
    std::vector<std::vector<std::array<double, 3>>> v;
    std::vector<std::vector<std::array<double, 3>>> vn;
    std::vector<std::vector<std::array<double, 2>>> vt;
    create_vectors(node, v, vn, vt);

    // std::vector<GLfloat> verts, normals, textcords;
    // create_vertex_array(verts, normals, textcords, converted_v, converted_vn, converted_vt);

    XEvent e;
    Atom wmDeleteMessage = XInternAtom(main_display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(main_display, main_window, &wmDeleteMessage, 1);
    while(1) {
        while (XPending(main_display)) {
            XNextEvent(main_display, &e);
            if (e.type == ClientMessage) {
                if ((Atom)e.xclient.data.l[0] == wmDeleteMessage) {
                    return;
                }
            }
            if (e.type == KeyPress) {
                KeySym ks = XLookupKeysym(&e.xkey, 0);
                if (ks == XK_Escape)
                    return;
                if (ks == XK_T || ks == XK_t) {
                    toggle = !toggle;
                }
            }
        }
        drawer->clear();
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
        // glScalef(0.5f, 0.5f, 0.5f);
        
        GLfloat lightPosition[] = {0.0f, 0.0f, 5.0f, 1.0f};
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
        
        glTranslatef(drawer->get_xPos(), drawer->get_yPos(), 0.0f);
        glRotatef(drawer->get_rl_rotation() * 90.0f, 0.0f, 1.0f, 0.0f);
        glRotatef(drawer->get_ud_rotation() * 90.0f, 1.0f, 0.0f, 0.0f);

        glBegin(GL_TRIANGLES);
        int amount = 0;
        std::vector<std::vector<std::array<double, 3>>>::iterator it_2 = vn.begin();
        std::vector<std::vector<std::array<double, 2>>>::iterator it_3 = vt.begin();
        for (std::vector<std::vector<std::array<double, 3>>>::iterator it_1 = v.begin(); it_1 != v.end(); it_1++) {
            if (toggle == false && material->is_missing() == false)
                setup_face_colors(amount);
            if (material->is_missing() == true)
                setup_face_colors(amount);
            if (it_2 == vn.end() && it_3 == vt.end()) {
                drawer->draw_triangle(*it_1, material, toggle);
            } else if (it_3 == vt.end()) {
                drawer->draw_triangle(*it_1, *it_2->begin(), material, toggle);
                it_2++;
            } else if (it_2 == vn.end()) {
                drawer->draw_triangle(*it_1, *it_3, material, toggle);
                it_3++;
            } else {
                drawer->draw_triangle(*it_1, *it_2->begin(), *it_3, material, toggle);
                it_2++;
                it_3++;
            }
            amount++;
        }
        glEnd();
        glXSwapBuffers((Display *)get_display(), scop_openGL->get_drawable());
        glFlush();
        drawer->inc_rl_rotation(0.005f);
    }
}

void Scop_window::setup_face_colors(int face) {
    float grayValues[6] = {0.09f, 0.11f, 0.14f, 0.16f, 0.04f, 0.06f};
    float gray = grayValues[face % 6];
    
    static const GLfloat params[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat faceColor[4] = {gray, gray, gray, 1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT, faceColor);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, faceColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, params);
    glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);
    glMaterialfv(GL_FRONT, GL_EMISSION, params);
    glColor3f(gray, gray, gray);
}

void Scop_window::create_vectors(std::vector<inner_elements>& node, std::vector<std::vector<std::array<double, 3>>>& v, std::vector<std::vector<std::array<double, 3>>>& vn, std::vector<std::vector<std::array<double, 2>>>& vt) {
    for (std::vector<inner_elements>::iterator list_it = node.begin(); list_it != node.end(); list_it++) {
        v.push_back(list_it->v);
        vn.push_back(list_it->vn);
        vt.push_back(list_it->vt);
    }

    std::vector<std::vector<std::array<double, 3>>> converted_v = split_and_group(v);
    std::vector<std::vector<std::array<double, 2>>> converted_vt;
    std::vector<std::vector<std::array<double, 3>>> converted_vn;
    if (vt.size() > 0)
        converted_vt = split_and_group(vt);
    if (vn.size() > 0)
        converted_vn = split_and_group(vn);

    reallign_highest_point(converted_v, 0);
    reallign_highest_point(converted_v, 1);
    reallign_highest_point(converted_v, 2);

    v = converted_v;
    vt = converted_vt;
    vn = converted_vn;
}

// void Scop_window::create_vertex_array(std::vector<GLfloat> &verts, std::vector<GLfloat> &normals, std::vector<GLfloat> &textcords, std::vector<std::vector<std::array<double, 3>>>& v,std::vector<std::vector<std::array<double, 3>>>& vt, std::vector<std::vector<std::array<double, 2>>>& vn) {
//     for (std::vector<std::vector<std::array<double, 3>>>::iterator verts_it = converted_v.begin(); verts_it != converted_v.end(); verts_it++) {
//         for (std::vector<std::array<double, 3>>::iterator pos_it = verts_it->begin(); pos_it != verts_it->end(); pos_it++) {
//             verts.push_back((*pos_it)[0]);
//             verts.push_back((*pos_it)[1]);
//             verts.push_back((*pos_it)[2]);
//         }
//     }
// }

void Scop_window::set_openGL(Scop_openGL*& scop_openGL) {
    this->scop_openGL = scop_openGL;
}

Display const *Scop_window::get_display() const {
    return main_display;
}

Window const &Scop_window::get_window() const {
    return main_window;
}

void Scop_window::set_drawer(Draw*& drawer) {
    this->drawer = drawer;
}

void Scop_window::set_faces(Faces*& faces) {
    this->faces = faces;
}

void Scop_window::set_material(Material*& material) {
    this->material = material;
}

const unsigned int &Scop_window::get_width() const {
    return width;
}

const unsigned int &Scop_window::get_height() const {
    return height;
}