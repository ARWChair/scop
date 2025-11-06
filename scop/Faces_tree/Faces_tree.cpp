#include "Faces_tree.hpp"

Faces_tree::Faces_tree(std::vector<std::array<double, 3>>& v, std::vector<std::array<double, 2>>& vn, std::vector<std::array<double, 3>>& vt) {
    tree = create_branch(v, vn, vt);
    if (!tree)
        throw TreeCreateException();
}

Faces_tree::~Faces_tree() {}

obj_node *Faces_tree::create_branch(std::vector<std::array<double, 3>> v, std::vector<std::array<double, 2>> vn, std::vector<std::array<double, 3>> vt) {
    obj_node *root = new obj_node();
    if (root) {
        root->verts = v;
        root->uvs = vn;
        root->normals = vt;
        root->children = NULL;
    }
    return root;
}

void Faces_tree::add_children(obj_node *base, obj_node *children) {
    if (!base || !children)
        return;
    if (base->children)
        base->children->push_back(children);
    base->children = new std::vector<obj_node *>();
    if (!base->children)
        throw ChildrenCreateException();
    base->children->push_back(children);
}

void Faces_tree::add_children(obj_node *children) {
    if (!children)
        return;
    if (!tree->children)
        tree->children->push_back(children);
    tree->children = new std::vector<obj_node *>();
    if (!tree->children)
        throw ChildrenCreateException();
    tree->children->push_back(children);
}

void print_obj(obj_node *node, int depth) {
    if (!node)
        return;
    for (int tabs = 0; tabs < depth; tabs++)
        std::cout << "\t";
    std::cout << "Verts: ";
    for (std::vector<std::array<double, 3>>::iterator it = node->verts.begin(); it != node->verts.end(); it++) {
        std::cout << (*it)[0] << ", " << (*it)[1] << ", " << (*it)[2] << "\n";
    }
    for (int tabs = 0; tabs < depth; tabs++)
        std::cout << "\t";
    std::cout << "UVs: ";
    for (std::vector<std::array<double, 2>>::iterator it = node->uvs.begin(); it != node->uvs.end(); it++) {
        std::cout << (*it)[0] << ", " << (*it)[1] << "\n";
    }
    for (int tabs = 0; tabs < depth; tabs++)
        std::cout << "\t";
    std::cout << "Normals: ";
    for (std::vector<std::array<double, 3>>::iterator it = node->normals.begin(); it != node->normals.end(); it++) {
        std::cout << (*it)[0] << ", " << (*it)[1] << ", " << (*it)[2] << "\n";
    }
    if (!node->children)
        return;
    int pos = 0;
    for (std::vector<obj_node *>::iterator it = node->children->begin(); it != node->children->end(); it++, pos++) {
        for (int tabs = 0; tabs < depth + 1; tabs++)
            std::cout << "\t";
        std::cout << "Child: " << pos << "\n";
        print_obj(*it, depth + 1);
        for (int tabs = 0; tabs < depth + 1; tabs++)
            std::cout << "\t";
        std::cout << "-----" << "\n";
    }
    std::cout << std::endl;
}

void Faces_tree::print_tree() {
    print_obj(tree, 0);
}

obj_node *Faces_tree::get_tree() {
    return tree;
}