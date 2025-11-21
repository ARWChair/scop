#include "Faces_list.hpp"

Faces_list::Faces_list(): list(NULL), size(0) {}

Faces_list::Faces_list(std::vector<std::array<double, 3>>& v, std::vector<std::array<double, 3>>& vn, std::vector<std::array<double, 2>>& vt): list(NULL), size(0) {
    list = create_list(v, vn, vt);
    if (!list)
        throw ListCreateException();
}

Faces_list::~Faces_list() {
    if (list)
        clear_list();
}

int &Faces_list::get_size() {
    return size;
}

obj_node *Faces_list::create_list(std::vector<std::array<double, 3>>& v, std::vector<std::array<double, 3>>& vn, std::vector<std::array<double, 2>>& vt) {
    obj_node *root = new obj_node();
    if (root) {
        inner_elements element;

        element.v = v;
        element.vn = vn;
        element.vt = vt;
        root->elements = element;
        root->next = NULL;
        size++;
    }
    return root;
}

void Faces_list::add_elements(std::vector<std::array<double, 3>>& v, std::vector<std::array<double, 3>>& vn, std::vector<std::array<double, 2>>& vt) {
    obj_node *new_node = create_list(v, vn, vt);
    
    if (!new_node)
        throw ChildrenCreateException();

    obj_node*& head = this->list;
    if (!head) {
        head = new_node;
    } else {
        obj_node* current = head;
        while (current->next)
            current = current->next;
        current->next = new_node;
    }
}

void Faces_list::add_line(obj_node *element) {
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
    
    size++;
    element->next = nullptr;
}

void Faces_list::print_list() {
    obj_node *current = this->list;
    int node_index = 0;

    if (size == 0) {
        std::cout << "List is empty!" << std::endl;
        return;
    }
    while (current) {
        std::cout << "=== Node " << node_index << " ===\n";
        for (std::vector<std::array<double, 3>>::iterator it = current->elements.v.begin(); it != current->elements.v.end(); it++) {
            std::cout << "V: " << (*it)[0] << ", " << (*it)[1] << ", " << (*it)[2] << "\n";
        }
        for (std::vector<std::array<double, 3>>::iterator it = current->elements.vn.begin(); it != current->elements.vn.end(); it++) {
            std::cout << "Vn: " << (*it)[0] << ", " << (*it)[1] << ", " << (*it)[2] << "\n";
        }
        for (std::vector<std::array<double, 2>>::iterator it = current->elements.vt.begin(); it != current->elements.vt.end(); it++) {
            std::cout << "Vt: " << (*it)[0] << ", " << (*it)[1] << "\n";
        }
        std::cout << std::endl;
        current = current->next;
        node_index++;
    }
}

void Faces_list::clear_line(obj_node *to_remove) {
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

void Faces_list::clear_list() {
    obj_node *current = this->list;
    
    while (current != nullptr) {
        obj_node *next = current->next;
        delete current;
        current = next;
    }
    
    size = 0;
    this->list = nullptr;
}

void Faces_list::set_v(std::vector<std::array<double, 3>>& v) {
    list->elements.v = v;
}

void Faces_list::set_v(std::vector<std::array<double, 3>>& v, obj_node *&node) {
    node->elements.v = v;
}

void Faces_list::set_vn(std::vector<std::array<double, 3>>& vn) {
    list->elements.vn = vn;
}

void Faces_list::set_vn(std::vector<std::array<double, 3>>& vn, obj_node *&node) {
    node->elements.vn = vn;
}

void Faces_list::set_vt(std::vector<std::array<double, 2>>& vt) {
    list->elements.vt = vt;
}

void Faces_list::set_vt(std::vector<std::array<double, 2>>& vt, obj_node *& node) {
    node->elements.vt = vt;
}

void Faces_list::set_list(obj_node *list) {
    this->list = list;
}

obj_node* Faces_list::get_list() {
    return list;
}
