#include <iostream>
#include <cmath>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// window dimension
const GLuint kWidth  = 800;
const GLuint kHeight = 600;

// prototypes
void event_handler (GLFWwindow*, int, int, int, int);

// vertex shader
const GLchar* kVertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "out vec4 orange_color;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
    "orange_color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

// fragment shader
const GLchar* kFragmentShaderOrange = "#version 330 core\n"
    "in vec4 orange_color;\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "color = orange_color;\n"
    "}\n\0";

const GLchar* kFragmentShaderDynamic = "#version 330 core\n"
    "out vec4 color;\n"
    "uniform vec4 dynamic_color;\n"
    "void main()\n"
    "{\n"
    "color = dynamic_color;\n"
    "}\n\0";

int main () {
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

    GLint success;
    GLchar log[512];

    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint shader_program;
    GLuint shader_program2;

    std::cout << "Compile: Vertex Shader" << std::endl;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &kVertexShaderSource, NULL);
    glCompileShader(vertex_shader);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << std::endl;
        std::cout << log << std::endl;
    }

    std::cout << "Compile: Fragment Shader" << std::endl;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &kFragmentShaderOrange, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << std::endl;
        std::cout << log << std::endl;
    }

    std::cout << "Linking shaders to program" << std::endl;
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, log);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED" << std::endl;
        std::cout << log << std::endl;
    }

    std::cout << "Compile: Fragment Shader" << std::endl;
    glShaderSource(fragment_shader, 1, &kFragmentShaderDynamic, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << std::endl;
        std::cout << log << std::endl;
    }

    std::cout << "Linking shaders to program" << std::endl;
    shader_program2 = glCreateProgram();
    glAttachShader(shader_program2, vertex_shader);
    glAttachShader(shader_program2, fragment_shader);
    glLinkProgram(shader_program2);

    glGetProgramiv(shader_program2, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program2, 512, NULL, log);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED" << std::endl;
        std::cout << log << std::endl;
    }

    // after the program link we don't need the shader objects anymore
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // initialize triangle vertices in normalized device coordinates (NDC)
    GLfloat first_triangle[] = {
         0.5f,  0.5f, 0.0f, // Top Right
         0.5f, -0.5f, 0.0f, // Bottom Right
        -0.5f, -0.5f, 0.0f, // Bottom Left
        -0.5f,  0.5f, 0.0f, // Top Left
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
    glVertexAttribPointer(
        0, // which vertex we want to configure
        3, // the size of the vertex attribute
        GL_FLOAT, // the type of the data (a vec* in GLSL consists of floating point values)
        GL_FALSE, // if we want the data to be normalized
        3 * sizeof(GLfloat), // the space between consecutive vextex attribute sets
        (GLvoid*)0 // the offset of where the position data begins in the buffer
    );
    glEnableVertexAttribArray(0);
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
        glUseProgram(shader_program);
        glBindVertexArray(VAO[0]);
        glDrawElements(
            GL_TRIANGLES, // the mode we want to draw
            6, // count or number of elements we'd like to draw
            GL_UNSIGNED_INT, // the type of the indices
            0 // offset of the EBO
        );

        glUseProgram(shader_program2);

        GLfloat time = glfwGetTime();
        GLfloat green_value = (sin(time) / 2) + 0.5;
        GLint location = glGetUniformLocation(
            shader_program2, "dynamic_color"
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


