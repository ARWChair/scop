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

void Draw::draw_triangle(std::vector<std::array<double, 3>> v, std::array<double, 3> vn, std::vector<std::array<double, 2>> vt) {
    std::vector<std::array<double, 3>>::iterator it = v.begin();
    std::vector<std::array<double, 2>>::iterator it_2 = vt.begin();
    glNormal3f(vn[0], vn[1], vn[2]);
    glTexCoord2f((*it_2)[0], (*it_2)[1]);
    glVertex3f((*it)[0], (*it)[1], (*it)[2]);
    it++;
    it_2++;
    glTexCoord2f((*it_2)[0], (*it_2)[1]);
    glVertex3f((*it)[0], (*it)[1], (*it)[2]);
    it++;
    it_2++;
    glTexCoord2f((*it_2)[0], (*it_2)[1]);
    glVertex3f((*it)[0], (*it)[1], (*it)[2]);
}

void Draw::draw_triangle(std::vector<std::array<double, 3>> v, std::vector<std::array<double, 2>> vt) {
    std::vector<std::array<double, 3>>::iterator it = v.begin();
    std::vector<std::array<double, 2>>::iterator it_2 = vt.begin();
    glTexCoord2f((*it_2)[0], (*it_2)[1]);
    glVertex3f((*it)[0], (*it)[1], (*it)[2]);
    it++;
    it_2++;
    glTexCoord2f((*it_2)[0], (*it_2)[1]);
    glVertex3f((*it)[0], (*it)[1], (*it)[2]);
    it++;
    it_2++;
    glTexCoord2f((*it_2)[0], (*it_2)[1]);
    glVertex3f((*it)[0], (*it)[1], (*it)[2]);
}

void Draw::draw_triangle(std::vector<std::array<double, 3>> v, std::array<double, 3> vn) {
    std::vector<std::array<double, 3>>::iterator it = v.begin();
    glNormal3f(vn[0], vn[1], vn[2]);
    glVertex3f((*it)[0], (*it)[1], (*it)[2]);
    it++;
    glVertex3f((*it)[0], (*it)[1], (*it)[2]);
    it++;
    glVertex3f((*it)[0], (*it)[1], (*it)[2]);
}

void Draw::draw_triangle(std::vector<std::array<double, 3>> v) {
    std::vector<std::array<double, 3>>::iterator it = v.begin();
    glVertex3f((*it)[0], (*it)[1], (*it)[2]);
    it++;
    glVertex3f((*it)[0], (*it)[1], (*it)[2]);
    it++;
    glVertex3f((*it)[0], (*it)[1], (*it)[2]);
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