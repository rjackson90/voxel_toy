#include "glwindow.h"

GLWindow::GLWindow()
{
    width = 1024;
    height = 768;
    title = "";
    init();
}

GLWindow::GLWindow(int width, int height, const char *title)
{
    width = width;
    height = height;
    title = title;
    init();
}

void GLWindow::init()
{
    // Connect to the X server and get a pointer to a Display
    display = XOpenDisplay(0);
    int screen = DefaultScreen(display);

    // Get a list of available GLXFBConfigs matching the filter parameters
    // Take the first one, because all members of the list meet requirements
    int *ccount = NULL;
    GLXFBConfig *configs = glXChooseFBConfig(display, screen, attribs, ccount);
    config = configs[0];
    XFree(configs);


    // Get an X Visual from the selected GLXFBConfig
    XVisualInfo *visual = glXGetVisualFromFBConfig(display, config);
    Window root = RootWindow(display, visual->screen);

    // Create an XSetWindowAttributes object and assign it a Colormap
    XSetWindowAttributes swa;
    swa.colormap =  cmap  = XCreateColormap(display, root, visual->visual, AllocNone);
    swa.background_pixmap = None;
    swa.border_pixel      = 0;
    swa.event_mask        = StructureNotifyMask;

    // Create an X Window using the previously created Visual and XSetWindowAttributes objects
    window = XCreateWindow(display, root, 0, 0, width, height, 0, visual->depth, 
                           InputOutput, visual->visual,
                           CWBorderPixel | CWColormap | CWEventMask, &swa);
    XStoreName(display, window, title);
    XMapWindow(display, window);
    XFree(visual);

    // Create a GLXWindow using the GLXFBConfig and Window objects
    glWindow = glXCreateWindow(display, config, window, NULL);
}

GLWindow::~GLWindow()
{
    glXDestroyWindow(display, glWindow);
    XDestroyWindow(display, window);
    XFreeColormap(display, cmap);
    XCloseDisplay(display);
}
