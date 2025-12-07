#include <exception>
#include <fstream>   
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <cfloat>
#include <array>
#pragma once

typedef struct inner_elements {
    std::vector<std::array<double, 3>> v;
    std::vector<std::array<double, 3>> vn;
    std::vector<std::array<double, 2>> vt;
}               inner_elements;

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

        std::vector<std::vector<std::string>> get_faces_indexes();
        const std::vector<std::string>& get_lines() const;
        const std::string& get_material_file_name() const;
        const std::string& get_material_from_file() const;
        const std::vector<inner_elements>& get_list() const;
        const std::vector<std::vector<unsigned int>>& get_indices() const;
        const int& get_amount() const;
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
        class ListNewException: public std::exception {
            public:
                virtual const char* what() const throw() {
                    return "Error creating list";
                }
        };
    private:
        int load_lanes_from_obj();
        v_vn_vt *split_parts();
        int split_in_tree(v_vn_vt *&, std::vector<std::vector<std::string>> &);
        void save_material_file_name();
        void save_faces_indices(std::vector<std::vector<std::string>>&);

        std::vector<std::string> lines;
        std::string material_from_file;
        std::string material_file_name;
        std::string filecontent;
        std::string filename;
        std::vector<inner_elements> list;
        std::vector<std::vector<unsigned int>> indices;
        int amount;
};

// Read whole file
// Get line by line
// Save in faces and v
// Display first triangle
// try do draw it in 3d space

std::vector<unsigned int> split_and_group(std::vector<std::vector<std::array<double, 3>>> &faces, std::vector<std::vector<unsigned int>> indices);
std::vector<std::vector<std::array<double, 3>>> split_and_group(std::vector<std::vector<std::array<double, 3>>> &faces);
std::vector<std::vector<std::array<double, 2>>> split_and_group(std::vector<std::vector<std::array<double, 2>>> &faces);
void reallign_highest_point(std::vector<std::vector<std::array< double, 3>>> &faces, int id);