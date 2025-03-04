#include "raylib.h"
#include <math.h>
#include "../res/BirdSprite.h" // 33kb
#include "../res/Fruit.h"
#include <stdio.h>
#include <string.h>

#define SCALE 4

#include <mach/mach_time.h>
#include <unistd.h>
#include <time.h>

void efficient_frame_sleep(double seconds) {
    // Sleep for most of the time (except last 1ms)
    useconds_t microseconds = (useconds_t)(seconds * 1000000);
    if (microseconds > 1000) {
        usleep(microseconds);
    }
    
    // Short busy wait with ARM-optimized yield for precision
    // mach_timebase_info_data_t timebase;
    // mach_timebase_info(&timebase);
    
    // uint64_t end = mach_absolute_time() + 
    //                (uint64_t)(seconds * 1000000000ULL) * 
    //                timebase.denom / timebase.numer;
    
    // while (mach_absolute_time() < end) {
    //     // ARM yield instruction (more power efficient than spinning)
    //     __builtin_arm_yield();  // ARM-specific yield
    // }
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1024;
    const int screenHeight = 720;

    // SetConfigFlags(FLAG_VSYNC_HINT);
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
    const float gravity = 0.25;
    float lastRightTrigger = 0;
    float wing_area = 0.3;
    float k = 0.5; // arbitrary air density and "feel good" factor
    float lift = 0;
    float thrust = 0;
    float mass = 1;
    Vector2 acceleration = { 0, 0 };
    Vector2 velocity = { 0, 0 };
    Vector2 camera = { 0, 0 };

    struct timespec sleep_time;
    sleep_time.tv_nsec = 0;
    sleep_time.tv_sec = 0;

    // float fmin = MAXFLOAT;
    // float fmax = -MAXFLOAT;

    int framesCounter = 0;
    // SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    double targetFrameTime = 1.0/60.0;
    //--------------------------------------------------------------------------------------
    char xdata[100] = { 0 };
    char ydata[100] = { 0 };

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        double startTime = GetTime();
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

        float extension = -(rightTrigger * rightTrigger) + 1;

        // float speed = leftStickX > 0 ? 1 : -1;

        // bournoulli's principle
        // accel shouldnt get crazy
        Vector2 force = {
            .x = (velocity.x * fabsf(velocity.x)) * wing_area,
            .y = (velocity.y * fabsf(velocity.y)) * wing_area,
        };
        // TODO cleanup. 13% cpu, but example uses 12%
        // TODO no momentum
        float coefficient_lift = sin(rotation) * cos(rotation);
        lift = coefficient_lift;
        lift = leftStickX > 0 ? lift : -lift;
        // float coefficient_thrust = sin(2*rotation) * cos(rotation);
        // thrust = coefficient_thrust * force;
        // i could do lin alg and diff eq to get one equation, but it would be slower a^3+b^2+c
        float thrust = fabs(sin(2*rotation));
        thrust *= leftStickX > 0 ? 1 : -1;
        thrust *= ((rightTrigger + 1) / 2);
        float penetration = 0.5 * lift * sin(rotation/2) * extension;

        // float C_D = (coefficient_lift * coefficient_lift) / PI * 0.7 * (1 / wing_area);
        // float drag = 0.5 * force * C_D;
        // what {1, -1}[leftStickX > 0]
        // TODO this might be wrong
        Vector2 drag = {
            .x = 0.5 * force.x * sin(rotation) * (leftStickY > 0 ? -1 : 1) + (wing_area * extension),
            .y = 0.5 * force.y * sin(rotation) * (leftStickY > 0 ? -1 : 1) + (wing_area * extension),
        };
        
        // TODO not equal for both directions
        acceleration.x = (thrust - drag.x + penetration) / mass;
        acceleration.y = (lift-drag.y) / mass + gravity;

        lastRightTrigger = rightTrigger;

        // if (rotation > fmax) fmax = rotation;
        // if (rotation < fmin) fmin = rotation;

        velocity.x += acceleration.x;
        velocity.y += acceleration.y;

        // clamp velocity
        if (velocity.x > 30) velocity.x = 30;
        if (velocity.x < -30) velocity.x = -30;

        if (velocity.y > 30) velocity.y = 30;
        if (velocity.y < -30) velocity.y = -30;

        // position1.x += velocity.x;
        // position1.y += velocity.y;
        camera.x += velocity.x;
        camera.y += velocity.y;

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
            int playerspace_y_offset = (int)camera.y % 32;
            int playerspace_x_offset = (int)camera.x % 32;

            // draw grid
            for (int i = 0; i < screenWidth; i+=32) {
                DrawLine(i-playerspace_x_offset, 0, i-playerspace_x_offset, screenHeight, LIGHTGRAY);
            }
            for (int i = 0; i < screenHeight; i+=32) {
                DrawLine(0, i-playerspace_y_offset, screenWidth, i-playerspace_y_offset, LIGHTGRAY);
            }

            DrawTexturePro(texture, frameRec, (Rectangle){position.x, position.y, 16*SCALE, 16*SCALE}, (Vector2){0, 0}, 0, WHITE);
            // TODO is it more intuitive to have head in direction you point, or direction you move?
            DrawTexturePro(texture, frameRec2, dest, (Vector2){16*SCALE/2, 16*SCALE/2}, (rotation)*RAD2DEG, WHITE);

            if (IsGamepadAvailable(gamepad)) {
                DrawText(GetGamepadName(gamepad), 190, 240, 20, LIGHTGRAY);
            } else {
                DrawText("Gamepad not connected", 190, 220, 20, LIGHTGRAY);
            }

            // x range is between -12 and -30(-inf)
            if (framesCounter == 0) {
                const char* tmp = TextFormat("x: rot: %02.02fpi; force: %02.02f thrust: %02.02f; drag: %02.02f; accel: %02.02f; vel: %02.02f",
                    rotation/PI, force.x, thrust, drag.x, acceleration.x, velocity.x);
                strcpy(xdata, tmp);
                
                tmp = TextFormat("y: rot: %02.02fpi; force: %02.02f lift: %02.02f; drag: %02.02f; accel: %02.02f; vel: %02.02f",
                    rotation/PI, force.y, lift, drag.y, acceleration.y, velocity.y);
                strcpy(ydata, tmp);
            }

            DrawText(xdata, 0, 500, 20, LIGHTGRAY);
            DrawText(ydata, 0, 530, 20, LIGHTGRAY);

            DrawText(TextFormat("trigger: %02.02f", rightTrigger), 0, 0, 20, LIGHTGRAY);
            DrawText(TextFormat("rotation: %02.02fpi", (rotation/PI)), 0, 24, 20, LIGHTGRAY);
            DrawText(TextFormat("delta: %02.02f", deltaTrigger), 0, 48, 20, LIGHTGRAY);
            DrawText(TextFormat("fps: %d", GetFPS()), 0, 192, 20, LIGHTGRAY);

            DrawLine(position1.x, position1.y, position1.x+velocity.x*32, position1.y+velocity.y*32, RED);

            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_MIDDLE_RIGHT)) DrawTriangle((Vector2){ 436, 168 }, (Vector2){ 436, 185 }, (Vector2){ 464, 177 }, RED);
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_UP)) DrawCircle(557, 144, 13, LIME);
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) DrawCircle(586, 173, 13, RED);
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) DrawCircle(557, 203, 13, VIOLET);
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) DrawCircle(527, 173, 13, PINK);

            DrawRectangle(611, 48, 15, 70, GRAY);
            DrawRectangle(611, 48, 15, (int)(((1 + rightTrigger)/2)*70), RED);

        EndDrawing();
        //----------------------------------------------------------------------------------

        double elapsedTime = GetTime() - startTime;
        double sleepTime = targetFrameTime - elapsedTime;

        if (sleepTime > 0) {
            sleep_time.tv_nsec = (long)(sleepTime * 1000000000);
            nanosleep(&sleep_time, NULL);
        }
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(texture);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}