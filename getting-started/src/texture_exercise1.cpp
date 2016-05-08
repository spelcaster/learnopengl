#include <iostream>
#include <cmath>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <SOIL/SOIL.h>

#include "Shader.hpp"

// window dimension
const GLuint kWidth  = 800;
const GLuint kHeight = 600;

// prototypes
void event_handler (GLFWwindow*, int, int, int, int);

int texture_exercise1 (GLfloat &mix_ratio) {
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

    std::cout << "Creating shader programs" << std::endl;

    Shader *shader = new Shader(
        "./shader/texture.vs",
        "./shader/texture.frag"
    );

    std::cout << "Managing VAO, VBO AND EBO" << std::endl;

    // initialize triangle vertices in normalized device coordinates (NDC)
    GLfloat vertices[] = {
        // positions        // colors         // texture coords
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
    };

    GLuint indices[] = {
        0, 1, 3, // First Triangle
        1, 2, 3  // Second Triangle
    };

    // vertex array object
    GLuint VAO;
    // vertex buffer object
    GLuint VBO;
    // element buffer object
    GLuint EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW
    );

    // vertex_shader location 0
    glVertexAttribPointer(
        0, // which vertex we want to configure
        3, // the size of the vertex attribute
        GL_FLOAT, // the type of the data (a vec* in GLSL consists of floating point values)
        GL_FALSE, // if we want the data to be normalized
        8 * sizeof(GLfloat), // the space between consecutive vextex attribute sets
        (GLvoid*)0 // the offset of where the position data begins in the buffer
    );
    glEnableVertexAttribArray(0);

    // vertex_shader location 1
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(GLfloat),
        (GLvoid*)(3 * sizeof(GLfloat))
    );
    glEnableVertexAttribArray(1);

    // vertex_shader location 2
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(GLfloat),
        (GLvoid*)(6 * sizeof(GLfloat))
    );
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    std::cout << "Loading textures" << std::endl;

    GLuint texture0;
    GLuint texture1;

    int width;
    int height;
    unsigned char *image;

    // ========================================================================
    // Texture 1
    // ========================================================================
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0);

    // texture wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    image = SOIL_load_image(
        "res/img/container.jpg",
        &width,
        &height,
        0,
        SOIL_LOAD_RGB
    );

    glTexImage2D(
        GL_TEXTURE_2D,      // texture target
        0,                  // the mipmap level to create a texture
        GL_RGB,             // the format to store the texture
        width,              // width of the texture
        height,             // height of the texture
        0,                  // always 0 (legacy stuff)
        GL_RGB,             // format of the source image
        GL_UNSIGNED_BYTE,   // data type of the source image
        image               // the image data
    );

    glGenerateMipmap(GL_TEXTURE_2D);

    // good practices
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    // ========================================================================
    // Texture 2
    // ========================================================================
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // texture wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    image = SOIL_load_image(
        "res/img/awesomeface.png",
        &width,
        &height,
        0,
        SOIL_LOAD_RGB
    );

    glTexImage2D(
        GL_TEXTURE_2D,      // texture target
        0,                  // the mipmap level to create a texture
        GL_RGB,             // the format to store the texture
        width,              // width of the texture
        height,             // height of the texture
        0,                  // always 0 (legacy stuff)
        GL_RGB,             // format of the source image
        GL_UNSIGNED_BYTE,   // data type of the source image
        image               // the image data
    );

    glGenerateMipmap(GL_TEXTURE_2D);

    // good practices
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

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
        shader->use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glUniform1i(glGetUniformLocation(shader->getProgram(),"texture0"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(shader->getProgram(),"texture1"), 1);

        glUniform1f(
            glGetUniformLocation(shader->getProgram(), "mix_ratio"),
            mix_ratio
        );

        glBindVertexArray(VAO);
        glDrawElements(
            GL_TRIANGLES, // the mode we want to draw
            6, // count or number of elements we'd like to draw
            GL_UNSIGNED_INT, // the type of the indices
            0 // offset of the EBO
        );

        // unbind Vertex Array Object
        glBindVertexArray(0);

        // swap the screen buffers
        glfwSwapBuffers(window);
    }

    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // terminate GLFW, clearing any resources allocated by GLFW
    glfwTerminate();

    return 0;
}

