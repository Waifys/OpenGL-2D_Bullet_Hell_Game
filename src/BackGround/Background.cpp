#include "Background.h"
#include "Assets/stb_image.h"
#include <glm/gtx/euler_angles.hpp>

Background::Background(int VAO)
{
	this->VAO = VAO;
    this->imageProgression = 0;

	/* Create Shader */
	shader = new Shader("src/Shaders/SimpleVertexShader.glsl", "src/Shaders/SimpleFragmentShader.glsl");

    /* Load texture */
    glGenTextures(1, &this->textureID);
    glBindTexture(GL_TEXTURE_2D, this->textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("src/Assets/Backgrounds/BackGround.png", &width, &height, &nrChannels, 4);



    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    else {
        std::cout << "Failed to load texture at: src/Assets/Backgrounds/BackGround.png" << std::endl;
    }
    stbi_image_free(data);
}

void Background::update(float deltaTime)
{
    imageProgression += deltaTime/10;
}

void Background::draw()
{
    shader->use();
    glBindVertexArray(this->VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->textureID);
    glm::mat4 mat = glm::mat4(1.0f);
    mat = glm::scale(mat, glm::vec3(15.0f, 50.0f, 15.0f));
    mat = glm::rotate(mat, glm::radians(-94.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), ((640.0f * 0.4f) / (480.0f * 0.98f)), 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 2.0f, 5.0f),
        glm::vec3(0.0f, 0.0f, 0.0f), 
        glm::vec3(0.0f, 1.0f, 0.0f)  
    );
    shader->setMat4("model", mat);
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    shader->setFloat("time", imageProgression);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}