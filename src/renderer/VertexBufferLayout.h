#pragma once

#include<glew.h>
#include <vector>

struct VertexBufferElement {
	unsigned int type; // OpenGl type
	unsigned int count;
	unsigned char normalized;

	static unsigned int getSizeOfType(unsigned int type) {
		switch (type)
		{
		case GL_BYTE:            return sizeof(GLbyte);
		case GL_UNSIGNED_BYTE:   return sizeof(GLubyte);
		case GL_SHORT:           return sizeof(GLshort);
		case GL_UNSIGNED_SHORT:  return sizeof(GLushort);
		case GL_INT:             return sizeof(GLint);
		case GL_UNSIGNED_INT:    return sizeof(GLuint);
		case GL_FLOAT:           return sizeof(GLfloat);
		case GL_DOUBLE:          return sizeof(GLdouble);
		case GL_2_BYTES:         return 2;
		case GL_3_BYTES:         return 3;
		case GL_4_BYTES:         return 4;
		default:
			// Unknown type
			return 0;
		}
	}
};

class VertexBufferLayout
{
	std::vector<VertexBufferElement> m_elements;
	unsigned int m_stride;
public:
	VertexBufferLayout() : m_stride(0) {}
	template <typename T>
	void pushElement(unsigned int count, unsigned char normalized) {
		static_assert(false);
	}

	template<>
	void pushElement<float>(unsigned int count, unsigned char normalized) {
		m_elements.push_back(VertexBufferElement{ GL_FLOAT,count, false });
		m_stride += VertexBufferElement::getSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void pushElement<unsigned int>(unsigned int count, unsigned char normalized) {
		m_elements.push_back(VertexBufferElement{ GL_UNSIGNED_INT,count,normalized });
		m_stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void pushElement<unsigned char>(unsigned int count, unsigned char normalized) {
		m_elements.push_back(VertexBufferElement{ GL_UNSIGNED_BYTE,count,normalized });
		m_stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE) * count;
	}
	inline const std::vector<VertexBufferElement> getElements() const { return m_elements; }
	inline unsigned int getStride() const { return m_stride; }
};