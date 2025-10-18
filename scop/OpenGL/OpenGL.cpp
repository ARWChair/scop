#include "OpenGL.hpp"

Scop_openGL::Scop_openGL() {}

Scop_openGL::Scop_openGL(const Display *display, int screen) {
    this->done = false;
    if (!display)
        throw DisplayNullException();
    this->display = (Display *)display;
    this->screen = screen;
    choose_display_fb_exception();
    create_glx_context();
    // make_current();
    this->done = true;
}

Scop_openGL& Scop_openGL::operator=(const Scop_openGL& copy) {
    if (this != &copy) {
        this->visual_info = copy.visual_info;
        this->fbconfig = copy.fbconfig;
        this->context = copy.context;
        this->display = copy.display;
        this->attrib_list = copy.attrib_list;
        this->nelements = copy.nelements;
        this->screen = copy.screen;
        this->done = copy.done;
    }
    return *this;
}

Scop_openGL::~Scop_openGL() {}

void Scop_openGL::choose_display_fb_exception() {
    const int nitems[] = {
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
    visual_info = glXChooseVisual(display, screen, attrib_list);
    if (!visual_info)
        throw VisualInfoNullException();
    context = glXCreateContext(display, visual_info, NULL, true);
    if (!context)
        throw CreateContextNullException();
}

void Scop_openGL::make_current(GLXDrawable drawable) {
    this->drawable = drawable;
    if (glXMakeCurrent(display, drawable, context) != true)
        throw MakeCurrentNullException();
}

void Scop_openGL::set_display(Display *display) {
    this->display = display;
}

void Scop_openGL::draw_box() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.5f, 0.5f);
    glVertex2f(-0.5f, 0.5f);
    glEnd();
    glXSwapBuffers(display, drawable);
    glFlush();
}