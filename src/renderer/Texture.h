#include "Buffer.h"
#include <string>

class Texture {
private:
	unsigned int m_renderId;
	std::string m_filePath;
	unsigned char* m_localBuffer;
	int m_Width, m_Height, m_BPP;
public:
	Texture(const std::string& path);
	~Texture();
	void bind(unsigned int slot = 0) const;
	void unbind() const;

	inline int getWidth() { return m_Width; }
	inline int getHeight() { return m_Height; }
};