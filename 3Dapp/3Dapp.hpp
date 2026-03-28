#ifndef APP3D_HPP
#define APP3D_HPP

#include <deps/glad/glad.h>
#include <deps/glfw/glfw3.h>
#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include <cmath>

// Basic 3D Application Class
class App3D {
public:
    App3D(int width, int height, const std::string& title) {
        // Initialize GLFW
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create Window
        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!window) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            exit(-1);
        }
        glfwMakeContextCurrent(window);

        // Load OpenGL functions
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            exit(-1);
        }

        glEnable(GL_DEPTH_TEST); // Enable 3D depth testing
    }

    ~App3D() {
        // Clean up terrain buffers if created
        if (terrainVAO) {
            glDeleteVertexArrays(1, &terrainVAO);
            glDeleteBuffers(1, &terrainVBO);
            glDeleteBuffers(1, &terrainEBO);
        }
        glfwTerminate();
    }

    // Set callback for game logic (update/render)
    void run(std::function<void(float)> updateLoop) {
        float lastFrame = 0.0f;
        while (!glfwWindowShouldClose(window)) {
            // Delta time
            float currentFrame = glfwGetTime();
            float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // Input handling
            processInput(window);

            // Render clear
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Execute custom game logic
            updateLoop(deltaTime);

            // Swap buffers and poll events
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    bool isKeyPressed(int key) {
        return glfwGetKey(window, key) == GLFW_PRESS;
    }

    // --- Terrain API -------------------------------------------------

    // Simple height function example (you can pass your own)
    static float defaultHeightFunc(float x, float z) {
        // Gentle waves
        return 0.5f * std::sin(0.2f * x) * std::cos(0.2f * z);
    }

    // Generate a grid-based terrain mesh and upload to GPU
    // width, depth: number of vertices in X/Z
    // scale: spacing between vertices
    // heightFunc: function mapping (x, z) -> y
    void generateTerrain(int width, int depth, float scale,
                         std::function<float(float, float)> heightFunc = defaultHeightFunc) {
        if (terrainVAO) {
            glDeleteVertexArrays(1, &terrainVAO);
            glDeleteBuffers(1, &terrainVBO);
            glDeleteBuffers(1, &terrainEBO);
            terrainVAO = terrainVBO = terrainEBO = 0;
            terrainIndexCount = 0;
        }

        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        vertices.reserve(width * depth * 8); // pos(3) + normal(3) + uv(2)

        // Generate positions and placeholder normals/UVs
        for (int z = 0; z < depth; ++z) {
            for (int x = 0; x < width; ++x) {
                float fx = (x - width / 2.0f) * scale;
                float fz = (z - depth / 2.0f) * scale;
                float fy = heightFunc(fx, fz);

                // Position
                vertices.push_back(fx);
                vertices.push_back(fy);
                vertices.push_back(fz);

                // Normal (temporary, will approximate later)
                vertices.push_back(0.0f);
                vertices.push_back(1.0f);
                vertices.push_back(0.0f);

                // UV
                vertices.push_back(static_cast<float>(x) / (width - 1));
                vertices.push_back(static_cast<float>(z) / (depth - 1));
            }
        }

        // Generate indices for triangle grid
        for (int z = 0; z < depth - 1; ++z) {
            for (int x = 0; x < width - 1; ++x) {
                int topLeft = z * width + x;
                int topRight = topLeft + 1;
                int bottomLeft = (z + 1) * width + x;
                int bottomRight = bottomLeft + 1;

                // First triangle
                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topRight);

                // Second triangle
                indices.push_back(topRight);
                indices.push_back(bottomLeft);
                indices.push_back(bottomRight);
            }
        }

        terrainIndexCount = static_cast<unsigned int>(indices.size());

        // Upload to GPU
        glGenVertexArrays(1, &terrainVAO);
        glGenBuffers(1, &terrainVBO);
        glGenBuffers(1, &terrainEBO);

        glBindVertexArray(terrainVAO);

        glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Layout: position (0), normal (1), uv (2)
        GLsizei stride = 8 * sizeof(float);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }

    // Draw the generated terrain (assumes an active shader with matching attributes)
    void drawTerrain() {
        if (!terrainVAO || terrainIndexCount == 0) return;
        glBindVertexArray(terrainVAO);
        glDrawElements(GL_TRIANGLES, terrainIndexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    GLFWwindow* window;

    // Terrain GPU resources
    unsigned int terrainVAO = 0;
    unsigned int terrainVBO = 0;
    unsigned int terrainEBO = 0;
    unsigned int terrainIndexCount = 0;

    void processInput(GLFWwindow* window) {
        if (isKeyPressed(GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(window, true);
    }
};

#endif