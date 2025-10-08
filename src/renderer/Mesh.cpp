#include "Mesh.h"
#include "../core/Math.h"
#include "VertexBufferLayout.h"
#include <spdlog.h>



Mesh::Mesh(TextureFrame texture, const Vertex2(&data)[4], VertexBufferLayout vbl, unsigned int indices[6]) {
	m_texture = texture;
	std::copy(indices, indices + 6, m_indices);
	if (data != nullptr) {
		std::copy(std::begin(data), std::end(data), std::begin(m_data));
	}
	VertexArray* va = new VertexArray;
	spdlog::debug("gen vertex array: {}", va->getRenderId());
	VertexBuffer* vb = new VertexBuffer(data, sizeof(data));
	IndexBuffer* ib = new IndexBuffer(m_indices, 6);

	va->addBuffer(*vb, vbl);
	m_renderInfo = new RendererInfo(va, ib);
}

void Mesh::initBoxMesh() {
    Vertex2 vertices[4] = {
        // top-left
        Vertex2(-0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 1.0f, 1.0f),
        // bottom-left
        Vertex2(-0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 1.0f, 1.0f),
        // bottom-right
        Vertex2(0.5f, -0.5f,  1.0f, 1.0f,  1.0f, 1.0f, 1.0f),
        // top-right
        Vertex2(0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 1.0f, 1.0f)
    };


    VertexBufferLayout vbl;
    vbl.pushElement<float>(2, false); // pos
    vbl.pushElement<float>(2, false); // texcoord
    vbl.pushElement<float>(3, false); // color
    unsigned int indices[6]{
        0,1,2,
        2,3,0
    };

    m_boxMesh = new Mesh(TextureFrame(), vertices, vbl, indices);

}

Mesh::Mesh(const std::vector<Vertex2>& vertices,
    const std::vector<unsigned int>& indices,
    VertexBufferLayout vbl)
{
    m_dataVec = vertices;
    m_indicesVec = indices;

    VertexArray* va = new VertexArray;
    spdlog::debug("gen vertex array: {}", va->getRenderId());

    VertexBuffer* vb = new VertexBuffer(m_dataVec.data(),
        m_dataVec.size() * sizeof(Vertex2));
    IndexBuffer* ib = new IndexBuffer(m_indicesVec.data(),
        (unsigned int)m_indicesVec.size());

    va->addBuffer(*vb, vbl);
    m_renderInfo = new RendererInfo(va, ib);
}