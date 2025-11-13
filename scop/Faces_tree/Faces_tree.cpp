#include "Faces_tree.hpp"

Faces_tree::Faces_tree(): list(NULL) {}

Faces_tree::Faces_tree(std::vector<std::array<double, 3>>& v, std::vector<std::array<double, 3>>& vn, std::vector<std::array<double, 2>>& vt): list(NULL) {
    list = create_list(v, vn, vt);
    if (!list)
        throw TreeCreateException();
}

Faces_tree::~Faces_tree() {
    if (list)
        clear_list();
}

obj_node *Faces_tree::create_list(std::vector<std::array<double, 3>>& v, std::vector<std::array<double, 3>>& vn, std::vector<std::array<double, 2>>& vt) {
    obj_node *root = new obj_node();
    if (root) {
        inner_elements element;
        element.verts = v;
        element.uvs = vn;
        element.normals = vt;
        root->elements.push_back(element);
    }
    return root;
}

void Faces_tree::add_elements(obj_node *element, std::vector<std::array<double, 3>>& v, std::vector<std::array<double, 3>>& vn, std::vector<std::array<double, 2>>& vt) {
    inner_elements inner;

    inner.verts = v;
    inner.uvs = vn;
    inner.normals = vt;
    element->elements.push_back(inner);
}

void Faces_tree::add_line(obj_node *element) {
    if (!element)
        return;
    
    if (!this->list) {
        this->list = element;
    } else {
        obj_node *current = this->list;
        
        while (current->next) {
            current = current->next;
        }
        current->next = element;
    }
    
    element->next = nullptr;
}

void Faces_tree::print_list() {
    obj_node *current = this->list;
    int node_index = 0;

    while (current) {
        std::cout << "=== Node " << node_index << " ===\n";
        
        int element_index = 0;
        for (std::vector<inner_elements>::iterator it = current->elements.begin(); 
             it != current->elements.end(); it++, element_index++) {
            
            std::cout << "Element " << element_index << ":\n";
            std::cout << "  Vertices (" << it->verts.size() << "):\n";
            for (size_t i = 0; i < it->verts.size(); i++) {
                std::cout << "    v" << i << ": " << it->verts[i][0] << ", " 
                          << it->verts[i][1] << ", " << it->verts[i][2] << "\n";
            }
            std::cout << "  UVs (" << it->uvs.size() << "):\n";
            for (size_t i = 0; i < it->uvs.size(); i++) {
                std::cout << "    uv" << i << ": " << it->uvs[i][0] << ", " 
                          << it->uvs[i][1] << "\n";
            }
            std::cout << "  Normals (" << it->normals.size() << "):\n";
            for (size_t i = 0; i < it->normals.size(); i++) {
                std::cout << "    n" << i << ": " << it->normals[i][0] << ", " 
                          << it->normals[i][1] << ", " << it->normals[i][2] << "\n";
            }
        }
        
        current = current->next;
        node_index++;
    }
}

void Faces_tree::clear_line(obj_node *to_remove) {
    if (!this->list || !to_remove)
        return;

    if (this->list == to_remove) {
        obj_node *temp = this->list;
        this->list = this->list->next;
        delete temp;
        return;
    }
    obj_node *current = this->list;
    
    while (current->next) {
        if (current->next == to_remove) {
            obj_node *temp = current->next;
            current->next = current->next->next;
            delete temp;
            return;
        }
        current = current->next;
    }
}

void Faces_tree::clear_list() {
    obj_node *current = this->list;
    
    while (current != nullptr) {
        current->elements.clear();

        obj_node *next = current->next;
        delete current;
        current = next;
    }
    
    this->list = nullptr;
}

void Faces_tree::set_v(std::vector<std::array<double, 3>>& v) {
    list->elements.front().verts = v;
}

void Faces_tree::set_v(std::vector<std::array<double, 3>>& v, obj_node *&node) {
    node->elements.front().verts = v;
}

