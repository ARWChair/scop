#include "../../GL/glew.h"
#include <exception>
#include <fstream>   
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <cfloat>
#include <array>
#pragma once

typedef struct inner_elements {
    std::vector<unsigned int> v;
    std::vector<unsigned int> vn;
    std::vector<unsigned int> vt;
}               inner_elements;

typedef struct v_vn_vt {
    std::vector<std::vector<double>> v_full;
    std::vector<std::vector<double>> vt_full;
    std::vector<std::vector<double>> vn_full;
}               v_vn_vt;

typedef struct flat {
    std::vector<GLfloat> vertexes;
    std::vector<GLfloat> textures;
    std::vector<GLfloat> normals;
}               flat;

typedef struct flat_indices {
    std::vector<unsigned int> vertexes_indice;
    std::vector<unsigned int> textures_indice;
    std::vector<unsigned int> normals_indice;
}               flat_indices;


class Faces {
    public:
        Faces(std::string filename);
        ~Faces();

        std::vector<std::vector<std::string>> get_faces_indexes();
        const std::vector<std::string>& get_lines() const;
        const std::string& get_material_file_name() const;
        const std::string& get_material_from_file() const;
        const std::vector<inner_elements>& get_list() const;
        const flat_indices& get_indices() const;
        std::vector<inner_elements> triangulate(std::vector<std::vector<std::string>>&);
        const flat& get_flattened() const;
        const bool& is_missing() const;
        const int& get_amount() const;
        const double& get_scale() const;
        void calculate_scale(std::vector<GLfloat>&, int);
        void inc_scale(double value);
        void dec_scale(double value);
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
        flat flatten_faces(v_vn_vt*&);
        inner_elements split_in_elems(std::vector<std::string>&);
        int load_lanes_from_obj();
        v_vn_vt *split_parts();
        void save_material_file_name();

        std::vector<std::string> lines;
        std::string material_from_file;
        std::string material_file_name;
        std::string filecontent;
        std::string filename;
        std::vector<inner_elements> list;
        flat_indices indices;
        flat flattened;
        double scale;
        bool missing;
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
void reallign_highest_point(std::vector<GLfloat> &faces, int id, int stride = 3);