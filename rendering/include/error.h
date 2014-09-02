#ifndef GLERROR_H
#define GLERROR_H

#include <GL/glew.h>
#include <iostream>
#include <string>
#include <sstream>

void FormatDebugOutputARB(std::string&, GLenum, GLenum, GLuint, GLenum, const char*);
void DebugCallBackARB(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar*, const GLvoid*);

#endif // GLERROR_H
