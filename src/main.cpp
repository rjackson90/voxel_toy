#include <GL/glew.h>
#include "glwindow.h"
#include "meshes.h"

#include <iostream>
#include <unistd.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

static const int WINDOW_WIDTH = 1024;
static const int WINDOW_HEIGHT = 768;

int main()
{
    cout << "Started program" << endl;

    // Create a new window, initialize OpenGL extensions
    cout << "Creating new window" << endl;
    GLWindow window = GLWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Demo");
    window.makeCurrent();
    GLenum err = glewInit();
    if(err != GLEW_OK)
    {
        // Something's up, GLEW initialization should never fail
        cout << "GLEW initialization failed: " << glewGetErrorString(err) << endl;
        return 1;
    }
    cout << "GLEW initialization OK. Version string: " << glGetString(GL_VERSION) << endl;

    // OpenGL scene initialization

    // Scene Matrix initialization
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 modelMatrix;
    glm::mat4 mvp;

    projectionMatrix = glm::perspective(90.0f,                                       // FOV 
                                        (float)WINDOW_WIDTH / (float) WINDOW_HEIGHT, // Viewport
                                        0.1f,                                        // z-min
                                        100.0f);                                     // z-max
    viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f, -5.0f));
    modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
    // mvp = projectionMatrix * viewMatrix * modelMatrix;
    cout << "Created mvp matrix" << endl;

    // Enable OpenGL functionality
    glEnable(GL_DEPTH_TEST);


    // Draw the scene
    int max = 5000;
    float shade = 0.0f;
    float rot = 1.0f / (float) max;
    Mesh cube;
    test_cube(cube);
    cube.loadProgram("res/flat.vs","res/flat.fs");
    for(int i = max; i > 0; i--)
    {
        shade = (float)i/max;
        glClearColor(shade, shade, shade, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        modelMatrix = glm::rotate(modelMatrix, rot * i, glm::vec3(0.0f, 1.0f, 0.0f));
        mvp = projectionMatrix * viewMatrix * modelMatrix;

        cube.draw(mvp);
        window.swap();
    }
    
    return 0;
}
