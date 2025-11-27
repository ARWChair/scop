#include "Draw.hpp"
#include "../Window/Window.hpp"
#include "../OpenGL/OpenGL.hpp"
#include "../Material/Material.hpp"

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

void Draw::draw_triangle(std::vector<std::array<double, 3>> v, std::array<double, 3> vn, std::vector<std::array<double, 2>> vt, Material *&material, bool toggle) {
    std::vector<std::array<double, 3>>::iterator it = v.begin();
    std::vector<std::array<double, 2>>::iterator it_2 = vt.begin();
    
    if (material->is_missing() == false && toggle == true)
        draw_texture(material);
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

void Draw::draw_triangle(std::vector<std::array<double, 3>> v, std::vector<std::array<double, 2>> vt, Material *&material, bool toggle) {
    std::vector<std::array<double, 3>>::iterator it = v.begin();
    std::vector<std::array<double, 2>>::iterator it_2 = vt.begin();

    if (material->is_missing() == false && toggle == true)
        draw_texture(material);
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

void Draw::draw_triangle(std::vector<std::array<double, 3>> v, std::array<double, 3> vn, Material *&material, bool toggle) {
    std::vector<std::array<double, 3>>::iterator it = v.begin();
    
    if (material->is_missing() == false && toggle == true)
        draw_texture(material);
    glNormal3f(vn[0], vn[1], vn[2]);
    glVertex3f((*it)[0], (*it)[1], (*it)[2]);
    it++;
    glVertex3f((*it)[0], (*it)[1], (*it)[2]);
    it++;
    glVertex3f((*it)[0], (*it)[1], (*it)[2]);
}

void Draw::draw_triangle(std::vector<std::array<double, 3>> v, Material *&material, bool toggle) {
    std::vector<std::array<double, 3>>::iterator it = v.begin();

    if (material->is_missing() == false && toggle == true)
        draw_texture(material);
    glVertex3f((*it)[0], (*it)[1], (*it)[2]);
    it++;
    glVertex3f((*it)[0], (*it)[1], (*it)[2]);
    it++;
    glVertex3f((*it)[0], (*it)[1], (*it)[2]);
}

void draw_individual_text(std::array<double, 3UL> type, int16_t type_name) {
    if (type.size() != 0) {
        float scale = 1;
        if (type_name == GL_AMBIENT)
            scale = 0.1f;
        GLfloat k[4] = {
            static_cast<GLfloat>(type[0]) * scale,
            static_cast<GLfloat>(type[1]) * scale, 
            static_cast<GLfloat>(type[2]) * scale,
            static_cast<GLfloat>(1.0f)
        };
        glMaterialfv(GL_FRONT, type_name, k);
    }
}

void Draw::draw_texture(Material *&material) {
    // std::cout << material << std::endl;
    // if (material->get_map_Kd().size() == 0)
    //     glDisable(GL_TEXTURE_2D);
    // // else {
        
    // //     glEnable();
    // // }

    draw_individual_text(material->get_Ka(), GL_AMBIENT);
    draw_individual_text(material->get_Kd(), GL_DIFFUSE);
    draw_individual_text(material->get_Ks(), GL_SPECULAR);
    draw_individual_text(material->get_Ke(), GL_EMISSION);
    if (material->get_Ns().size() != 0) {
        GLfloat blender_ns = static_cast<GLfloat>(material->get_Ns()[0]);
        GLfloat opengl_ns;
        opengl_ns = blender_ns * (128.0f / 1000.0f);
        if (blender_ns > 1000.0f)
            opengl_ns = 128.0f;
        if (blender_ns < 200.0f)
            opengl_ns = blender_ns * 0.3f;
        else if (blender_ns < 600.0f)
            opengl_ns = blender_ns * 0.2f;
        else
            opengl_ns = blender_ns * 0.128f;
        glMaterialf(GL_FRONT, GL_SHININESS, opengl_ns);
    }
    
    // if (!material->get_d().empty()) {
    //     glEnable(GL_BLEND);
    //     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //     glColor4f(1.0f, 1.0f, 1.0f, material->get_d()[0]);
    // } else {
    //     glColor3f(1.0f, 1.0f, 1.0f);
    //     glDisable(GL_BLEND);
    // }
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