#include "Font.h"
#include <spdlog.h>
#include "VertexBufferLayout.h"

TextureFont::TextureFont(const std::string& fontPath, const FT_Library& lib) {
    m_fontfilePath = fontPath;
    FT_Face face;
    if (FT_New_Face(lib, fontPath.c_str(), 0, &face)) {
        spdlog::error("could not load font {}", fontPath);
        return;
    }
    FT_Set_Pixel_Sizes(face, 0, 48);

    m_atlasSize = Vector2(0, 0);

    // calculate atlas size (pack in a single row)
    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) continue;
        m_atlasSize.x += face->glyph->bitmap.width;
        m_atlasSize.y = std::max((int)m_atlasSize.y, (int)face->glyph->bitmap.rows);
    }

    // create atlas texture
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
    glCall(glGenTextures(1, &m_renderId));
    glCall(glBindTexture(GL_TEXTURE_2D, m_renderId));
    glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
        (int)m_atlasSize.x, (int)m_atlasSize.y,
        0, GL_RED, GL_UNSIGNED_BYTE, nullptr));

    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

    // upload glyphs + build characters
    int x = 0;
    spdlog::info("loading {} font glyphs", fontPath);

    for (unsigned char c = 0; c < 128; c++) {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
        spdlog::warn("could not load glyph: {}", c);
        continue;
    }

    FT_GlyphSlot glyph = face->glyph;

    glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0,
        glyph->bitmap.width, glyph->bitmap.rows,
        GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);

    float u0 = (float)x / m_atlasSize.x;
    float v0 = 0.0f;
    float u1 = (float)(x + glyph->bitmap.width) / m_atlasSize.x;
    float v1 = (float)glyph->bitmap.rows / m_atlasSize.y;

    Vertex2 vertices[4] = {
        Vertex2(-0.5f,  0.5f, u0, v0, 1,1,1),
        Vertex2(-0.5f, -0.5f, u0, v1, 1,1,1),
        Vertex2(0.5f, -0.5f, u1, v1, 1,1,1),
        Vertex2(0.5f,  0.5f, u1, v0, 1,1,1),
    };

    VertexBufferLayout vbl;
    vbl.pushElement<float>(2, false);
    vbl.pushElement<float>(2, false);
    vbl.pushElement<float>(3, false);
    unsigned int indices[6]{ 0, 1, 2, 2, 3, 0 };

    Mesh mesh(TextureFrame(u0, v0, u1, v1), vertices, vbl, indices);

    Character* ch = new Character{
        mesh,
        Vector2(glyph->bitmap.width, glyph->bitmap.rows),
        Vector2(glyph->bitmap_left, glyph->bitmap_top),
        (unsigned int)(glyph->advance.x >> 6)
    };
    m_characters.insert({ c, ch });

    x += glyph->bitmap.width;
}


    spdlog::info("finished loading {}", fontPath);
    FT_Done_Face(face);
}



void TextureFont::bind(unsigned int slot) const {
    glCall(glActiveTexture(GL_TEXTURE0 + slot));
    glCall(glBindTexture(GL_TEXTURE_2D, m_renderId));
}
void TextureFont::unbind() const {
    glCall(glBindTexture(GL_TEXTURE_2D, 0));
}