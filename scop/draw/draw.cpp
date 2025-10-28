#include "draw.hpp"
#include "../Window/Window.hpp"

draw::draw():
scop_window(*(Scop_window*)0), scop_openGL(*(Scop_openGL*)0), rl_rot(0.0f), ud_rot(0.0f), xPos(0.0f), yPos(0.0f)
{}

draw::draw(Scop_openGL &scop_openGL, Scop_window &scop_window):
scop_window(scop_window), scop_openGL(scop_openGL), rl_rot(0.0f), ud_rot(0.0f), xPos(0.0f), yPos(0.0f)
{}

draw& draw::operator=(const draw& copy) {
    if (this != &copy) {
        this->scop_openGL = copy.scop_openGL;
        this->scop_window = copy.scop_window;
        this->rl_rot = copy.rl_rot;
        this->ud_rot = copy.ud_rot;
    }
    return *this;
}

draw::~draw() {}

void draw::make_current(GLXDrawable drawable) {
    scop_openGL.make_current(drawable);
}

void draw::make_current() {
    scop_openGL.make_current((GLXDrawable)scop_window.get_window());
}

void draw::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void draw::draw_line() {
    glBegin(GL_LINES);
        glVertex3f(-0.5f, 0.0f, 0.0f);
        glVertex3f(0.5f, 0.0f, 0.0f);
    glEnd();
}

void draw::draw_plane(double xPos, double yPos) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (rl_rot < 0.5f && rl_rot > -0.5f)
        glColor3f(0.0f, 1.0f, 0.0f);
    else
        glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.5f + xPos + rl_rot, -0.5f + yPos - ud_rot / 2);
    glVertex2f(0.5f + xPos - rl_rot, -0.5f + yPos + ud_rot / 2);
    glVertex2f(0.5f + xPos - rl_rot, 0.5f + yPos - ud_rot / 2);
    glVertex2f(-0.5f + xPos + rl_rot, 0.5f + yPos + ud_rot / 2);
    glEnd();
    glXSwapBuffers((Display *)scop_window.get_display(), scop_openGL.get_drawable());
    glFlush();
}

void draw::inc_rl_rotation(double delta) {
    rl_rot += delta;
    rl_rot = std::round(rl_rot * 10000.0) / 10000.0;
}

void draw::inc_ud_rotation(double delta) {
    ud_rot += delta;
    ud_rot = std::round(ud_rot * 10000.0) / 10000.0;
}

void draw::dec_rl_rotation(double delta) {
    rl_rot -= delta;
    rl_rot = std::round(rl_rot * 10000.0) / 10000.0;
}

void draw::dec_ud_rotation(double delta) {
    ud_rot -= delta;
    ud_rot = std::round(ud_rot * 10000.0) / 10000.0;
}

double const &draw::get_ud_rotation() const {
    return ud_rot;
}

double const &draw::get_rl_rotation() const {
    return rl_rot;
}

double const &draw::get_xPos() const {
    return xPos;
}

double const &draw::get_yPos() const {
    return yPos;
}

void draw::set_xPos(double x) {
    xPos = x;
}

void draw::set_yPos(double y) {
    yPos = y;
}