#include <exception>
#include <GL/gl.h>
#include "../Faces_tree/Faces_tree.hpp"
#include <fstream>   
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <array>
#pragma once

typedef struct v_vn_vt {
    std::vector<std::vector<double>> v_full;
    std::vector<std::vector<double>> vt_full;
    std::vector<std::vector<double>> vn_full;
}               v_vn_vt;

typedef struct v_vn_vt_layer {
    std::vector<double> v_full;
    std::vector<double> vt_full;
    std::vector<double> vn_full;
}               v_vn_vt_layer;

class Faces {
    public:
        Faces(std::string filename);
        ~Faces();

        int load_lanes_from_obj();
        v_vn_vt *split_parts();
        std::vector<std::vector<std::string>> get_faces_indexes();
        int split_in_tree(v_vn_vt *&, std::vector<std::vector<std::string>> &);
        const std::vector<std::string>& get_lines() const;
        const Faces_tree& get_list() const;
        // ---------- Exception ---------- \\'
        class LoadException: public std::exception {
            public:
                virtual const char* what() const throw() {
                    return "Error while loading .obj file";
                }
        };
        class SplitException: public std::exception {
            public:
                virtual const char* what() const throw() {
                    return "Error while splitting .obj file";
                }
        };
        class ArrangeException: public std::exception {
            public:
                virtual const char* what() const throw() {
                    return "Error while arranging faces from .obj file";
                }
        };
    private:
        std::vector<std::array<double, 3>> verts;
        std::vector<std::string> lines;
        std::string filecontent;
        std::string filename;
        Faces_tree list;
};

// Read whole file
// Get line by line
// Save in faces and verts
// Display first triangle
// try do draw it in 3d space