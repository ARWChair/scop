#include <iostream>
#define VK_USE_PLATFORM_XLIB_KHR
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_xlib.h>
#include <X11/keysym.h>
#include <vector>
#include <exception>
#define VK_KHR_XLIB_SURFACE_EXTENSION_NAME "VK_KHR_xlib_surface"
#pragma once

class Scop_vulkan {
    public:
        Scop_vulkan();
        Scop_vulkan& operator=(const Scop_vulkan &copy);
        ~Scop_vulkan();

        void create_surface(Display *display, Window window);
        void setup_devices();
        void create_queues();
        bool isDeviceSuitable(VkPhysicalDevice device);
        VkInstance const & get_instance() const;
        VkApplicationInfo const & get_appinfo() const;
        VkInstanceCreateInfo const & get_instanceinfo() const;

        // exceptions
        class VKInstanceExceptions : public std::exception {
            public:
                virtual const char* what() const throw() {
                    return "Failed to create Vulkan instance";
                }
        };
        class VKSurfaceExceptions : public std::exception {
            public:
                virtual const char* what() const throw() {
                    return "Failed to create Vulkan surface";
                }
        };
        class VKGPUCountExceptions : public std::exception {
            public:
                virtual const char* what() const throw() {
                    return "No gpu found";
                }
        };
        class VKPhysicalDeviceExceptions : public std::exception {
            public:
                virtual const char* what() const throw() {
                    return "Failed to find suitable GPU";
                }
        };
    protected:
    private:
        VkApplicationInfo appinfo;
        VkInstanceCreateInfo instanceinfo;
        VkInstance instance;
        VkSurfaceKHR surface;
        VkXlibSurfaceCreateInfoKHR surfaceinfo;
        VkPhysicalDevice physicalDevice;
};