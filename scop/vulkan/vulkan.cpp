#include "vulkan.hpp"

// Scop_vulkan::Scop_vulkan() {}

Scop_vulkan::Scop_vulkan() {
    appinfo = {};
    appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appinfo.pApplicationName = "Scop";
    appinfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appinfo.pEngineName = "No Engine";
    appinfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appinfo.apiVersion = VK_API_VERSION_1_3;

    const char* extensions[] = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_XLIB_SURFACE_EXTENSION_NAME
    };

    instanceinfo = {};
    instanceinfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceinfo.pApplicationInfo = &appinfo;
    instanceinfo.enabledExtensionCount = 2;
    instanceinfo.ppEnabledExtensionNames = extensions;

    if (vkCreateInstance(&instanceinfo, nullptr, &instance) != VK_SUCCESS)
        throw VKInstanceExceptions();
}

Scop_vulkan& Scop_vulkan::operator=(const Scop_vulkan& copy) {
    if (this != &copy) {
        this->appinfo = copy.appinfo;
        this->instance = copy.instance;
        this->instanceinfo = copy.instanceinfo;
    }
    return *this;
}

Scop_vulkan::~Scop_vulkan() {
    // vkDeviceWaitIdle(device);
}


void Scop_vulkan::create_surface(Display *display, Window window) {
    surfaceinfo = {};
    surfaceinfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    surfaceinfo.dpy = display;
    surfaceinfo.window = window;

    if (vkCreateXlibSurfaceKHR(instance, &surfaceinfo, NULL, &surface) != VK_SUCCESS)
        throw VKSurfaceExceptions();
}

void Scop_vulkan::setup_devices() {
    uint32_t devices_count = 0;
    vkEnumeratePhysicalDevices(instance, &devices_count, NULL);
    if (devices_count == 0)
        throw VKGPUCountExceptions();
    
    std::vector<VkPhysicalDevice> devices(devices_count);
    vkEnumeratePhysicalDevices(instance, &devices_count, devices.data());
    for (const auto& device: devices) {
        if (isDeviceSuitable(device)) {
            physicalDevice = device;
            break;
        }
    }
    if (physicalDevice == VK_NULL_HANDLE)
        throw VKPhysicalDeviceExceptions();
}

bool Scop_vulkan::isDeviceSuitable(VkPhysicalDevice device) {
    //https://vulkan-tutorial.com/Drawing_a_triangle/Setup/Physical_devices_and_queue_families
    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceProperties(device, &device_properties);
}


VkInstance const &Scop_vulkan::get_instance() const {
    return instance;
}

VkApplicationInfo const &Scop_vulkan::get_appinfo() const {
    return appinfo;
}

VkInstanceCreateInfo   const &Scop_vulkan::get_instanceinfo() const {
    return instanceinfo;
}