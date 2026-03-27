#pragma once
#include <glm/glm.hpp>
#include <Shaders/Shader.hpp>
#include <vector>

struct Bullet
{
	glm::vec2 position;
	glm::vec2 velocity;
};

class BulletManager
{
public:
	static const int MAX_BULLETS = 5000;
	std::vector<Bullet> bulletPositions;

	BulletManager();
	//BulletManager(unsigned int VAO);

	void addBullet(glm::vec2 startPos, glm::vec2 direction);
	void update(float deltaTime);
	void draw(glm::mat4 projection);
private:
	unsigned int VBO, VAO, EBO, iVBO,textureID;
	Shader* shader;
};
