#include <GL/glew.h>
#include "glwindow.h"
#include "instrumentation.h"
#include "meshes.h"

#include <iostream>
#include <unistd.h>

using namespace std;

int main()
{
    instrumentation_status(true);
    cout << "Started program" << endl;

    cout << "Creating new window" << endl;
    GLWindow window = GLWindow(1024, 768, "OpenGL Demo");

    window.makeCurrent();

    GLenum err = glewInit();
    if(err != GLEW_OK)
    {
        // Something's up, GLEW initialization should never fail
        cout << "GLEW initialization failed: " << glewGetErrorString(err) << endl;
        instrumentation_status(false);
        return 1;
    }
    cout << "GLEW initialization OK. Version string: " << glGetString(GL_VERSION) << endl;

    int max = 10000;
    float shade = 0.0f;
    Mesh cube;
    test_cube(cube);
    for(int i = max; i > 0; i--)
    {
        shade = (float)i/max;
        glClearColor(shade, shade, shade, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        cube.draw();
        window.swap();
    }
    
    cout << "Shutting down" << endl;
    instrumentation_status(false);
    return 0;
}
