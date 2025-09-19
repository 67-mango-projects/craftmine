#include "game.h"
#include<iostream>

#include <glew.h>
#include <glfw3.h>
#include <sstream>
#include<fstream>

#include "renderer/VertexArray.h"
#include "renderer/IndexBuffer.h"

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];

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
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(),ss[1].str() };
}

static GLuint CompileShader(GLenum type, const std::string& source) {
    GLuint shader_id = glCreateShader(type);
    const char* c_source = source.c_str();
    glShaderSource(shader_id, 1, &c_source, nullptr);
    glCompileShader(shader_id);

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

    // TODO: error handling

    return shader_id;
}

static GLuint CreateShader(const std::string& vertexshader, const std::string& fragmentshader) {
    GLuint opengl_program = glCreateProgram();
    GLuint vertex_shader = CompileShader(GL_VERTEX_SHADER, vertexshader);
    GLuint fragment_shader = CompileShader(GL_FRAGMENT_SHADER, fragmentshader);

    glAttachShader(opengl_program, vertex_shader);
    glAttachShader(opengl_program, fragment_shader);
    glLinkProgram(opengl_program);
    glValidateProgram(opengl_program);

    //glDetachShader(opengl_program, fragment_shader);
    //glDetachShader(opengl_program, vertex_shader);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return opengl_program;
}



static void glfwError(int id, const char* description)
{
    printf("[!] opengl error \'%s\'\n", description);
}

int Game::run() {
    if (!glfwInit()) {
        printf("could not iniitialize glfw\n");
        return EXIT_FAILURE;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwSetErrorCallback(&glfwError);

    GLFWwindow* window = glfwCreateWindow(900, 900, "arrushisbad", NULL, NULL);

    if (!window)
    {
        printf("could not create window\n");
        glfwTerminate();
        return EXIT_FAILURE;
        // Window or OpenGL context creation failed
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        printf("could not initialize glew\n");
        return EXIT_FAILURE;
    }

    printf("opengl version %s\n", glGetString(GL_VERSION));


    struct Vertex2 {
        GLfloat position[2];
        GLfloat color[3];
        Vertex2(GLfloat pos[2], GLfloat col[3]) {
            std::copy(pos, pos + 2, position);
            std::copy(col, col + 3, color);
        }
        Vertex2(GLfloat x, GLfloat y, GLfloat r, GLfloat g, GLfloat b) : position{ x,y }, color{ r,g,b } {}
    };

    struct Quad {
        Vertex2 data[4] = {
            Vertex2(-0.5, 0.5, 1.0, 0, 0.0), // 0
            Vertex2(-0.5,  -0.5, 0.0, 0.0, 1.0),// 1
            Vertex2(0.5, 0.5, 0.0, 1.0, 0), // 2
            Vertex2(0.5, -0.5, 1.0, 1.0, 1.0) // 3
        };
        unsigned int indices[6]{
            0,1,2,
            2,3,1
        };
    };
    std::string shaderPath = "src/shaders/voxel.";
    
    ShaderProgramSource source = ParseShader("shaders/voxel.glsl");
    std::cout << source.VertexSource << '\n';
    std::cout << source.FragmentSource << '\n';
    
    GLuint shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);

    Quad q;
    VertexBuffer vb(q.data,sizeof(Quad::data));
    VertexArray vao;
    VertexBufferLayout vbl;
    vbl.pushElement<float>(2, 0);
    vbl.pushElement<float>(3, 0);
    vao.addBuffer(vb, vbl);
    IndexBuffer ib(q.indices, 6);


    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        vao.bind();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}