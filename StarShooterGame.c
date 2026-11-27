#include <raylib.h>
#include <raymath.h>
#include "./src/enemyManager/enemyManager.h"
#include "./src/bulletManager/bulletManager.h"

    #define WIDTH 1280
    #define HEIGHT 720
    #define PLYR_POS_X 380
    #define PLYR_POS_Y 700
    #define FPS 60
    #define PLYR_SPEED 300.0f

int main(){

            SetConfigFlags(FLAG_WINDOW_RESIZABLE);
            InitWindow(WIDTH, HEIGHT, "Star Shooter"); //Window nato
            SetWindowMinSize(320, 240); //Minsize og Maxsize
            SetTargetFPS(FPS); //target FPS
            float player_X = WIDTH / 2.0f; //Player pos-x
            float player_Y = HEIGHT - 2.0f * 15.0f; //Player pos-y
            float shootTimer = 0.0f;
            const float shootInterval = 0.25f;
            int previousWidth = WIDTH;
            int previousHeight = HEIGHT;
            float enemySpawnTimer = 0.0f; //Enemy spawn timer
            const float enemySpawnInterval = 1.5f; //Spanwn interval
            bool gameOver = false;

    while(!WindowShouldClose()){
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();
        float scaleX = (float)screenWidth / previousWidth;
        float scaleY = (float)screenHeight / previousHeight;
        player_X *= scaleX;
        player_Y *= scaleY;
        previousWidth = screenWidth;
        previousHeight = screenHeight;

        float radius = Clamp((float)screenHeight / HEIGHT * 15.0f, 10.0f, 24.0f);
        float deltaTime = GetFrameTime();

            autoFire(&shootTimer, shootInterval, deltaTime, player_X, player_Y, radius);
            updateBullets(deltaTime);
            spawner(&enemySpawnTimer, enemySpawnInterval, deltaTime, screenWidth);
            checkIfGameOver(deltaTime, screenWidth, screenHeight, player_X, player_Y, radius, &gameOver);

        float speed = PLYR_SPEED * ((float)screenHeight / HEIGHT);
        if(IsKeyDown(KEY_W)){
            player_Y -= speed * deltaTime;
        }
        if(IsKeyDown(KEY_S)){
            player_Y += speed * deltaTime;
        }
        if(IsKeyDown(KEY_A)){
            player_X -= speed * deltaTime;
        }
        if(IsKeyDown(KEY_D)){
            player_X += speed * deltaTime;
        }

        // Keep the full player circle inside the current window.
        player_X = Clamp(player_X, radius, screenWidth - radius);
        player_Y = Clamp(player_Y, radius, screenHeight - radius);

        BeginDrawing();
            ClearBackground(BLACK);
            drawBullets();
            drawEnemies();
            DrawCircle((int)player_X, (int)player_Y, (int)radius, WHITE);
            if(gameOver) {
                DrawText("GAME OVER", 520, 330, 40, RED);
            }
        EndDrawing();
    }

    freeEnemies(); CloseWindow(); freeBullets(); freeEnemies();
return 0;
}
