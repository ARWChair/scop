#include <exception>
#include <GL/gl.h>
#include <fstream>   
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <array>
#pragma once

class Faces {
    public:
        Faces(std::string filename);
        ~Faces();

        int load_lanes_from_obj();
        int split_faces_from_lines();
        int load_faces_from_array(int pos);
        const std::vector<std::string>& get_lines() const;
        const std::vector<std::array<std::array<double, 3>, 3>>& get_faces() const;

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
        std::vector<std::array<std::array<double, 3>, 3>> faces;
        std::vector<std::string> lines;
        std::string filecontent;
        std::string filename;
};

// Read whole file
// Get line by line
// Save in faces and verts
// Display first triangle
// try do draw it in 3d space