#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class GameUI {
public:
    void init(GLFWwindow* window);
    void render();
    void cleanup();
};
