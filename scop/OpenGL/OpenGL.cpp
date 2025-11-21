#include "OpenGL.hpp"

Scop_openGL::Scop_openGL() {
    this->done = false;
}

Scop_openGL::Scop_openGL(const Scop_window *window, const Display *display, int screen) {
    this->done = false;
    if (!display)
        throw DisplayNullException();
    this->display = (Display *)display;
    this->window = (Scop_window *)window;
    this->screen = screen;
    choose_display_fb_exception();
    create_glx_context();
    make_current(window->get_window());
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    glDepthFunc(GL_LESS);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    
    this->done = true;
}

Scop_openGL& Scop_openGL::operator=(const Scop_openGL& copy) {
    if (this != &copy) {
        this->visual_info = copy.visual_info;
        this->fbconfig = copy.fbconfig;
        this->context = copy.context;
        this->display = copy.display;
        this->window = copy.window;
        this->attrib_list = copy.attrib_list;
        this->nelements = copy.nelements;
        this->screen = copy.screen;
        this->done = copy.done;
    }
    return *this;
}

Scop_openGL::~Scop_openGL() {}

void Scop_openGL::choose_display_fb_exception() {
    static const int nitems[] = {
        GLX_X_RENDERABLE, True,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
        GLX_RED_SIZE,   8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE,  8,
        GLX_ALPHA_SIZE, 8,
        GLX_DEPTH_SIZE, 24,
        GLX_DOUBLEBUFFER, True,
        0
    };
    attrib_list = (int *)nitems;
    if (!display)
        throw DisplayNullException();
    fbconfig = glXChooseFBConfig((Display *)display, screen, nitems, &nelements);
    if (!fbconfig)
        throw GLXBSetupException();
}

void Scop_openGL::create_glx_context() {
    if (!fbconfig || nelements <= 0)
        throw VisualInfoNullException();

    visual_info = glXGetVisualFromFBConfig(display, fbconfig[0]);
    if (!visual_info)
        throw VisualInfoNullException();

    context = glXCreateContext(display, visual_info, NULL, True);
    if (!context)
        throw CreateContextNullException();
}

void Scop_openGL::create_viewport() {
    int width = window->get_width(), height = window->get_height();
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)width / (float)height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "OpenGL Error after viewport: " << error << std::endl;
    }
}

void Scop_openGL::make_current(GLXDrawable drawable) {
    this->drawable = drawable;
    if (glXMakeCurrent(display, drawable, context) != true)
        throw MakeCurrentNullException();
}

void Scop_openGL::set_display(Display *display) {
    this->display = display;
}

GLXDrawable const &Scop_openGL::get_drawable() const {
    return drawable;
}

Scop_window const *Scop_openGL::get_window() const {
    return window;
}