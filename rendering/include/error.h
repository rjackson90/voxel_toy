#ifndef GLERROR_H
#define GLERROR_H

#include "dispatch.h"

bool _check_gl_error(const char *, int);
void _quit_false(bool);

#define glCheckError() _check_gl_error(__FILE__, __LINE__)
#define glQuitOnError() _quit_false(_check_gl_error(__FILE__, __LINE__))

#include <GL/glew.h>
#include <iostream>
#include <string>

#endif // GLERROR_H
