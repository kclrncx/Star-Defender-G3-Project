#include <raylib.h>
#include <stdlib.h>
#include "bulletManager.h"

#define BULLETSPEED 700.0f

typedef struct Bullets {
    float x;
    float y;
    struct Bullets *next;
} Bullets;

static Bullets *bullets = NULL;

void addBullet(float x, float y) {
    Bullets *newBullet = malloc(sizeof *newBullet);

    if (newBullet == NULL) {
        return;
    }

    newBullet->x = x;
    newBullet->y = y;
    newBullet->next = bullets;
    bullets = newBullet;
}

void autoFire(float *shootTimer, float shootInterval, float deltaTime,
              float player_X, float player_Y, float radius) {
    *shootTimer += deltaTime;

    if (*shootTimer >= shootInterval) {
        addBullet(player_X, player_Y - radius);
        *shootTimer -= shootInterval;
    }
}

void updateBullets(float deltaTime) {
    Bullets **link = &bullets;

    while (*link != NULL) {
        Bullets *bullet = *link;
        bullet->y -= BULLETSPEED * deltaTime;

        // Remove a bullet after it has moved above the window.
        if (bullet->y < -5.0f) {
            *link = bullet->next;
            free(bullet);
        } else {
            link = &bullet->next;
        }
    }
}

bool consumeBulletAt(float x, float y, float targetRadius) {
    Bullets **link = &bullets;

    while (*link != NULL) {
        Bullets *bullet = *link;

        if (CheckCollisionCircles(
                (Vector2){bullet->x, bullet->y}, 5.0f,
                (Vector2){x, y}, targetRadius)) {
            *link = bullet->next;
            free(bullet);
            return true;
        }

        link = &bullet->next;
    }

    return false;
}

void drawBullets(void) {
    Bullets *bullet = bullets;

    while (bullet != NULL) {
        DrawCircle((int)bullet->x, (int)bullet->y, 5, YELLOW);
        bullet = bullet->next;
    }
}

void freeBullets(void) {
    while (bullets != NULL) {
        Bullets *next = bullets->next;
        free(bullets);
        bullets = next;
    }
}
