#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <exception>
#include <iostream>
#pragma once

class Scop_openGL {
    public:
        Scop_openGL();
        Scop_openGL(const Display *display, int screen);
        Scop_openGL& operator=(const Scop_openGL& copy);
        ~Scop_openGL();

        void choose_display_fb_exception();
        void create_glx_context();
        void make_current(GLXDrawable drawable);
        void set_display(Display *display);
        void draw_box();
        // ---------- Exceptions ---------- \\'
        class GLXBSetupException: public std::exception {
            public:
                const char* what() const throw() {
                    return "Error when creating OpenGL object.";
                }
        };
        class DisplayNullException: public std::exception {
            public:
                const char* what() const throw() {
                    return "Error Display is NULL";
                }
        };
        class VisualInfoNullException: public std::exception {
            public:
                const char* what() const throw() {
                    return "Error Visualinfo is NULL";
                }
        };
        class CreateContextNullException: public std::exception {
            public:
                const char* what() const throw() {
                    return "Error Context is NULL";
                }
        };
        class MakeCurrentNullException: public std::exception {
            public:
                const char* what() const throw() {
                    return "Error while executing MakeCurrent";
                }
        };
    protected:
    private:
        XVisualInfo *visual_info;
        GLXFBConfig *fbconfig;
        GLXDrawable drawable;
        GLXContext context;
        Display *display;
        int *attrib_list;
        int nelements;
        int screen;
        bool done;
};

// https://wikis.khronos.org/opengl/Programming_OpenGL_in_Linux:_GLX_and_Xlib