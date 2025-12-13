#include <X11/keysym.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <exception>
#include <stdbool.h>
#include <cstring>
#include <vector>
#include <array>
#include <iostream>
#include "../Faces/Faces.hpp"
#include "../../GL/glew.h"
#pragma once

class Draw;
class Faces;
class Scop_openGL;
class Material;
class BMP;

class Scop_window {
    public:
        Scop_window();
        Scop_window(int x, int y, unsigned int width, unsigned int height, unsigned int border_width = 0,
                    int depth = 0, unsigned int ui_class = 0, Visual *visual = CopyFromParent,
                    unsigned int valuemask = 0, XSetWindowAttributes attributes = {});
        Scop_window& operator=(const Scop_window&);
        ~Scop_window();

        void create_window();
        void hold_open();
        Display const *get_display() const;
        Window const &get_window() const;
        void set_drawer(Draw*& drawer);
        void set_faces(Faces*& faces);
        void set_openGL(Scop_openGL*&);
        void set_material(Material*&);
        void set_bmp(BMP *&bmp);
        
        const unsigned int &get_width() const;
        const unsigned int &get_height() const;
        // Exceptions
        class WindowCreationFailed : public std::exception {
            public:
            virtual const char* what() const throw() {
                return "Error while creating a window";
            }
        };
    protected:
    private:
        void apply_material(std::string&);
        std::vector<GLfloat> summarize_vectors(flat_indices&, flat&);

        Display* main_display;
        Window root_window, main_window;
        unsigned int width, height, border_width, ui_class;
        unsigned long valuemask;
        int depth, x, y;
        Visual *visual;
        XSetWindowAttributes attributes;
        bool created_window;
        Scop_openGL* scop_openGL;
        Material *material;
        Faces* faces;
        Draw* drawer;
        BMP *bmp;
        bool toggle;
};