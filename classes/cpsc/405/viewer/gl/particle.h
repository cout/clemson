#ifndef PARTICLE_H
#define PARTICLE_H

#include <GL/gl.h>
#include "math/matrix.h"

#define PREVIOUS (5)

typedef struct {
    Vector position;
    Vector velocity;
    Vector color;
    Vector prevpos[PREVIOUS];
    int begin;
    int is_dead;
    } particle_type;

typedef struct {
	particle_type *particles;
	int num_particles;
	int old_num_particles;
	float gravity;
	Vector initpos;
	Vector maxvel;
	Vector minvel;
	int random_velocity;
	int random_colors;
	int trailsize;
	int trailon;
} particle_system;

void initparticle(particle_type* p);
void move_particle(particle_type* p, GLfloat delta);
void show_particle(particle_type* p);

void init_particle_system(particle_system *ps);

#endif
