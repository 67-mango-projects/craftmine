#include "Mesh.h"
#include "../core/Math.h"
#include "VertexBufferLayout.h"

Mesh::Mesh(TextureFrame texture, const Vertex2(&data)[4], VertexBufferLayout vbl) {
	m_texture = texture;
	if (data != nullptr) {
		std::copy(std::begin(data), std::end(data), std::begin(m_data));
	}
	VertexArray va;
	VertexBuffer vb(data, sizeof(data));
	IndexBuffer ib(m_indices, 6);

	va.addBuffer(vb,vbl);
	m_renderInfo = new RendererInfo(va,ib);
}