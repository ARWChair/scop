#include <iostream>
#include "../OpenGL/OpenGL.hpp"
#include "../Window/Window.hpp"
#include <cmath>
#pragma once

class draw {
    public:
        draw();
        draw(Scop_openGL &scop_openGL, Scop_window &scop_window);
        draw& operator=(const draw& copy);
        ~draw();

        void make_current(GLXDrawable drawable);
        void make_current();
        void clear();
        void draw_line();
        void draw_plane(double xPos, double yPos);

        double const &get_ud_rotation() const;
        double const &get_rl_rotation() const;
        double const &get_xPos() const;
        double const &get_yPos() const;
        void inc_ud_rotation(double delta);
        void dec_ud_rotation(double delta);
        void inc_rl_rotation(double delta);
        void dec_rl_rotation(double delta);
        void set_xPos(double x);
        void set_yPos(double y);
    protected:
    private:
        Scop_window &scop_window;
        Scop_openGL &scop_openGL;
        double rl_rot, ud_rot, xPos, yPos;
};
