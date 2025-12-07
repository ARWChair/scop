#include <iostream>
#include "../OpenGL/OpenGL.hpp"
#include "../Window/Window.hpp"
#include <vector>
// #include <GL/glx.h>
// #include "../../../../libs/glew-2.2.0/include/GL/glew.h"
#include <array>
#include <cmath>
#pragma once

struct FinalVertex {
    float px, py, pz;
    float nx, ny, nz;
    float u, v;
};

class Faces;

class Draw {
    public:
        Draw(Scop_openGL &scop_openGL, Scop_window &scop_window);
        Draw& operator=(const Draw& copy);
        ~Draw();

        void make_current(GLXDrawable drawable);
        void make_current();
        void clear();
        void set_color(float r, float g, float b);
        void draw_triangle(std::vector<std::array<double, 3>>, std::array<double, 3> vn, std::vector<std::array<double, 2>> vt, Material *&material, bool toggle);
        void draw_triangle(std::vector<std::array<double, 3>>, std::array<double, 3> vn, Material *&material, bool toggle);
        void draw_triangle(std::vector<std::array<double, 3>>, std::vector<std::array<double, 2>> vt, Material *&material, bool toggle);
        void draw_triangle(std::vector<std::array<double, 3>> v, Material *&material, bool toggle);
        void draw_triangle(std::vector<GLfloat>& f);
        void setup_face_colors(std::vector<GLfloat>& verts);
        void draw_texture(Material *&material);
        void create_vbo(std::vector<GLfloat>&, std::vector<unsigned int>&, std::vector<GLfloat>&, std::vector<GLfloat>&);
        void render_vbo(GLsizei verts); // vboid1 = vertexes, vboid2 = indices

        double const &get_ud_rotation() const;
        double const &get_rl_rotation() const;
        double const &get_xPos() const;
        double const &get_yPos() const;
        void inc_ud_rotation(double delta);
        void dec_ud_rotation(double delta);
        void inc_rl_rotation(double delta);
        void dec_rl_rotation(double delta);
        void set_faces(Faces*& faces);
        void set_xPos(double x);
        void set_yPos(double y);
    protected:
    private:
        Scop_window &scop_window;
        Scop_openGL &scop_openGL;
        Faces       *faces;
        GLuint      v_int, v_ind, vn_int, vt_int, attribVertex, attribTextures, attribNormals;
        double      rl_rot, ud_rot, xPos, yPos;
        bool        vn_bool, vt_bool;
};
