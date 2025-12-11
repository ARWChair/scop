#include <iostream>
#include "../OpenGL/OpenGL.hpp"
#include "../Window/Window.hpp"
#include <vector>
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
        void create_vbo(std::vector<GLfloat>&, std::vector<unsigned int>&);
        void render_vbo(GLsizei verts, bool state);
        void draw_individual_text(std::array<double, 3UL> type, int16_t type_name);
        void split_elements(std::vector<inner_elements>&);

        double const &get_ud_rotation() const;
        double const &get_rl_rotation() const;
        double const &get_xPos() const;
        double const &get_yPos() const;
        void inc_ud_rotation(double delta);
        void dec_ud_rotation(double delta);
        void inc_rl_rotation(double delta);
        void dec_rl_rotation(double delta);
        void inc_yPos(double value);
        void inc_xPos(double value);
        void set_faces(Faces*& faces);
        void set_xPos(double x);
        void set_yPos(double y);
        void inc_scroll(double value);
        void dec_scroll(double value);
        void set_vt(bool value);
        void set_vn(bool value);
        bool get_vt();
        bool get_vn();
        double get_scroll();
        
        GLuint get_v_int() {
            return v_int;
        }
    protected:
    private:
        Scop_window &scop_window;
        Scop_openGL &scop_openGL;
        Faces       *faces;
        GLuint      v_int, v_ind;
        double      rl_rot, ud_rot, xPos, yPos, scroll;
        bool        vn_bool, vt_bool;
};
