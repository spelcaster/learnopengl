/*!
 * \file  Shader.hpp
 * \brief Class definition to compile shaders and create a program to be used
 *        in an OpenGL program
 */

#ifndef __SHADER_HPP
#define __SHADER_HPP

#include <string>
#include <fstream>
#include <iostream>

#include <GL/glew.h>

//! Shader
/*!
 * Shader is used to read shaders from file, compile them and create a program
 * to be used in an OpenGL program
 */
class Shader
{
 private:
    /*!
     * The program that will be created
     */
    GLuint program_;

 public:
    /*!
     * Shader constructor
     *
     * \param[in] vertex_path   The path to the vertex shader source code
     * \param[in] fragment_path The path to the fragment shader source code
     */
    Shader (const GLchar* vertex_path, const GLchar* fragment_path);

    /*!
     * Shader destructor
     */
    ~Shader ();

    /*!
     * Load the shader file
     *
     * \param[in] filepath The path to the shader source code
     *
     * \return The shader source code
     */
    std::string readShaderFile (const GLchar* filepath);

    /*!
     * Compile the vertex shader
     *
     * \param[in] code The vertex shader source code
     *
     * \return Resource id
     */
    GLuint compileVertexShader (const GLchar* code);

    /*!
     * Compile the fragment shader
     *
     * \param[in] code The fragment shader source code
     *
     * \return Resource id
     */
    GLuint compileFragmentShader (const GLchar* code);

    /*!
     * Create a link between shaders and the program
     *
     * \param[in] vertex_shader   The resource id for the vertex shader
     * \param[in] fragment_shader The resource id for the fragment shader
     *
     * \return void
     */
    void linkProgram (GLuint vertex_shader, GLuint fragment_shader);

    /*!
     * Get the program
     *
     * \return The program resource id
     */
    GLuint getProgram ();

    /*!
     * Use the shader program
     *
     * \return void
     */
    void use ();
};

#endif // __SHADER_HPP
