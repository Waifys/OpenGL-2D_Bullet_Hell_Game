#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <Shaders/Shader.hpp>

class Background
{
public:
	Shader* shader;
	Background(int VAO);

	void update(float deltaTime);
	void draw();

private:
	float imageProgression;
	unsigned int VAO, textureID;
};