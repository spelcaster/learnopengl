#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// window dimension
const GLuint kWidth  = 800;
const GLuint kHeight = 600;

// prototypes
void keyEventHandler (GLFWwindow*, int, int, int, int);

// vertex shader
const GLchar* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
    "}\0";

// fragment shader
const GLchar* fragmentShaderSource = "#version 330 core\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

const GLchar* fragmentShaderCustomSource = "#version 330 core\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "color = vec4(1.0f, 1.0f, 0.0f, 1.0f)\n"
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
    glfwSetKeyCallback(window, keyEventHandler);

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

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint shaderProgram;

    std::cout << "Compile: Vertex Shader" << std::endl;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << std::endl;
        std::cout << log << std::endl;
    }

    std::cout << "Compile: Fragment Shader" << std::endl;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << std::endl;
        std::cout << log << std::endl;
    }

    std::cout << "Linking shaders to program" << std::endl;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, log);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED" << std::endl;
        std::cout << log << std::endl;
    }

    // after the program link we don't need the shader objects anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // initialize triangle vertices in normalized device coordinates (NDC)
    GLfloat vertices[] = {
         0.5f,  0.5f, 0.0f, // Top Right
         0.5f, -0.5f, 0.0f, // Bottom Right
        -0.5f, -0.5f, 0.0f, // Bottom Left
        -0.5f,  0.5f, 0.0f, // Top Left
         0.6f,  0.0f, 0.0f, // Left point
         0.75f,  0.4f, 0.0f, // Top point
         0.9f,  0.0f, 0.0f, // Right point
         0.75f, -0.4f, 0.0f  // Bottom Point
    };

    GLuint indices[] = {
        0, 1, 3, // First Triangle
        1, 2, 3, // Second Triangle
        5, 4, 6, // Third Triangle
        7, 6, 4  // Fourth Triangle
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

    std::cout << "Managing VAO, VBO AND EBO" << std::endl;
    // ..:: Initialization code (done once (unless your object frequently changes)) ::..
    // 1. Bind Vertex Array Object
    glBindVertexArray(VAO);

    // 2. Bind Vertex Buffer Object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 3. Bind Element Buffer Object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW
    );

    // 4. Set the vertex attributes pointers
    glVertexAttribPointer(
        0, // which vertex we want to configure
        3, // the size of the vertex attribute
        GL_FLOAT, // the type of the data (a vec* in GLSL consists of floating point values)
        GL_FALSE, // if we want the data to be normalized
        3 * sizeof(float), // the space between consecutive vextex attribute sets
        (GLvoid*)0 // the offset of where the position data begins in the buffer
    );
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // 5. Unbind the VAO
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
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(
            GL_TRIANGLES, // the mode we want to draw
            12, // count or number of elements we'd like to draw
            GL_UNSIGNED_INT, // the type of the indices
            0 // offset of the EBO
        );
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

// key event handler
void keyEventHandler (
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


