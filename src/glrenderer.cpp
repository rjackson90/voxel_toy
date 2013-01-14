#include "glrenderer.h"

GLRenderer::GLRenderer(int width, int height, const char* title)
{
    this->window = GLWindow(width, height, title);
}

GLRenderer::GLRenderer()
{
    this->window = GLWindow();
}
