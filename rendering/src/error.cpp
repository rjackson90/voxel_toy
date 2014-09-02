#include "error.h"

using namespace std;

void FormatDebugOutputARB(string& outString, GLenum source, GLenum type,
        GLuint id, GLenum severity, const char *msg)
{
    stringstream message;
    message << "OpenGL: " << msg << " [";

    // Push error source into stream
    message << " source=";
    switch(source)
    {
    case GL_DEBUG_SOURCE_API_ARB:               message << "API";              break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:     message << "WINDOW_SYSTEM";    break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:   message << "SHADER_COMPILER";  break;
    case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:        message << "THIRD_PARTY";      break;
    case GL_DEBUG_SOURCE_APPLICATION_ARB:       message << "APPLICATION";      break;
    case GL_DEBUG_SOURCE_OTHER_ARB:             message << "OTHER";            break;
    default:   message << "UNDEFINED(0x" << hex << source << dec << ")";  break;
    }

    // Push error type into stream
    message << " type=";
    switch(type)
    {
    case GL_DEBUG_TYPE_ERROR_ARB:               message << "ERROR";                 break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB: message << "DEPRECATED_BEHAVIOR";   break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:  message << "UNDEFINED_BEHAVIOR";    break;
    case GL_DEBUG_TYPE_PORTABILITY_ARB:         message << "PORTABILITY";           break;
    case GL_DEBUG_TYPE_PERFORMANCE_ARB:         message << "PERFORMANCE";           break;
    case GL_DEBUG_TYPE_OTHER_ARB:               message << "OTHER";                 break;
    default:   message << "UNDEFINED(0x" << hex << type << dec << ")";
    }

    // Push severity into stream
    message << " severity=";
    switch(severity)
    {
    case GL_DEBUG_SEVERITY_HIGH_ARB:    message << "HIGH";      break;
    case GL_DEBUG_SEVERITY_MEDIUM_ARB:  message << "MEDIUM";    break;
    case GL_DEBUG_SEVERITY_LOW_ARB:     message << "LOW";       break;
    }

    // Finish up the string
    message << " id=" << id << " ]";
    outString = message.str();
}

void DebugCallBackARB(GLenum source, GLenum type, GLuint id, GLenum severity, 
        __attribute__((unused))GLsizei length, const GLchar *message, 
        __attribute__((unused))const GLvoid *userParam)
{
    string text;
    FormatDebugOutputARB(text, source, type, id, severity, message);
    cerr << text << endl;
}
