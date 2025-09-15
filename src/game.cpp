#include "game.h"
#include<iostream>

#include <glew.h>
#include <glfw3.h>

int Game::run() {
    if (!glfwInit()) {
        printf("could not iniitialize glfw\n");
        return EXIT_FAILURE;
    }


    GLFWwindow* window = glfwCreateWindow(900, 900, "OPENGL", NULL, NULL);

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

}