void Faces_tree::set_vn(std::vector<std::array<double, 2>>& vn) {
    list->elements.front().normals = vn;
}

void Faces_tree::set_vn(std::vector<std::array<double, 2>>& vn, obj_node *&node) {
    node->elements.front().normals = vn;
}

void Faces_tree::set_vt(std::vector<std::array<double, 3>>& vt) {
    list->elements.front().uvs = vt;
}

void Faces_tree::set_vt(std::vector<std::array<double, 3>>& vt, obj_node *& node) {
    node->elements.front().uvs = vt;
}

obj_node* Faces_tree::get_list() {
    return list;
}

// void Faces_tree::add_children(obj_node *base, obj_node *children) {
//     if (!base || !children)
//         return;
//     if (base->children) {
//         base->children->push_back(children);
//         return;
//     }
//     base->children = new std::vector<obj_node *>();
//     if (!base->children)
//         throw ChildrenCreateException();
//     base->children->push_back(children);
// }

// void Faces_tree::add_children(obj_node *children) {
//     if (!children)
//         return;
//     if (tree->children) {
//         tree->children->push_back(children);
//         return;
//     }
//     tree->children = new std::vector<obj_node *>();
//     if (!tree->children)
//         throw ChildrenCreateException();
//     tree->children->push_back(children);
// }

// void Faces_tree::remove_children(obj_node *&base) {
//     if (!base)
//         return;
//     if (base->children) {
//         for (obj_node* child : *base->children) {
//             remove_children(child);
//             delete child;
//         }
//         delete base->children;
//         base->children = nullptr;
//     }
//     base->normals.clear();
//     base->uvs.clear();
//     base->verts.clear();
// }

// void Faces_tree::clear_tree_nodes() {
//     if (!tree)
//         return;
//     remove_children(tree);
//     delete tree;
//     tree = nullptr;
// }

// void Faces_tree::clear_tree() {
//     clear_tree_nodes();
//     delete tree;
//     tree = NULL;
// }

// void print_obj(obj_node *node, int depth) {
//     if (!node)
//         return;
//     for (int tabs = 0; tabs < depth; tabs++)
//         std::cout << "\t";
//     std::cout << "Verts: ";
//     for (std::vector<std::array<double, 3>>::iterator it = node->verts.begin(); it != node->verts.end(); it++) {
//         std::cout << (*it)[0] << ", " << (*it)[1] << ", " << (*it)[2] << "\n";
//     }
//     for (int tabs = 0; tabs < depth; tabs++)
//         std::cout << "\t";
//     std::cout << "UVs: ";
//     for (std::vector<std::array<double, 2>>::iterator it = node->uvs.begin(); it != node->uvs.end(); it++) {
//         std::cout << (*it)[0] << ", " << (*it)[1] << "\n";
//     }
//     for (int tabs = 0; tabs < depth; tabs++)
//         std::cout << "\t";
//     std::cout << "Normals: ";
//     for (std::vector<std::array<double, 3>>::iterator it = node->normals.begin(); it != node->normals.end(); it++) {
//         std::cout << (*it)[0] << ", " << (*it)[1] << ", " << (*it)[2] << "\n";
//     }
//     if (!node->children)
//         return;
//     int pos = 0;
//     for (std::vector<obj_node *>::iterator it = node->children->begin(); it != node->children->end(); it++, pos++) {
//         for (int tabs = 0; tabs < depth + 1; tabs++)
//             std::cout << "\t";
//         std::cout << "Child: " << pos << "\n";
//         print_obj(*it, depth + 1);
//         for (int tabs = 0; tabs < depth + 1; tabs++)
//             std::cout << "\t";
//         std::cout << "-----" << "\n";
//     }
//     std::cout << std::endl;
// }

// void Faces_tree::print_tree() {
//     print_obj(list, 0);
// }

// obj_node *Faces_tree::get_tree() {
//     return list;
// }