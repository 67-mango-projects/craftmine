#include "game.h"
#include<iostream>

#include <glew.h>
#include <glfw3.h>
#include <sstream>
#include<fstream>

#include "renderer/VertexArray.h"
#include "renderer/IndexBuffer.h"
#include "renderer/Shader.h"


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
    
    Shader shader("shaders/voxel.glsl");
    shader.bind();
        
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