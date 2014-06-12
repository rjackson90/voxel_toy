#include "glwindow.h"

using namespace Rendering;
using std::pair;
using std::vector;

GLWindow::GLWindow() : window(nullptr), gl_context(nullptr) {}

bool GLWindow::init(int width, int height, const std::string& title)
{
    vector< pair< SDL_GLattr, int > > attribs = {
        {SDL_GL_RED_SIZE, 8},
        {SDL_GL_GREEN_SIZE, 8},
        {SDL_GL_BLUE_SIZE, 8},
        {SDL_GL_ALPHA_SIZE, 8},
        {SDL_GL_ACCELERATED_VISUAL, 1},
        {SDL_GL_CONTEXT_MAJOR_VERSION, 3},
        {SDL_GL_CONTEXT_MINOR_VERSION, 3},
        {SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE},
        {SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG}
    };
    // Iterate over user-supplied attributes
    for(const auto &pair : attribs)
    {
        // Set the attribute
        if(SDL_GL_SetAttribute(pair.first, pair.second) < 0)
        {
            // An error occured
            std::cerr << "Error: " << SDL_GetError() << std::endl;
        }
    }

    // Enumerate available displays
    std::cout << "Available displays: " << std::endl;
    
    int displays = SDL_GetNumVideoDisplays();
    SDL_Rect* dpy_bounds = new SDL_Rect();

    for(int i = 0; i < displays; i++)
    {
        const char* dpy_name = SDL_GetDisplayName(i);

        if(dpy_name != NULL){
            SDL_GetDisplayBounds(i, dpy_bounds);
            std::cout << "[" << i << "]: " << dpy_name << "\t"
                      << "(" << dpy_bounds->w << "x" << dpy_bounds->h << ")@(" 
                      << dpy_bounds->x << "," << dpy_bounds->y << ")" << std::endl;
        } else {
            std::cerr << "[ERROR]" <<  SDL_GetError() << std::endl;
            break;
        }
    }

    delete dpy_bounds;


    // Create a window, checking for errors
    std::cout << "Creating a new window...";
    window = SDL_CreateWindow(
            title.c_str(),
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            width,
            height,
            SDL_WINDOW_OPENGL
            );
    if(window == NULL)
    {
        std::cout << " FAIL: " << SDL_GetError() << std::endl;
        std::cout << "Attempting to recover by getting a non-debug context...";
        // Try to recover by getting a non-debug compatibility context
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, 0);

        window = SDL_CreateWindow(
                title.c_str(),
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                width,
                height,
                SDL_WINDOW_OPENGL
                );
        if(window == NULL)
        {
            std::cout << "FAIL: " << SDL_GetError() << std::endl;
            return false;
        }
    }
    std::cout << " OK!" << std::endl;

    // Create a context, checking for errors
    std::cout << "Creating a new OpenGL context...";
    gl_context = SDL_GL_CreateContext(window);
    if(gl_context == NULL)
    {
        std::cout << " FAIL: " << SDL_GetError() << std::endl;
        return false;
    }
    std::cout << " OK!" << std::endl;
    SDL_GL_MakeCurrent(window, gl_context);

    // Initialize GLEW
    std::cout << "Binding pointers to available OpenGL functions...";
    glewExperimental = GL_TRUE;
    GLenum error = glewInit();
    if(error != GLEW_OK)
    {
        std::cout << " FAIL: " << glewGetErrorString(error) << std::endl;
        return false;
    }
    std::cout << " OK!" << std::endl;

    // Register a callback to handle debug output, if supported
    std::cout << "Checking for GL_ARB_debug_output support...";
    if(GLEW_ARB_debug_output)
    {
        std::cout << " OK!" << std::endl;
        glDebugMessageCallbackARB(DebugCallBackARB, nullptr);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
    }else {
        std::cout << " NOT SUPPORTED" << std::endl;
    }

    return true;
}

GLWindow::~GLWindow()
{
    std::cout << "Destroying window" << std::endl;
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
}

void GLWindow::makeCurrent()
{
    // Check window and context for validity before doing context switch
    if( window != NULL && gl_context != NULL ){
        SDL_GL_MakeCurrent(window, gl_context);
    }
}

void GLWindow::swap()
{
    if( window != nullptr ){
        SDL_GL_SwapWindow(window);
    }
}

std::pair<int, int> GLWindow::getDimensions() 
{
    if( window != nullptr ){
        int width = 0, height = 0;
        SDL_GetWindowSize(window, &width, &height);
        return std::pair<int, int>(width, height);
    }
    return std::pair<int, int> (0,0);
}
