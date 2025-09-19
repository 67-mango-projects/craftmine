#include "Shader.h"
#include "Renderer.h"

#include <fstream>
#include <sstream>
#include <iostream>
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
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);


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
        printf("compiled shader succesfully\n");
    }

    // TODO: error handling

    return shader_id;
}


Shader::Shader(const std::string& filepath) {
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

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    m_renderId = opengl_program;

    
}

void Shader::bind() const {
    glCall(glUseProgram(m_renderId));
}

void Shader::unbind() const {
    glCall(glUseProgram(0));
}