#ifndef GL_WINDOW_H
#define GL_WINDOW_H

#include <list>
#include <iostream>
#include <pthread.h>
#include <string>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <GL/glew.h>
#include <GL/glx.h>

using namespace std;

#define MAX_THREADS 10

/* This class is a platform-independent wrapper around an OpenGL rendering context, as well as
 * the platform's conception of a window and any other ancillary data that needs to be hidden 
 * from the rest of the engine.
 */
class GLWindow
{
    public:
        GLWindow();
        GLWindow(int, int, string);
        ~GLWindow();
        
        void makeCurrent(bool=false);
        void swap();

    private:
        Display *display;
        GLXFBConfig config;
        Window window;
        GLXWindow glWindow;
        Colormap cmap;
        int width, height;
        string title;
        static int attribs[];
        GLXContext *contexts;
        
        void init();
};
#endif






































