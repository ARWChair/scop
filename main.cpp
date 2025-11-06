#include "scop/scop.hpp"

int main() {
    // try {
    //     Scop_window window = Scop_window(200, 200, 1200, 800);
    //     Scop_openGL opengl = Scop_openGL(&window, window.get_display(), 0);
    //     opengl.create_viewport();
    //     Draw my_draw = Draw(opengl, window);
    //     Faces faces = Faces("../scop/data/resources/teapot2.obj");
    //     window.set_openGL(&opengl);
    //     window.set_drawer(&my_draw);
    //     window.set_faces(&faces);
    //     opengl.make_current((GLXDrawable)window.get_window());
    //     window.hold_open();
    // } catch (const std::exception& e) {
    //     std::cout << e.what() << std::endl;
    // }
    // Faces faces = Faces("../scop/data/resources/teapot2.obj");
    // (void)filename;

    std::array<double, 3> va = {1.000, 2.101, 3.0001};
    std::array<double, 2> vna = {0.49494, 4.29292};
    std::array<double, 3> vta = {2.0003, 2.34545, 2.3434};
    std::vector<std::array<double, 3>> v;
    std::vector<std::array<double, 2>> vn;
    std::vector<std::array<double, 3>> vt;
    v.push_back(va);
    vn.push_back(vna);
    vt.push_back(vta);

    Faces_tree tree = Faces_tree(v, vn, vt);
    
    obj_node* b1 = tree.create_branch(v, vn, vt);
    obj_node* b2 = tree.create_branch(v, vn, vt);
    obj_node* b3 = tree.create_branch(v, vn, vt);
    std::vector<obj_node *> children;
    tree.add_children(b1);
    tree.add_children(b2);
    tree.add_children(b3);
    tree.print_tree();
}
