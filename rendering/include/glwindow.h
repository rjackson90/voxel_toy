#ifndef GL_WINODW_H
#define GL_WINDOW_H

// System includes
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <vector>

// Rendering includes
#include "error.h"

namespace Rendering
{
    class GLWindow
    {
    public:
        GLWindow(int, int, const std::string&, const std::vector<std::pair<SDL_GLattr, int>>&);
        ~GLWindow();

        void makeCurrent();
        void swap();

    private:
        SDL_Window* window;
        SDL_GLContext gl_context;
    };
}
#endif // GL_WINDOW_H
