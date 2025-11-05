#include "scop/scop.hpp"

int main() {
    try {
        Scop_window window = Scop_window(200, 200, 1200, 800);
        Scop_openGL opengl = Scop_openGL(&window, window.get_display(), 0);
        opengl.create_viewport();
        Draw my_draw = Draw(opengl, window);
        Faces faces = Faces("../scop/data/resources/teapot2.obj");
        window.set_openGL(&opengl);
        window.set_drawer(&my_draw);
        window.set_faces(&faces);
        opengl.make_current((GLXDrawable)window.get_window());
        window.hold_open();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}
