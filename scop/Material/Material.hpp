#include <array>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#pragma once

class Material {
public:
    Material(std::string filename);
    ~Material();

    void load_entities();
    void split();
    void set_filename(std::string &filename);
    const std::string &get_filename() const;
    const std::vector<std::string> &get_lines() const;
    const std::array<double, 3> &get_Kd() const;
    const std::array<double, 3> &get_Ka() const;
    const std::array<double, 3> &get_Ks() const;
    const std::array<double, 3> &get_Ke() const;
    const std::array<double, 3> &get_Tf() const;
    const std::array<double, 1> &get_Ns() const;
    const std::array<double, 1> &get_Ni() const;
    const std::array<double, 1> &get_d() const;
    const std::array<double, 1> &get_Tr() const;
    const std::array<double, 1> &get_sharpness() const;
    const std::array<int, 1> &get_illum() const;
    const std::string &get_map_Kd() const;
private:
    std::string filename;
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
};