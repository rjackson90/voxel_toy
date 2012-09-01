#include "glwindow.h"
#include <iostream>
#include <unistd.h>

#include <GL/gl.h>
#include <GL/glx.h>

using namespace std;

int main()
{
    cout << "Started program" << endl;

    cout << "Creating new window" << endl;
    GLWindow window = GLWindow(1024, 768, "OpenGL Demo");

    window.makeCurrent();

    int max = 1;
    float shade = 0.0f;
    for(int i = max; i > 0; i--)
    {
        shade = (float)i/max;
        glClearColor(shade, shade, shade, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        window.swap();
    }
    
    cout << "Shutting down" << endl;
    return 0;
}
