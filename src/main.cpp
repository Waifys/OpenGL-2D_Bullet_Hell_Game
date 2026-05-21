#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "Shaders/Shader.hpp"
#include "Player/Player.h"
#include "BulletManager/BulletSpawner/BulletSpawner.h"
#include "CollisionManager/CollisionManager.h"
#include "BackGround/Background.h"
#include "UI/UI.h"

#include "BulletManager/BulletManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Assets/stb_image.h"

/*TODO: remove later tmp GUI*/
#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void readInput(GLFWwindow* window, BulletManager* bulletmanager);

/*TODO: remove this is for tmp GUI*/
void beginFrame(BulletSpawner* bulletSpawner,BulletManager* bulletManager, Player* player)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Window");
    if (ImGui::CollapsingHeader("Program Variables"))
    {
        ImGui::InputInt("Bullets per shot", &bulletSpawner->bulletPerShot);
        ImGui::SliderFloat("Bullet Spread", &bulletSpawner->bulletSpread, 1.0f, 360.0f);
        ImGui::SliderFloat("Bullet direction", &bulletSpawner->direction, 1.0f, 360.0f);
        ImGui::SliderFloat("Spin speed", &bulletSpawner->spinSpeed, 0.0f, 1000.0f);
        ImGui::InputFloat("Cooldown between Bullets", &bulletSpawner->fireRate);
        ImGui::InputFloat2("Bullets Spawn Position", &bulletSpawner->position.x);
        ImGui::InputFloat("Bullet Speed", &bulletManager->bulletSpeed);
        ImGui::InputFloat("Player Health", &player->health);
        ImGui::InputFloat("Player Immunity Window", &player->imunityWindow);
    }
    ImGui::End();
}

float vertices[] = {
    // positions        // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // top left 
};
unsigned int indices[] = {
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};


float width = 640, height=360;
int main()
{

    glfwSetErrorCallback([](int error, const char* desc) {
        std::cerr << "GLFW Error " << error << ": " << desc << "\n";
        });
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
    CollisionManager* collisionManager = new CollisionManager();

    /*TODO: remove this is for tmp GUI */
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    /* Create and set buffers */

    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glm::mat4 projection = glm::ortho(0.0f, 640.0f*0.4f, 360.0f*0.95f, 0.0f, -1.0f, 1.0f);
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

    Player* player = new Player(glm::vec2(320, 180), VAO);
    BulletSpawner* bulletSpawner2 = new BulletSpawner(bulletmanager, player);
    BulletSpawner* bulletSpawner = new BulletSpawner(bulletmanager);
    Background* background = new Background(VAO);
    UI* ui = new UI(VAO);


    float lastFrame = 0.0f;

    while(!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glViewport(0, 0, width, height);
        ui->draw(&shader);
        glViewport(width * 0.05, height * 0.05, width*0.4, height*0.90);
        glEnable(GL_SCISSOR_TEST);
        glScissor(width * 0.05, height * 0.05, width * 0.4, height * 0.90);
        glClearColor(0.05, 0.02, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_SCISSOR_TEST);
        background->update(deltaTime);
        background->draw();

        beginFrame(bulletSpawner, bulletmanager, player);

        glBindVertexArray(VAO);
        player->ProcessInput(window, deltaTime);
        readInput(window, bulletmanager);
        player->Draw(shader);

        bulletmanager->update(deltaTime);
        bulletmanager->draw(projection);
        bulletSpawner->update(deltaTime);
		bulletSpawner2->update(deltaTime);
        collisionManager->checkColission(player, bulletmanager);
       


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width_local, int height_local)
{
    width = width_local;
    height = height_local;
    glViewport(0, 0, width, height);
    
}

void readInput(GLFWwindow* window, BulletManager* bulletmanager)
{
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        bulletmanager->addBullet(glm::vec2(300, 300), glm::vec2(0, 1));
    }
}