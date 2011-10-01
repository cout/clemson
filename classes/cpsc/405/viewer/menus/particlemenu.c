#include <stdlib.h>
#include <gtk/gtk.h>
#include "widgets/widgets.h"
#include "misc/_time.h"
#include "gl/particle.h"
#include "gl/gl.h"
#include "gl/trackball/trackball.h"
#include "widgets/widgets.h"
#include "misc/geometry.h"
#include <math.h>

gint idle_tag;
static gint oldtime = 0;
static gint newtime = 0, difftime;
double ts; /* Time scale */

/* This will change later */
particle_system ps = {
	NULL, 0, 0
};

static float frand() {
	return (float)(rand()) / RAND_MAX;
}

static void setvel(particle_type *p) {
	int j;

	for(j = 0; j < 3; j++)
		p->velocity[j] = frand() * (ps.maxvel[j] - ps.minvel[j]) +
			ps.minvel[j];
}

static void trans_particle(particle_type *p, pVector t, pQuat q) {
	float m[4][4];
	vv_add(p->position, t);
	vv_add(p->prevpos[(p->begin+PREVIOUS)%PREVIOUS], t);
	build_rotmatrix(m, q);
	vm_mul(p->position, &m[0][0]);
	vm_mul(p->prevpos[(p->begin+PREVIOUS)%PREVIOUS], &m[0][0]);
	vm_mul(p->velocity, &m[0][0]);
}

static void untrans_particle(particle_type *p, pVector t, pQuat q) {
	float m[4][4];
	Quat qtmp;
	qtmp[0] = -q[0]; qtmp[1] = -q[1]; qtmp[2] = -q[2]; qtmp[3] = q[3];
	build_rotmatrix(m, qtmp);
	vm_mul(p->position, &m[0][0]);
	vm_mul(p->prevpos[(p->begin+PREVIOUS)%PREVIOUS], &m[0][0]);
	vm_mul(p->velocity, &m[0][0]);
	vv_sub(p->position, t);
	vv_sub(p->prevpos[(p->begin+PREVIOUS)%PREVIOUS], t);
}

static gint particle_iterate(gpointer data) {
	static int j = 0;
	int f, i;
	Object *o;
	particle_type *p;
	Vector v;
	float mag;

	if(oldtime == 0) {
		oldtime = get_time();
		newtime = get_time();
	}

	if(j >= ps.num_particles) {
		j = 0;
		oldtime = newtime;
		newtime = get_time();
		gl_redraw_all();
	}

	difftime = newtime - oldtime;

	p = &ps.particles[j];
	if(p->is_dead) {
		initparticle(p);

		if(ps.random_velocity) {
			setvel(p);
		}

		if(ps.random_colors) {
			p->color[0] = (float)(rand()) / RAND_MAX;
			p->color[1] = (float)(rand()) / RAND_MAX;
			p->color[2] = (float)(rand()) / RAND_MAX;
		}
		p->is_dead = 0;
	}
	move_particle(p, (float)difftime*ts);

	/* Collision detection */
	/* This can be sped up with an oct-tree */
	for(i = 0; i < num_objects; i++) {
		o = &aw_object[i];
		// Untranslate and unrotate the particle
		untrans_particle(p, o->trans, o->q);
		for(f = 0; f < o->faces.count; f++) {
			Vector point;
			if(ray_intersect_plane(
				p->prevpos[(p->begin+PREVIOUS)%PREVIOUS],
				p->position,
				o->face_normals.data[f],
				o->face_centers.data[f],
				o->d.data[f],
				point)) {

/*
				g_print("Collision with plane at "
					"[%f %f %f].\n",
					point[0], point[1], point[2]);
				g_print("P1: [%f %f %f]; P2: [%f %f %f]",
					p->position[0], p->position[1],
					p->position[2],
					p->prevpos[(
					p->begin+PREVIOUS)%PREVIOUS][0],
					p->prevpos[(
					p->begin+PREVIOUS)%PREVIOUS][1],
					p->prevpos[(
					p->begin+PREVIOUS)%PREVIOUS][2]);
*/

				if(point_inside_face(
					point,
					o->faces.data[f],
					&o->verts)) {

/*
					g_print("Collision with poly.\n");
*/

					/* We did collide */
					vv_cpy(p->position, point);
					vv_cpy(v, o->face_normals.data[f]);
					mag = sqrt(vv_dot(p->velocity, p->velocity));
					vc_mul(v, -2.0*vv_dot(o->face_normals.data[f], p->velocity));
					vv_add(v, p->velocity);
					v_norm(v);
					vc_mul(v, mag);
					vv_cpy(p->velocity, v);

					trans_particle(p, o->trans, o->q);
					move_particle(p, (float)difftime*ts);
					untrans_particle(p, o->trans, o->q);
				}
			}
		}
		/* Unapply the untranslation and unrotation */
		trans_particle(p, o->trans, o->q);
	}
						
/*	g_print("Particle %d at %f,%f,%f; ts = %f\n", j, p->position[0],
		p->position[1], p->position[2], ts);*/

	j++;	
	return TRUE;
}

