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
    vkDeviceWaitIdle(device);
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
        if (is_device_suitable(device)) {
            physicalDevice = device;
            break;
        }
    }
    if (physicalDevice == VK_NULL_HANDLE)
        throw VKPhysicalDeviceExceptions();
}

uint32_t Scop_vulkan::get_queue_family() {
    uint32_t queueFamily = 0;
    int graphicsFamily = -1;

    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamily, NULL);
    VkQueueFamilyProperties queueFamilies[queueFamily];
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamily, queueFamilies);
    for (uint32_t i = 0; i < queueFamily; i++) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphicsFamily = i;
            break;
        }
    }
    return graphicsFamily;
}

void Scop_vulkan::create_queues() {
    uint32_t queues_count = get_queue_family();
    float queue_priority = 1.0f;

    queue_createinfo = {};
    device_features = {};
    createinfo = {};
    queue_createinfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_createinfo.queueFamilyIndex = queues_count;
    queue_createinfo.queueCount = 1;
    queue_createinfo.pQueuePriorities = &queue_priority;
    createinfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createinfo.pQueueCreateInfos = &queue_createinfo;
    createinfo.queueCreateInfoCount = 1;
    createinfo.pEnabledFeatures = &device_features;
}

void Scop_vulkan::create_vk_device() {
    if (vkCreateDevice(physicalDevice, &createinfo, NULL, &device) != VK_SUCCESS)
        throw VKDeviceExceptions();
}

void Scop_vulkan::create_swapchain() {
    uint32_t format_count;

    surface_capabilities = {};
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surface_capabilities);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &format_count, NULL);
    std::vector<VkSurfaceFormatKHR> formats(format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &format_count, formats.data());
    
}

bool Scop_vulkan::is_device_suitable(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties device_properties;
    VkPhysicalDeviceFeatures device_features;
    vkGetPhysicalDeviceProperties(device, &device_properties);
    vkGetPhysicalDeviceFeatures(device, &device_features);
    return device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && device_features.geometryShader;
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