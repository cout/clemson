#ifndef TEXTURES_H
#define TEXTURES_H

enum tex_return_values {
        TEX_OK,
        TEXERR_OPEN,
        TEXERR_MAGIC,
        TEXERR_PLANES,
        TEXERR_COLORS,
        TEXERR_MALLOC
};

int bind_tex(const char* filename, int width, int height, int texnum);
int free_tex(int texnum);

#endif
