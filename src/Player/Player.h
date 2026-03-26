#pragma once
#include <glm/glm.hpp>
#include <Shaders/Shader.hpp>
#include <GLFW/glfw3.h>
#include "Player/Player_Attack/Player_Attack.h"

class Player {
public:
	glm::vec2 position;
	float speed;
	float radius;

	// Constructor declaration
	Player(glm::vec2 startPos);

	// Method declaration
	void ProcessInput(GLFWwindow* window, float deltaTime);

	// You could also add a Draw method here later
	void Draw(Shader &shader);
private:
	BulletManager* bulletManager;
};
