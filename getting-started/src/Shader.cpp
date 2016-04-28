#include "Shader.hpp"

/*!
 * Shader constructor
 *
 * \param[in] vertex_path   The path to the vertex shader source code
 * \param[in] fragment_path The path to the fragment shader source code
 */
Shader::Shader (const GLchar* vertex_path, const GLchar* fragment_path)
{
    std::string tmp;
    GLuint vertex_shader;
    GLuint fragment_shader;

    tmp = readShaderFile(vertex_path);
    vertex_shader = compileVertexShader(tmp.c_str());

    tmp = readShaderFile(fragment_path);
    fragment_shader = compileFragmentShader(tmp.c_str());

    linkProgram(vertex_shader, fragment_shader);
}

/*!
 * Shader destructor
 */
Shader::~Shader ()
{
}

/*!
 * Load the shader file
 *
 * \param[in] filepath The path to the shader source code
 *
 * \return The shader source code
 */
std::string Shader::readShaderFile (const GLchar* filepath)
{
    std::string code;
    std::ifstream file;

    file.exceptions(std::ifstream::badbit);

    try {
        file.open(filepath);

        file.seekg(0, std::ios::end);

        code.reserve(file.tellg());

        file.seekg(0, std::ios::beg);

        code.assign(
            (std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>()
        );

        file.close();
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FAILED_TO_READ_FILE" << std::endl;
    }

    return code;
}

/*!
 * Compile the vertex shader
 *
 * \param[in] code The vertex shader source code
 *
 * \return Resource id
 */
GLuint Shader::compileVertexShader (const GLchar* code)
{
    GLint success;
    GLchar log[512];
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertex, 1, &code, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n";
        std::cout << log << std::endl;
    }

    return vertex;
}

/*!
 * Compile the fragment shader
 *
 * \param[in] code The fragment shader source code
 *
 * \return Resource id
 */
GLuint Shader::compileFragmentShader (const GLchar* code)
{
    GLint success;
    GLchar log[512];
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragment, 1, &code, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n";
        std::cout << log << std::endl;
    }

    return fragment;
}

/*!
 * Create a link between shaders and the program
 *
 * \param[in] vertex_shader   The resource id for the vertex shader
 * \param[in] fragment_shader The resource id for the fragment shader
 *
 * \return void
 */
void Shader::linkProgram (GLuint vertex_shader, GLuint fragment_shader)
{
    GLint success;
    GLchar log[512];

    program_ = glCreateProgram();
    glAttachShader(program_, vertex_shader);
    glAttachShader(program_, fragment_shader);

    glLinkProgram(program_);

    glGetProgramiv(program_, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(program_, 512, NULL, log);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n";
        std::cout << log << std::endl;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

/*!
 * Get the program
 *
 * \return The program resource id
 */
GLuint Shader::getProgram ()
{
    return program_;
}

/*!
 * Use the shader program
 *
 * \return void
 */
void Shader::use ()
{
    glUseProgram(program_);
}
