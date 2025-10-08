#include "Shader.h"
#include "Renderer.h"
#include "spdlog.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <spdlog.h>

enum class ShaderType {
    NONE = -1,
    VERTEX = 0,
    FRAGMENT = 1
};

int Shader::compileShader(GLenum type, const std::string& source) {
    GLuint shader_id = glCreateShader(type);
    const char* c_source = source.c_str();
    glCall(glShaderSource(shader_id, 1, &c_source, nullptr));
    glCall(glCompileShader(shader_id));

    int result;
    glCall(glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result));


    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
        char* message = new char;
        glGetShaderInfoLog(shader_id, length, &length, message);
        printf("failed to compile %s shader\n%s\n", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), message);
        glDeleteShader(shader_id);
        return 0;

    }
    else {
        spdlog::debug("compiled shader ({0}, {1}) succesfully",m_filePath.c_str(), (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT"));
    }

    // TODO: error handling

    return shader_id;
}

Shader::~Shader() {
    glCall(glDeleteProgram(m_renderId));
}

Shader::Shader(const std::string& filepath) {
    m_filePath = filepath;
	std::ifstream stream(filepath);

    std::string line;
    std::stringstream shader[2];

    ShaderType type = ShaderType::NONE;

    while (std::getline(stream, line)) {

        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            shader[(int)type] << line << '\n';
        }
    }

    GLuint opengl_program = glCreateProgram();
    GLuint vertex_shader = compileShader(GL_VERTEX_SHADER,shader[(int)ShaderType::VERTEX].str());
    GLuint fragment_shader = compileShader(GL_FRAGMENT_SHADER, shader[(int)ShaderType::FRAGMENT].str());

    glCall(glAttachShader(opengl_program, vertex_shader));
    glCall(glAttachShader(opengl_program, fragment_shader));
    glCall(glLinkProgram(opengl_program));
    glCall(glValidateProgram(opengl_program));

    //glDetachShader(opengl_program, fragment_shader);
    //glDetachShader(opengl_program, vertex_shader);

    glCall(glDeleteShader(vertex_shader));
    glCall(glDeleteShader(fragment_shader));

    m_renderId = opengl_program;
    
    
}


int Shader::getUniformLocation(const std::string& name) {

    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end()) {
        return m_uniformLocationCache[name];
    }

    glCall(int location = glGetUniformLocation(m_renderId, name.c_str()));
    if (location == -1) {
        spdlog::error("could not find uniform, {0}", name.c_str());
    }
    m_uniformLocationCache[name] = location;
    return location;
}

void Shader::setUniform1i(const std::string& name, int v1) {
    glCall(glUseProgram(m_renderId));
    glCall(glUniform1i(getUniformLocation(name), v1));
}

void Shader::setUniform4f(const std::string& name,
    float x, float y, float z, float w) {
    glCall(glUseProgram(m_renderId));
    glCall(glUniform4f(getUniformLocation(name), x, y, z, w));
}

void Shader::setUniformMat4f(const std::string& name,const glm::mat4& matrix) {
    glCall(glUseProgram(m_renderId));
    glCall(glUniformMatrix4fv(getUniformLocation(name), 1, false, &matrix[0][0]));
}

void Shader::setUniform1f(const std::string& name, float v1) {
    glCall(glUseProgram(m_renderId));
    glCall(glUniform1f(getUniformLocation(name), v1));
}

void Shader::setUniform3f(const std::string& name, float x,float y, float z) {
    glCall(glUseProgram(m_renderId));
    glCall(glUniform3f(getUniformLocation(name),x,y,z));
}


void Shader::bind() const {
    glCall(glUseProgram(m_renderId));
}

void Shader::unbind() const {
    glCall(glUseProgram(0));
}