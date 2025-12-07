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

void force_outward_normals(std::vector<GLfloat>& vertices, 
                          std::vector<GLfloat>& normals) {
    float center[3] = {0,0,0};
    
    // Berechne Center
    for(size_t i = 0; i < vertices.size(); i += 3) {
        center[0] += vertices[i];
        center[1] += vertices[i+1];
        center[2] += vertices[i+2];
    }
    center[0] /= vertices.size() / 3;
    center[1] /= vertices.size() / 3;
    center[2] /= vertices.size() / 3;
    
    // Normals = Richtung von Center zu Vertex
    normals.clear();
    for(size_t i = 0; i < vertices.size(); i += 3) {
        float dx = vertices[i] - center[0];
        float dy = vertices[i+1] - center[1];
        float dz = vertices[i+2] - center[2];
        
        // Normalisieren
        float length = sqrt(dx*dx + dy*dy + dz*dz);
        normals.push_back(dx / length);
        normals.push_back(dy / length);
        normals.push_back(dz / length);
    }
}

// void test_with_simple_normals(GLuint v_int, std::vector<GLfloat> v_indices) {
//     std::cout << "\n=== TEST WITH SIMPLE NORMALS ===" << std::endl;
    
//     // 1. EINFACHSTE Normals (alle nach vorne)
//     std::vector<GLfloat> simple_normals;
//     int vertex_count = v_indices.size() / 3;
//     for(int i = 0; i < vertex_count; i++) {
//         simple_normals.push_back(0.0f);   // X
//         simple_normals.push_back(0.0f);   // Y
//         simple_normals.push_back(1.0f);   // Z (nach vorne)
//     }
    
//     // 2. Normal Buffer erstellen
//     GLuint normal_buffer;
//     glGenBuffers(1, &normal_buffer);
//     glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
//     glBufferData(GL_ARRAY_BUFFER, 
//                  simple_normals.size() * sizeof(GLfloat),
//                  simple_normals.data(),
//                  GL_STATIC_DRAW);
    
//     std::cout << "Created simple normals buffer: " 
//               << simple_normals.size() << " floats" << std::endl;
    
//     // 3. LIGHTING SETUP (einfach)
//     glEnable(GL_LIGHTING);
//     glEnable(GL_LIGHT0);
    
//     GLfloat light_pos[] = {0.0f, 0.0f, 5.0f, 1.0f};  // Licht von vorne
//     GLfloat light_ambient[] = {0.5f, 0.5f, 0.5f, 1.0f};
//     GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    
//     glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
//     glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
//     glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    
//     std::cout << "Lighting enabled, light at: " 
//               << light_pos[0] << ", " << light_pos[1] << ", " << light_pos[2] << std::endl;
    
//     // 4. EINFACHES Material (ROT)
//     GLfloat material_ambient[] = {0.2f, 0.0f, 0.0f, 1.0f};
//     GLfloat material_diffuse[] = {1.0f, 0.0f, 0.0f, 1.0f};  // KNALLROT
//     GLfloat material_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};
    
//     glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_ambient);
//     glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse);
//     glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
//     glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
    
//     std::cout << "Material set to RED" << std::endl;
    
//     // 5. RENDERN
//     // Vertex Buffer
//     glBindBuffer(GL_ARRAY_BUFFER, v_int);
//     glEnableClientState(GL_VERTEX_ARRAY);
//     glVertexPointer(3, GL_FLOAT, 0, 0);
    
//     // Normal Buffer (unsere einfachen)
//     glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
//     glEnableClientState(GL_NORMAL_ARRAY);
//     glNormalPointer(GL_FLOAT, 0, 0);
    
//     std::cout << "Drawing " << vertex_count << " vertices..." << std::endl;
    
//     // Draw
//     glDrawArrays(GL_TRIANGLES, 0, vertex_count);
    
//     // 6. CLEANUP
//     glDisableClientState(GL_VERTEX_ARRAY);
//     glDisableClientState(GL_NORMAL_ARRAY);
//     glBindBuffer(GL_ARRAY_BUFFER, 0);
//     glDeleteBuffers(1, &normal_buffer);
    
//     // 7. ERROR CHECK
//     GLenum error = glGetError();
//     if(error == GL_NO_ERROR) {
//         std::cout << "SUCCESS: No OpenGL errors" << std::endl;
//     } else {
//         std::cout << "ERROR: OpenGL error " << error << std::endl;
//     }
// }

