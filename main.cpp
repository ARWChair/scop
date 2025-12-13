#include "./scop/Draw/Draw.hpp"
#include "./scop/Window/Window.hpp"
#include "./scop/OpenGL/OpenGL.hpp"
#include "./scop/Faces/Faces.hpp"
#include "./scop/Material/Material.hpp"
#include "./scop/BMP/BMP.hpp"

int main(int argc, char **argv) {
    Scop_window *window = nullptr;
    Scop_openGL *opengl = nullptr;
    Draw *draw = nullptr;
    Faces *faces = nullptr;
    Material *material = nullptr;
    BMP *bmp = nullptr;

    if (argc != 2) {
        std::cerr << "Input filename to start" << std::endl;
        return 1;
    }
    try {
        window = new Scop_window(200, 200, 1200, 800);
        if (window == nullptr) {
            std::cerr << "Error. Window allocation failed" << std::endl;
            return 1;
        }
        opengl = new Scop_openGL(window, window->get_display(), 0);
        if (opengl == nullptr) {
            std::cerr << "Error. OpenGL allocation failed" << std::endl;
            goto delete_all_error;
        }
        opengl->create_viewport();
        draw = new Draw(*opengl, *window);
        if (draw == nullptr) {
            std::cerr << "Error. Draw allocation failed" << std::endl;
            goto delete_all_error;
        }
        faces = new Faces(argv[1]);
        if (faces == nullptr) {
            std::cerr << "Error. Draw allocation failed" << std::endl;
            goto delete_all_error;
        }
        draw->set_faces(faces);
        std::string arg = argv[1];
        size_t pos = arg.find_last_of('/');
        arg.erase(pos + 1, arg.length());
        std::string material_name = faces->get_material_file_name();
        bool state = faces->is_missing();
        if (material_name.length() == 0)
            material = new Material("", faces, state);
        else
            material = new Material(arg + material_name, faces, state);
        if (material == nullptr) {
            std::cerr << "Error. Material allocation failed" << std::endl;
            goto delete_all_error;
        }
        std::string name = faces->get_material_from_file();
        if (name.length() != 0) {
            name = material->get_map_Kd(name);
            bmp = new BMP(arg, name);
            if (bmp == nullptr) {
                std::cerr << "Error. BMP allocation failed" << std::endl;
                goto delete_all_error;
            }
            window->set_bmp(bmp);
        }
        window->set_openGL(opengl);
        window->set_drawer(draw);
        window->set_faces(faces);
        window->set_material(material);
        opengl->make_current((GLXDrawable)window->get_window());
        window->hold_open();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        goto delete_all_error;
    }
    delete bmp;
    delete material;
    delete faces;
    delete draw;
    delete opengl;
    delete window;
    return 0;

    delete_all_error:
        if (bmp != nullptr)
            delete bmp;
        if (material != nullptr)
            delete material;
        if (faces != nullptr)
            delete faces;
        if (draw != nullptr)
            delete draw;
        if (opengl != nullptr)
            delete opengl;
        if (window != nullptr)
            delete window;
        return 1;
}
