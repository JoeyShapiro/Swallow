#include "raylib.h"
#include <math.h>
#include "../res/BirdSprite.h" // 33kb
#include "../res/Fruit.h"

#define SCALE 4

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
    // Texture2D texture = LoadTexture("res/BirdSprite.png");
    

    // Image img = LoadImage("res/BirdSprite.png");
    // ExportImageAsCode(img, "res/BirdSprite.h");

    int gamepad = 0;

    Image image = {
        .data = BIRDSPRITE_DATA,
        .width = BIRDSPRITE_WIDTH,
        .height = BIRDSPRITE_HEIGHT,
        .format = BIRDSPRITE_FORMAT,
        .mipmaps = 1
    };

    Texture2D texture = LoadTextureFromImage(image);
    // TEXTURE_FILTER_NEARESTNEIGHBOR;
    SetTextureFilter(texture, TEXTURE_FILTER_ANISOTROPIC_4X);

    Vector2 position = { 350.0f, 280.0f };
    Vector2 position1 = { 350.0f, 400.0f };
    Rectangle frameRec = { 0.0f, 0.0f, 16, 16 };
    Rectangle frameRec2 = { 0, 16, 16, 16 };
    int currentFrame = 0;
    int currentFrame2 = 0;
    float deltaX = 0;
    float deltaY = 0;
    const int gravity = 1;

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

            if (currentFrame > 1) currentFrame = 0;

            frameRec.x = (float)currentFrame*16;
            frameRec2.x = (float)currentFrame2*16;
        }

        // Get axis values
        float leftStickX = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_X);
        float leftStickY = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_Y);

        float rotation = atan2f(-leftStickY, -leftStickX) * RAD2DEG;
        frameRec2.height = leftStickX > 0 ? -16 : 16;

        float rightTrigger = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_TRIGGER);
        currentFrame2 = (((1 + rightTrigger)/2)*4); // does it all automatically
        frameRec2.x = (float)currentFrame2*16;

        // position1.y += gravity;

        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
            position1.x += leftStickX * 2;
            position1.y += leftStickY * 2;
        }

        // (Rectangle){leftStickX > 0 ? position1.x+8 : position1.x - 8, position1.y, 16*SCALE, 16*SCALE};
        Rectangle dest = {
            .x = position1.x,
            .y = position1.y,
            .width = 16*SCALE,
            .height = 16*SCALE,
        };
        
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawTexturePro(texture, frameRec, (Rectangle){position.x, position.y, 16*SCALE, 16*SCALE}, (Vector2){0, 0}, 0, WHITE);
            DrawTexturePro(texture, frameRec2, dest, (Vector2){16*SCALE/2, 16*SCALE/2}, rotation, WHITE);

            if (IsGamepadAvailable(gamepad)) {
                DrawText(GetGamepadName(gamepad), 190, 240, 20, LIGHTGRAY);
            } else {
                DrawText("Gamepad not connected", 190, 220, 20, LIGHTGRAY);
            }

            DrawText(TextFormat("Button: %02.02f", rightTrigger), 190, 260, 20, LIGHTGRAY);

            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_MIDDLE_RIGHT)) DrawTriangle((Vector2){ 436, 168 }, (Vector2){ 436, 185 }, (Vector2){ 464, 177 }, RED);
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_UP)) DrawCircle(557, 144, 13, LIME);
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) DrawCircle(586, 173, 13, RED);
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) DrawCircle(557, 203, 13, VIOLET);
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) DrawCircle(527, 173, 13, PINK);

            DrawRectangle(611, 48, 15, 70, GRAY);
            DrawRectangle(611, 48, 15, (int)(((1 + rightTrigger)/2)*70), RED);

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