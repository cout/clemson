#ifndef DRAWTANK_H
#define DRAWTANK_H

typedef struct {
	char name[64];
	char model[64];
	char weapon[64];
	char skin[64];
	float weapon_translate[3];
	float weapon_rotate[3];
} tank_t;

enum td_return_values {
	TD_OK,
	TDERR_OPEN,
	TDERR_BADINPUT
};

int read_tankdef(const char *filename, tank_t *tank);

#endif

