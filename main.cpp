#include "scop/scop.hpp"

int main() {
    try {
        Scop_window *window = new Scop_window(200, 200, 1200, 800);
        if (!window) {
            std::cout << "Error. Window allocation failed" << std::endl;
            return 1;
        }
        Scop_openGL *opengl = new Scop_openGL(window, window->get_display(), 0);
        if (!opengl) {
            std::cout << "Error. OpenGL allocation failed" << std::endl;
            delete window;
            return 1;
        }
        opengl->create_viewport();
        Draw *my_draw = new Draw(*opengl, *window);
        if (!my_draw) {
            std::cout << "Error. Draw allocation failed" << std::endl;
            delete opengl;
            delete window;
            return 1;
        }
        Faces *faces = new Faces("../scop/data/resources/test-cube.obj");
        if (!faces) {
            std::cout << "Error. Draw allocation failed" << std::endl;
            delete my_draw;
            delete opengl;
            delete window;
            return 1;
        }
        window->set_openGL(opengl);
        window->set_drawer(my_draw);
        window->set_faces(faces);
        opengl->make_current((GLXDrawable)window->get_window());
        window->hold_open();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}
