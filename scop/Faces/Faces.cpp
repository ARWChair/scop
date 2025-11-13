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
        if (iden != "v" && iden != "vn" && iden != "vt")
            continue;
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
            vn.clear();
        } else if (vt.size() != 0) {
            elements->vt_full.push_back(vt);
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
        while (ss >> l)
            row.push_back(l);
        f_elements.push_back(row);
        row.clear();
    }
    return f_elements;
}

std::vector<std::string> split(std::string &str, char c) {
    std::vector<std::string> returned;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, c)) {
        returned.push_back(token);
    }
    return returned;
}

v_vn_vt_layer check_and_split(std::vector<std::string>& row) {
    v_vn_vt_layer temp;
    std::vector<std::string> check;
    std::vector<std::string>::iterator inner;

    for (std::vector<std::string>::iterator it = row.begin(); it != row.end(); it++) {
        check = split(*it, '/');
        inner = check.begin();
        temp.v_full.push_back(std::stod(*inner));
        if (check.size() == 1)
            continue;
        inner++;
        if (inner->length() != 0)
            temp.vt_full.push_back(std::stod(*inner));
        inner++;
        if (inner->length() != 0)
            temp.vn_full.push_back(std::stod(*inner));
    }
    return temp;
}

inner_elements get_indices_verts_normals(v_vn_vt_layer& row, v_vn_vt*& elements) {
    inner_elements elem;
    // std::array<double, 3> v;
    // std::array<double, 3> vt;
    // std::array<double, 2> vn;

    for (std::vector<std::vector<double>>::iterator it = elements->vn_full.begin(); it != elements->vn_full.end(); it++) {
        for (std::vector<double>::iterator inner = it->begin(); inner != it->end(); inner++)
            std::cout << *inner << ", ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
    for (std::vector<double>::iterator it = row.v_full.begin(); it != row.v_full.end(); it++) {
        std::cout << *it << ", " << elements->v_full.begin()->front() << std::endl;
    }
    std::cout << *row.v_full.begin() << ", " << row.v_full.size() << std::endl;
    return elem;
}

int Faces::split_in_tree(v_vn_vt*& elements, std::vector<std::vector<std::string>>& face_indexes) {
    inner_elements elems;
    list = Faces_tree();
    int i = 0;

    for (std::vector<std::vector<std::string>>::iterator it = face_indexes.begin(); it != face_indexes.end(); it++, i++) {
        v_vn_vt_layer row = check_and_split(*it);
        elems = get_indices_verts_normals(row, elements);
        if (i == 0) {
            // list.set_v(row.v_full);
        }
        std::cout << row.v_full.size() << ", " << row.vn_full.size() << ", " << row.vt_full.size() << std::endl;
        break;
    }
    delete elements;
    return i;
}

const std::vector<std::string>& Faces::get_lines() const {
    return lines;
}

const Faces_tree& Faces::get_list() const {
    return list;
}
