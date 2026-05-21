#include "BulletSpawner.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/rotate_vector.hpp"
#include <math.h>

#include <iostream>

BulletSpawner::BulletSpawner(BulletManager* bulletManager)
{
	this->bulletManager = bulletManager;
	direction = 0.0f;
	position = glm::vec2(130, 70);
}

BulletSpawner::BulletSpawner(BulletManager* bulletManager, Player* player)
{
	this->bulletManager = bulletManager;
	direction = 0.0f;
	position = glm::vec2(130, 70);
	this->player = player;
}



void BulletSpawner::update(float deltaTime)
{
	if (bulletPerShot < 0 || fireRate <= 0)
		return;
	if (currentCooldown < 0)
	{
		currentCooldown = fireRate;
		if (bulletPerShot)
		{
			fireBullet();
		}
	}
	currentCooldown -= deltaTime;
	direction = fmod(direction + spinSpeed * deltaTime, 360.0f);
}

void BulletSpawner::fireBullet()
{
	glm::vec2 shotDirection;
	if (player)
	{
		shotDirection = glm::normalize(this->position - player->position);
		bulletManager->addBullet(position, shotDirection * 300.0f);
	}

	if (bulletPerShot == 1 || bulletSpread == 0)
	{
		bulletManager->addBullet(position, glm::rotate(glm::vec2(0,-1),glm::radians(direction)) * 300.0f);
		return; 
	}
	float startAngle = direction - (bulletSpread / 2.0f);
	float angleStep = bulletSpread / (bulletPerShot - 1);
	for (int i = 0; i < bulletPerShot; i++)
	{
		float currentAngle = startAngle + (angleStep * i);
		float radians = glm::radians(currentAngle);
		bulletManager->addBullet(position, glm::rotate(glm::vec2(0, -1), radians)*300.0f);
	}
}