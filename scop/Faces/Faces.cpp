#include "Faces.hpp"

Faces::Faces(std::string filename) {
    this->filename = filename;
    this->amount = 0;
    this->missing = false;
    this->scale = 1;

    this->filename += ".obj";
    if (load_lanes_from_obj() == -1)
        throw LoadException();
    v_vn_vt* elements = split_parts();
    if (!elements)
        throw SplitException();
    std::vector<std::vector<std::string>> face_indexes = get_faces_indexes();
    std::vector<inner_elements> elems = triangulate(face_indexes);
    flattened = flatten_faces(elements);
    for (std::vector<inner_elements>::iterator it = elems.begin(); it != elems.end(); it++) {
        for (std::vector<unsigned int>::iterator inner = it->v.begin(); inner != it->v.end(); inner++) {
            indices.vertexes_indice.push_back(*inner);
        }
        for (std::vector<unsigned int>::iterator inner = it->vt.begin(); inner != it->vt.end(); inner++) {
            indices.textures_indice.push_back(*inner);
        }
        for (std::vector<unsigned int>::iterator inner = it->vn.begin(); inner != it->vn.end(); inner++) {
            indices.normals_indice.push_back(*inner);
        }
    }
    save_material_file_name();
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

void split_in_elements_helper(std::istringstream& ss, std::vector<unsigned int>& type) {
    unsigned int temp_int;
    std::string temp;

    std::getline(ss, temp, '/');
    if (temp.length() != 0) {
        temp_int = std::stoul(temp);
        type.push_back(temp_int - 1);
    }
}

inner_elements Faces::split_in_elems(std::vector<std::string>& row) {
    unsigned int temp_int;
    inner_elements elem;
    std::string temp;

    if (row[0].find('/') == std::string::npos) {
        temp_int = std::stoul(row[0]);
        elem.v.push_back(temp_int - 1);
        temp_int = std::stoul(row[1]);
        elem.v.push_back(temp_int - 1);
        temp_int = std::stoul(row[2]);
        elem.v.push_back(temp_int - 1);
        elem.vn.push_back(1);
        elem.vn.push_back(1);
        elem.vn.push_back(1);
        return elem;
    }
    std::istringstream ss_first(row[0]);
    std::istringstream ss_second(row[1]);
    std::istringstream ss_third(row[2]);

    split_in_elements_helper(ss_first, elem.v);
    split_in_elements_helper(ss_first, elem.vt);
    split_in_elements_helper(ss_first, elem.vn);

    split_in_elements_helper(ss_second, elem.v);
    split_in_elements_helper(ss_second, elem.vt);
    split_in_elements_helper(ss_second, elem.vn);

    split_in_elements_helper(ss_third, elem.v);
    split_in_elements_helper(ss_third, elem.vt);
    split_in_elements_helper(ss_third, elem.vn);
    
    if (elem.vn.size() == 0) {
        elem.vn.push_back(0);
        elem.vn.push_back(0);
        elem.vn.push_back(0);
    }
    return elem;
}

std::vector<inner_elements> Faces::triangulate(std::vector<std::vector<std::string>>& faces_index) {
    std::vector<std::vector<std::string>> new_faces;
    std::vector<inner_elements> elems;

    for (std::vector<std::vector<std::string>>::iterator it = faces_index.begin(); it != faces_index.end(); it++) {
        if (it->size() == 3) {
            new_faces.push_back(*it);
            elems.push_back(split_in_elems(*it));
        } else if (it->size() == 4) {
            std::vector<std::string> temp_face;
            temp_face.push_back((*it)[0]);
            temp_face.push_back((*it)[1]);
            temp_face.push_back((*it)[2]);
            new_faces.push_back(temp_face);
            elems.push_back(split_in_elems(temp_face));
            temp_face.clear();
            temp_face.push_back((*it)[0]);
            temp_face.push_back((*it)[2]);
            temp_face.push_back((*it)[3]);
            new_faces.push_back(temp_face);
            elems.push_back(split_in_elems(temp_face));
        } else {
            for (unsigned long i = 1; i < it->size() - 1; i++) {
                std::vector<std::string> temp_face;
                temp_face.push_back((*it)[0]);
                temp_face.push_back((*it)[i]);
                temp_face.push_back((*it)[i + 1]);
                new_faces.push_back(temp_face);
                elems.push_back(split_in_elems(temp_face));
            }
        }
    }
    faces_index = new_faces;
    return elems;
}

flat Faces::flatten_faces(v_vn_vt*& faces) {
    flat flattened;

    for (std::vector<std::vector<double>>::iterator v = faces->v_full.begin(); v != faces->v_full.end(); v++) {
        for (std::vector<double>::iterator it = v->begin(); it != v->end(); it++)
            flattened.vertexes.push_back(*it);
    }
    for (std::vector<std::vector<double>>::iterator vt = faces->vt_full.begin(); vt != faces->vt_full.end(); vt++) {
        for (std::vector<double>::iterator it = vt->begin(); it != vt->end(); it++)
            flattened.textures.push_back(*it);
    }
    for (std::vector<std::vector<double>>::iterator vn = faces->vn_full.begin(); vn != faces->vn_full.end(); vn++) {
        for (std::vector<double>::iterator it = vn->begin(); it != vn->end(); it++)
            flattened.normals.push_back(*it);
    }
    if (flattened.normals.size() == 0) {
        flattened.normals.push_back(0.000000f);
        flattened.normals.push_back(1.000000f);
        flattened.normals.push_back(0.000000f);
    }
    return flattened;
}

void Faces::save_material_file_name() {
    int saved = 0;
    bool found = false;
    for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end() && saved < 2; it++) {
        std::istringstream ss(*it);
        std::string line, temp;

        ss >> line;
        if (line == "mtllib") {
            saved++;
            ss >> material_file_name;
        }
        if (line == "usemtl") {
            found = true;
            ss >> temp;
            if (temp.length() != 0) {
                saved++;
                material_from_file = temp;
            } else {
                missing = true;
            }
        }
    }
    if (found == false)
        missing = true;
}

