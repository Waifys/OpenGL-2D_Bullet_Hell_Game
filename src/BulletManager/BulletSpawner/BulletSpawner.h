#pragma once

#include "BulletManager/BulletManager.h"
#include <glm/glm.hpp>
#include <Player/Player.h>

class BulletSpawner
{
public:
	float fireRate = 0.3f, currentCooldown = 0;
	float bulletSpread = 0.0f, direction = 0; // Direction 0 means down, Sorry for the hard coded, seems the most efficient way
	int bulletPerShot = 20;
	float spinSpeed = 0.0f;
	glm::vec2 position;

	BulletSpawner(BulletManager* bulletManager);
	BulletSpawner(BulletManager* bulletManager, Player* player);
	void update(float deltaTime);

private:
	BulletManager* bulletManager;

	void fireBullet();

	Player* player;
};