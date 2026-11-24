#include <raylib.h>

#define WIDTH 800
#define LENGTH 800
#define PLYR_POS_X 380
#define PLYR_POS_Y 700
#define FPS 60
#define PLYR_SPEED 300.0f

int main(){
    InitWindow(WIDTH, LENGTH, "Star Shooter");
    SetTargetFPS(FPS);
float player_Y = PLYR_POS_Y;
float player_X = PLYR_POS_X;
    while(!WindowShouldClose()){

        float deltaTime = GetFrameTime();
        if(IsKeyDown(KEY_W)){
            player_Y -= PLYR_SPEED * deltaTime;
        }
        if(IsKeyDown(KEY_S)){
            player_Y += PLYR_SPEED * deltaTime;
        }
        if(IsKeyDown(KEY_A)){
            player_X -= PLYR_SPEED * deltaTime;
        }
        if(IsKeyDown(KEY_D)){
            player_X += PLYR_SPEED * deltaTime;
        }
        
        BeginDrawing();
            ClearBackground(RED);
            DrawCircle(player_X, player_Y, 15, WHITE);
        EndDrawing();
    }

    CloseWindow();
return 0;
}
