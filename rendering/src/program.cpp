#include "program.h"

using namespace Rendering;

Program::Program()
{
    // Create program and shader objects
    program_obj = glCreateProgram();
}

ProgramPtr Program::ProgramFromConfig(const Core::ConfigParser &parser, const std::string &section)
{
    auto pgm = std::make_shared<Program>();
    bool result = false;
    try
    {
        // compile vertex shader
        if(parser.exists("vertexShader", section))
        {
            result = pgm->attachShader(
                    parser.get("vertexShader", section), ShaderType::VertexShader);
            if(!result) throw std::runtime_error("Failed to compile vertex shader");
        }

        // compile fragment shader
        if(parser.exists("fragmentShader", section))
        {
            result = pgm->attachShader(
                    parser.get("fragmentShader", section), ShaderType::FragmentShader);
            if(!result) throw std::runtime_error("Failed to compile fragment shader");
        }

        // link the program if the shaders compiled
        if(result)
        {
            if(! (result = pgm->link()) ) throw std::runtime_error("Linking failed");
        }
    }
    catch(const std::exception &ex)
    {
        std::cerr << "Error creating program: " << ex.what() << std::endl;
    }

    return pgm;
}

bool Program::attachShader(const std::string &path, ShaderType shader_type)
{
    GLuint shader_obj = glCreateShader(shader_type);
    if(!compileShader(path, shader_obj)) return false;

    glAttachShader(program_obj, shader_obj);
    return true;
}

void Program::bind() const {
    glUseProgram(program_obj);
}

GLuint Program::getProgramObj() const {
    return program_obj;
}

bool Program::compileShader(const std::string &path, GLuint shader_obj)
{
    std::cout << "Compiling shader " << path << "... ";

    // Get text from file
    std::string source(Core::getText(path));

    // Send source to GPU for compilation
    const char *source_ptr = source.c_str();
    glShaderSource(shader_obj, 1, &source_ptr, NULL);
    glCompileShader(shader_obj);

    // Report any errors
    GLint compiled;
    glGetShaderiv(shader_obj, GL_COMPILE_STATUS, &compiled);
    if(!compiled)
    {
        std::cout << "FAILURE:" << std::endl;
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

    std::cout << "OK!" << std::endl;
    return true;
}

bool Program::link()
{
    std::cout << "Linking program... ";
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

    std::cout << "OK!" << std::endl;
    return true;
}

