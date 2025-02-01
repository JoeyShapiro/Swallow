#include "raylib.h"

#define SCALE 1

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1024;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Swallow");
    
    // scale texture to 2x
    Image img = LoadImage("res/BirdSprite.png");
    ImageResize(&img, img.width * SCALE, img.height * SCALE);
    Texture2D texture = LoadTextureFromImage(img);

    Vector2 position = { 350.0f, 280.0f };
    Vector2 position1 = { 350.0f, 400.0f };
    Rectangle frameRec = { 0.0f, 0.0f, 16*SCALE, 16*SCALE };
    Rectangle frameRec2 = { 0, 16*SCALE, 16*SCALE, 16*SCALE };
    int currentFrame = 0;
    int currentFrame2 = 0;

    int framesCounter = 0;
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        framesCounter++;

        if (framesCounter >= (16))
        {
            framesCounter = 0;
            currentFrame++;
            currentFrame2++;

            if (currentFrame > 1) currentFrame = 0;
            if (currentFrame2 > 7) currentFrame2 = 0;

            frameRec.x = (float)currentFrame*16*SCALE;
            frameRec2.x = (float)currentFrame2*16*SCALE;
        }
        
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawTextureRec(texture, frameRec, position, WHITE);
            DrawTextureRec(texture, frameRec2, position1, WHITE);
            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(texture);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}