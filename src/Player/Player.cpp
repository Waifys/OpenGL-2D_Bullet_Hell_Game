#include "Player.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Assets/stb_image.h"

Player::Player(glm::vec2 startPos, int VAO) {
	position = startPos;
	speed = 80.0f;
	radius = 15.0f;
	health = 100.0f;

	bulletManager = new AttackManager(VAO);
	this->VAO = VAO;

	/* Load texture */
	glGenTextures(1, &this->textureID);
	glBindTexture(GL_TEXTURE_2D, this->textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("src/Assets/Player/Player_Sprite.png", &width, &height, &nrChannels, 4);


	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);
}

void Player::ProcessInput(GLFWwindow* window, float deltaTime) {
	float totalSpeed = speed;
	if (imunityWindow > 0)
	{
		imunityWindow -= deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		totalSpeed *= 0.4;
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		position.y -= totalSpeed * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		position.y += totalSpeed * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		position.x += totalSpeed * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		position.x -= totalSpeed * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		bulletManager->fire(position - glm::vec2(0.0f, 16.0f));
	}

	position = glm::clamp(position, glm::vec2(0, 0), glm::vec2(640.0f* 0.4f, 360.0f* 0.95f));
	bulletManager->update(deltaTime);
}

void Player::Draw(Shader& shader)
{
	shader.use();

	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::scale(model, glm::vec3(420.0/7.0f, 70.0f, 1.0f)/3.0f);
	std::cout << position.x << " " << position.y << std::endl;

	GLuint MatrixID = glGetUniformLocation(shader.ID, "model");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &model[0][0]);

	float timePerFrame = 0.1f; // 10 fps
	int frameCount = 7;
	int currentFrame = int(fmod(glfwGetTime() / timePerFrame, double(frameCount)));
	shader.setInt("amountOfSprites", frameCount);
	shader.setInt("currentSprite", currentFrame);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	shader.setInt("Texture", 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	bulletManager->draw(shader);

}

void Player::getHit(float damage)
{
	health -= damage;
	if (health < 0)
	{
		//TODO: Lose screen
	}
	imunityWindow = 3.0f;
}