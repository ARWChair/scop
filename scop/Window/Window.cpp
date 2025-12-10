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
        if (vn == true) {
            summarized.push_back(values.normals[(*vn_it) * 3]);
            summarized.push_back(values.normals[((*vn_it) * 3) + 1]);
            summarized.push_back(values.normals[((*vn_it) * 3) + 2]);
            vn_it++;
        } else {
            summarized.push_back(0.0f);
            summarized.push_back(0.0f);
            summarized.push_back(1.0f);
            drawer->set_vn(true);
        }
        float gray = grayValues[current % 6];
        GLfloat faceColor[4] = {gray, gray, gray, 1.0f};
        summarized.push_back(faceColor[0]);
        summarized.push_back(faceColor[1]);
        summarized.push_back(faceColor[2]);
        summarized.push_back(faceColor[3]);
        current++;
        std::cout << summarized.size() << std::endl;
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

    for (unsigned int pos = 0; pos < indices.normals_indice.size(); pos++) {
        indice.push_back(pos);
    }
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
                if (ks == XK_T || ks == XK_t) {
                    toggle = !toggle;
                }
            }
        }
        drawer->clear();
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
        // Create Scale calc
        // glScalef(0.5f, 0.5f, 0.5f);
        
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


        // std::cout << "HERE2" << std::endl;
        apply_material(mat_name);
        // std::cout << "HERE2" << std::endl;
        // if (drawer->has_texture()) {
        //     glEnable(GL_TEXTURE_2D);
        //     glBindTexture(GL_TEXTURE_2D, drawer->get_texture_id);
        // }

        // std::cout << "HERE3" << std::endl;
        drawer->render_vbo(indice_size, toggle);
        // std::cout << "HERE4" << std::endl;
        // (void)indice_size;
        // test_with_normals_only(drawer->get_v_int(), v_indices);
        // test_with_simple_normals(drawer->get_v_int(), v_indices);

        // if (drawer->has_texture())
        //     glDisable(GL_TEXTURE_2D);

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
        // std::cout << "lol" << std::endl;
        return;
    }
    // std::cout << "lol" << std::endl;
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // std::cout << "lol2" << std::endl;
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

// std::vector<unsigned int> Scop_window::create_vectors(std::vector<inner_elements>& node, std::vector<std::vector<std::array<double, 3>>>& v, std::vector<std::vector<std::array<double, 3>>>& vn, std::vector<std::vector<std::array<double, 2>>>& vt) {
void Scop_window::create_vectors(std::vector<inner_elements>& node, std::vector<GLfloat>& v, std::vector<GLfloat>& vn, std::vector<GLfloat>& vt) {
    // std::vector<std::array<double, 3>> converted_v;
    // std::vector<std::array<double, 2>> converted_vt;
    // std::vector<std::array<double, 3>> converted_vn;


    (void)v;
    (void)vn;
    (void)vt;
    // std::cout << "Before" << std::endl;
    // for (std::vector<inner_elements>::iterator it = node.begin(); it != node.end(); it++)
    //     std::cout << it->v.size() << ", ";
    // std::cout << std::endl << std::endl;
    // std::cout << "After" << std::endl;
    drawer->split_elements(node);
    // for (std::vector<inner_elements>::iterator it = node.begin(); it != node.end(); it++)
    //     std::cout << it->v.size() << ", ";
    // std::cout << std::endl;

    // for (std::vector<inner_elements>::iterator it = node.begin(); it != node.end(); it++) {
    //     std::vector<std::array<double, 3>>::iterator inner_vn = it->vn.begin();
    //     std::vector<std::array<double, 2>>::iterator inner_vt = it->vt.begin();
    //     for (std::vector<std::array<double, 3>>::iterator inner = it->v.begin(); inner != it->v.end(); inner++) {
    //         std::cout << (*inner)[0] << "/" << (*inner_vt)[0] << "/" << (*inner)[0] << " "
    //         << (*inner)[1] << "/" << (*inner_vt)[1] << "/" << (*inner_vn)[1] << " "
    //         << (*inner)[2] << "/" << (*inner_vt)[2] << "/" << (*inner_vn)[2] << std::endl;
    //         inner_vn++;
    //         inner_vt++;
    //     }
    // }
    // std::cout << std::endl;


    // GLfloat temp;
    // for (std::vector<inner_elements>::iterator it = node.begin(); it != node.end(); it++) {
    //     for (std::vector<std::array<double, 3>>::iterator inner = it->v.begin(); inner != it->v.end(); inner++) {
    //         temp = static_cast<GLfloat>((*inner)[0]);
    //         v.push_back(temp);
    //         temp = static_cast<GLfloat>((*inner)[1]);
    //         v.push_back(temp);
    //         temp = static_cast<GLfloat>((*inner)[2]);
    //         v.push_back(temp);
    //     }
    //     for (std::vector<std::array<double, 3>>::iterator inner = it->vn.begin(); inner != it->vn.end(); inner++) {
    //         temp = static_cast<GLfloat>((*inner)[0]);
    //         vn.push_back(temp);
    //         temp = static_cast<GLfloat>((*inner)[1]);
    //         vn.push_back(temp);
    //         temp = static_cast<GLfloat>((*inner)[2]);
    //         vn.push_back(temp);
    //     }
    //     for (std::vector<std::array<double, 2>>::iterator inner = it->vt.begin(); inner != it->vt.end(); inner++) {
    //         temp = static_cast<GLfloat>((*inner)[0]);
    //         vt.push_back(temp);
    //         temp = static_cast<GLfloat>((*inner)[1]);
    //         vt.push_back(temp);
    //     }
    // }


    // std::cout << "\n" << v.size() << std::endl;

    // std::vector<std::vector<std::array<double, 3>>> converted_v = v;
    // std::vector<std::vector<std::array<double, 2>>> converted_vt;
    // std::vector<std::vector<std::array<double, 3>>> converted_vn;
    // std::vector<unsigned int> indices = split_and_group(converted_v, faces->get_indices());
    // converted_v = split_and_group(v);
    // if (vt.size() > 0)
    //     converted_vt = split_and_group(vt);
    // if (vn.size() > 0)
    //     converted_vn = split_and_group(vn);

    // reallign_highest_point(converted_v, 0);
    // reallign_highest_point(converted_v, 1);
    // reallign_highest_point(converted_v, 2);
    

    // v = converted_v;
    // vt = converted_vt;
    // vn = converted_vn;
    // indices.clear();
    // return indices;
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