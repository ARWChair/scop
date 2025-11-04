#include "Faces.hpp"

Faces::Faces(std::string filename) {
    this->filename = filename;
    if (load_lanes_from_obj() != 0)
        throw LoadException();
    int pos = split_faces_from_lines();
    if (pos < 0)
        throw SplitException();
    if (load_faces_from_array(pos) < 0) 
        throw ArrangeException();
}

Faces::~Faces() {}

int Faces::load_lanes_from_obj() {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return -1;
    }
    std::string line;
    while (std::getline(file, line))
        lines.push_back(line);
    file.close();
    return 0;
}

int Faces::split_faces_from_lines() {
    int pos = -1;

    for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it, pos++) {
        if (it->substr(0, 2) == "v ") {
            if (pos == 3473 - 1)
                std::cout << "Test string: " << *it << std::endl;
            std::istringstream s(*it);
            std::string temp;
            std::array<double, 3> vertex;
            s >> temp;
            for (int i = 0; i < 3; ++i) {
                s >> vertex[i];
            }
            verts.push_back(vertex);
        } else {
            pos++;
            std::cout << "Pos: " << pos << ", " << *it << std::endl;
            break;
        }
    }
    return pos;
}

int Faces::load_faces_from_array(int pos) {
    int save_pos = pos;

    std::cout << lines[0] << std::endl;
    if (lines.size() > (size_t)pos)
        lines.erase(lines.begin(), lines.begin() + pos);
    for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it, pos++) {
        if (it->substr(0, 2) == "f ")
            break;
    }
    if (lines.size() > (size_t)pos)
        lines.erase(lines.begin(), lines.begin() + pos);
    pos = save_pos;
    int f1 = 0, f2 = 0, f3 = 0;
    std::array<std::array<double, 3>, 3> temp_arr;
    int ppos = 0;
    for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++, ppos++) {
        if (it->substr(0, 2) == "f ") {
            // std::cout << "Test string: " << *it << std::endl;
            std::istringstream s(*it);
            std::string temp;
            s >> temp;
            s >> f1 >> f2 >> f3;
            temp_arr[0] = verts[f1 - 1];
            temp_arr[1] = verts[f2 - 1];
            temp_arr[2] = verts[f3 - 1];
            faces.push_back(temp_arr);
            // std::cout << "Face: " << verts[f1] << std::endl;
            // std::array<double, 3>::iterator v = verts[f1].begin();
            //     std::cout << "Vertex: " << v[0] << ", " << v[1] << ", " << v[2] << std::endl;
            
        }
        if (pos == 100)
            break;
    }
    return 0;
}

const std::vector<std::array<std::array<double, 3>, 3>>& Faces::get_faces() const {
    return faces;
}
