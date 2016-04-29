#include <iostream>
#include <cmath>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include "Shader.hpp"

// prototypes
void event_handler (GLFWwindow*, int, int, int, int);
void hello_triangle ();
void shader_exercise1 ();
void shader_exercise2 ();
void shader_exercise3 ();

int main () {
    //hello_triangle();
    //shader_exercise1();
    //shader_exercise2();
    shader_exercise3();
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
    }
}


