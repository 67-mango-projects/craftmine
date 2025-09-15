#pragma once

#include <glew.h>


#define assert(x) if (!(x)) {__debugbreak();}
#define glCall(x) \
    glClearError(); \
    x; \
    assert(glLogCall(#x,__FILE__,__LINE__))

void glClearError();
bool glLogCall(const char* func, const char* file, int line);