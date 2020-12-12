#include "particle_world.h"
#include <stdlib.h>

const static float R_SMOOTH = 0.05; // amount of repel when particles overlap ( I think )

// both from https://stackoverflow.com/questions/2128728/allocate-matrix-in-c
static AttractionProfile getAttractionProfile(World *world, int from_type, int to_type)
{
    return world->type_to_attraction_profile_matrix[from_type * world->num_particle_types + to_type];
}
static void setAttractionProfile(World *world, int from_type, int to_type, AttractionProfile in)
{
    world->type_to_attraction_profile_matrix[from_type * world->num_particle_types + to_type] = in;
}

// from https://stackoverflow.com/questions/13408990/how-to-generate-random-float-number-in-c
static float randFloat(float min, float max)
{
    float scale = rand() / (float)RAND_MAX; /* [0, 1.0] */
    return min + scale * (max - min);       /* [min, max] */
}

static float getAttraction(World *world, int from_type, int to_type, float distance)
{
    AttractionProfile cur_profile = getAttractionProfile(world, from_type, to_type);
    if (distance > cur_profile.max_radius)
    {
        return 0.0;
    }
    else if (distance < world->particle_radius)
    {
        return 5000.0 * (R_SMOOTH * world->particle_radius * (1.0 / (world->particle_radius + R_SMOOTH) - 1.0 / (distance + R_SMOOTH)));
    }
    // roughly from https://github.com/HackerPoet/Particle-Life/blob/master/Universe.cpp#L132
    float numer = 2.0 * abs(distance - 0.5 * (cur_profile.max_radius + world->particle_radius));
    float denom = cur_profile.max_radius - world->particle_radius;
    return cur_profile.weight * (1.0 - (numer / denom));
};

void constructWorld(World *world)
{
    srand(world->seed); // randomly initializes particles

    // construct particles
    world->particles = malloc(sizeof(*world->particles) * world->num_particles);

    for (int i = 0; i < world->num_particles; i++)
    {
        world->particles[i] = (Particle){
            .position = (Vector2){randFloat(0.0, world->width), randFloat(0.0, world->height)},
            .type = rand() % world->num_particle_types,
        };

        // construct the type to attraction profile matrix
        world->type_to_attraction_profile_matrix = malloc(sizeof(*world->type_to_attraction_profile_matrix) * world->num_particle_types * world->num_particle_types);
        for (int r = 0; r < world->num_particle_types; r++)
        {
            for (int c = 0; c < world->num_particle_types; c++)
            {
                setAttractionProfile(world, r, c, (AttractionProfile){
                                                      .weight = randFloat(-200.0, 200.0),
                                                      //   .weight = randFloat(-100.0, 100.0),
                                                      .max_radius = randFloat(30.0, 150.0),
                                                      //   .max_radius = randFloat(100.0, 400.0),
                                                  });
            }
        }
    }
}

void iterateWorld(World *world)
{
    for (int i_me = 0; i_me < world->num_particles; i_me++)
    {
        Vector2 final_accel = {0};
        for (int i_them = 0; i_them < world->num_particles; i_them++)
        {
            if (i_them == i_me)
                continue;
            Particle me = world->particles[i_me];
            Particle them = world->particles[i_them];
            Vector2 target_vector = Vector2Subtract(them.position, me.position);

            // make wrapping force work https://github.com/HackerPoet/Particle-Life/blob/master/Universe.cpp#L99
            if (target_vector.x >= world->width * 0.5)
            {
                target_vector.x -= world->width;
            }
            else if (target_vector.x < -world->width * 0.5)
            {
                target_vector.x += world->width;
            }
            if (target_vector.y >= world->height * 0.5)
            {
                target_vector.y -= world->height;
            }
            else if (target_vector.y < -world->height * 0.5)
            {
                target_vector.y += world->height;
            }

            // use squared distance to see if the target vector is zero. Given enough time,
            // two particles will exactly overlap and the target vector will be (0, 0). This makes its
            // normalized form nan, corrupting every other particle. In the case that this happens,
            // this if statement will skip over it.
            if (target_vector.x * target_vector.x + target_vector.y * target_vector.y <= 0.0)
            {
                continue;
            }

            float attraction = getAttraction(world, me.type, them.type, Vector2Length(target_vector));
            final_accel = Vector2Add(final_accel, Vector2Scale(Vector2Normalize(target_vector), attraction));
        }
        world->particles[i_me].vel = Vector2Add(world->particles[i_me].vel, Vector2Scale(final_accel, world->timestep));
        world->particles[i_me].vel = Vector2Scale(world->particles[i_me].vel, 1.0 - world->friction);
        world->particles[i_me].position = Vector2Add(world->particles[i_me].position, Vector2Scale(world->particles[i_me].vel, world->timestep));

        // TODO add bounce option by reflecting velocity when touching edge
        // wrapping
        if (world->particles[i_me].position.x < 0.0)
        {
            world->particles[i_me].position.x += world->width;
        }
        else if (world->particles[i_me].position.x >= world->width)
        {
            world->particles[i_me].position.x -= world->width;
        }
        if (world->particles[i_me].position.y < 0.0)
        {
            world->particles[i_me].position.y += world->height;
        }
        else if (world->particles[i_me].position.y >= world->height)
        {
            world->particles[i_me].position.y -= world->height;
        }
    }
}

void destroyWorld(World *world)
{
    free(world->particles);
    free(world->type_to_attraction_profile_matrix);
}
