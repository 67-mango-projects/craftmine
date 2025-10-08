#pragma once
#include <glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "../core/Math.h"
#include "Texture.h"

#define assert(x) if (!(x)) {__debugbreak();}
#define glCall(x) \
    glClearError(); \
    x; \
    assert(glLogCall(#x,__FILE__,__LINE__))

void glClearError();
bool glLogCall(const char* func, const char* file, int line);

struct RendererInfo {
    VertexArray* m_vao;
    IndexBuffer* m_ibo;
    
    RendererInfo(VertexArray* vao, IndexBuffer* ibo) : m_ibo(ibo), m_vao(vao) {

    }
};

struct Vertex2 {
    Vector2 position;
    Vector2 texcoord;
    Vector3 color;
    inline Vertex2(const Vector2& pos, const Vector2& uv, const Vector3& col)
        : position(pos), texcoord(uv), color(col) {
    }   
    inline Vertex2() {};
    inline Vertex2(
        GLfloat x, GLfloat y,
        GLfloat xt, GLfloat yt,
        GLfloat r, GLfloat g, GLfloat b) : position( x,y ), texcoord(xt, yt), color(r, g,b) {
    }
};

class Shader;
class Renderer {   
private:
    inline static Texture* worldTextures; // in world textures
    const Vector2 m_viewPortSize = Vector2(900, 900);
public:
    void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void draw(const VertexArray* va, const IndexBuffer* ib, Shader* shader) const;
    void clear() const;
    static void setWorldTextures(Texture* texture) { worldTextures = texture; }
    static Texture* getWorldTextures() { return worldTextures; }
    inline Vector2 getViewPortSize() const { return m_viewPortSize; }

};