const bool& Faces::is_missing() const {
    return missing;
}

const flat& Faces::get_flattened() const {
    return flattened;
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

const std::vector<std::string>& Faces::get_lines() const {
    return lines;
}

const std::vector<inner_elements>& Faces::get_list() const {
    return list;
}

const int& Faces::get_amount() const {
    return amount;
}

const flat_indices& Faces::get_indices() const {
    return indices;
}

const std::string& Faces::get_material_file_name() const {
    return material_file_name;
}

const std::string& Faces::get_material_from_file() const {
    return material_from_file;
}

const double& Faces::get_scale() const {
    return scale;
}

void Faces::inc_scale(double value) {
    scale += value;
}

void Faces::dec_scale(double value) {
    scale -= value;
}


void Faces::calculate_scale(std::vector<GLfloat>& flattened) {
    if (flattened.empty() || flattened.size() < 3)
        return;
    
    double min_x = DBL_MAX, max_x = -DBL_MAX;
    double min_y = DBL_MAX, max_y = -DBL_MAX;
    double min_z = DBL_MAX, max_z = -DBL_MAX;
    for (std::size_t i = 0; i < flattened.size(); i += 3) {
        double x = static_cast<double>(flattened[i]);
        double y = static_cast<double>(flattened[i+1]);
        double z = static_cast<double>(flattened[i+2]);
        
        if (x < min_x) min_x = x;
        if (x > max_x) max_x = x;
        if (y < min_y) min_y = y;
        if (y > max_y) max_y = y;
        if (z < min_z) min_z = z;
        if (z > max_z) max_z = z;
    }
    
    double extent_x = max_x - min_x;
    double extent_y = max_y - min_y;
    double extent_z = max_z - min_z;
    double max_extent = std::max(extent_x, std::max(extent_y, extent_z));
    if (max_extent <= 0.0)
        return;
    
    double target_size = 2.0;
    double scale_factor = target_size / max_extent;
    this->scale = scale_factor;
}

void reallign_highest_point(std::vector<GLfloat> &vertices, int id, int stride) {
    if (vertices.empty() || stride <= 0)
        return;
    
    double highest = -DBL_MAX;
    double lowest = DBL_MAX;
    
    for (size_t i = id; i < vertices.size(); i += stride) {
        double val = static_cast<double>(vertices[i]);
        if (val > highest)
            highest = val;
        if (val < lowest)
            lowest = val;
    }
    double diff = (highest + lowest) / 2.0;
    for (size_t i = id; i < vertices.size(); i += stride) {
        vertices[i] -= static_cast<GLfloat>(diff);
    }
}
