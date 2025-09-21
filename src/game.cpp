#include "game.h"
#include<iostream>

#include <glew.h>
#include <glfw3.h>
#include <sstream>
#include<fstream>

#include "renderer/VertexArray.h"
#include "renderer/IndexBuffer.h"
#include "renderer/Shader.h"
#include "renderer/Renderer.h"
#include "renderer/VertexBufferLayout.h"
#include "renderer/Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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

    Renderer renderer;

    Vector2 windowDimensions = renderer.getViewPortSize();
    GLFWwindow* window = glfwCreateWindow(windowDimensions.x, windowDimensions.y, "arrushisbad", NULL, NULL);

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


    

    struct Quad {
        Vector2 position = Vector2(300, 300);
        Vector2 size = Vector2(100, 100);

        Vertex2 data[4] = {
            Vertex2(
                position.x - size.x, position.y + size.x, // pos
                0.0,1.0, // texture pos
                1.0, 0, 0.0 // color
            ), // 0
            Vertex2(
                 position.x - size.x, position.y - size.x,
                0.0,0.0,
                0.0, 0.0, 1.0
            ),// 1
            Vertex2(
                 position.x + size.x, position.y + size.x,
                1.0,1.0,
                0.0, 1.0, 0
            ), // 2
            Vertex2(
                 position.x + size.x, position.y - size.x,
                1.0,0.0,
                1.0, 1.0, 1.0
            ) // 3
        };
        unsigned int indices[6]{
            0,1,2,
            2,3,1
        };
    };

    glm::mat4 proj = glm::ortho(0.0f, windowDimensions.x, 0.0f, windowDimensions.y, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 0, 0));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 0, 0));

    glm::mat4 mvp = proj * view * model;

    Shader shader("shaders/voxel.glsl");
    shader.bind();
    Quad q;

    VertexBuffer vb(q.data,sizeof(Quad::data));
    VertexArray vao;
    VertexBufferLayout vbl;
    IndexBuffer ib(q.indices, 6);
    

    vbl.pushElement<float>(2, 0);
    vbl.pushElement<float>(2, 0);
    vbl.pushElement<float>(3, 0);
    vao.addBuffer(vb, vbl);

    Texture tex("res/player.png");
    tex.bind();

    shader.setUniform1f("u_opacity", 0);
    shader.setUniform1i("u_texture", 0);
    shader.setUniformMat4f("u_mvp", mvp);

    float opacity = 1;
    float inc = 0;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); /* how alpha pixels get rendered*/


    while (!glfwWindowShouldClose(window)) {
        renderer.clear();

        renderer.draw(vao, ib, shader);

        if (opacity >= 1)
            inc = -0.01;
        else if (opacity <= 0)
            inc = 0.01;
        opacity += inc;
        shader.setUniform1f("u_opacity", opacity);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}