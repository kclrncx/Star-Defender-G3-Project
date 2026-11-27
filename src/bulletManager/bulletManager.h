#ifndef BULLET_MANAGER_H
#define BULLET_MANAGER_H

#include <stdbool.h>

void autoFire(float *shootTimer, float shootInterval, float deltaTime,
              float playerX, float playerY, float radius);
void updateBullets(float deltaTime);
void drawBullets(void);
void freeBullets(void);
bool consumeBulletAt(float x, float y, float targetRadius);

#endif
