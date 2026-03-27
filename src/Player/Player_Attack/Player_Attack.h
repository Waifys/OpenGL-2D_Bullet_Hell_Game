#pragma once

#include <glm/glm.hpp>
#include <Shaders/Shader.hpp>

struct Attack
{
	glm::vec2 position;
	glm::vec2 velocity;
	bool active = false;
};

class AttackManager {
public:
	static const int MAX_BULLETS = 500;
	float fireCooldown = 0.0f;
	float fireRate = 0.1f;
	Attack pool[MAX_BULLETS];

	AttackManager(unsigned int VAO);

	void fire(glm::vec2 startPos);
	void update(float deltaTime);
	void draw(Shader& shader);

private:
	unsigned int VAO, textureID;
};