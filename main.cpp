#include "scop/scop.hpp"

int main() {
    try {
        Scop_window window = Scop_window(200, 200, 800, 600);
        Scop_vulkan vulkan = Scop_vulkan();
        vulkan.create_surface((Display *)window.get_display(), (Window)window.get_window());
        vulkan.setup_devices();
        vulkan.create_queues();
        vulkan.create_vk_device();
        window.hold_open();
    } catch (...) {
        std::cout << "Error" << std::endl;
    }
}