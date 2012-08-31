#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <GL/gl.h>
#include <GL/glx.h>

#define GLX_CONTEXT_MAJOR_VERSION_ARB	0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB	0x2092

class GLWindow
{
    public:
        GLWindow();
        GLWindow(int, int, const char*);
        ~GLWindow();

    private:
        Display *display;
        GLXFBConfig config;
        Window window;
        GLXWindow glWindow;
        Colormap cmap;
        int width, height;
        const char* title;
        static int attribs[];
        
        void init();
};

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







































