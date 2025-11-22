#include "Material.hpp"

Material::Material(std::string filename) {
    filename += ".mtl";
    set_filename(filename);
    load_entities();
    split();
}

Material::~Material() {}

void Material::load_entities() {
    std::ifstream s(filename);
    std::string line;
    std::string temp;

    while (std::getline(s, line)) {
        std::stringstream ss(line);
        ss >> temp;
        if (temp != "#") {
            lines.push_back(line);
        }
    }
}

std::string get_map_kd(std::string &line) {
    std::stringstream ss(line);
    std::string l;

    ss >> l;
    ss >> l;
    return l;
}

std::array<double, 1> single_array_double(std::string &line) {
    std::array<double, 1> returning;
    std::stringstream ss(line);
    std::string l;

    ss >> l;
    ss >> l;
    returning[0] = std::stod(l);
    return returning;
}

std::array<int, 1> single_array_int(std::string &line) {
    std::array<int, 1> returning;
    std::stringstream ss(line);
    std::string l;

    ss >> l;
    ss >> l;
    returning[0] = std::stod(l);
    return returning;
}

std::array<double, 3> split_in_array(std::string &line) {
    std::array<double, 3> returning;
    std::stringstream ss(line);
    std::string l;

    ss >> l;
    ss >> l;
    returning[0] = std::stod(l);
    ss >> l;
    returning[1] = std::stod(l);
    ss >> l;
    returning[2] = std::stod(l);
    return returning;
}

void Material::split() {
    std::string iden;
    std::string line;

    for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++) {
        std::stringstream ss(*it);
        ss >> iden;
        ss >> line;
        if (iden == "Kd")
            Kd = split_in_array(line);
        else if (iden == "Ka")
            Ka = split_in_array(line);
        else if (iden == "Kd")
            Kd = split_in_array(line);
        else if (iden == "Ks")
            Ks = split_in_array(line);
        else if (iden == "Ke")
            Ke = split_in_array(line);
        else if (iden == "Tf")
            Tf = split_in_array(line);
        else if (iden == "Ns")
            Ns = single_array_double(line);
        else if (iden == "Ni")
            Ni = single_array_double(line);
        else if (iden == "d")
            d = single_array_double(line);
        else if (iden == "Tr")
            Tr = single_array_double(line);
        else if (iden == "sharpness")
            sharpness = single_array_double(line);
        else if (iden == "illum")
            illum = single_array_int(line);
        else if (iden == "map_Kd")
            map_Kd = get_map_kd(line);
    }
}

void Material::set_filename(std::string &filename) {
    this->filename = filename;
}

const std::string &Material::get_filename() const {
    return filename;
}

const std::vector<std::string> &Material::get_lines() const {
    return lines;
}

const std::array<double, 3> &Material::get_Kd() const {
    return Kd;
}

const std::array<double, 3> &Material::get_Ka() const {
    return Ka;
}

const std::array<double, 3> &Material::get_Ks() const {
    return Ks;
}

const std::array<double, 3> &Material::get_Ke() const {
    return Ke;
}

const std::array<double, 3> &Material::get_Tf() const {
    return Tf;
}

const std::array<double, 1> &Material::get_Ns() const {
    return Ns;
}

const std::array<double, 1> &Material::get_Ni() const {
    return Ni;
}

const std::array<double, 1> &Material::get_d() const {
    return d;
}

const std::array<double, 1> &Material::get_Tr() const {
    return Tr;
}

const std::array<double, 1> &Material::get_sharpness() const {
    return sharpness;
}

const std::array<int, 1> &Material::get_illum() const {
    return illum;
}

const std::string &Material::get_map_Kd() const {
    return map_Kd;
}