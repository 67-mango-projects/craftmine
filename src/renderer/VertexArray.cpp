#include "VertexArray.h"
#include "Renderer.h"
VertexArray::VertexArray()
{
	glCall(glGenVertexArrays(1, &m_renderId));
	glCall(glBindVertexArray(m_renderId));
}

VertexArray::~VertexArray()
{
	glCall(glDeleteVertexArrays(1, &m_renderId));
}

void VertexArray::bind() const
{
	glCall(glBindVertexArray(m_renderId));
}

void VertexArray::unbind() const
{
	glCall(glBindVertexArray(0));
}


void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	bind();
	vb.bind();
	std::vector<VertexBufferElement> elements = layout.getElements();

	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {

		const auto& element = elements[i];
		glCall(glEnableVertexAttribArray(i));
		glCall(glVertexAttribPointer(i, element.count, element.type, (unsigned char)element.normalized, layout.getStride(), (void*)offset));
		offset += element.count * VertexBufferElement::getSizeOfType(element.type);
	}

}