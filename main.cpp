#include "scop/window/window.hpp"

int main() {
    Scop_window window = Scop_window(200, 200, 800, 600);
    window.hold_open();
}