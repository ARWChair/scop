#include <array>
#include <vector>
#include <iostream>
#include <exception>
#pragma once

typedef struct inner_elements {
    std::vector<std::array<double, 3>> verts;
    std::vector<std::array<double, 3>> uvs;
    std::vector<std::array<double, 2>> normals;
}               inner_elements;

typedef struct obj_node {
    std::vector<inner_elements> elements;
    obj_node *next;
}               obj_node;

class Faces_tree {
    public:
        Faces_tree();
        Faces_tree(std::vector<std::array<double, 3>>& v, std::vector<std::array<double, 3>>& vn, std::vector<std::array<double, 2>>& vt);
        ~Faces_tree();

        obj_node* create_list(std::vector<std::array<double, 3>>&, std::vector<std::array<double, 3>>&, std::vector<std::array<double, 2>>&);
        void add_elements(obj_node *, std::vector<std::array<double, 3>>&, std::vector<std::array<double, 3>>&, std::vector<std::array<double, 2>>&);
        void add_line(obj_node *);
        void print_list();
        void clear_line(obj_node *);
        void clear_list();
        obj_node* get_list();

        void set_v(std::vector<std::array<double, 3>>&);
        void set_v(std::vector<std::array<double, 3>>&, obj_node *&);
        void set_vn(std::vector<std::array<double, 2>>&);
        void set_vn(std::vector<std::array<double, 2>>&, obj_node *&);
        void set_vt(std::vector<std::array<double, 3>>&);
        void set_vt(std::vector<std::array<double, 3>>&, obj_node *&);

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
        // void clear_tree_nodes();
        obj_node* list;
};