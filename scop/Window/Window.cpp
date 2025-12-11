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
    XSelectInput(main_display, main_window, ExposureMask | KeyPressMask | ButtonPressMask);
    XMapWindow(main_display, main_window);
}

std::vector<GLfloat> Scop_window::summarize_vectors(flat_indices& flattened, flat &values) {
    std::vector<GLfloat> summarized;
    std::size_t current = 0;
    bool vt, vn;
    vt = vn = false;
    float grayValues[6] = {0.09f, 0.11f, 0.14f, 0.16f, 0.04f, 0.06f};

    std::vector<unsigned int>::iterator v_it = flattened.vertexes_indice.begin();
    std::vector<unsigned int>::iterator vt_it = flattened.textures_indice.begin();
    std::vector<unsigned int>::iterator vn_it = flattened.normals_indice.begin();
    if (vt_it != flattened.textures_indice.end()) {
        vt = true;
        drawer->set_vt(true);
    }
    if (vn_it != flattened.normals_indice.end()) {
        vn = true;
        drawer->set_vn(true);
    }
    for (; v_it != flattened.vertexes_indice.end(); v_it++) {
        summarized.push_back(values.vertexes[(*v_it) * 3]);
        summarized.push_back(values.vertexes[((*v_it) * 3) + 1]);
        summarized.push_back(values.vertexes[((*v_it) * 3) + 2]);
        if (vt == true) {
            summarized.push_back(values.textures[(*vt_it) * 2]);
            summarized.push_back(values.textures[((*vt_it) * 2) + 1]);
            vt_it++;
        }
        summarized.push_back(values.normals[(*vn_it) * 3]);
        summarized.push_back(values.normals[((*vn_it) * 3) + 1]);
        summarized.push_back(values.normals[((*vn_it) * 3) + 2]);
        vn_it++;
        float gray = grayValues[current % 6];
        GLfloat faceColor[4] = {gray, gray, gray, 1.0f};
        summarized.push_back(faceColor[0]);
        summarized.push_back(faceColor[1]);
        summarized.push_back(faceColor[2]);
        summarized.push_back(faceColor[3]);
        current++;
    }
    return summarized;
}

void Scop_window::hold_open() {
    flat_indices indices = faces->get_indices();
    flat flattened = faces->get_flattened();
    reallign_highest_point(flattened.vertexes, 0);
    reallign_highest_point(flattened.vertexes, 1);
    reallign_highest_point(flattened.vertexes, 2);
    std::vector<GLfloat> concatinated_vector = summarize_vectors(indices, flattened);
    std::string mat_name = "Material";
    std::vector<unsigned int> indice;
    bool lock = true;

    for (unsigned int pos = 0; pos < indices.normals_indice.size(); pos++) {
        indice.push_back(pos);
    }
    std::cout << concatinated_vector.size() << std::endl;
    drawer->create_vbo(concatinated_vector, indice);

    XEvent e;
    Atom wmDeleteMessage = XInternAtom(main_display, "WM_DELETE_WINDOW", False);
    GLsizei indice_size = static_cast<GLsizei>(concatinated_vector.size());
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
                if ((ks == XK_T || ks == XK_t) && !material->is_missing())
                    toggle = !toggle;
                if (ks == XK_F || ks == XK_f)
                    lock = !lock;
                if (lock == false) {
                    if (ks == XK_W || ks == XK_w)
                        drawer->inc_yPos(0.05f);
                    if (ks == XK_S || ks == XK_s)
                        drawer->inc_yPos(-0.05f);
                    if (ks == XK_A || ks == XK_a)
                        drawer->inc_xPos(-0.05f);
                    if (ks == XK_D || ks == XK_d)
                        drawer->inc_xPos(0.05f);
                    if (ks == XK_E || ks == XK_e)
                        drawer->inc_rl_rotation(0.01f);
                    if (ks == XK_Q || ks == XK_q)
                        drawer->inc_rl_rotation(-0.01f);
                    if (ks == XK_Z || ks == XK_z)
                        drawer->inc_ud_rotation(-0.01f);
                    if (ks == XK_X || ks == XK_x)
                        drawer->inc_ud_rotation(0.01f);
                }
            }
            if (e.type == ButtonPress && lock == false) {
                switch(e.xbutton.button) {
                    case 4:
                        drawer->dec_scroll(0.01f);
                        break;
                    case 5:
                        drawer->inc_scroll(0.01f);
                        break;
                }
            }
        }
        drawer->clear();
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
        // Create Scale calc
        glScalef(drawer->get_scroll(), drawer->get_scroll(), drawer->get_scroll());
        
        GLfloat light_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
        GLfloat light_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
        GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat light_position[] = {0.0f, 0.0f, 5.0f, 1.0f};

        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        
        glTranslatef(drawer->get_xPos(), drawer->get_yPos(), 0.0f);
        glRotatef(drawer->get_rl_rotation() * 90.0f, 0.0f, 1.0f, 0.0f);
        glRotatef(drawer->get_ud_rotation() * 90.0f, 1.0f, 0.0f, 0.0f);


        apply_material(mat_name);
        // if (drawer->has_texture()) {
        //     glEnable(GL_TEXTURE_2D);
        //     glBindTexture(GL_TEXTURE_2D, drawer->get_texture_id);
        // }

        drawer->render_vbo(indice_size, toggle);
        // if (drawer->has_texture())
        //     glDisable(GL_TEXTURE_2D);

        glXSwapBuffers((Display *)get_display(), scop_openGL->get_drawable());
        glFlush();
        if (lock)
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

