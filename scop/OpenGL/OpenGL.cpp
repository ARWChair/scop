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
    this->visual_info = NULL;
    this->context = NULL;
    this->fbconfig = NULL;
    choose_display_fb_exception();
    create_glx_context();
    make_current(window->get_window());
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    create_lighting();

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
        this->nelements = copy.nelements;
        this->screen = copy.screen;
        this->done = copy.done;
    }
    return *this;
}

Scop_openGL::~Scop_openGL() {
    XFree(fbconfig);
    XFree(visual_info);
    glXDestroyContext(display, context);
}

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
    if (fbconfig) {
        XFree(fbconfig);
        fbconfig = NULL;
    }
    if (!display)
        throw DisplayNullException();
    fbconfig = glXChooseFBConfig(display, screen, nitems, &nelements);
    if (!fbconfig)
        throw GLXBSetupException();
}

void Scop_openGL::create_glx_context() {
    if (!fbconfig || nelements <= 0)
        throw VisualInfoNullException();

    XVisualInfo* new_visual = glXGetVisualFromFBConfig(display, fbconfig[0]);
    if (!new_visual)
        throw VisualInfoNullException();

    if (visual_info)
        XFree(visual_info);
    visual_info = new_visual;
    if (context != 0)
        glXDestroyContext(display, context);
    context = glXCreateContext(display, visual_info, NULL, True);
    if (context == 0)
        throw CreateContextNullException();
}

void Scop_openGL::create_viewport() {
    int width = window->get_width(), height = window->get_height();
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)width / (float)height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "OpenGL Error after viewport: " << error << std::endl;
    }
}

void Scop_openGL::create_lighting() {
    GLfloat lightAmbient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat lightSpecular[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat lightPosition[] = {3.0f, 3.0f, 3.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    GLfloat globalAmbient[] = {0.1f, 0.1f, 0.1f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
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