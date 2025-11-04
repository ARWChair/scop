#include "scop/scop.hpp"

int main() {
    try {
        Scop_window window = Scop_window(200, 200, 1200, 800);
        Scop_openGL opengl = Scop_openGL(&window, window.get_display(), 0);
        opengl.create_viewport();
        Draw my_draw = Draw(opengl, window);
        Faces faces = Faces("/home/yannis/Desktop/scop/data/resources/teapot.obj");
        window.set_openGL(&opengl);
        window.set_drawer(&my_draw);
        window.set_faces(&faces);
        opengl.make_current((GLXDrawable)window.get_window());
        std::cout << "GL_RENDERER: " << (const char*)glGetString(GL_RENDERER) << "\n";
        std::cout << "GL_VENDOR: "   << (const char*)glGetString(GL_VENDOR)   << "\n";
        std::cout << "GL_VERSION: "  << (const char*)glGetString(GL_VERSION)  << "\n";
        window.hold_open();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}
