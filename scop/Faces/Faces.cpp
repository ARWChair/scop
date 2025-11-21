#include "Faces.hpp"

Faces::Faces(std::string filename) {
    this->filename = filename;
    this->list = NULL;
    this->amount = 0;

    if (load_lanes_from_obj() == -1)
        throw LoadException();
    v_vn_vt* elements = split_parts();
    if (!elements)
        throw SplitException();
    std::vector<std::vector<std::string>> face_indexes = get_faces_indexes();
    split_in_tree(elements, face_indexes);
}

Faces::~Faces() {
    delete list;
}

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

inner_elements get_indices_v_normals(v_vn_vt_layer& row, v_vn_vt*& elements) {
    inner_elements elem;
    std::array<double, 3> v;
    std::array<double, 2> vt;
    std::array<double, 3> vn;

    int arr_pos;
    for (std::vector<double>::iterator it = row.v_full.begin(); it != row.v_full.end(); it++) {
        arr_pos = 0;
        std::vector<std::vector<double>>::iterator row_loop = elements->v_full.begin() + *it;
        row_loop--;
        for (std::vector<double>::iterator inner = row_loop->begin(); inner != row_loop->end(); inner++)
            v[arr_pos++] = *inner;
        elem.v.push_back(v);
    }
    for (std::vector<double>::iterator it = row.vn_full.begin(); it != row.vn_full.end(); it++) {
        arr_pos = 0;
        std::vector<std::vector<double>>::iterator row_loop = elements->vn_full.begin() + *it;
        row_loop--;
        for (std::vector<double>::iterator inner = row_loop->begin(); inner != row_loop->end(); inner++)
            vn[arr_pos++] = *inner;
        elem.vn.push_back(vn);
    }
    for (std::vector<double>::iterator it = row.vt_full.begin(); it != row.vt_full.end(); it++) {
        arr_pos = 0;
        std::vector<std::vector<double>>::iterator row_loop = elements->vt_full.begin() + *it;
        row_loop--;
        for (std::vector<double>::iterator inner = row_loop->begin(); inner != row_loop->end(); inner++)
            vt[arr_pos++] = *inner;
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
            list = new Faces_list(elems.v, elems.vn, elems.vt);
            if (!list)
                throw ListNewException();
            continue;
        }
        list->add_elements(elems.v, elems.vn, elems.vt);
    }
    delete elements;
    return i;
}

const std::vector<std::string>& Faces::get_lines() const {
    return lines;
}

const Faces_list* Faces::get_list() const {
    return list;
}

obj_node* Faces::get_line(int node) {
    int current_node = 0;
    Faces_list *current = this->list;
    obj_node* current_list = current->get_list();

    if (node >= current->get_size())
        return NULL;
    while (current_node < node) {
        current_list = current_list->next;
        current_node++;
    }
    return current_list;
}


const int& Faces::get_amount() const {
    return amount;
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
