#ifndef PCX_H
#define PCX_H

enum pcx_return_values {
	PCX_OK,
	PCXERR_OPEN,
	PCXERR_MAGIC,
	PCXERR_PLANES,
	PCXERR_COLORS,
	PCXERR_MALLOC
};

#ifdef byte
#define pcx_byte byte
#else
#define pcx_byte unsigned char
#endif

int load_pcx(pcx_byte** buffer, const char* filename, int* width, int* height); 
int bind_pcx(const char* filename, int width, int height, int texnum);

/* Redefine this to get better colors on different platforms */
#ifndef PCX_TEXRES
#define PCX_TEXRES GL_RGB
#endif

#endif

