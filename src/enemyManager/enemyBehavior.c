#include "enemyManager.h"
#include "../bulletManager/bulletManager.h"
#include <math.h>
#include <stdlib.h>

#include <raylib.h>
#include <raymath.h>

typedef struct Enemy {
    EnemyType type;
    float x;
    float y;
    float startX;
    float hoverY;
    float radius;
    float speed;
    float age;
    float fireTimer;
    struct Enemy *next;
} Enemy;

typedef struct EnemyProjectile {
    float x;
    float y;
    float velocityX;
    float velocityY;
    float radius;
    struct EnemyProjectile *next;
} EnemyProjectile;

static Enemy *enemies = NULL;
static EnemyProjectile *enemyProjectiles = NULL;

static void addEnemyProjectile(float x, float y,
                               float playerX, float playerY) {
    EnemyProjectile *projectile = malloc(sizeof *projectile);
    if (projectile == NULL) return;

    Vector2 direction = Vector2Normalize(
        (Vector2){playerX - x, playerY - y}
    );

    projectile->x = x;
    projectile->y = y;
    projectile->velocityX = direction.x * 260.0f;
    projectile->velocityY = direction.y * 260.0f;
    projectile->radius = 5.0f;

    projectile->next = enemyProjectiles;
    enemyProjectiles = projectile;
}

static void spawnEnemy(EnemyType type, int screenWidth) {
    Enemy *enemy = malloc(sizeof *enemy);
    if (enemy == NULL) return;

    enemy->type = type;
    enemy->age = 0.0f;
    enemy->fireTimer = 0.0f;
    enemy->y = -25.0f;
    enemy->hoverY = 120.0f;

    // Set each enemy type's size and falling speed.
    switch (type) {
        case ENEMY_FAST:
            enemy->radius = 12.0f;
            enemy->speed = 220.0f;
            enemy->hoverY = 160.0f;
            break;

        case ENEMY_ZIGZAG:
            enemy->radius = 16.0f;
            enemy->speed = 140.0f;
            enemy->hoverY = 130.0f;
            break;

        case ENEMY_BASIC:
        default:
            enemy->radius = 20.0f;
            enemy->speed = 100.0f;
            enemy->hoverY = 110.0f;
            break;
    }

    int margin = (int)enemy->radius;
    enemy->x = (float)GetRandomValue(margin, screenWidth - margin);
    enemy->startX = enemy->x;

    // Put the new enemy at the front of the linked list.
    enemy->next = enemies;
    enemies = enemy;
}

void spawner(float *spawnTimer, float spawnInterval,
             float deltaTime, int screenWidth) {
    *spawnTimer += deltaTime;

    if (*spawnTimer >= spawnInterval) {
        EnemyType type = (EnemyType)GetRandomValue(0, 2);
        spawnEnemy(type, screenWidth);
        *spawnTimer -= spawnInterval;
    }
}

bool updateEnemies(float deltaTime, int screenWidth, int screenHeight,
                   float playerX, float playerY, float playerRadius) {
    bool playerWasHit = false;
    Enemy **enemyLink = &enemies;

    while (*enemyLink != NULL) {
        Enemy *enemy = *enemyLink;
        enemy->age += deltaTime;

        if (enemy->y < enemy->hoverY) {
            // Descend until reaching the hover height.
            enemy->y += enemy->speed * deltaTime;

            if (enemy->y > enemy->hoverY) {
                enemy->y = enemy->hoverY;
            }
        } else {
            // Hover side to side. Each type uses a different width and pace.
            float amplitude = 30.0f;
            float frequency = 1.0f;

            switch (enemy->type) {
                case ENEMY_FAST:
                    amplitude = 70.0f;
                    frequency = 1.8f;
                    break;

                case ENEMY_ZIGZAG:
                    amplitude = 100.0f;
                    frequency = 1.2f;
                    break;

                case ENEMY_BASIC:
                default:
                    amplitude = 30.0f;
                    frequency = 0.8f;
                    break;
            }

            enemy->x = enemy->startX
                     + sinf(enemy->age * frequency) * amplitude;

            if (enemy->x < enemy->radius) {
                enemy->x = enemy->radius;
            }
            if (enemy->x > screenWidth - enemy->radius) {
                enemy->x = screenWidth - enemy->radius;
            }

            // Fire one aimed projectile every 1.3 seconds.
            enemy->fireTimer += deltaTime;
            if (enemy->fireTimer >= 1.3f) {
                addEnemyProjectile(
                    enemy->x,
                    enemy->y + enemy->radius,
                    playerX,
                    playerY
                );
                enemy->fireTimer -= 1.3f;
            }
        }

         if (consumeBulletAt(enemy->x, enemy->y, enemy->radius)) {
                *enemyLink = enemy->next;
                free(enemy);
                continue;
         }

        if (CheckCollisionCircles(
                (Vector2){playerX, playerY}, playerRadius,
                (Vector2){enemy->x, enemy->y}, enemy->radius)) {
            playerWasHit = true;
        }

        // Remove an enemy if it goes below the window.
        if (enemy->y - enemy->radius > screenHeight) {
            *enemyLink = enemy->next;
            free(enemy);
        } else {
            enemyLink = &enemy->next;
        }
    }

    EnemyProjectile **projectileLink = &enemyProjectiles;

    while (*projectileLink != NULL) {
        EnemyProjectile *projectile = *projectileLink;

        projectile->x += projectile->velocityX * deltaTime;
        projectile->y += projectile->velocityY * deltaTime;

        if (CheckCollisionCircles(
                (Vector2){playerX, playerY}, playerRadius,
                (Vector2){projectile->x, projectile->y},
                projectile->radius)) {
            playerWasHit = true;
        }

        bool outsideWindow =
            projectile->x < -projectile->radius ||
            projectile->x > screenWidth + projectile->radius ||
            projectile->y < -projectile->radius ||
            projectile->y > screenHeight + projectile->radius;

        if (outsideWindow || playerWasHit) {
            *projectileLink = projectile->next;
            free(projectile);
        } else {
            projectileLink = &projectile->next;
        }
    }

    return playerWasHit;
}

void drawEnemies(void) {
    Enemy *enemy = enemies;

    while (enemy != NULL) {
        Color color = RED;

        switch (enemy->type) {
            case ENEMY_FAST:
                color = ORANGE;
                break;

            case ENEMY_ZIGZAG:
                color = PURPLE;
                break;

            case ENEMY_BASIC:
            default:
                color = GREEN;
                break;
        }

        DrawCircle((int)enemy->x, (int)enemy->y,
                   (int)enemy->radius, color);

        enemy = enemy->next;
    }

    EnemyProjectile *projectile = enemyProjectiles;

    while (projectile != NULL) {
        DrawCircle((int)projectile->x, (int)projectile->y,
                   (int)projectile->radius, YELLOW);
        projectile = projectile->next;
    }
}

void checkIfGameOver(float deltaTime, int screenWidth, int screenHeight, float player_X, float player_Y, float radius, bool *gameOver){
    if(!*gameOver && updateEnemies(deltaTime, screenWidth, screenHeight, player_X, player_Y, radius)){
        *gameOver = true;
    }
}

void freeEnemies(void) {
    while (enemies != NULL) {
        Enemy *next = enemies->next;
        free(enemies);
        enemies = next;
    }

    while (enemyProjectiles != NULL) {
        EnemyProjectile *next = enemyProjectiles->next;
        free(enemyProjectiles);
        enemyProjectiles = next;
    }
}
