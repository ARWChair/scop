#include "Faces.hpp"

Faces::Faces(std::string filename) {
    this->filename = filename;
    if (load_lanes_from_obj() == -1)
        throw LoadException();
    v_vn_vt* elements = split_parts();
    if (!elements)
        throw SplitException();
    std::vector<std::vector<std::string>> face_indexes = get_faces_indexes();
    split_in_tree(elements, face_indexes);
}

Faces::~Faces() {}

int Faces::load_lanes_from_obj() {
    std::ifstream file(filename);
    if (!file.is_open())
        return -1;
    std::string line;
    while (std::getline(file, line))
        lines.push_back(line);
    file.close();
    return 0;
}

v_vn_vt *Faces::split_parts() {
    std::vector<double> v;
    std::vector<double> vn;
    std::vector<double> vt;
    std::string l, iden;
    v_vn_vt *elements = new v_vn_vt();

    if (!elements)
        return NULL;
    for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++) {
        std::istringstream ss(*it);
        ss >> l;
        iden = l;
        while (ss >> l) {
            if (iden == "v")
                v.push_back(std::stod(l));
            else if (iden == "vn")
                vn.push_back(std::stod(l));
            else if (iden == "vt")
                vt.push_back(std::stod(l));
        }
        if (v.size() != 0) {
            elements->v_full.push_back(v);
            v.clear();
        } else if (vn.size() != 0) {
            elements->vn_full.push_back(vn);
            vt.clear();
        } else if (vt.size() != 0) {
            elements->vn_full.push_back(vt);
            vt.clear();
        }
    }
    return elements;
}

std::vector<std::vector<std::string>> Faces::get_faces_indexes() {
    std::vector<std::vector<std::string>> f_elements;
    std::vector<std::string> row;
    std::string iden, l;

    for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++) {
        std::istringstream ss(*it);
        ss >> l;
        iden = l;
        if (iden != "f")
            continue;
        while (ss >> l) {
            row.push_back(l);
            std::cout << l << ", ";
        }
        std::cout << std::endl;
        f_elements.push_back(row);
        row.clear();
    }
    std::cout << "Size: " << f_elements.size() << std::endl;
    return f_elements;
}

int Faces::split_in_tree(v_vn_vt *& elements, std::vector<std::vector<std::string>>& face_indexes) {
    for (std::vector<std::vector<std::string>>::iterator it = face_indexes.begin(); it != face_indexes.end(); it++) {
        
    }
    delete elements;
    return 0;
}

const std::vector<std::string>& Faces::get_lines() const {
    return lines;
}

const Faces_tree& Faces::get_faces() const {
    return tree;
}
