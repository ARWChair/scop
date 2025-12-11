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
    Material(std::string filename, Faces *&faces, bool state);
    ~Material();

    void load_entities();
    void split();
    void set_filename(std::string &filename);
    const std::string &get_filename() const;
    const std::vector<std::string> &get_lines() const;
    const std::array<double, 3> &get_Kd(std::string &name) const;
    const std::array<double, 3> &get_Ka(std::string &name) const;
    const std::array<double, 3> &get_Ks(std::string &name) const;
    const std::array<double, 3> &get_Ke(std::string &name) const;
    const std::array<double, 3> &get_Tf(std::string &name) const;
    const std::array<double, 1> &get_Ns(std::string &name) const;
    const std::array<double, 1> &get_Ni(std::string &name) const;
    const std::array<double, 1> &get_d(std::string &name) const;
    const std::array<double, 1> &get_Tr(std::string &name) const;
    const std::array<double, 1> &get_sharpness(std::string &name) const;
    const std::array<int, 1> &get_illum(std::string &name) const;
    const std::string &get_map_Kd(std::string &name) const;
    const bool &is_missing() const;
private:
    void create_mtl_file();
    void check_and_fill();

    std::vector<Materials> materials;
    std::string filename;
    std::vector<std::string> lines;
    bool missing;
    Faces *faces;
};