void Scop_window::apply_material(std::string& material_name) {
    if (!material || material->is_missing()) {
        GLfloat default_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
        GLfloat default_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
        GLfloat default_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};
        
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, default_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, default_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, default_specular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
        glColor3f(0.8f, 0.8f, 0.8f);
        return;
    }
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    drawer->draw_individual_text(material->get_Ka(material_name), GL_AMBIENT);
    auto Kd = material->get_Kd(material_name);
    if(!Kd.empty()) {
        drawer->draw_individual_text(Kd, GL_DIFFUSE);
        glColor3f(Kd[0], Kd[1], Kd[2]);
    }
    drawer->draw_individual_text(material->get_Ks(material_name), GL_SPECULAR);
    drawer->draw_individual_text(material->get_Ke(material_name), GL_EMISSION);
    if (material->get_Ns(material_name).size() != 0) {
        GLfloat blender_ns = static_cast<GLfloat>(material->get_Ns(material_name)[0]);
        GLfloat opengl_ns;
        
        opengl_ns = blender_ns * (128.0f / 1000.0f);
        opengl_ns = std::max(0.0f, std::min(128.0f, opengl_ns));
        
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, opengl_ns);
    } else {
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
    }
    
    std::array<double, 1> d = material->get_d(material_name);
    if(!d.empty() && d[0] < 1.0f) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 1.0f, 1.0f, d[0]);
    } else {
        glDisable(GL_BLEND);
    }
}

std::vector<GLfloat> Scop_window::create_GLfloat_array(std::vector<std::vector<std::array<double, 3>>>& v) {
    std::vector<GLfloat> va_v;
    
    for (std::vector<std::vector<std::array<double, 3>>>::iterator verts_it = v.begin(); verts_it != v.end(); verts_it++) {
        for (std::vector<std::array<double, 3>>::iterator pos_it = verts_it->begin(); pos_it != verts_it->end(); pos_it++) {
            va_v.push_back(static_cast<GLfloat>((*pos_it)[0]));
            va_v.push_back(static_cast<GLfloat>((*pos_it)[1]));
            va_v.push_back(static_cast<GLfloat>((*pos_it)[2]));
        }
    }
    return va_v;
}

std::vector<GLfloat> Scop_window::create_GLfloat_array(std::vector<std::vector<std::array<double, 2>>>& v) {
    std::vector<GLfloat> va_v;
    
    for (std::vector<std::vector<std::array<double, 2>>>::iterator verts_it = v.begin(); verts_it != v.end(); verts_it++) {
        for (std::vector<std::array<double, 2>>::iterator pos_it = verts_it->begin(); pos_it != verts_it->end(); pos_it++) {
            va_v.push_back(static_cast<GLfloat>((*pos_it)[0]));
            va_v.push_back(static_cast<GLfloat>((*pos_it)[1]));
        }
    }
    return va_v;
}

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