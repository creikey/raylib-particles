#include "raylib.h"
#include "stdlib.h" // the stdlib included in raylib/core.c , used for its GetRandomValue , is not exposed through this header
#include <time.h>   // random time initializer

#include "particle_world.h"

#include <stdio.h> // debugging

#define ARRSIZE(arr) (sizeof(arr) / sizeof(arr[0]))

// from http://alumni.media.mit.edu/~wad/color/palette.html
static const int particle_colors_hex[] = {
    0x352222ff,
    0x575757ff,
    0xa0a0a0ff,
    0xffffffff,
    0x2a4bd7ff,
    0x1d6914ff,
    0x814a19ff,
    0x8126c0ff,
    0x9dafffff,
    0x81c57aff,
    0xe9debbff,
    0xad2323ff,
    0x29d0d0ff,
    0xffee33ff,
    0xff9233ff,
    0xff9233ff,
};
static Color particle_colors[ARRSIZE(particle_colors_hex)];

int main(void)
{

    // Fill particle colors with hexidecimal to color function
    for (int i = 0; i < ARRSIZE(particle_colors); i++)
    {
        particle_colors[i] = GetColor(particle_colors_hex[i]);
    }

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    World world = {
        .width = (float)screenWidth,
        .height = (float)screenHeight,
        .timestep = 1.0 / 60.0,
        .friction = 0.05,
        // .friction = 0.08,

        .seed = (unsigned int)time(NULL),
        // .seed = 1607658350,
        .num_particles = 1000,
        // .num_particle_types = 16,
        .num_particle_types = 16,
        .particle_radius = 5.0,
    };

    printf("Using seed %d ...\n", world.seed);

    InitWindow(screenWidth, screenHeight, "Particles Life");

    constructWorld(&world);

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {

        // printf("FPS: %d\tActive time: %f\tFirst particle x: %f\n", GetFPS(), GetTime(), world.particles[0].position.x);
        // Update
        //----------------------------------------------------------------------------------
        // for (int i = 0; i < 10; i++)
        // {
        iterateWorld(&world);
        // }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        for (int i = 0; i < world.num_particles; i++)
        {
            Color draw_color = particle_colors[world.particles[i].type % ARRSIZE(particle_colors)];
            DrawCircleV(world.particles[i].position, world.particle_radius, draw_color);
        }
        // DrawFPS(500, 500);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    destroyWorld(&world);

    return 0;
}