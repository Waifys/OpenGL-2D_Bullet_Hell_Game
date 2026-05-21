#include "CollisionManager.h"



void CollisionManager::checkColission(Player* player, BulletManager* bulletManager)
{
    if (player->imunityWindow > 0)
        return;

	int playerCellX = (int)(player->position.x / CELL_SIZE);
	int playerCellY = (int)(player->position.y / CELL_SIZE);

	for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; i++) {
		grid[i].clear();
	}

    for (int i = 0; i < bulletManager->bulletPositions.size(); i++) {
        float bx = bulletManager->bulletPositions[i].position.x;
        float by = bulletManager->bulletPositions[i].position.y;

        int cellX = (int)(bx / CELL_SIZE);
        int cellY = (int)(by / CELL_SIZE);

        if (cellX >= 0 && cellX < GRID_WIDTH &&
            cellY >= 0 && cellY < GRID_HEIGHT)
        {
            int index = getIndex(cellX, cellY);
            grid[index].push_back(i);
        }
    }

    for (int ox = -1; ox <= 1; ox++) {
        for (int oy = -1; oy <= 1; oy++) {

            int cx = playerCellX + ox;
            int cy = playerCellY + oy;

            if (cx < 0 || cy < 0 || cx >= GRID_WIDTH || cy >= GRID_HEIGHT)
                continue;

            int index = getIndex(cx, cy);

            for (int bulletIndex : grid[index]) {

                float bx = bulletManager->bulletPositions[bulletIndex].position.x;
                float by = bulletManager->bulletPositions[bulletIndex].position.y;

                float dx = bx - player->position.x;
                float dy = by - player->position.y;

                if (abs(dx) > radius || abs(dy) > radius)
                    continue;

                float distSq = dx * dx + dy * dy;

                if (distSq < radiusSq) {
                    player->getHit(10.0f);
                    return; 
                }
            }
        }
    }
}