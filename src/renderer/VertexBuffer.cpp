#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
	glCall(glGenBuffers(1, &m_renderId));
	glCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderId));
	glCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
}
VertexBuffer::~VertexBuffer() {
	glCall(glDeleteBuffers(1, &m_renderId));
}

void VertexBuffer::bind() const {
	glCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderId));
}
void VertexBuffer::unbind() const {
	glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}