#include "Draw.hpp"
#include "../Window/Window.hpp"
#include "../OpenGL/OpenGL.hpp"

Draw::Draw(Scop_openGL &scop_openGL, Scop_window &scop_window):
scop_window(scop_window), scop_openGL(scop_openGL), rl_rot(0.0f), ud_rot(0.0f), xPos(0.0f), yPos(0.0f)
{}

Draw& Draw::operator=(const Draw& copy) {
    if (this != &copy) {
        this->scop_openGL = copy.scop_openGL;
        this->scop_window = copy.scop_window;
        this->rl_rot = copy.rl_rot;
        this->ud_rot = copy.ud_rot;
    }
    return *this;
}

Draw::~Draw() {}

void Draw::make_current(GLXDrawable drawable) {
    scop_openGL.make_current(drawable);
}

void Draw::make_current() {
    scop_openGL.make_current((GLXDrawable)scop_window.get_window());
}

void Draw::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Draw::set_color(float r, float g, float b) {
    glColor3f(r, g, b);
}

void Draw::draw_line() {
    glBegin(GL_LINES);
        glVertex3f(-0.5f, 0.0f, 0.0f);
        glVertex3f(0.5f, 0.0f, 0.0f);
    glEnd();
}

void Draw::draw_triangle(std::array<double, 3> xPos, std::array<double, 3> yPos, std::array<double, 3> zPos, std::array<double, 3> vn) {
    glNormal3f(vn[0], vn[1], vn[2]);
    glVertex3f(xPos[0], xPos[1], xPos[2]);
    glVertex3f(yPos[0], yPos[1], yPos[2]);
    glVertex3f(zPos[0], zPos[1], zPos[2]);
}

void Draw::draw_triangle(std::array<double, 3> xPos, std::array<double, 3> yPos, std::array<double, 3> zPos) {
    glVertex3f(xPos[0], xPos[1], xPos[2]);
    glVertex3f(yPos[0], yPos[1], yPos[2]);
    glVertex3f(zPos[0], zPos[1], zPos[2]);
}

void Draw::draw_plane(double xPos, double yPos) {
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

void Draw::inc_rl_rotation(double delta) {
    rl_rot += delta;
    rl_rot = std::round(rl_rot * 10000.0) / 10000.0;
}

void Draw::inc_ud_rotation(double delta) {
    ud_rot += delta;
    ud_rot = std::round(ud_rot * 10000.0) / 10000.0;
}

void Draw::dec_rl_rotation(double delta) {
    rl_rot -= delta;
    rl_rot = std::round(rl_rot * 10000.0) / 10000.0;
}

void Draw::dec_ud_rotation(double delta) {
    ud_rot -= delta;
    ud_rot = std::round(ud_rot * 10000.0) / 10000.0;
}

double const &Draw::get_ud_rotation() const {
    return ud_rot;
}

double const &Draw::get_rl_rotation() const {
    return rl_rot;
}

double const &Draw::get_xPos() const {
    return xPos;
}

double const &Draw::get_yPos() const {
    return yPos;
}

void Draw::set_xPos(double x) {
    xPos = x;
}

void Draw::set_yPos(double y) {
    yPos = y;
}