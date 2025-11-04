#include "Window.hpp"
#include "../draw/draw.hpp"
#include "../Faces/Faces.hpp"

Scop_window::Scop_window() {
    main_display = XOpenDisplay(0);
    root_window = XDefaultRootWindow(main_display);
    x = y = width = height = border_width = ui_class = depth = valuemask = 0;
    visual = CopyFromParent;
    attributes = {};
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
    std::vector<std::array<std::array<double, 3>, 3>> f = faces->get_faces();
    // drawer->draw_triangle(temp_arr[0],temp_arr[1], temp_arr[2]);

    for(;;) {
        XEvent e;
        XNextEvent(main_display, &e);

        switch (e.type) {
            case Expose:
                break;
            case KeyPress: {
                KeySym ks = XLookupKeysym(&e.xkey, 0);
                if (ks == XK_Escape) {
                    return;
                }
                if (ks == 101) { // e key
                    drawer->inc_rl_rotation(0.025f); // 5 Grad statt 0.01
                }
                else if (ks == 113) { // q key  
                    drawer->dec_rl_rotation(0.025f);
                }
                if (ks == 119) {
                    drawer->set_yPos(drawer->get_yPos() + 0.1f);
                }
                if (ks == 115) {
                    drawer->set_yPos(drawer->get_yPos() + -0.1f);
                }
                if (ks == 97) {
                    drawer->set_xPos(drawer->get_xPos() + -0.1f);
                }
                if (ks == 100) {
                    drawer->set_xPos(drawer->get_xPos() + 0.1f);
                }
                std::cout << "Key pressed: " << XKeysymToString(ks) << ", " << ks << std::endl;
                break;
            }
            default:
                break;
        }
        drawer->clear();
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        gluLookAt(0, 0, 5,
                  0, 0, 0,
                  0, 1, 0);
        glScalef(0.5f, 0.5f, 0.5f);
        
        glTranslatef(drawer->get_xPos(), drawer->get_yPos(), 0.0f);
        glRotatef(drawer->get_rl_rotation() * 90.0f, 0.0f, 1.0f, 0.0f);
        glRotatef(drawer->get_ud_rotation() * 90.0f, 1.0f, 0.0f, 0.0f);
        
        glBegin(GL_TRIANGLES);
        for (size_t i = 0; i < f.size(); ++i) {
            const auto &tri = f[i];
            if (i % 2 == 0)
                drawer->set_color(0.0f, 1.0f, 0.0f);
            else
                drawer->set_color(0.0f, 0.5f, 0.0f);
            drawer->draw_triangle(tri[0], tri[1], tri[2]);
        }
        glEnd();
        glXSwapBuffers((Display *)get_display(), scop_openGL->get_drawable());
        glFlush();
    }
}

void Scop_window::set_openGL(Scop_openGL* scop_openGL) {
    this->scop_openGL = scop_openGL;
}

Display const *Scop_window::get_display() const {
    return main_display;
}

Window const &Scop_window::get_window() const {
    return main_window;
}

void Scop_window::set_drawer(Draw* drawer) {
    this->drawer = drawer;
}

void Scop_window::set_faces(Faces* faces) {
    this->faces = faces;
}

const unsigned int &Scop_window::get_width() const {
    return width;
}

const unsigned int &Scop_window::get_height() const {
    return height;
}
