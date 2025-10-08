#include "Texture.h"
#include "Renderer.h"
#include "stb_image.h"
#include "nlohmann/json.hpp"
#include "spdlog.h"


#include <fstream>


using json = nlohmann::json;

Texture::Texture(const std::string& path, const std::string& atlaspath) : m_filePath(path), m_atlasFilePath(atlaspath) {
	glCall(glGenTextures(1, &m_renderId));
	glCall(glBindTexture(GL_TEXTURE_2D, m_renderId));

	stbi_set_flip_vertically_on_load(true);
	m_localBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, STBI_rgb_alpha /* desired channels is how many channels we want per pixel, that is RGBA so 4*/);

	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER /*how the texture will be resampled down if sized down*/, GL_NEAREST));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER /*how the texture will be resampled down if sized up*/, GL_NEAREST));

	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));
	glCall(glBindTexture(GL_TEXTURE_2D, 0));

	loadAtlas(atlaspath);

	if (m_localBuffer)
		stbi_image_free(m_localBuffer);
}

Texture::~Texture() {
	glCall(glDeleteTextures(1, &m_renderId));
}

TextureFrame Texture::getTexture(const std::string& name) {
	if (m_textures.find(name) == m_textures.end()) {
		spdlog::error("could not find texture {}", name.c_str());
		return TextureFrame();
	}
	return m_textures[name];
}

void Texture::loadAtlas(const std::string& atlas) {
	std::ifstream atlasFile(atlas);
	spdlog::info("Loading Textures");
	if (!atlasFile.is_open()) {
		std::printf("failed to open json texture file %s\n", atlas.c_str());
		return;
	}
	json j;
	atlasFile >> j;
	for (auto& [name, data] : j["frames"].items()) {
		auto& f = data["frame"]; // shorthand

		float u0 = static_cast<float>(f["x"].get<int>()) / m_Width;
		float v0 = 1.0f - (static_cast<float>(f["y"].get<int>() + f["h"].get<int>()) / m_Height);
		float u1 = static_cast<float>(f["x"].get<int>() + f["w"].get<int>()) / m_Width;
		float v1 = 1.0f - (static_cast<float>(f["y"].get<int>()) / m_Height);


		TextureFrame t(
			u0,v0,u1,v1
		);
		m_textures[name] = t;
		spdlog::debug("[texture] {}", name.c_str());

	}
}


void Texture::bind(unsigned int slot) const {
    glCall(glActiveTexture(GL_TEXTURE0 + slot));
	glCall(glBindTexture(GL_TEXTURE_2D, m_renderId));
}
void Texture::unbind() const {
	glCall(glBindTexture(GL_TEXTURE_2D, 0));
}
