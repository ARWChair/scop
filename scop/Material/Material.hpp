#include <array>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#pragma once

typedef struct Materials {
    std::string name;
    std::vector<std::string> lines;
    std::array<double, 3> Kd;
    std::array<double, 3> Ka;
    std::array<double, 3> Ks;
    std::array<double, 3> Ke;
    std::array<double, 3> Tf;
    std::array<double, 1> Ns;
    std::array<double, 1> Ni;
    std::array<double, 1> d;
    std::array<double, 1> Tr;
    std::array<double, 1> sharpness;
    std::array<int, 1> illum;
    std::string map_Kd;
}               Materials;

class Faces;

class Material {
public:
    Material(std::string, Faces*&, bool);
    ~Material();

    void load_entities();
    void split();
    void set_filename(std::string);
    std::array<double, 3> get_Kd(std::string);
    std::array<double, 3> get_Ka(std::string);
    std::array<double, 3> get_Ks(std::string);
    std::array<double, 3> get_Ke(std::string);
    std::array<double, 3> get_Tf(std::string);
    std::array<double, 1> get_Ns(std::string);
    std::array<double, 1> get_Ni(std::string);
    std::array<double, 1> get_d(std::string);
    std::array<double, 1> get_Tr(std::string);
    std::array<double, 1> get_sharpness(std::string);
    std::array<int, 1> get_illum(std::string);
    std::string get_map_Kd(std::string);
    bool is_missing();
private:
    void create_mtl_file();
    void check_and_fill();

    std::vector<Materials> materials;
    std::string filename;
    std::vector<std::string> lines;
    bool missing;
    Faces *faces;
};