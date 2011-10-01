#include <stdio.h>
#include <string.h>
#include "tankdef.h"
#include "_string.h"

static char* find_space1(char *s) {
	for(; *s != ' ' && *s != '\t' && *s != 0; s++);
	return s;
}

static char* find_space2(char *s) {
	for(; (*s == ' ' || *s == '\t') && *s != 0; s++);
	return s;
}

static void strip_newline(char *s) {
	for(; *s != 0; s++)
		if(*s == '\r' || *s == '\n') *s = 0;
}

int read_tankdef(const char *filename, tank_t *tank) {
	FILE *fp;
	char s[2048], *t;
	int j, retval;

	if((fp = fopen(filename, "rt")) == NULL)
		return TDERR_OPEN;

	tank->name[0] = 0;
	tank->model[0] = 0;
	tank->weapon[0] = 0;
	tank->skin[0] = 0;
	for(j = 0; j < 3; j++) {
		tank->weapon_translate[j] = 0.0;
		tank->weapon_rotate[j] = 0.0;
	}

	retval = TD_OK;

	while(!feof(fp)) {
		fgets(s, sizeof(s), fp);
		if(s[0] == '#' || s[0] == '\r' || s[0] == '\n') continue;

		strip_newline(s);
		t = find_space1(s);
		if(*t == 0) {
			retval = TDERR_BADINPUT;
			continue;
		} else {
			*t = 0;
			t = find_space2(t+1);
		}
		
		if(!strcmpi(s, "name")) {
			strncpy(tank->name, t, sizeof(tank->name));
			tank->name[sizeof(tank->name)-1] = 0;
			printf("Name = %s\n", tank->name);
		} else if(!strcmpi(s, "model")) {
			strncpy(tank->model, t, sizeof(tank->model));
			tank->model[sizeof(tank->model)-1] = 0;
			printf("Model = %s\n", tank->model);
		} else if(!strcmpi(s, "weapon")) {
			strncpy(tank->weapon, t, sizeof(tank->weapon));
			tank->weapon[sizeof(tank->weapon)-1] = 0;
			printf("Weapon = %s\n", tank->weapon);
		} else if(!strcmpi(s, "skin")) {
			strncpy(tank->skin, t, sizeof(tank->skin));
			tank->skin[sizeof(tank->skin)-1] = 0;
			printf("Skin = %s\n", tank->skin);
		} else if(!strcmpi(s, "weapon_translate")) {
			sscanf(s, "%f %f %f", &tank->weapon_translate[0],
				&tank->weapon_translate[1], &tank->weapon_translate[2]);
		} else if(!strcmpi(s, "weapon_rotate")) {
			sscanf(s, "%f %f %f", &tank->weapon_rotate[0],
				&tank->weapon_rotate[1], &tank->weapon_rotate[2]);
		} else {
			retval = TDERR_BADINPUT;
		}
	}

	fclose(fp);
	return retval;
}
