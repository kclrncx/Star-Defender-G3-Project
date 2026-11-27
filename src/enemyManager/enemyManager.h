#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include <stdbool.h>

typedef enum EnemyType {
    ENEMY_BASIC,
    ENEMY_FAST,
    ENEMY_ZIGZAG
} EnemyType;

void spawner(float *spawnTimer, float spawnInterval,
             float deltaTime, int screenWidth);

bool updateEnemies(float deltaTime, int screenWidth, int screenHeight,
                   float playerX, float playerY, float playerRadius);
void checkIfGameOver(float deltaTime, int screenWidth, int screenHeight, float player_X, float player_Y, float radius, bool *gameOver);

void drawEnemies(void);
void freeEnemies(void);

#endif
