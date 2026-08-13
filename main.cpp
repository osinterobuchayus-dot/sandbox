// main.cpp
#include <GLFW/glfw3.h>
#include <iostream>

bool InitGUI(GLFWwindow* window);
void RenderGUI();
void CleanupGUI();

static void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

int main() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Pixel Sandbox", nullptr, nullptr);
    if (window == nullptr)
        return 1;
        
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!InitGUI(window)) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        RenderGUI();
    }

    CleanupGUI();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
