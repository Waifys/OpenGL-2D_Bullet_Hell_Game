#include "Player_Attack.h"
#include "Assets/stb_image.h"

AttackManager::AttackManager(unsigned int VAO)
{
	this->VAO = VAO;

	/* Load texture */
    glGenTextures(1, &this->textureID);
    glBindTexture(GL_TEXTURE_2D, this->textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("src/Assets/Player/ball.png", &width, &height, &nrChannels, 4);



    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    stbi_image_free(data);
}

void AttackManager::update(float dt) {
    if (fireCooldown > 0) fireCooldown -= dt;
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (pool[i].active) {
            pool[i].position.y -= 400.0f * dt; 

            // Deactivate if off-screen
            if (pool[i].position.y < -20.0f) pool[i].active = false;
        }
    }
}

void AttackManager::fire(glm::vec2 pos) {
    if (fireCooldown <= 0) {
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (!pool[i].active) {
                pool[i].active = true;
                pool[i].position = pos;
                fireCooldown = fireRate;
                return;
            }
        }
    }
}

void AttackManager::draw(Shader& shader) {
    shader.use();
    glBindVertexArray(this->VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->textureID);

    for (int i = 0; i < MAX_BULLETS; i++) {
        if (pool[i].active) {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(pool[i].position, 0.0f));
            model = glm::scale(model, glm::vec3(8.0f, 8.0f, 1.0f)); 
            shader.setMat4("model", model);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
    }
}