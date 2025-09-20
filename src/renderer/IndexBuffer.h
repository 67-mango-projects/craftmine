#pragma once
#include "Buffer.h"


class IndexBuffer : public Buffer
{
	unsigned int m_count;
public:

	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void bind() const override;
	void unbind() const override;

	inline unsigned int getCount() const { return m_count; }
};