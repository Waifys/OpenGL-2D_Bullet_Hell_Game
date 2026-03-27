#include "BulletManager.h"
#include "Assets/stb_image.h"
#include <iostream>

/* TODO: remove the place holder, it held it's concept, the instancing is working */
float vertices2[] = {
	// positions        // texture coords
	 0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // top right
	 0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // top left 
};
unsigned int indices2[] = {
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};

BulletManager::BulletManager()
{
	 this->shader = new Shader("src/Shaders/enemyBulletShaderVertexShader.glsl", "src/Shaders/enemyBulletShaderFragmentShader.glsl");

	 glGenVertexArrays(1, &VAO);
	 glBindVertexArray(VAO);

	 glGenBuffers(1, &VBO);
	 glGenBuffers(1, &EBO);
	 glGenBuffers(1, &iVBO);

	 glBindBuffer(GL_ARRAY_BUFFER, VBO);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	 glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);


	 glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	 glEnableVertexAttribArray(0);
	 glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	 glEnableVertexAttribArray(1);

	 glBindBuffer(GL_ARRAY_BUFFER, iVBO);
	 glBufferData(GL_ARRAY_BUFFER, 5000 * sizeof(Bullet), NULL, GL_DYNAMIC_DRAW);

	 glEnableVertexAttribArray(2);
	 glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Bullet), (void*)0);
	 glVertexAttribDivisor(2, 1); 

	 glBindBuffer(GL_ARRAY_BUFFER, 0);
	 glBindVertexArray(0);

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

void BulletManager::addBullet(glm::vec2 startPos, glm::vec2 direction)
{
	if(bulletPositions.size() < 5000)
		bulletPositions.push_back({startPos, direction});
}

bool isInside(glm::vec2 pos) {
	return glm::all(glm::greaterThanEqual(pos, glm::vec2(0.0f, 0.0f))) &&
		glm::all(glm::lessThanEqual(pos, glm::vec2(640.0f, 360.0f)));
}

void BulletManager::update(float deltaTime)
{
	if (!bulletPositions.empty())
	{
		std::cout << bulletPositions[0].position.y << std::endl;
		std::cout << bulletPositions.size() << std::endl;
	}

	for (int i = 0; i < (int)bulletPositions.size(); ) {
			bulletPositions[i].position -= deltaTime * bulletPositions[i].velocity;
			if (!isInside(bulletPositions[i].position))
			{
				bulletPositions[i] = bulletPositions.back();
				bulletPositions.pop_back();
			}
			else
				i++;
	}
}

void BulletManager::draw(glm::mat4 projection)
{
	if (bulletPositions.empty())
		return;
	shader->use();
	shader->setMat4("projection", projection);
	glBindVertexArray(this->VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->textureID);

	glBindBuffer(GL_ARRAY_BUFFER, iVBO);
	glBufferData(GL_ARRAY_BUFFER, bulletPositions.size() * sizeof(Bullet), bulletPositions.data(), GL_STREAM_DRAW);

	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, (GLsizei)bulletPositions.size());
	glBindVertexArray(0);
}