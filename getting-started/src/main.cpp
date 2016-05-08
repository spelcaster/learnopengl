#include <iostream>
#include <cmath>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include "Shader.hpp"

// used in texture exercise
GLfloat mix_ratio = 0.5f;

// prototypes
void event_handler (GLFWwindow*, int, int, int, int);
void hello_triangle ();
void shader_exercise1 ();
void shader_exercise2 ();
void shader_exercise3 ();
void texture_exercise1 (GLfloat &mix_ratio);
void texture_exercise2 (GLfloat &mix_ratio);

int main () {
    //hello_triangle();
    //shader_exercise1();
    //shader_exercise2();
    //shader_exercise3();
    //texture_exercise1(mix_ratio);
    texture_exercise2(mix_ratio);
    return 0;
}

// key event handler
void event_handler (
    GLFWwindow* window, int key, int scancode, int action, int mode
) {
    std::cout << "Key: " << key << std::endl;
    std::cout << "Action: " << action << std::endl;

    if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS)) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    } else if ((key == GLFW_KEY_W) && (action == GLFW_PRESS)) {
        // enable wireframe mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else if ((key == GLFW_KEY_F) && (action == GLFW_PRESS)) {
        // enable wireframe mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } else if (key == GLFW_KEY_UP) {
        mix_ratio += 0.1f;

        if (mix_ratio > 1.0f)
            mix_ratio = 1.0f;
    } else if (key == GLFW_KEY_DOWN) {
        mix_ratio -= 0.1f;

        if (mix_ratio < 0.0f)
            mix_ratio = 0.0f;
    }
}


