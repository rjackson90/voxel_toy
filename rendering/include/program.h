#ifndef PROGRAM_H
#define PROGRAM_H

#include <GL/glew.h>
#include <string>
#include <iostream>

#include "fileops.h"

namespace Rendering
{
    enum ShaderType {
        VertexShader = GL_VERTEX_SHADER,
        FragmentShader = GL_FRAGMENT_SHADER
    };

    class Program
    {
    public:
        Program();
        bool attachShader(const std::string&, ShaderType);        
        void bind() const;
        GLuint getProgramObj() const;
        bool link();

    private:
        GLuint program_obj;

        bool compileShader(const std::string&, GLuint);
    };
}

#endif // PROGRAM_H
