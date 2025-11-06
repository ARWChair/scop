#include <array>
#include <vector>
#include <iostream>
#include <exception>
#pragma once

typedef struct obj_node {
    std::vector<std::array<double, 3>> verts;
    std::vector<std::array<double, 2>> uvs;
    std::vector<std::array<double, 3>> normals;
    std::vector<obj_node *> *children;
}               obj_node;

class Faces_tree {
    public:
        Faces_tree(std::vector<std::array<double, 3>>& v, std::vector<std::array<double, 2>>& vn, std::vector<std::array<double, 3>>& vt);
        ~Faces_tree();

        obj_node* create_branch(std::vector<std::array<double, 3>> v, std::vector<std::array<double, 2>> vn, std::vector<std::array<double, 3>> vt);
        void add_children(obj_node *);
        void add_children(obj_node *, obj_node *);
        void print_tree();
        void remove_children();
        void clear_tree();
        obj_node* get_tree();
        // ---------- exceptions ---------- \\'
        class TreeCreateException: public std::exception {
            public:
                virtual const char *what() const throw() {
                    return "Error while creating tree";
                }
        };
        class ChildrenCreateException: public std::exception {
            public:
                virtual const char *what() const throw() {
                    return "Error while creating children";
                }
        };
    private:
        obj_node* tree;
};