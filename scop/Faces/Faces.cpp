#include "Faces.hpp"

Faces::Faces(std::string filename) {
    this->filename = filename;
    if (load_lanes_from_obj() == -1)
        throw LoadException();
    
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

int Faces::split_parts() {
    std::vector<std::string> v;
    std::vector<std::string> vn;
    std::vector<std::string> vt;
    std::vector<std::string> line;
    std::string l;

    for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++) {
        std::istringstream ss(*it);
        while (ss >> l)
            line.push_back(l);
        std::vector<std::string>::iterator
    }
}

const std::vector<std::string>& Faces::get_lines() const {

}

const Faces_tree& Faces::get_faces() const {
    return tree;
}
