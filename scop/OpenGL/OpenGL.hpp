#include <vector>
// #include "../Draw/Draw.hpp"
#include "../../GL/glew.h"
// #include <GL/gl.h>
// #include <GL/glx.h>
// #include <GL/glu.h>
#include <GL/glx.h>
// #include <GL/glext.h>
#include <exception>
#include <iostream>
#include "../Window/Window.hpp"
#pragma once

class Scop_openGL {
    public:
        Scop_openGL();
        Scop_openGL(const Scop_window *window, const Display *display, int screen);
        Scop_openGL& operator=(const Scop_openGL& copy);
        ~Scop_openGL();

        void choose_display_fb_exception();
        void create_glx_context();
        void make_current(GLXDrawable drawable);
        void create_viewport();
        void create_lighting();
        void set_display(Display *display);
        void draw_plane(float rotation);

        GLXDrawable const &get_drawable() const;
        Scop_window const *get_window() const;
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
        class GlewCreationFailed : public std::exception {
            public:
            virtual const char* what() const throw() {
                return "Error while initializing Glew";
            }
        };
    protected:
    private:
        XVisualInfo *visual_info;
        GLXFBConfig *fbconfig;
        GLXDrawable drawable;
        Scop_window *window;
        GLXContext context;
        Display *display;
        int nelements;
        int screen;
        bool done;
};

// https://wikis.khronos.org/opengl/Programming_OpenGL_in_Linux:_GLX_and_Xlib
