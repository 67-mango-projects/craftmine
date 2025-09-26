#include "QuadRenderer.h"
#include "VertexBufferLayout.h"
#include <spdlog.h>

void QuadRenderer::loadMeshesFromTexture(Texture t) {
	const auto& map = t.getTextureMap();
	VertexBufferLayout defaultQuadMesh;
	defaultQuadMesh.pushElement<float>(2, false);
	defaultQuadMesh.pushElement<float>(2, false);
	defaultQuadMesh.pushElement<float>(3, false);


    Vertex2 defaultQuadData[4] = { // Default Quad Data
       Vertex2(
           -1.0f,  1.0f, // pos
           0.0,1.0, // texture pos
           1.0, 0, 0.0 // color
       ),
       Vertex2(
           -1.0f, -1.0f,
           0.0,0.0,
           0.0, 0.0, 1.0
       ),
       Vertex2(
           1.0f,  1.0f,
           1.0,1.0,
           0.0, 1.0, 0
       ),
       Vertex2(
           1.0f, -1.0f,
           1.0,0.0,
           1.0, 1.0, 1.0
       )
    };
	for (const auto& texture : map) {
		m_meshes[texture.first] = Mesh(texture.second, defaultQuadData, defaultQuadMesh);
        spdlog::debug("loading mesh: TEXTURE ({0}), ", texture.first);
	}
}
Mesh QuadRenderer::getMesh(std::string name) {
    if (m_meshes.find(name) == m_meshes.end()) {
        spdlog::error("could not find mesh {}", m_meshes);
        return Mesh();
    }
    return m_meshes[name];
}