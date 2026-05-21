#include "UI.h"
#include "Assets/stb_image.h"

UI::UI(int VAO)
{
    this->VAO = VAO;

    /* Create Shader */
    //shader = new Shader("src/Shaders/SimpleVertexShader.glsl", "src/Shaders/SimpleFragmentShader.glsl");

    /* Load texture */
    glGenTextures(1, &this->textureID);
    glBindTexture(GL_TEXTURE_2D, this->textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("src/Assets/UI/UI.png", &width, &height, &nrChannels, 4);



    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    else {
        std::cout << "Failed to load texture at: src/Assets/Backgrounds/BackGround.png" << std::endl;
    }
    stbi_image_free(data);
}

void UI::draw(Shader* shader)
{
    shader->use();
    glBindVertexArray(this->VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->textureID);
    glm::mat4 mat = glm::mat4(1.0f);
    shader->setInt("amountOfSprites", 1);

    glm::mat4 model = glm::mat4(1.0f);
    float posX = 0.0f;
    float posY = 0.0f;
    float width = 640.0f * 0.4f;
    float height = 360.0f * 0.95f;

    // Move to position + half the size to account for centered vertices
    model = glm::translate(model, glm::vec3(posX + (width / 2.0f), posY + (height / 2.0f), 0.0f));
    model = glm::scale(model, glm::vec3(width, height, 1.0f));
    
    shader->setMat4("model", model);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}