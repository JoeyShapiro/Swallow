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
    const float gravity = 0.5;
    float lastRightTrigger = 0;
    float wing_area = 0.3;
    float k = 0.5; // arbitrary air density and "feel good" factor
    float lift = 0;
    float thrust = 0;
    Vector2 momentum = { 0, 0 };

    float fmin = MAXFLOAT;
    float fmax = -MAXFLOAT;

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
        }

        // Get axis values
        float leftStickX = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_X);
        float leftStickY = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_Y);

        float rotation = atan2f(leftStickY, leftStickX)+PI;
        frameRec2.height = leftStickX > 0 ? -16 : 16;

        float rightTrigger = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_TRIGGER);
        currentFrame2 = (((1 + rightTrigger)/2)*4); // does it all automatically
        frameRec2.x = (float)currentFrame2*16;

        float deltaTrigger = (rightTrigger - lastRightTrigger) * 7;

        float velocity = leftStickX > 0 ? 1 : -1;

        float force = (velocity * velocity) * wing_area;
        float coefficient_lift = 2*PI * sin(rotation) * cos(rotation);
        lift = coefficient_lift * force;
        // float coefficient_thrust = sin(2*rotation) * cos(rotation);
        // thrust = coefficient_thrust * force;

        // float C_D = (coefficient_lift * coefficient_lift) / PI * 0.7 * (1 / wing_area);
        // float drag = 0.5 * force * C_D;
        float drag = velocity * sin(rotation); // TODO make this sharp
        
        momentum.x = velocity-drag;
        momentum.y = lift;

        lastRightTrigger = rightTrigger;

        if (rotation > fmax) fmax = rotation;
        if (rotation < fmin) fmin = rotation;

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
            // TODO is it more intuitive to have head in direction you point, or direction you move?
            DrawTexturePro(texture, frameRec2, dest, (Vector2){16*SCALE/2, 16*SCALE/2}, (rotation)*RAD2DEG, WHITE);

            if (IsGamepadAvailable(gamepad)) {
                DrawText(GetGamepadName(gamepad), 190, 240, 20, LIGHTGRAY);
            } else {
                DrawText("Gamepad not connected", 190, 220, 20, LIGHTGRAY);
            }

            DrawText(TextFormat("trigger: %02.02f", rightTrigger), 0, 0, 20, LIGHTGRAY);
            DrawText(TextFormat("rotation: %02.02fpi", (rotation/PI)), 0, 24, 20, LIGHTGRAY);
            DrawText(TextFormat("delta: %02.02f", deltaTrigger), 0, 48, 20, LIGHTGRAY);
            DrawText(TextFormat("Thrust: %02.02f", momentum.x), 0, 96, 20, LIGHTGRAY);
            DrawText(TextFormat("Lift: %02.02f", momentum.y), 0, 120, 20, LIGHTGRAY);
            DrawText(TextFormat("( %02.02f <-> %02.02f )", fmin, fmax), 0, 144, 20, LIGHTGRAY);
            DrawText(TextFormat("momentum: (%02.02f, %02.02f)", momentum.x*32, momentum.y*32), 0, 168, 20, LIGHTGRAY);
            DrawText(TextFormat("fps: %d", GetFPS()), 0, 192, 20, LIGHTGRAY);

            DrawLine(position1.x, position1.y, position1.x+momentum.x*32, position1.y+momentum.y*32, RED);

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