void test_with_normals_only(GLuint v_int, std::vector<GLfloat> v_indices) {
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    
    // 2. Einfache feste Farbe
    glColor3f(1.0f, 0.0f, 0.0f);  // KNALLROT
    
    // 3. Nur Vertex Buffer
    glBindBuffer(GL_ARRAY_BUFFER, v_int);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    
    // 4. DrawArrays (keine Indices)
    GLsizei vertex_count = v_indices.size() / 3;
    glDrawArrays(GL_TRIANGLES, 0, vertex_count);
    
    // 5. Cleanup
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void setup_proper_lighting() {
    // 1. GLOBAL AMBIENT (wichtigstes!)
    GLfloat global_ambient[] = {0.4f, 0.4f, 0.4f, 1.0f};  // 40% Helligkeit überall
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    
    // 2. MAIN LIGHT (Key Light)
    glEnable(GL_LIGHT0);
    
    // Licht von OBEN-VORNE (nicht direkt vorne)
    GLfloat light0_pos[] = {0.0f, 5.0f, 5.0f, 1.0f};  // Oben-vorne
    GLfloat light0_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat light0_diffuse[] = {0.7f, 0.7f, 0.7f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    
    // 3. FILL LIGHT (von der Seite)
    glEnable(GL_LIGHT1);
    GLfloat light1_pos[] = {5.0f, 0.0f, 0.0f, 1.0f};  // Rechts
    GLfloat light1_diffuse[] = {0.3f, 0.3f, 0.3f, 1.0f};  // Schwächer
    
    glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    
    // 4. BACK LIGHT (von hinten)
    glEnable(GL_LIGHT2);
    GLfloat light2_pos[] = {0.0f, 0.0f, -5.0f, 1.0f};  // Hinten
    GLfloat light2_diffuse[] = {0.2f, 0.2f, 0.2f, 1.0f};  // Sehr schwach
    
    glLightfv(GL_LIGHT2, GL_POSITION, light2_pos);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
    
    // 5. TWO-SIDED LIGHTING
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    
    std::cout << "Lighting: 3-point setup (key, fill, back) + ambient" << std::endl;
}





void create_flat_arrays(
    const std::vector<inner_elements>& faces,
    std::vector<GLfloat>& vertex_buffer,
    std::vector<GLfloat>& normal_buffer,
    std::vector<GLfloat>& tex_buffer
) {
    vertex_buffer.clear();
    normal_buffer.clear();
    tex_buffer.clear();

    for (const auto& face : faces) {
        size_t count = face.v.size();  // Anzahl Vertices in diesem Face

        for (size_t i = 0; i < count; i++) {

            // -------- Position (v) --------
            vertex_buffer.push_back(face.v[i][0]);
            vertex_buffer.push_back(face.v[i][1]);
            vertex_buffer.push_back(face.v[i][2]);

            // -------- Normale (vn) --------
            if (i < face.vn.size()) {
                normal_buffer.push_back(face.vn[i][0]);
                normal_buffer.push_back(face.vn[i][1]);
                normal_buffer.push_back(face.vn[i][2]);
            } else {
                // Falls OBJ keine Normale hat → default: 0,0,1
                normal_buffer.push_back(0.0f);
                normal_buffer.push_back(0.0f);
                normal_buffer.push_back(1.0f);
            }

            // -------- UV (vt) --------
            if (i < face.vt.size()) {
                tex_buffer.push_back(face.vt[i][0]);
                tex_buffer.push_back(face.vt[i][1]);
            } else {
                tex_buffer.push_back(0.0f);
                tex_buffer.push_back(0.0f);
            }
        }
    }
}


void Scop_window::hold_open() {
    std::vector<inner_elements> node = faces->get_list();
    std::vector<std::vector<std::array<double, 3>>> v;
    std::vector<std::vector<std::array<double, 3>>> vn;
    std::vector<std::vector<std::array<double, 2>>> vt;
    std::vector<unsigned int> indices = create_vectors(node, v, vn, vt);
    std::vector<GLfloat> v_indices, vt_indices, vn_indices;

    v_indices = create_GLfloat_array(v);
    vn_indices = create_GLfloat_array(vn);
    vt_indices = create_GLfloat_array(vt);

    // create_flat_arrays(node, v_indices, vn_indices, vt_indices);
    // reallign_highest_point(v_indices, 0);
    // reallign_highest_point(v_indices, 1);
    // reallign_highest_point(v_indices, 2);

    for (std::vector<unsigned int>::iterator it = indices.begin(); it != indices.end(); it++)
        std::cout << *it << ", ";
    // indices.clear();
    // for (unsigned int pos = 0; pos < v_indices.size(); pos++)
    //     indices.push_back(pos);
    std::cout << std::endl;
    drawer->create_vbo(v_indices, indices, vt_indices, vn_indices);
    std::string mat_name = faces->get_material_from_file();;
    std::cout << mat_name << std::endl;
    std::cout << "Size: " << vn_indices.size() << std::endl;
    // force_outward_normals(v_indices, vn_indices);
    // drawer->setup_face_colors(verts_indices);
    // create_vertex_array(verts, normals, textcords, converted_v, converted_vn, converted_vt);

    
    XEvent e;
    Atom wmDeleteMessage = XInternAtom(main_display, "WM_DELETE_WINDOW", False);
    GLsizei indice_size = static_cast<GLsizei>(indices.size());
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
        
        GLfloat lightPosition[] = {0.0f, 0.0f, 5.0f, 1.0f};
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
        
        glTranslatef(drawer->get_xPos(), drawer->get_yPos(), 0.0f);
        glRotatef(drawer->get_rl_rotation() * 90.0f, 0.0f, 1.0f, 0.0f);
        glRotatef(drawer->get_ud_rotation() * 90.0f, 1.0f, 0.0f, 0.0f);

        apply_material(mat_name);
        // if (drawer->has_texture()) {
        //     glEnable(GL_TEXTURE_2D);
        //     glBindTexture(GL_TEXTURE_2D, drawer->get_texture_id);
        // }
        GLfloat light_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
        GLfloat light_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
        GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat light_position[] = {0.0f, 0.0f, 5.0f, 1.0f};

        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);

        drawer->render_vbo(indice_size / 3);
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

std::vector<unsigned int> Scop_window::create_vectors(std::vector<inner_elements>& node, std::vector<std::vector<std::array<double, 3>>>& v, std::vector<std::vector<std::array<double, 3>>>& vn, std::vector<std::vector<std::array<double, 2>>>& vt) {
    for (std::vector<inner_elements>::iterator list_it = node.begin(); list_it != node.end(); list_it++) {
        v.push_back(list_it->v);
        vn.push_back(list_it->vn);
        vt.push_back(list_it->vt);
    }

    std::vector<std::vector<std::array<double, 3>>>::iterator v_it = v.begin();
    std::vector<std::vector<std::array<double, 3>>>::iterator vn_it = vn.begin();
    std::vector<std::vector<std::array<double, 2>>>::iterator vt_it = vt.begin();
    for (; v_it != v.end(); v_it++, vn_it++, vt_it++) {
        std::vector<std::array<double, 3>>::iterator v_inner = v_it->begin();
        std::vector<std::array<double, 3>>::iterator vn_inner = vn_it->begin();
        std::vector<std::array<double, 2>>::iterator vt_inner = vt_it->begin();
        for (; v_inner != v_it->end(); v_inner++, vn_inner++, vt_inner++) {
            std::cout << (*v_inner)[0] << "/" << (*vt_inner)[0] << "/" << (*vn_inner)[0] << std::endl;
        }
        std::cout << std::endl;
    }

    std::vector<std::vector<std::array<double, 3>>> converted_v = v;
    std::vector<std::vector<std::array<double, 2>>> converted_vt;
    std::vector<std::vector<std::array<double, 3>>> converted_vn;
    std::vector<unsigned int> indices = split_and_group(converted_v, faces->get_indices());
    std::cout << "Size: " << indices.size() << std::endl;
    // converted_v = split_and_group(v);
    // if (vt.size() > 0)
    //     converted_vt = split_and_group(vt);
    // if (vn.size() > 0)
    //     converted_vn = split_and_group(vn);

    // reallign_highest_point(converted_v, 0);
    // reallign_highest_point(converted_v, 1);
    // reallign_highest_point(converted_v, 2);
    

    v = converted_v;
    vt = converted_vt;
    vn = converted_vn;
    // indices.clear();
    return indices;
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