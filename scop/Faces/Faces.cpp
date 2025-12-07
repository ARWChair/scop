#include "Faces.hpp"

Faces::Faces(std::string filename) {
    this->filename = filename;
    this->amount = 0;

    this->filename += ".obj";
    if (load_lanes_from_obj() == -1)
        throw LoadException();
    v_vn_vt* elements = split_parts();
    if (!elements)
        throw SplitException();
    std::vector<std::vector<std::string>> face_indexes = get_faces_indexes();
    save_faces_indices(face_indexes);
    save_material_file_name();
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

void Faces::save_material_file_name() {
    int saved = 0;
    for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end() && saved < 2; it++) {
        std::istringstream ss(*it);
        std::string line;

        ss >> line;
        if (line == "mtllib") {
            saved++;
            ss >> material_file_name;
        }
        if (line == "usemtl") {
            saved++;
            ss >> material_from_file;
            std::cout << material_from_file << std::endl;
        }
    }
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

void Faces::save_faces_indices(std::vector<std::vector<std::string>>& base) { 
    std::vector<unsigned int> faces_inner_temp; 
    unsigned int temp;

    for (std::vector<std::vector<std::string>>::iterator it = base.begin(); it != base.end(); it++) {
        for (std::vector<std::string>::iterator it_inner = it->begin(); it_inner != it->end(); it_inner++) {
            temp = std::stoul(*it_inner);
            faces_inner_temp.push_back(temp);
        }
        indices.push_back(faces_inner_temp);
        faces_inner_temp.clear();
    }
}

inner_elements get_indices_v_normals(v_vn_vt_layer& row, v_vn_vt*& elements) {
    inner_elements elem;
    std::array<double, 3> v;
    std::array<double, 2> vt;
    std::array<double, 3> vn;

    int arr_pos;
    for (size_t i = 0; i < row.v_full.size(); i++) {
        arr_pos = 0;
        size_t row_index = row.v_full[i];
        std::vector<double>& row_loop = elements->v_full[row_index - 1];
        
        for (size_t j = 0; j < row_loop.size(); j++) {
            v[arr_pos++] = row_loop[j];
        }
        elem.v.push_back(v);
    }
    for (size_t i = 0; i < row.vn_full.size(); i++) {
        arr_pos = 0;
        size_t row_index = row.vn_full[i];
        std::vector<double>& row_loop = elements->vn_full[row_index - 1];
        
        for (size_t j = 0; j < row_loop.size(); j++) {
            vn[arr_pos++] = row_loop[j];
        }
        elem.vn.push_back(vn);
    }
    for (size_t i = 0; i < row.vt_full.size(); i++) {
        arr_pos = 0;
        size_t row_index = row.vt_full[i];
        std::vector<double>& row_loop = elements->vt_full[row_index - 1];
        
        for (size_t j = 0; j < row_loop.size(); j++) {
            vt[arr_pos++] = row_loop[j];
        }
        elem.vt.push_back(vt);
    }
    return elem;
}

int Faces::split_in_tree(v_vn_vt*& elements, std::vector<std::vector<std::string>>& face_indexes) {
    v_vn_vt_layer row;
    int i = 0;

    for (std::vector<std::vector<std::string>>::iterator it = face_indexes.begin(); it != face_indexes.end(); it++, i++) {
        amount++;
        row = check_and_split(*it);
        inner_elements elems = get_indices_v_normals(row, elements);

        if (i == 0) {
            list.push_back(elems);
            continue;
        }
        list.push_back(elems);
        // std::cout << "Added element: " << amount << " of " << face_indexes.size() << std::endl;
    }
    delete elements;
    return i;
}

const std::vector<std::string>& Faces::get_lines() const {
    return lines;
}

const std::vector<inner_elements>& Faces::get_list() const {
    return list;
}

const int& Faces::get_amount() const {
    return amount;
}

const std::vector<std::vector<unsigned int>>& Faces::get_indices() const {
    return indices;
}

const std::string& Faces::get_material_file_name() const {
    return material_file_name;
}

const std::string& Faces::get_material_from_file() const {
    return material_from_file;
}

std::vector<unsigned int> split_and_group(std::vector<std::vector<std::array<double, 3>>> &faces, std::vector<std::vector<unsigned int>> indices) {
    std::vector<std::vector<std::array<double, 3>>> new_faces;
    std::vector<unsigned int> finished_indices;
    std::vector<std::array<double, 3>> temp;

    std::vector<std::vector<unsigned int>>::iterator indices_it = indices.begin();
    for (std::vector<std::vector<std::array<double, 3>>>::iterator it = faces.begin(); it != faces.end(); it++, indices_it++) {
        if (it->size() < 3)
            continue;
        if (it->size() == 3) {
            new_faces.push_back(*it);
            finished_indices.push_back((*indices_it)[0]);
            finished_indices.push_back((*indices_it)[1]);
            finished_indices.push_back((*indices_it)[2]);
        } else if (it->size() == 4) {
            temp.clear();
            temp.push_back((*it)[0]);
            finished_indices.push_back((*indices_it)[0]);
            temp.push_back((*it)[1]);
            finished_indices.push_back((*indices_it)[1]);
            temp.push_back((*it)[2]);
            finished_indices.push_back((*indices_it)[2]);
            new_faces.push_back(temp);
            temp.clear();
            temp.push_back((*it)[0]);
            finished_indices.push_back((*indices_it)[0]);
            temp.push_back((*it)[2]);
            finished_indices.push_back((*indices_it)[2]);
            temp.push_back((*it)[3]);
            finished_indices.push_back((*indices_it)[3]);
            new_faces.push_back(temp);
        } else {
            for (unsigned long i = 1; i < it->size() - 1; i++) {
                temp.clear();
                temp.push_back((*it)[0]);
                finished_indices.push_back((*indices_it)[0]);
                temp.push_back((*it)[i]);
                finished_indices.push_back((*indices_it)[i]);
                temp.push_back((*it)[i + 1]);
                finished_indices.push_back((*indices_it)[i + 1]);
                new_faces.push_back(temp);
            }
        }
    }
    faces = new_faces;
    return finished_indices;
}

std::vector<std::vector<std::array<double, 3>>> split_and_group(std::vector<std::vector<std::array<double, 3>>> &faces) {
    std::vector<std::vector<std::array<double, 3>>> new_faces;
    std::vector<std::array<double, 3>> temp;

    for (std::vector<std::vector<std::array<double, 3>>>::iterator it = faces.begin(); it != faces.end(); it++) {
        if (it->size() < 3)
            continue;
        if (it->size() == 3)
            new_faces.push_back(*it);
        else if (it->size() == 4) {
            temp.clear();
            temp.push_back((*it)[0]);
            temp.push_back((*it)[1]);
            temp.push_back((*it)[2]);
            new_faces.push_back(temp);
            temp.clear();
            temp.push_back((*it)[0]);
            temp.push_back((*it)[2]);
            temp.push_back((*it)[3]);
            new_faces.push_back(temp);
        } else {
            for (unsigned long i = 1; i < it->size() - 1; i++) {
                temp.clear();
                temp.push_back((*it)[0]);
                temp.push_back((*it)[i]);
                temp.push_back((*it)[i + 1]);
                new_faces.push_back(temp);
            }
        }
    }
    return new_faces;
}

std::vector<std::vector<std::array<double, 2>>> split_and_group(std::vector<std::vector<std::array<double, 2>>> &faces) {
    std::vector<std::vector<std::array<double, 2>>> new_faces;
    std::vector<std::array<double, 2>> temp;

    for (std::vector<std::vector<std::array<double, 2>>>::iterator it = faces.begin(); it != faces.end(); it++) {
        if (it->size() < 3)
            continue;
        if (it->size() == 3)
            new_faces.push_back(*it);
        else if (it->size() == 4) {
            temp.clear();
            temp.push_back((*it)[0]);
            temp.push_back((*it)[1]);
            temp.push_back((*it)[2]);
            new_faces.push_back(temp);
            temp.clear();
            temp.push_back((*it)[0]);
            temp.push_back((*it)[2]);
            temp.push_back((*it)[3]);
            new_faces.push_back(temp);
        } else {
            for (unsigned long i = 1; i < it->size() - 1; i++) {
                temp.clear();
                temp.push_back((*it)[0]);
                temp.push_back((*it)[i]);
                temp.push_back((*it)[i + 1]);
                new_faces.push_back(temp);
            }
        }
    }
    return new_faces;
}

void reallign_highest_point(std::vector<std::vector<std::array< double, 3>>> &faces, int id) {
    double highest = DBL_MIN;
    double lowest = DBL_MAX;

    for (std::vector<std::vector<std::array<double, 3>>>::iterator it = faces.begin(); it != faces.end(); it++) {
        for (std::vector<std::array<double, 3>>::iterator inner_it = it->begin(); inner_it != it->end(); inner_it++) {
            if (highest < (*inner_it)[id])
                highest = (*inner_it)[id];
            if (lowest > (*inner_it)[id])
                lowest = (*inner_it)[id];
        }
    }
    double diff = 0;
    diff = (lowest / 2) - diff;
    diff = (highest / 2) + diff;
    for (std::vector<std::vector<std::array<double, 3>>>::iterator it = faces.begin(); it != faces.end(); it++) {
        for (std::vector<std::array<double, 3>>::iterator inner_it = it->begin(); inner_it != it->end(); inner_it++) {
            (*inner_it)[id] -= diff;
        }
    }
}


