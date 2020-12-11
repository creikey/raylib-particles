#ifndef H_PARTICLE_WORLD
#define H_PARTICLE_WORLD

#ifndef RAYLIB_H
#define RAYMATH_STANDALONE
#endif
#include "raymath.h" // I include all of raylib for vector math. Forgive me, for I have sinned

typedef struct AttractionProfile
{
    float weight;
    float max_radius;
} AttractionProfile;

typedef struct Particle
{
    Vector2 position;
    Vector2 vel;
    int type;
} Particle;

typedef struct World
{
    float width;
    float height;
    float timestep;
    float friction;

    int seed;
    int num_particles;
    int num_particle_types;
    float particle_radius;
    Particle *particles;
    AttractionProfile *type_to_attraction_profile_matrix; // matrix of [type from row, type to column] attraction profile
} World;

void constructWorld(World *world);
void iterateWorld(World *world);
void destroyWorld(World *world);

#endif // H_PARTICLE_WORLD