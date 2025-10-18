#include "scop/scop.hpp"

int main() {
    try {
        Scop_window window = Scop_window(200, 200, 800, 600);
        Scop_openGL opengl = Scop_openGL(window.get_display(), 0);
        opengl.make_current((GLXDrawable)window.get_window());
        opengl.draw_box();
        window.hold_open();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}
