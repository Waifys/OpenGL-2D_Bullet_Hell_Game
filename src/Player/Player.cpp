#include "Player.h"
#include <glm/gtc/matrix_transform.hpp>

// The :: tells C++ "This function belongs to the Player class"
Player::Player(glm::vec2 startPos) {
	position = startPos;
	speed = 300.0f;
	radius = 15.0f;
	//TODO change the VAO
	bulletManager = new BulletManager(0);
}

void Player::ProcessInput(GLFWwindow* window, float deltaTime) {
	float totalSpeed = speed;
	if (glfwGetKey(window, GLFW_KEY_X))
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

	position = glm::clamp(position, glm::vec2(0, 0), glm::vec2(640, 360));
	bulletManager->update(deltaTime);
}

void Player::Draw(Shader& shader)
{
	shader.use();

	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::scale(model, glm::vec3(32.0f, 32.0f, 1.0f));


	GLuint MatrixID = glGetUniformLocation(shader.ID, "model");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &model[0][0]);

	glActiveTexture(GL_TEXTURE0);
	shader.setInt("Texture", 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	bulletManager->draw(shader);

}