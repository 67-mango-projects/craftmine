#include "Interface.h"
#include<ft2build.h>
#include <spdlog.h>
#include "Shader.h"
#include "../core/Controller.h"
int Interface::addFont(const std::string& filePath) {
	m_fonts.push_back(new TextureFont(filePath,m_ftLib));
	int index = m_fonts.size() - 1; // index of the element we just added
	return index;
}

void Interface::renderImage(Vector2 position, Vector2 size, Texture* texture, Mesh* mesh, float zindex) {
    Quad q = Quad(mesh, position, size);
    q.zIndex = zindex;
    m_fontShader->bind();
    texture->bind();
    m_fontShader->setUniform1i("u_texture", 0);
    m_fontShader->setUniform1i("u_useTexture", 1);
    m_fontShader->setUniform1i("u_isFont", 0);
    m_fontShader->setUniform4f("u_color", 1,1,1,1);
    m_quadRenderer->renderOnScreen(&q, m_fontShader);
}
void Interface::handlePress(int key) {
    if (key == GLFW_MOUSE_BUTTON_LEFT) {
        Interface::currentInterface->m_leftMouseDown = true;
    }
}
void Interface::handleRelease(int key) {
    if (key == GLFW_MOUSE_BUTTON_LEFT) {
        Interface::currentInterface->m_leftMouseDown = false;
    }
}

Interface::Interface(Shader* fontShader, QuadRenderer* quad, FT_Library& lib, Vector2 windowSize) : m_fontShader(fontShader), m_quadRenderer(quad), m_ftLib(lib), m_windowSize(windowSize) {
    currentInterface = this;
    Controller::bindPress(handlePress);
    Controller::bindRelease(handleRelease);
}
void Interface::renderBox(Vector2 position, Vector2 size, Vector4 color, float zindex) {
    Quad q = Quad(Mesh::getBoxMesh(), position, size);
    q.zIndex = zindex;
    m_fontShader->bind();
    m_fontShader->setUniform1i("u_texture", 0);
    m_fontShader->setUniform1i("u_useTexture", 0);
    m_fontShader->setUniform1i("u_isFont", 0);
    m_fontShader->setUniform4f("u_color", color.x, color.y, color.z, color.w);
    m_quadRenderer->renderOnScreen(&q, m_fontShader);
}

bool Interface::renderButton(const std::string& text, int fontIndex, Vector2 position, Vector2 size, Vector4 color,float textscale, Vector2 textOffset, float zindex) {
    if (fontIndex < 0 || fontIndex >= (int)m_fonts.size()) {
        spdlog::error("Invalid font index: {}", fontIndex);
        return false;   
    }
    Vector2 mousePos = Controller::getMousePositionScreen();
    bool hovered = (mousePos.x >= position.x &&
        mousePos.x <= position.x + size.x &&
        mousePos.y >= position.y &&
        mousePos.y <= position.y + size.y);
    bool clicked = false;


    if (hovered && m_leftMouseDown && !m_leftMouseDownLastFrame)
        clicked = true;

    Vector4 modColor = color;
    if (hovered)
        color = Vector4(color.x + 0.1f, color.y + 0.1f, color.z + 0.1f, 1.0f);
    if (hovered && m_leftMouseDown)
        color = Vector4(color.x * 0.8f, color.y * 0.8f, color.z * 0.8f, 1.0f);

    renderBox(position, size, color, zindex);
    Vector2 textPos(position.x+ textOffset.x,
        position.y+ textOffset.y);

    renderText(text, fontIndex, textPos, textscale , Vector4(1, 1, 1, 1), zindex + 0.01f);

    return clicked;
}

void Interface::renderText(const std::string& text, int fontIndex, Vector2 position, float scale, Vector4 color, float zindex) {
    if (fontIndex < 0 || fontIndex >= (int)m_fonts.size()) {
        spdlog::error("Invalid font index: {}", fontIndex);
        return;
    }
    m_fontShader->bind();
    m_fontShader->setUniform1i("u_texture", 0);
    m_fontShader->setUniform1i("u_useTexture", 1);
    m_fontShader->setUniform1i("u_isFont", 1);
    TextureFont* font = m_fonts[fontIndex];
    font->bind(); // bind the font atlas
    
    // start pen position
    float x = position.x;
    float y = position.y;

    for (char c : text) {
        auto it = font->getCharacters().find(c);
        if (it == font->getCharacters().end()) continue;

        Character* ch = it->second;

        Quad q(&ch->mesh,
            Vector2(x + ch->bearing.x * scale,
                y - (ch->size.y - ch->bearing.y) * scale),
            Vector2(ch->size.x * scale, ch->size.y * scale));
        q.zIndex = zindex;
        m_fontShader->setUniform4f("u_color", color.x, color.y, color.z, color.w);
        m_quadRenderer->renderOnScreen(&q, m_fontShader);

        x += ch->advance * scale;
    }


    font->unbind();
}
