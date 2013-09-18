#ifndef PROGRAM_H
#define PROGRAM_H

#include <GL/glew.h>
#include <string>
#include <iostream>

#include "fileops.h"

namespace Rendering
{
    class Program
    {
    public:
        Program(const std::string&, const std::string&);
        bool isValid() const;
        void bind() const;
        GLuint getProgramObj() const { return program_obj; }

    private:
        bool valid;
        GLuint program_obj;

        bool link();
        bool compileShader(const std::string&, GLuint);
    };
}

#endif // PROGRAM_H
