#ifndef SCENE_H
#define SCENE_H

#include "md2.h"

#define MAX_ITEMS 1024

typedef struct {
	char name[64];
	char modelfile[64];
	char skinfile[64];
	float rotate1[4];
	float rotate2[4];
	float rotate3[4];
	float translate[3];
	float scale[3];
	md2_model_t model;
	int item_type;
	float params[9];
	int draw_outline;
	int tex_type;
	float tex_s[3];
	float tex_t[3];
} item_t;

enum item_types {
	ITEM_NONE,
	ITEM_MD2,
	ITEM_BOX,
	ITEM_TERRAIN
};

typedef struct {
	int num_items;
	item_t item[MAX_ITEMS];
} scene_t;

enum sd_return_values {
	SD_OK,
	SDERR_OPEN,
	SDERR_BADINPUT
};

int read_scenedef(const char *filename, scene_t *scene);
void make_models();
void init_scene();
void draw_scene();
void draw_stars();

#endif

