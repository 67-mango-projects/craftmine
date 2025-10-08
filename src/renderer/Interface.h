#pragma once

// for gui stuff

#include "../core/Math.h"
#include <string>
#include "QuadRenderer.h"
#include "Font.h"
typedef struct FT_LibraryRec_* FT_Library;
typedef struct FT_FaceRec_* FT_Face;

class Shader;

inline int globalPixelFont;

class Interface {
	Shader* m_fontShader;
	QuadRenderer* m_quadRenderer;
	FT_Library& m_ftLib;
	std::vector<TextureFont*> m_fonts;
	Vector2 m_windowSize;
	bool m_leftMouseDown = false;
	bool m_leftMouseDownLastFrame = false;
	inline static Interface* currentInterface;
public:	
	void update() { m_leftMouseDownLastFrame = m_leftMouseDown; }
	Interface(Shader* fontShader, QuadRenderer* quad, FT_Library& lib, Vector2 windowSize);
	/* font index */ int addFont(const std::string& fontPath);
	const Vector2& getWindowDimensions() { return m_windowSize; }
	void renderImage(Vector2 position, Vector2 size,Texture* texture,Mesh* mesh, float zindex = 1);
	void renderBox(Vector2 position, Vector2 size, Vector4 color,float zindex = 1);
	void renderText(const std::string& text, int fontIndex, Vector2 position, float scale, Vector4 colorm, float zindex = 1);
	bool renderButton(const std::string& text, int fontIndex, Vector2 position, Vector2 size, Vector4 color, float textscale, Vector2 textOffset, float zindex = 1);
	static void handlePress(int key);
	static void handleRelease(int key);
};
     

