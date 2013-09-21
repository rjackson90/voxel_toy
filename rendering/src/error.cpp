#include "error.h"

using namespace std;

bool _check_gl_error(const char *file, int line)
{
    GLenum error = GL_NO_ERROR;
    bool no_error = true;

    while((error = glGetError()) != GL_NO_ERROR)
    {
        no_error = false;
        string errText;

        switch(error)
        {
            case GL_INVALID_OPERATION:  errText="INVALID_OPERATION";    break;
            case GL_INVALID_ENUM:       errText="INVALID_ENUM";         break;
            case GL_INVALID_VALUE:      errText="INVALID_VALUE";        break;
            case GL_OUT_OF_MEMORY:      errText="OUT_OF_MEMORY";        break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:  errText="INVALID_FRAMEBUFFER_OPERATION";    break;
        }

        cerr << "GL_" << errText << " at \"" << file << "\":" << line << endl;
    }

    return no_error;
}

void _quit_false(bool condition)
{
    if(!condition) Dispatch::quit();
}
