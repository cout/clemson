#ifndef TEXCOORDS_H
#define TEXCOORDS_H

void do_tex_coords(float s, float t, float x, float y, float z, int tt);
void start_tex(int tt);
void init_texgen();

enum texture_types {
        TEX_NONE,
        TEX_STD,
        TEX_GRID,
        TEX_DBLGRID,
	TEX_CONTOUR,
	TEX_MAP2,
	TEX_GRID3
};

#endif

