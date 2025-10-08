#pragma once
#include <unordered_map>
#include "../core/Math.h"
#include <ft2build.h>
#include "Mesh.h"
#include FT_FREETYPE_H

struct Character {
	const Mesh mesh; 
	Vector2 size;    
	Vector2 bearing;  
	unsigned int advance; 
	Character(const Mesh m,
		Vector2 s,
		Vector2 b,
		unsigned int a) : mesh(m), size(s), bearing(b), advance(a) {
	};
};

class TextureFont {
	std::unordered_map<char, Character*> m_characters;
	unsigned int m_renderId;
	std::string m_fontfilePath;
	Vector2 m_atlasSize;
public:
	void bind(unsigned int slot = 0) const;
	void unbind() const;
	TextureFont(const std::string& fontpath, const FT_Library& lib);
	const std::unordered_map<char, Character*>& getCharacters() const { return m_characters; }
};
