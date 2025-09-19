#include "Buffer.h"
#include <string>

struct ShaderProgramSource {
	std::string fragmentShader;
	std::string vertexShader;
};

class Shader : Buffer {
private:
	std::string m_filePath;
public:
	Shader(const std::string& filepath);
	~Shader();
	void bind() const;
	void unbind() const;

	template<typename T>
	void setUniform4(const std::string& name, T v1, T v2, T v3, T v4);
private:
	unsigned int getUniformLocation(const std::string& name);
	int compileShader(GLenum type /* Vertex or Fragment*/, const std::string& source);
};