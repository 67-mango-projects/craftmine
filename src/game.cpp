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
#include "core/Controller.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "spdlog.h"
#include "core/Camera.h"

static void glfwError(int id, const char* description)
{
    printf("[!] opengl error \'%s\'\n", description);
}

void Game::updateStats() {
    static double lastTime = 0.0;

    double currentTime = glfwGetTime();
    m_deltaTime = currentTime - lastTime;
    lastTime = currentTime;
}

int Game::run() {
    // INITIALIZATION START
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
    GLFWwindow* window = glfwCreateWindow(windowDimensions.x, windowDimensions.y, "gay gay gay game", NULL, NULL);

    if (!window)
    {
        printf("could not create window\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        printf("could not initialize glew\n");
        return EXIT_FAILURE;
    }

    spdlog::info("opengl version {}", (const char*)glGetString(GL_VERSION));
    Controller::initialize(window);
    spdlog::debug("controller initialized");
    Shader shader("shaders/voxel.glsl");
    shader.bind();
    
    Texture tex("res/textures.png", "res/textures.json");
    Renderer::setWorldTextures(&tex);
    tex.bind();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); /* how alpha pixels get rendered*/

    Player player;

    // INITIALIZATION END


    Quad q(tex.getTexture("aarush.png"), Vector2(300, 300), Vector2(100, 100));

    VertexBuffer vb(q.data,sizeof(Quad::data));

    VertexArray vao;
    VertexBufferLayout vbl;
    IndexBuffer ib(q.indices, 6);
    

    vbl.pushElement<float>(2, 0);
    vbl.pushElement<float>(2, 0);
    vbl.pushElement<float>(3, 0);
    vao.addBuffer(vb, vbl);

    shader.setUniform1f("u_opacity", 0);
    shader.setUniform1i("u_texture", 0); // tells opengl to bind to texture unit GL_TEXTURE0

    float opacity = 1;
    float inc = 0;

    while (!glfwWindowShouldClose(window)) {
        renderer.clear();
        updateStats();
        glm::mat4 proj = glm::ortho(0.0f, windowDimensions.x, 0.0f, windowDimensions.y, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        model = glm::rotate(model, glm::radians(q.rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(q.size.x, q.size.y, 1.0f));
        
        q.rotation += 0.6;
        q.data[0].position.x += 0.1;
        vb.bind();
        vao.addBuffer(vb, vbl);
        glm::mat4 mvp = proj * view * model;
        shader.setUniformMat4f("u_mvp", mvp);
        renderer.draw(vao, ib, shader);
        player.updatePlayer();
        model = glm::translate(glm::mat4(1.0f), glm::vec3(200.0f, 0, 0));
        model = glm::scale(model, glm::vec3(q.size.x, q.size.y, 1.0f));
        mvp = proj * view * model;
        shader.setUniformMat4f("u_mvp", mvp);
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