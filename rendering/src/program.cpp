#include "program.h"

using namespace Rendering;

Program::Program(const std::string &vertex_path, const std::string &fragment_path)
{
    valid = false;

    // Create program and shader objects
    program_obj = glCreateProgram();
    GLuint vertex_obj = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_obj = glCreateShader(GL_FRAGMENT_SHADER);

    // Compile shaders
    valid = compileShader(vertex_path, vertex_obj) && compileShader(fragment_path, fragment_obj);
    if(!valid)
    {
        glDeleteProgram(program_obj);
        program_obj = 0;
        return;
    }

    // Attach shaders and link program
    glAttachShader(program_obj, vertex_obj);
    glAttachShader(program_obj, fragment_obj);
    valid = link();
    if(!valid)
    {
        glDeleteProgram(program_obj);
        program_obj = 0;
        return;
    }
}

bool Program::isValid() const
{
    return valid;
}

void Program::bind() const
{
    glUseProgram(program_obj);
}

bool Program::compileShader(const std::string &path, GLuint shader_obj)
{
    std::cout << "Compiling shader " << path << ".......... ";

    // Get text from file
    std::string source(getText(path));

    // Send source to GPU for compilation
    const char *source_ptr = source.c_str();
    glShaderSource(shader_obj, 1, &source_ptr, NULL);
    glCompileShader(shader_obj);

    // Report any errors
    GLint compiled;
    glGetShaderiv(shader_obj, GL_COMPILE_STATUS, &compiled);
    if(!compiled)
    {
        std::cout << "FAILURE!" << std::endl;
        GLint blen = 0;
        GLsizei slen = 0;

        glGetShaderiv(shader_obj, GL_INFO_LOG_LENGTH, &blen);

        if(blen > 1)
        {
            GLchar* compiler_log = new GLchar[blen];

            glGetShaderInfoLog(shader_obj, blen, &slen, compiler_log);
            std::cout << compiler_log << std::endl << std::endl;

            delete[] compiler_log;
        }
        else
        {
            std::cout << "No information available" << std::endl << std::endl;
        }
        return false;
    }

    std::cout << "Done." << std::endl;
    return true;
}

bool Program::link()
{
    std::cout << "Linking program.......... ";
    // Link program and check for errors
    glLinkProgram(program_obj);

    // Check for errors
    GLint linked;
    glGetProgramiv(program_obj, GL_LINK_STATUS, &linked);
    if(!linked)
    {
        std::cout << "FAILURE!" << std::endl;
        GLint blen = 0;
        GLsizei slen = 0;

        glGetProgramiv(program_obj, GL_INFO_LOG_LENGTH, &blen);

        if(blen > 1)
        {
            GLchar* link_log = new GLchar[blen];

            glGetProgramInfoLog(program_obj, blen, &slen, link_log);
            std::cout << link_log << std::endl << std::endl;

            delete[] link_log;
        }
        else
        {
            std::cout << "No information available" << std::endl << std::endl;
        }
        return false;
    }

    std::cout << "Done." << std::endl;
    return true;
}

