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
	float health;
	float imunityWindow;

	// Constructor declaration
	Player(glm::vec2 startPos, int VAO);

	void ProcessInput(GLFWwindow* window, float deltaTime);

	void Draw(Shader &shader);

	void getHit(float damage);

private:
	AttackManager* bulletManager;
	unsigned int VAO, textureID;

	/* Variables for sprite animation */
	int currentFrame = 0;
	int totalFrames = 8;
	float frameDuration = 0.1f;
	float frameTimer = 0.0f;
};
