#pragma once
#include "Buffer.h"
class VertexBuffer : public Buffer
{
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void bind() const override;
	void unbind() const override;
};
