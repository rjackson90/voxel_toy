#ifndef GL_WINODW_H
#define GL_WINDOW_H

// System includes
#include "SDL.h"
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <memory>

// Rendering includes
#include "error.h"

namespace Rendering
{
    class GLWindow
    {
    public:
        GLWindow();
        ~GLWindow();
        bool init(int, int, const std::string&);

        void makeCurrent();
        void swap();

        std::pair<int,int> getDimensions();

    private:
        SDL_Window* window;
        SDL_GLContext gl_context;
    };
}
#endif // GL_WINDOW_H
