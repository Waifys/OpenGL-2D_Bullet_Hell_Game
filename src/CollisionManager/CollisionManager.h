#pragma once

#include "Player/Player.h"
#include "BulletManager/BulletManager.h"

class CollisionManager
{
public:
	void checkColission(Player* player, BulletManager* bulletManager);

private: 

	static constexpr float radius = 8.0f;
	static constexpr float radiusSq = radius * radius;
	static constexpr int CELL_SIZE = 32.0f;
	static constexpr int GRID_WIDTH = 640.0f / CELL_SIZE;
	static constexpr int GRID_HEIGHT = 360.0f / CELL_SIZE;

	int getIndex(int x, int y) const {
		return x + y * GRID_WIDTH;
	}

	std::vector<int> grid[GRID_WIDTH * GRID_HEIGHT];
};