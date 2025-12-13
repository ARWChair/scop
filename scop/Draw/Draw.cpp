#include "Draw.hpp"
#include "../Faces/Faces.hpp"
#include "../Window/Window.hpp"
#include "../OpenGL/OpenGL.hpp"
#include "../Material/Material.hpp"

Draw::Draw(Scop_openGL &scop_openGL, Scop_window &scop_window):
scop_window(scop_window), scop_openGL(scop_openGL), rl_rot(0.0f), ud_rot(0.0f), xPos(0.0f), yPos(0.0f), vt_bool(false), text_bool(false)
{}

Draw& Draw::operator=(const Draw& copy) {
    if (this != &copy) {
        this->scop_openGL = copy.scop_openGL;
        this->scop_window = copy.scop_window;
        this->rl_rot = copy.rl_rot;
        this->ud_rot = copy.ud_rot;
        this->xPos = copy.xPos;
        this->yPos = copy.yPos;
        this->vt_bool = copy.vt_bool;
        this->text_bool = copy.text_bool;
    }
    return *this;
}

Draw::~Draw() {
    glDeleteBuffers(1, &v_int);
    glDeleteBuffers(1, &v_ind);
    if (text_bool == true)
        glDeleteBuffers(1, &v_text);
}

void Draw::make_current(GLXDrawable drawable) {
    scop_openGL.make_current(drawable);
}

void Draw::make_current() {
    scop_openGL.make_current((GLXDrawable)scop_window.get_window());
}

void Draw::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Draw::create_texture(std::vector<unsigned char> pixels, int bits_per_pixels, GLsizei width, GLsizei height) {
    glGenTextures(1, &v_text);
    glBindTexture(GL_TEXTURE_2D, v_text);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    GLenum format = GL_RGB;
    if (bits_per_pixels == 32)
        format = GL_RGBA;
    else if (bits_per_pixels == 24)
        format = GL_RGB;
    else {
        std::cerr << "Unsupported BPP: " << bits_per_pixels << std::endl;
        glDeleteBuffers(1, &v_text);
        return;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels.data());
    glBindTexture(GL_TEXTURE_2D, 0);
    text_bool = true;
}

void Draw::create_vbo(std::vector<GLfloat>& data, std::vector<unsigned int>& indices) {
    glGenBuffers(1, &v_int);
    glBindBuffer(GL_ARRAY_BUFFER, v_int);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);
    
    glGenBuffers(1, &v_ind);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, v_ind);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Draw::render_vbo(GLsizei indexCount, bool state)
{
    std::size_t offset_color = 6;
    if (vt_bool)
        offset_color += 2;
    offset_color *= sizeof(GLfloat);

    GLsizei stride = 6;
    if (vt_bool)
        stride += 2;
    stride += 4;
    stride *= sizeof(GLfloat);

    glBindBuffer(GL_ARRAY_BUFFER, v_int);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, stride, (void*)0);
    if (vt_bool == true && state == true) {
        if (text_bool == true) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, v_text);
        }
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, stride, (void*)(3 * sizeof(GLfloat)));
    }
    glEnableClientState(GL_NORMAL_ARRAY);
    if (vt_bool == false)
        glNormalPointer(GL_FLOAT, stride, (void*)(3 * sizeof(GLfloat)));
    else
        glNormalPointer(GL_FLOAT, stride, (void*)(5 * sizeof(GLfloat)));

    if (state) {
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4, GL_FLOAT, stride, (void*)offset_color);
        glDisable(GL_COLOR_MATERIAL);
    } else {
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4, GL_FLOAT, stride, (void*)offset_color);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, v_ind);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glDisableClientState(GL_VERTEX_ARRAY);
    if (vt_bool == true)
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    if (text_bool == true) {
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Draw::inc_yPos(double value) {
    yPos += value;
}

void Draw::inc_xPos(double value) {
    xPos += value;
}

void Draw::set_vt(bool value) {
    vt_bool = value;
}

bool Draw::get_vt() {
    return vt_bool;
}

void Draw::inc_scroll(double value) {
    faces->inc_scale(value);
}

void Draw::dec_scroll(double value) {
    faces->dec_scale(value);
}

void Draw::draw_individual_text(std::array<double, 3UL> type, int16_t type_name) {
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
        glMaterialfv(GL_FRONT_AND_BACK, type_name, k);
    }
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

void Draw::set_faces(Faces*& faces) {
    this->faces = faces;
}
