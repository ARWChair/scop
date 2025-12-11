#include "Material.hpp"
#include "../Faces/Faces.hpp"

Material::Material(std::string filename, Faces *&faces, bool state) {
    if (filename.length() == 0) {
        missing = true;
    } else if (state == true) {
        missing = true;
    } else {
        this->faces = faces;
        set_filename(filename);
        load_entities();
        if (missing == true) {
            create_mtl_file();
            load_entities();
        }
        split();
        check_and_fill();
    }
}

Material::~Material() {}

void Material::load_entities() {
    std::ifstream s(filename);
    std::string line;
    std::string temp;

    if (!s.good()) {
        missing = true;
        return;
    }
    while (std::getline(s, line)) {
        std::stringstream ss(line);
        ss >> temp;
        if (temp != "#") {
            lines.push_back(line);
        }
    }
    missing = false;
}

void Material::check_and_fill() {
    std::vector<Materials> temp = materials;
    std::vector<Materials>::iterator it = temp.begin();
    bool found = false;

    for (; it != temp.end(); it++) {
        if (it->name == faces->get_material_from_file()) {
            found = true;
            break;
        }
    }
    if (found == true)
        return;
    std::ofstream file(filename, std::ios::app);
    file << "\nnewmtl " << faces->get_material_from_file() << "\n";
    file << "Ka 0.200000 0.200000 0.200000\n";
    file << "Kd 0.800000 0.800000 0.800000\n";
    file << "Ks 0.000000 0.000000 0.000000\n";
    file << "Ns 0.000000\n";
    file << "d 1.000000\n";
    file.close();

    lines.clear();
    materials.clear();
    load_entities();
    split();
}

void Material::create_mtl_file() {
    std::ofstream file(filename);
    if (faces->get_material_from_file().length() == 0)
        file << "newmtl Material\n";
    else
        file << "newmtl " << faces->get_material_from_file() << "\n";

    file << "Ka 0.200000 0.200000 0.200000\n";
    file << "Kd 0.800000 0.800000 0.800000\n";
    file << "Ks 0.000000 0.000000 0.000000\n";
    file << "Ns 0.000000\n";
    file << "d 1.000000\n";
    file.close();
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
    bool begin = false;

    Materials material;
    for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++) {
        std::stringstream ss(*it);
        ss >> iden;
        if (iden == "newmtl") {
            if (begin == true)
                materials.push_back(material);
            ss >> line;
            material.name = line;
            begin = true;
        }
        std::getline(ss, line);
        if (iden == "Kd")
            material.Kd = split_in_array(line);
        else if (iden == "Ka")
            material.Ka = split_in_array(line);
        else if (iden == "Kd")
            material.Kd = split_in_array(line);
        else if (iden == "Ks")
            material.Ks = split_in_array(line);
        else if (iden == "Ke")
            material.Ke = split_in_array(line);
        else if (iden == "Tf")
            material.Tf = split_in_array(line);
        else if (iden == "Ns")
            material.Ns = single_array_double(line);
        else if (iden == "Ni")
            material.Ni = single_array_double(line);
        else if (iden == "d")
            material.d = single_array_double(line);
        else if (iden == "Tr")
            material.Tr = single_array_double(line);
        else if (iden == "sharpness")
            material.sharpness = single_array_double(line);
        else if (iden == "illum")
            material.illum = single_array_int(line);
        else if (iden == "map_Kd")
            material.map_Kd = get_map_kd(line);
        line = "";
        iden = "";
    }
    materials.push_back(material);
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

const std::array<double, 3> &Material::get_Kd(std::string &name) const {
    std::vector<Materials> temp = materials;
    std::vector<Materials>::iterator it = temp.begin();
    for (; it != materials.end(); it++)
        if (it->name == name)
            break;
    return it->Kd;
}

const std::array<double, 3> &Material::get_Ka(std::string &name) const {
    std::vector<Materials> temp = materials;
    std::vector<Materials>::iterator it = temp.begin();
    for (; it != materials.end(); it++)
        if (it->name == name)
            break;
    return it->Ka;
}

const std::array<double, 3> &Material::get_Ks(std::string &name) const {
    std::vector<Materials> temp = materials;
    std::vector<Materials>::iterator it = temp.begin();
    for (; it != materials.end(); it++)
        if (it->name == name)
            break;
    return it->Ks;
}

const std::array<double, 3> &Material::get_Ke(std::string &name) const {
    std::vector<Materials> temp = materials;
    std::vector<Materials>::iterator it = temp.begin();
    for (; it != materials.end(); it++)
        if (it->name == name)
            break;
    return it->Ke;
}

const std::array<double, 3> &Material::get_Tf(std::string &name) const {
    std::vector<Materials> temp = materials;
    std::vector<Materials>::iterator it = temp.begin();
    for (; it != materials.end(); it++)
        if (it->name == name)
            break;
    return it->Tf;
}

const std::array<double, 1> &Material::get_Ns(std::string &name) const {
    std::vector<Materials> temp = materials;
    std::vector<Materials>::iterator it = temp.begin();
    for (; it != materials.end(); it++)
        if (it->name == name)
            break;
    return it->Ns;
}

const std::array<double, 1> &Material::get_Ni(std::string &name) const {
    std::vector<Materials> temp = materials;
    std::vector<Materials>::iterator it = temp.begin();
    for (; it != materials.end(); it++)
        if (it->name == name)
            break;
return it->Ni;
}

const std::array<double, 1> &Material::get_d(std::string &name) const {
    std::vector<Materials> temp = materials;
    std::vector<Materials>::iterator it = temp.begin();
    for (; it != materials.end(); it++)
        if (it->name == name)
            break;
    return it->d;
}

const std::array<double, 1> &Material::get_Tr(std::string &name) const {
    std::vector<Materials> temp = materials;
    std::vector<Materials>::iterator it = temp.begin();
    for (; it != materials.end(); it++)
        if (it->name == name)
            break;
    return it->Tr;
}

const std::array<double, 1> &Material::get_sharpness(std::string &name) const {
    std::vector<Materials> temp = materials;
    std::vector<Materials>::iterator it = temp.begin();
    for (; it != materials.end(); it++)
        if (it->name == name)
            break;
    return it->sharpness;
}

const std::array<int, 1> &Material::get_illum(std::string &name) const {
    std::vector<Materials> temp = materials;
    std::vector<Materials>::iterator it = temp.begin();
    for (; it != materials.end(); it++)
        if (it->name == name)
            break;
    return it->illum;
}

const std::string &Material::get_map_Kd(std::string &name) const {
    std::vector<Materials> temp = materials;
    std::vector<Materials>::iterator it = temp.begin();
    for (; it != materials.end(); it++)
        if (it->name == name)
            break;
    return it->map_Kd;
}

const bool &Material::is_missing() const {
    return missing;
}