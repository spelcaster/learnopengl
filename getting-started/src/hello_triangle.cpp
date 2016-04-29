#include <iostream>
#include <cmath>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include "Shader.hpp"

// window dimension
const GLuint kWidth  = 800;
const GLuint kHeight = 600;

// prototypes
void event_handler (GLFWwindow*, int, int, int, int);

int hello_triangle () {
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;

    // init GLFW
    glfwInit();

    // set required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    //  create a GLFWwindow object
    GLFWwindow* window = glfwCreateWindow(
        kWidth,
        kHeight,
        "Learning OpenGL",
        nullptr,
        nullptr
    );

    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return -1;
    }

    glfwMakeContextCurrent(window);

    // configure key event handler
    glfwSetKeyCallback(window, event_handler);

    // use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;

    // initialize GLEW to setup OpenGL function pointers
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;

        return -1;
    }

    // define viewport dimensions
    glViewport(0, 0, kWidth, kHeight);

    Shader *shader1 = new Shader("./shader/vshader.vs", "./shader/fshader.frag");
    Shader *shader2 = new Shader("./shader/vshader.vs", "./shader/fshader1.frag");

    // initialize triangle vertices in normalized device coordinates (NDC)
    GLfloat first_triangle[] = {
         0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // Blue Top Right
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Red Bottom Right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // Blue Bottom Left
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Green Top Left
    };

    // initialize triangle vertices in normalized device coordinates (NDC)
    GLfloat second_triangle[] = {
         0.6f,  0.0f, 0.0f, // Left point
         0.75f,  0.4f, 0.0f, // Top point
         0.9f,  0.0f, 0.0f, // Right point
         0.75f, -0.4f, 0.0f  // Bottom Point
    };

    GLuint indices[] = {
        0, 1, 3, // First Triangle
        1, 2, 3  // Second Triangle
    };

    // vertex array object
    GLuint VAO[2];
    // vertex buffer object
    GLuint VBO[2];
    // element buffer object
    GLuint EBO;

    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);
    glGenBuffers(1, &EBO);

    std::cout << "Managing VAO, VBO AND EBO" << std::endl;

    // First triangle
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(first_triangle),
        first_triangle,
        GL_STATIC_DRAW
    );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW
    );
    // vertex_shader position
    glVertexAttribPointer(
        0, // which vertex we want to configure
        3, // the size of the vertex attribute
        GL_FLOAT, // the type of the data (a vec* in GLSL consists of floating point values)
        GL_FALSE, // if we want the data to be normalized
        6 * sizeof(GLfloat), // the space between consecutive vextex attribute sets
        (GLvoid*)0 // the offset of where the position data begins in the buffer
    );
    glEnableVertexAttribArray(0);
    // vertex_shader color
    glVertexAttribPointer(
        1, // which vertex we want to configure
        3, // the size of the vertex attribute
        GL_FLOAT, // the type of the data (a vec* in GLSL consists of floating point values)
        GL_FALSE, // if we want the data to be normalized
        6 * sizeof(GLfloat), // the space between consecutive vextex attribute sets
        (GLvoid*) (3 * sizeof(GLfloat)) // the offset of where the position data begins in the buffer
    );
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Second triangle
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(second_triangle),
        second_triangle,
        GL_STATIC_DRAW
    );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW
    );
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0
    );
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // game loop
    while (!glfwWindowShouldClose(window)) {
        // check if any events have been fired (key press/release, mouse moved,
        // etc) and call corresponding response functions
        glfwPollEvents();

        // render
        // clear the color buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // ..:: Drawing code (in Game Loop) ::..
        shader1->use();
        glBindVertexArray(VAO[0]);
        glDrawElements(
            GL_TRIANGLES, // the mode we want to draw
            6, // count or number of elements we'd like to draw
            GL_UNSIGNED_INT, // the type of the indices
            0 // offset of the EBO
        );

        shader2->use();
        GLfloat time = glfwGetTime();
        GLfloat green_value = (sin(time) / 2) + 0.5;
        GLint location = glGetUniformLocation(
            shader2->getProgram(), "dynamic_color"
        );

        glUniform4f(location, 0.0f, green_value, 0.0f, 1.0f);

        glBindVertexArray(VAO[1]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // unbind Vertex Array Object
        glBindVertexArray(0);

        // swap the screen buffers
        glfwSwapBuffers(window);
    }

    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    glDeleteBuffers(1, &EBO);

    // terminate GLFW, clearing any resources allocated by GLFW
    glfwTerminate();

    return 0;
}

