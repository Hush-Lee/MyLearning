// src/main.cpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include <stdexcept>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

void checkVk(VkResult result, const char* msg) {
    if (result != VK_SUCCESS) {
        throw std::runtime_error(msg);
    }
}

int main(int argc, char* argv[]) {
    // 初始化 SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL2 + Vulkan Init",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT,
        SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Failed to create SDL_Window\n";
        return -1;
    }

    // Vulkan 实例
    VkInstance instance;
    std::vector<const char*> extensions;

    unsigned extCount;
    if (!SDL_Vulkan_GetInstanceExtensions(window, &extCount, nullptr)) {
        std::cerr << "Failed to get Vulkan extensions from SDL\n";
        return -1;
    }
    extensions.resize(extCount);
    SDL_Vulkan_GetInstanceExtensions(window, &extCount, extensions.data());

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "3D Mota";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = extCount;
    createInfo.ppEnabledExtensionNames = extensions.data();

    checkVk(vkCreateInstance(&createInfo, nullptr, &instance), "Failed to create Vulkan instance");

    // 创建 surface
    VkSurfaceKHR surface;
    if (!SDL_Vulkan_CreateSurface(window, instance, &surface)) {
        std::cerr << "Failed to create Vulkan surface\n";
        return -1;
    }

    std::cout << "SDL2 + Vulkan initialized successfully.\n";

    // 主循环
    bool quit = false;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                quit = true;
        }
        SDL_Delay(16); // ~60 FPS
    }

    // 清理
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

