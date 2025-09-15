
#include "Renderer.h"
#include<iostream>

void glClearError() {
    while (glGetError() != GL_NO_ERROR);

}

bool glLogCall(const char* func, const char* file, int line) {
    while (GLenum gl_error = glGetError()) {

        printf("[opengl_error] (%X), function (%s), line (%i), file (%s) \n", gl_error, func, line, file);
        std::cout << "" << gl_error << '\n';
        return false;
    }
    return true;
}
