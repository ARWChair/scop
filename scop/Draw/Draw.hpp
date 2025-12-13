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
        Draw(Scop_openGL&, Scop_window&);
        Draw& operator=(const Draw&);
        ~Draw();

        void make_current(GLXDrawable);
        void make_current();
        void clear();
        void draw_texture(Material*&);
        void create_vbo(std::vector<GLfloat>&, std::vector<unsigned int>&);
        void render_vbo(GLsizei, bool);
        void draw_individual_text(std::array<double, 3UL>, int16_t);
        void create_texture(std::vector<unsigned char>, int, GLsizei, GLsizei);

        double const &get_ud_rotation() const;
        double const &get_rl_rotation() const;
        double const &get_xPos() const;
        double const &get_yPos() const;
        void inc_ud_rotation(double);
        void dec_ud_rotation(double);
        void inc_rl_rotation(double);
        void dec_rl_rotation(double);
        void inc_yPos(double);
        void inc_xPos(double);
        void set_faces(Faces*&);
        void inc_scroll(double);
        void dec_scroll(double);
        void set_vt(bool);
        bool get_vt();
        
    protected:
    private:
        Scop_window &scop_window;
        Scop_openGL &scop_openGL;
        Faces       *faces;
        GLuint      v_int, v_ind, v_text;
        double      rl_rot, ud_rot, xPos, yPos;
        bool        vt_bool, text_bool;
};
