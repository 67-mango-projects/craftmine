#pragma once
#include "Buffer.h"
#include <string>
#include <unordered_map>

struct TextureFrame {
	float left, top, right, bottom;
	TextureFrame() : left(0), top(0), right(0), bottom(0) {} // default constructor
	TextureFrame(float vleft, float vtop, float vright, float vbottom) : left(vleft), top(vtop), right(vright), bottom(vbottom) {}
};

class Texture {
private:
	
	std::unordered_map<std::string, TextureFrame> m_textures;
	unsigned int m_renderId;

	std::string m_atlasFilePath;
	std::string m_filePath;

	unsigned char* m_localBuffer;
	int m_Width, m_Height, m_BPP;
public:
	Texture(const std::string& path, const std::string& atlaspath = "");
	~Texture();
	void bind(unsigned int slot = 0) const;
	void unbind() const;

	inline int getWidth() { return m_Width; }
	inline int getHeight() { return m_Height; }

	TextureFrame getTexture(const std::string& name);

private:
	void loadAtlas(const std::string& atlas);
};