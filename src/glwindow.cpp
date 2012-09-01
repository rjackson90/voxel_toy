#include "glwindow.h"

int GLWindow::attribs[] = 
{
    GLX_X_RENDERABLE    , True,
    GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
    GLX_RENDER_TYPE     , GLX_RGBA_BIT,
    GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
    GLX_CONFIG_CAVEAT   , GLX_NONE,
    GLX_RED_SIZE        , 8,
    GLX_GREEN_SIZE      , 8,
    GLX_BLUE_SIZE       , 8,
    GLX_ALPHA_SIZE      , 8,
    GLX_DEPTH_SIZE      , 24,
    GLX_STENCIL_SIZE    , 8,
    GLX_DOUBLEBUFFER    , True,
    GLX_SAMPLE_BUFFERS  , 0,
    GLX_SAMPLES         , 0,
    None
};

GLWindow::GLWindow()
{
    width = 1024;
    height = 768;
    title = "";

    cout << "Requested a standard window, 1024x768, no title" << endl;
    init();
}

GLWindow::GLWindow(int width, int height, const char *title)
{
    this->width = width;
    this->height = height;
    this->title = title;
    
    cout << "Requested a non-standard window, " << width << 'x' << height << ", '" << title
         << "'" << endl;
    init();
}

void GLWindow::init()
{
    //Initialize context table
    contexts = new GLXContext[MAX_THREADS];
    for(int i = 0; i < MAX_THREADS; i++)
    {
        contexts[i] = NULL;
    }

    // Connect to the X server and get a pointer to a Display
    display = XOpenDisplay(0);
    int screen = DefaultScreen(display);
    XSync(display, False);
    cout << "Connected to X11 server." << endl;

    // Get a list of available GLXFBConfigs matching the filter parameters
    // Take the first one, because all members of the list meet requirements
    int count = 0;
    GLXFBConfig *configs = glXChooseFBConfig(display, screen, attribs, &count);
    config = configs[0];
    XSync(display, False);
    cout << count << " display profiles matched requested settings, using the first one." << endl;
    
    XFree(configs);


    // Get an X Visual from the selected GLXFBConfig
    XVisualInfo *visual = glXGetVisualFromFBConfig(display, config);
    Window root = RootWindow(display, visual->screen);
    XSync(display, False);
    if(visual != NULL)
        cout << "Got a valid X11 Visual from the chosen framebuffer, ID: 0x" << hex 
             << visual->visualid << dec << endl;
    else
        cout << "WARNING: retrieved an invalid X11 Visual from chosed framebuffer" << endl;

    // Create an XSetWindowAttributes object and assign it a Colormap
    XSetWindowAttributes swa;
    swa.colormap =  cmap  = XCreateColormap(display, root, visual->visual, AllocNone);
    swa.background_pixmap = None;
    swa.border_pixel      = 0;
    swa.event_mask        = StructureNotifyMask;
    XSync(display, False);
    cout << "Created colormap" << endl;

    // Create an X Window using the previously created Visual and XSetWindowAttributes objects
    window = XCreateWindow(display, root, 0, 0, width, height, 0, visual->depth, 
                           InputOutput, visual->visual,
                           CWBorderPixel | CWColormap | CWEventMask, &swa);
    XSync(display, False);
    if(window)
        cout << "Created new X11 window, onscreen" << endl;
    else
        cout << "WARNING: Failed to create new X11 window" << endl;
    
    XStoreName(display, window, title);
    XMapWindow(display, window);
    XFree(visual);

    // Create a GLXWindow using the GLXFBConfig and Window objects
    glWindow = glXCreateWindow(display, config, window, NULL);
    XSync(display, False);
    if(glWindow)
        cout << "Created a new GLX window" << endl;
    else
        cout << "WARNING: Failed to create a GLX window" << endl;
}

GLWindow::~GLWindow()
{
    cout << "Destroying GLWindow..." << endl;
    makeCurrent(true);

    for(int i = 0; i < MAX_THREADS; i++)
    {
        if(contexts[i] != NULL)
        {
            cout << "\t...destroying GLXContext " << i << endl;
            glXDestroyContext(display, contexts[i]);
            contexts[i] = NULL;
        }
    }
    delete[] contexts;

    glXDestroyWindow(display, glWindow);
    XDestroyWindow(display, window);
    XFreeColormap(display, cmap);
    XCloseDisplay(display);
    cout << "Done" << endl;
}

void GLWindow::makeCurrent(bool disconnect)
{
    if(disconnect)
    {
        glXMakeContextCurrent(display, None, None, NULL);
        return;
    }

    int id = (unsigned long)pthread_self % MAX_THREADS;
    if(contexts[id] == NULL)
    {
        contexts[id] = glXCreateNewContext(display, config, GLX_RGBA_TYPE, NULL, True);
        if(!contexts[id])
        {
            cout << "WARNING: Attempt to create GLXContext " << id << " failed" << endl;
            return;
        }
        else
        {
            cout << "Created new GLXContext, id " << id << endl;
        }
    }
    
    cout << "Switching GLXContext, new context id: " << id << endl;
    glXMakeContextCurrent(display, glWindow, glWindow, contexts[id]);
}

void GLWindow::swap()
{
    glXSwapBuffers(display, glWindow);
}
