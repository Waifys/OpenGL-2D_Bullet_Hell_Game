#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "Shaders/Shader.hpp"
#include "Player/Player.h"

#include "BulletManager/BulletManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Assets/stb_image.h"

float cooldown = 0.05;
float currentDegree = 0;

void spawnBall(BulletManager* bm, float deltaTime)
{
    if (cooldown < 0)
    {
        bm->addBullet(glm::vec2(640.0f/2, 100.0f), glm::rotate(glm::vec2(0, -100), glm::sin(currentDegree)));
        cooldown = 0.05;
    }
     
    currentDegree -= deltaTime*5;
    cooldown -= deltaTime;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void readInput(GLFWwindow* window, BulletManager* bulletmanager);

float vertices[] = {
    // positions        // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // top left 
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

int main()
{
    if(!glfwInit())
    {
        std::cout << "Failed to Initialize GLFW\n";
        return -1;
    }
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(640, 360, "Window", NULL, NULL);
    if(!window)
    {
        std::cout << "Failed to Initialize a window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW init failed\n";
        return -1;
    }

    glfwSetWindowAspectRatio(window, 16, 9);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Create Shader Program */
    Shader shader("src/Shaders/spriteVertexShader.glsl", "src/Shaders/spriteFragmentShader.glsl");

    BulletManager* bulletmanager = new BulletManager();

    /* Create and set buffers */

    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glm::mat4 projection = glm::ortho(0.0f, 640.0f, 360.0f, 0.0f, -1.0f, 1.0f);
    shader.use();
    shader.setMat4("projection", projection);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Player player(glm::vec2(320, 180), VAO);

    float lastFrame = 0.0f;

    while(!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        player.ProcessInput(window, deltaTime);
        readInput(window, bulletmanager);
        player.Draw(shader);

        bulletmanager->update(deltaTime);
        bulletmanager->draw(projection);
        spawnBall(bulletmanager, deltaTime);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void readInput(GLFWwindow* window, BulletManager* bulletmanager)
{
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        bulletmanager->addBullet(glm::vec2(300, 300), glm::vec2(0, 1));
    }
}