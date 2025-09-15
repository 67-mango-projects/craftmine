#pragma once

#include <glew.h>

class Buffer {
protected:
	unsigned int m_renderId;
public:
	Buffer() = default;
	virtual ~Buffer() = default;

	virtual void bind() const = 0;
	virtual void unbind() const = 0;

	inline unsigned int getId() const { return m_renderId; }

};