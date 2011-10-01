#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtkwidget.h>
#include <gtkgl/gtkglarea.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "particle.h"
#include "math/matrix.h"

static particle_system *ps;

void init_particle_system(particle_system *part_sys) {
	int j;

	ps = part_sys;

	if(ps->num_particles == 0) return;

	/* Create the particles */
	if(ps->particles) {
		if(ps->old_num_particles != ps->num_particles)
			ps->particles = realloc(ps->particles,
				sizeof(particle_type)*ps->num_particles);
	} else {
		ps->particles = malloc(sizeof(particle_type)*ps->num_particles);
	}

        /* Initially all particles are dead */
        for(j = ps->old_num_particles; j < ps->num_particles; j++) {
                ps->particles[j].is_dead = 1;
        }

	ps->old_num_particles = ps->num_particles;
}


void initparticle(particle_type* p) {
   p->position[0] = ps->initpos[0];
   p->position[1] = ps->initpos[1];
   p->position[2] = ps->initpos[2];
   p->position[3] = 0.0;
   v_zero(p->velocity);
   p->color[0] = 1.0;
   p->color[1] = 1.0;
   p->color[2] = 1.0;
   p->color[3] = 0.0;
   vv_cpy(p->prevpos[0], p->position);
   vv_cpy(p->prevpos[1], p->position);
   vv_cpy(p->prevpos[2], p->position);
   vv_cpy(p->prevpos[3], p->position);
   vv_cpy(p->prevpos[4], p->position);
   p->begin = 0;
}

void move_particle(particle_type* p, GLfloat delta) {
   Vector accel, tempa, tempv;
   GLfloat deltasq = delta*delta;
   accel[0] = 0.0; accel[1] = -ps->gravity; accel[2] = 0.0; accel[3] = 0.0;
   vv_cpy(tempa, accel);
   vc_mul(tempa, 0.5);
   vc_mul(tempa, deltasq);
   vv_cpy(tempv, p->velocity);
   vc_mul(tempv, (double)delta);
   vv_add(tempa, tempv);
   vv_cpy(p->prevpos[p->begin], p->position);
   p->begin = (p->begin + 1) % PREVIOUS;
   vv_add(p->position, tempa);
   vv_cpy(tempa, accel);
   vc_mul(tempa, delta);
   vv_add(p->velocity, tempa);
}

void show_particle(particle_type* p) {
   int i;
	/* And draw where the particle has been */
   if(ps->trailon) {
	glPointSize(2.0);
   glBegin(GL_POINTS);
	for(i=PREVIOUS-1; i>=0; i--) {
	   glColor3f(p->color[0] / (PREVIOUS-i), p->color[1] / (PREVIOUS-i),			p->color[2] / (PREVIOUS-i));
	   glVertex3fv(p->prevpos[(p->begin+i)%PREVIOUS]);    }
   glEnd();
   }
	/* Draw the object's point */
        glPointSize(3.0);
   glBegin(GL_POINTS);
	glColor3f(p->color[0], p->color[1], p->color[2]);
	glVertex3fv(p->position);
   glEnd();
}
