#include "Faces.hpp"

Faces::Faces(std::string filename) {
    // (void)filename;

    // std::array<double, 3> va = {1.000, 2.101, 3.0001};
    // std::array<double, 2> vna = {0.49494, 4.29292};
    // std::array<double, 3> vta = {2.0003, 2.34545, 2.3434};
    // std::vector<std::array<double, 3>> v;
    // std::vector<std::array<double, 2>> vn;
    // std::vector<std::array<double, 3>> vt;
    // v.push_back(va);
    // vn.push_back(vna);
    // vt.push_back(vta);
    // obj_node* tree = create_obj_tree(v, vn, vt);
    // obj_node* b1 = create_obj_tree(v, vn, vt);
    // obj_node* b2 = create_obj_tree(v, vn, vt);
    // obj_node* b3 = create_obj_tree(v, vn, vt);
    // std::vector<obj_node *> children;
    // children.push_back(b1);
    // children.push_back(b2);
    // children.push_back(b3);
    // add_nodes(tree, &children);
    // print_obj_tree(tree);
    (void)filename;
}

Faces::~Faces() {}


const std::vector<std::array<std::array<double, 3>, 3>>& Faces::get_faces() const {
    return faces;
}
