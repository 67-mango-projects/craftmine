#pragma once
#include "Buffer.h"
#include <string>
#include<unordered_map>

#include "glm/glm.hpp"

struct ShaderProgramSource {
	std::string fragmentShader;
	std::string vertexShader;
};

class Shader : Buffer {
private:
	std::string m_filePath;
	std::unordered_map<std::string, int> m_uniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();
	void bind() const;
	void unbind() const;

	void setUniform4f(const std::string& name, float v1, float v2, float v3, float v4);
	void setUniform1f(const std::string& name, float v1);
	void setUniform3f(const std::string& name, float x, float y, float z);
	void setUniform1i(const std::string& name, int v1);
	void setUniformMat4f(const std::string& name, const glm::mat4& matrix);
private:
	int getUniformLocation(const std::string& name);
	int compileShader(GLenum type /* Vertex or Fragment*/, const std::string& source);
};