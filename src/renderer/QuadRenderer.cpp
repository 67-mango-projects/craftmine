#include "QuadRenderer.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include <spdlog.h>
#include <glm/gtc/matrix_transform.hpp>

void QuadRenderer::loadMeshesFromTexture(Texture *t) {
	const auto& map = t->getTextureMap();
	VertexBufferLayout defaultQuadLayout;
    defaultQuadLayout.pushElement<float>(2, false);
    defaultQuadLayout.pushElement<float>(2, false);
    defaultQuadLayout.pushElement<float>(3, false);


    
	for (const auto& texture : map) {
        
        Vertex2 defaultQuadData[4] = { // Default Quad Data
    { -0.5f,  0.5f, texture.second.left,  texture.second.bottom, 1, 1, 1 },
    { -0.5f, -0.5f, texture.second.left,  texture.second.top,    1, 1, 1 },
    {  0.5f,  0.5f, texture.second.right, texture.second.bottom, 1, 1, 1 },
    {  0.5f, -0.5f, texture.second.right, texture.second.top,    1, 1, 1 }
        };
        unsigned int m_indices[6]{
            0,1,2,
            2,3,1
        };
		m_meshes[texture.first] = new Mesh(texture.second, defaultQuadData, defaultQuadLayout, m_indices);
        spdlog::debug("loading mesh: TEXTURE ({0}), ", texture.first);
	}
}
const Mesh* QuadRenderer::getMesh(std::string name) {
    if (m_meshes.find(name) == m_meshes.end()) {
        spdlog::error("could not find mesh {}", name.c_str());
        return new Mesh;
    }
    return m_meshes[name];
} 
void QuadRenderer::render(const Quad* q, bool usingTexture /*convenient for debugging hitboxes*/) const {
    auto renderInfo = q->mesh->getRendererInfo();
    Vector2 viewport = m_renderer->getViewPortSize();

    
    glm::mat4 model = q->getModelMatrix(); // convert vertex pos into world coordinates
    glm::mat4 view  = Camera::getCurrentCamera()->getViewMatrix(); // convert those into camera relative coordinates
    // implement an orthographic projection to convert into screen positions
    glm::mat4 proj = glm::ortho(
        -viewport.x * 0.5f, viewport.x * 0.5f,
        -viewport.y * 0.5f, viewport.y * 0.5f,
        -2.0f, 2.0f
    );
    // 67 mango tuff advanced math
    glm::mat4 mvp = proj * view * model;

    worldtexture->bind();
    worldshader->bind();
    worldshader->setUniform1i("u_useTexture", usingTexture);
    worldshader->setUniform4f("u_color", q->color.x, q->color.y, q->color.z, q->color.w);
    worldshader->setUniformMat4f("u_mvp", mvp);
    m_renderer->draw(renderInfo->m_vao, renderInfo->m_ibo, worldshader);
}
bool Quad::isCollidingWith(const Quad& other) const {
    // Get both sets of corners
    auto getCorners = [](const Quad& q) {
        float hw = q.size.x * 0.5f;
        float hh = q.size.y * 0.5f;
        float rad = glm::radians(q.rotation);

        glm::vec2 center(q.position.x, q.position.y);
        glm::vec2 axes[2] = {
            glm::vec2(std::cos(rad), std::sin(rad)),        // X axis
            glm::vec2(-std::sin(rad), std::cos(rad))        // Y axis
        };

        glm::vec2 corners[4];
        corners[0] = center + (axes[0] * -hw) + (axes[1] * hh); // top-left
        corners[1] = center + (axes[0] * -hw) + (axes[1] * -hh); // bottom-left
        corners[2] = center + (axes[0] * hw) + (axes[1] * -hh); // bottom-right
        corners[3] = center + (axes[0] * hw) + (axes[1] * hh); // top-right

        return std::array<glm::vec2, 4>{ corners[0], corners[1], corners[2], corners[3] };
        };

    auto A = getCorners(*this);
    auto B = getCorners(other);

    auto overlapOnAxis = [](const std::array<glm::vec2, 4>& a, const std::array<glm::vec2, 4>& b, const glm::vec2& axis) {
        auto project = [&](const std::array<glm::vec2, 4>& pts) {
            float min = glm::dot(axis, pts[0]);
            float max = min;
            for (int i = 1; i < 4; ++i) {
                float p = glm::dot(axis, pts[i]);
                if (p < min) min = p;
                if (p > max) max = p;
            }
            return std::pair<float, float>(min, max);
            };

        auto [minA, maxA] = project(a);
        auto [minB, maxB] = project(b);

        return !(maxA < minB || maxB < minA);
        };

    glm::vec2 axes[4] = {
        glm::normalize(A[1] - A[0]),
        glm::normalize(A[3] - A[0]),
        glm::normalize(B[1] - B[0]),
        glm::normalize(B[3] - B[0])
    };

    for (const auto& axis : axes)
        if (!overlapOnAxis(A, B, axis))
            return false;

    return true;
}

Vector2 QuadRenderer::worldToScreen(const Vector2& world) {
    glm::vec4 pos = glm::vec4(world.x, world.y, 0.0f, 1.0f);
    Vector2 viewport = m_renderer->getViewPortSize();

    // same as renderWorld
    glm::mat4 view = Camera::getCurrentCamera()->getViewMatrix();
    glm::mat4 proj = glm::ortho(
        -viewport.x * 0.5f, viewport.x * 0.5f,
        -viewport.y * 0.5f, viewport.y * 0.5f,
        -2.0f, 2.0f
    );

    glm::vec4 clip = proj * view * pos;
    glm::vec3 ndc = glm::vec3(clip) / clip.w;

    Vector2 screen;
    screen.x = (ndc.x * 0.5f + 0.5f) * viewport.x;
    screen.y = (ndc.y * 0.5f + 0.5f) * viewport.y;

    return screen;
}

void QuadRenderer::renderOnScreen(const Quad* q, Shader* shader) const { // same thing as render function but without viewmatrix cuz guis dont need cameras
    auto renderInfo = q->mesh->getRendererInfo();
    Vector2 viewport = m_renderer->getViewPortSize();

    // Adjust position to bottom-left alignment since quad is centered
    Vector2 alignedPos = q->position + q->size * 0.5f;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(alignedPos.x, alignedPos.y, q->zIndex));
    model = glm::rotate(model, glm::radians(q->rotation), glm::vec3(0, 0, 1));
    model = glm::scale(model, glm::vec3(q->size.x, q->size.y, 1));

    // worst possible fucking decision to keep these ortho ranges inconsistent, completely fucked over my rendering and gui code
    glm::mat4 proj = glm::ortho(
        0.0f, viewport.x,
        0.0f, viewport.y,
        -2.0f, 2.0f
    );

    glm::mat4 mvp = proj * model;

    shader->setUniformMat4f("u_mvp", mvp);
    m_renderer->draw(renderInfo->m_vao, renderInfo->m_ibo, shader);
}

glm::mat4 Quad::getModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(position.x, position.y,zIndex));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 0, 1));
    model = glm::scale(model, glm::vec3(size.x, size.y, 1));

    return model;
}