#ifndef EZAPP_HPP
#define EZAPP_HPP

#include "deps/glfw/glfw3.h"
#include <string>
#include <vector>

// Simple GLFW Wrapper for Window Setup
class EzApp {
public:
    EzApp(int width, int height, const std::string& title) {
        if (!glfwInit()) return;
        window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    }

    ~EzApp() {
        if (window) glfwDestroyWindow(window);
        glfwTerminate();
    }

    // Set Window Icon using raw RGBA pixels
    void setIcon(int width, int height, const unsigned char* pixels) {
        GLFWimage images[1];
        images[0].width = width;
        images[0].height = height;
        images[0].pixels = const_cast<unsigned char*>(pixels);
        glfwSetWindowIcon(window, 1, images);
    }

    bool shouldClose() { return glfwWindowShouldClose(window); }
    void swapBuffers() { glfwSwapBuffers(window); }
    void pollEvents() { glfwPollEvents(); }

private:
    GLFWwindow* window;
};

#endif // EZAPP_HPP