void on_particle_apply_clicked() {
	/* Get the widgets -- this is slow, but we don't do it often */
	GtkWidget *initposx = glade_xml_get_widget(self, "initposx_scale");
	GtkWidget *initposy = glade_xml_get_widget(self, "initposy_scale");
	GtkWidget *initposz = glade_xml_get_widget(self, "initposz_scale");
	GtkWidget *gravity = glade_xml_get_widget(self, "gravity_scale");
	GtkWidget *trailsize = glade_xml_get_widget(self, "motiontrail_scale");
	GtkWidget *particles = glade_xml_get_widget(self, "numparticles_scale");
	GtkWidget *time_scale = glade_xml_get_widget(self, "time_scale");
	GtkWidget *maxvelx = glade_xml_get_widget(self, "maxvelx_scale");
	GtkWidget *maxvely = glade_xml_get_widget(self, "maxvely_scale");
	GtkWidget *maxvelz = glade_xml_get_widget(self, "maxvelz_scale");
	GtkWidget *minvelx = glade_xml_get_widget(self, "minvelx_scale");
	GtkWidget *minvely = glade_xml_get_widget(self, "minvely_scale");
	GtkWidget *minvelz = glade_xml_get_widget(self, "minvelz_scale");
	GtkWidget *rvel = glade_xml_get_widget(self,
		"particle_random_velocity");
	GtkWidget *rcol = glade_xml_get_widget(self,
		"particle_random_colors");
	GtkWidget *trailon = glade_xml_get_widget(self, "motion_trail");

	/* Get the settings for the particle system */
	ps.num_particles = GTK_RANGE(particles)->adjustment->value;
	ps.gravity = GTK_RANGE(gravity)->adjustment->value;
	ps.initpos[0] = GTK_RANGE(initposx)->adjustment->value;
	ps.initpos[1] = GTK_RANGE(initposy)->adjustment->value;
	ps.initpos[2] = GTK_RANGE(initposz)->adjustment->value;
	ps.maxvel[0] = GTK_RANGE(maxvelx)->adjustment->value;
	ps.maxvel[1] = GTK_RANGE(maxvely)->adjustment->value;
	ps.maxvel[2] = GTK_RANGE(maxvelz)->adjustment->value;
	ps.minvel[0] = GTK_RANGE(minvelx)->adjustment->value;
	ps.minvel[1] = GTK_RANGE(minvely)->adjustment->value;
	ps.minvel[2] = GTK_RANGE(minvelz)->adjustment->value;
	ps.random_velocity = gtk_toggle_button_get_active(	
		GTK_TOGGLE_BUTTON(rvel));
	ps.random_colors = gtk_toggle_button_get_active(	
		GTK_TOGGLE_BUTTON(rcol));
	ts = pow(10.0, GTK_RANGE(time_scale)->adjustment->value);
	ps.trailsize = GTK_RANGE(trailsize)->adjustment->value;
	ps.trailon = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(trailon));

	/* Initialize the particle system */
	init_particle_system(&ps);
}

void on_animate_activate(GtkMenuItem *menuitem, gpointer user_data) {
	if(ps.particles == NULL) {
		on_particle_apply_clicked();
		init_particle_system(&ps);
	}
	oldtime = 0;
	idle_tag = gtk_idle_add(particle_iterate, NULL);
}

void on_animate_clicked(GtkWidget *w, gpointer user_data) {
	if(ps.particles == NULL) {
		on_particle_apply_clicked();
		init_particle_system(&ps);
	}
	oldtime = 0;
	idle_tag = gtk_idle_add(particle_iterate, NULL);
}
	
void on_particle_system_settings_activate(GtkMenuItem *menuitem,
                                          gpointer user_data) {
	gtk_widget_show(particle_system_dlg);
}

void on_stop_activate(GtkMenuItem *menuitem, gpointer user_data) {
	gtk_idle_remove(idle_tag);
}

void on_stop_clicked(GtkWidget *w, gpointer user_data) {
	gtk_idle_remove(idle_tag);
}

void on_particle_okay_clicked(GtkWidget *w, gpointer user_data) {
	on_particle_apply_clicked(w, user_data);
	gtk_widget_hide(particle_system_dlg);
}

void on_particle_cancel_clicked(GtkWidget *w, gpointer user_data) {
	/* This should reset the old values of the dialog box, but doesn't */
	gtk_widget_hide(particle_system_dlg);
}

void on_reset_clicked(GtkWidget *w, gpointer user_data) {
	ps.old_num_particles = 0;
	init_particle_system(&ps);
}

void on_reset_activate(GtkMenuItem *m, gpointer user_data) {
	ps.old_num_particles = 0;
	init_particle_system(&ps);
}
