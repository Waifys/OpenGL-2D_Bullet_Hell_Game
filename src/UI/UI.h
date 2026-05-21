#pragma once

#include <glm/glm.hpp>
#include <Shaders/Shader.hpp>

class UI
{
public:
	UI(int VAO);

	void draw(Shader* shader);

private:
	unsigned int VAO, textureID;
	Shader* shader;
};