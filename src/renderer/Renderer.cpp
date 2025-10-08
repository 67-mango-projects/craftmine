#include "Renderer.h"
#include "Shader.h"
#include<iostream>
#include <spdlog.h>
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

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    shader.bind();
    va.bind();
    ib.bind();
    
    glCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::draw(const VertexArray* va, const IndexBuffer* ib, Shader* shader) const {
    shader->bind();
    va->bind();
    ib->bind();
    
    glCall(glDrawElements(GL_TRIANGLES, ib->getCount(), GL_UNSIGNED_INT, nullptr));
}


void Renderer::clear() const {
    glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}