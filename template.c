/*
 *  TEMPLATE FOR THE GAME
 * */

#include <raylib.h>
#include <stdlib.h>
#include <raymath.h>

// Our Bullets
typedef struct Bullet {
    float x;
    float y;
} Bullet;

// Our Enemies Structure
typedef struct Enemies {
    float x;
    float y;
    float speed;
    float radius;
} Enemies;

int main(){

    // Player
    float ballX = 400;
    float ballY = 700;
    int radius = 15;

    // Colors
    Color backgroundColor = {191, 126, 46, 183};
    Color ballColor = {22, 242, 216, 200};
    Color bulletColor = {242, 22, 22, 180};

    // Bullet array
    Bullet *bullets = NULL;
    int bulletCount = 0;

    // Automatic shooting timer
    float shootTimer = 0.0f;
    float shootInterval = 0.25f;

    // Player Speed
    float playerspeed = 300.0f;

    // Bullet Speed
    float bulletSpeed = 700.0f;

    // Initialize window
    InitWindow(800, 800, "This a test!");
    SetTargetFPS(60);

    // Define the enemy
    Enemies enemies;

    enemies.x = GetRandomValue(20, 780);
    enemies.y = -20;
    enemies.speed = 150.0f;
    enemies.radius = 20;

    while(!WindowShouldClose()){

        float deltaTime = GetFrameTime();

        // Player movement
        if(IsKeyDown(KEY_D)){
            ballX += playerspeed * deltaTime;
        }

        if(IsKeyDown(KEY_A)){
            ballX -= playerspeed * deltaTime;
        }

        if(IsKeyDown(KEY_W)){
            ballY -= playerspeed * deltaTime;
        }

        if(IsKeyDown(KEY_S)){
            ballY += playerspeed * deltaTime;
        }

        // Player boundaries
        ballX = Clamp(ballX, radius, 800 - radius);
        ballY = Clamp(ballY, radius, 800 - radius);

        // Automatic bullet shooting
        shootTimer += deltaTime;

        if(shootTimer >= shootInterval){

            Bullet *temp = realloc(
                bullets,
                (bulletCount + 1) * sizeof(Bullet)
            );

            if(temp != NULL){

                bullets = temp;

                bullets[bulletCount].x = ballX;
                bullets[bulletCount].y = ballY;

                bulletCount++;

                shootTimer -= shootInterval;
            }
        }

        // Update bullets
        for(int i = 0; i < bulletCount; i++){

            bullets[i].y -= bulletSpeed * deltaTime;

        }

        // Enemy movement
        enemies.y += enemies.speed * deltaTime;

        // Enemy respawn
        if(enemies.y > 820){

            enemies.x = GetRandomValue(20, 780);
            enemies.y = -20;
        }

        // Bullet and enemy collision
        for(int i = 0; i < bulletCount; i++){

            if(CheckCollisionCircles(
                (Vector2){bullets[i].x, bullets[i].y},
                5,
                (Vector2){enemies.x, enemies.y},
                enemies.radius
            )){

                // Remove bullet
                bullets[i] = bullets[bulletCount - 1];
                bulletCount--;

                // Respawn enemy
                enemies.x = GetRandomValue(20, 780);
                enemies.y = -20;

                break;
            }
        }

        // Remove bullets outside the screen
        for(int i = 0; i < bulletCount; ){

            if(bullets[i].y < 0){

                bullets[i] = bullets[bulletCount - 1];
                bulletCount--;

            } else {
                i++;
            }
        }

        BeginDrawing();

            ClearBackground(backgroundColor);

            // Draw player
            DrawCircle(ballX, ballY, radius, ballColor);

            // Draw bullets
            for(int i = 0; i < bulletCount; i++){

                DrawCircle(
                    bullets[i].x,
                    bullets[i].y,
                    5,
                    bulletColor
                );

            }

            // Draw enemy
            DrawCircle(
                enemies.x,
                enemies.y,
                enemies.radius,
                RED
            );

        EndDrawing();
    }

    // Free allocated memory
    free(bullets);

    CloseWindow();

    return 0;
}

