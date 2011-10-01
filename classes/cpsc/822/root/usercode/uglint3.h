/* #define O_RDWR (0x2) */

struct u_glint_device {
	unsigned int *control_base;
	unsigned int *local_buffer_base;
	unsigned int *frame_buffer_base;
	struct dmagob {
		unsigned int phys_base;
		unsigned int *virt_base;
		unsigned int *kbase;
		} dmagob[2];
} glint_board;

#define THE_BIG_BUCKET (128*1024)
#define ONE_PAGE (4096) 
#define MAX_DMA (THE_BIG_BUCKET - ONE_PAGE)
#define V_OFF 0
#define V_1024x768_60Hz 1
#define V_640x480_75Hz 2
#define V_800x600_60Hz 3

#define EVAL_CONTROL(register) (*(glint_board.control_base+(register>>2)))
#define LOAD_CONTROL(register,value) ((*(glint_board.control_base+(register>>2)))=value)

#define BIND_CONTROL 0
#define BIND_FRAMEB 1
#define BIND_LOCALB 2
#define BIND_DMA 3
#define VMODE 4

/* registers the user needs to hit directly */
#define InFIFOSpace (0x0018)
#define DMAAddress (0x0028)
#define DMACount (0x0030)
#define RStart (0x8780)
#define GStart (0x8798)
#define BStart (0x87b0)
#define dRdyDom (0x8790)
#define dGdyDom (0x87a8)
#define dBdyDom (0x87c0)
#define dRdx (0x8788)
#define dGdx (0x87a0)
#define dBdx (0x87b8)
#define RasterizerMode (0x80a0)
#define StartXDom (0x8000)
#define dXDom (0x8008)
#define StartXSub (0x8010)
#define dXSub (0x8018)
#define StartY (0x8020)
#define YStart (0x8020)
#define dY (0x8028)
#define Count (0x8030)
#define Render (0x8038)
#define ContinueNewSub (0x8050)
#define DepthMode (0x89a0)
#define ZStartU (0x89b0)
#define ZStartL (0x89b8)
#define dZdyDomU (0x89d0)
#define dZdyDomL (0x89d8)
#define dZdxU (0x89c0)
#define dZdxL (0x89c8)

/* register values */
#define RENDER_TRAPEZOID_SPC (0x10040)
#define RENDER_POINT (0x0080)

#define FIFOSYNC while(EVAL_CONTROL(InFIFOSpace)<16)

/* 
color values are 9.15 two's complement, so 0x007fffff is max; 
coordinates are 16.16 two's complement, so 0x7fffffff is max;
we'll use floats on the arithmetic with coordinates in pixels and
colors in the range 0.0 - 255.0, and then convert before loading
*/

/*
these cryptic little buggers:
1) convert float color in range [-255.0,255.0] to fixed point 9.15 two's 
   complement integer 

2) convert float coord in range [-32767.0,32767.0] to fixed point 16.16 two's 
   complement integer

floats outside either range are just capped; since cap test is on exponent,
you can actually go fractionally above these limits, e.g.
255.99995 -> 0x007ffffe, 1 point below max

3) convert register to associated tag
*/

#define EX0 (127)

int makecolor(float x)
{
int ix;
int sx;
unsigned char ex;

ix = *((int *)&x);
sx = ix>>31;
ex = (unsigned char)(ix>>23);
if(ex<(EX0 - 15)) return(0);
if(ex>=(EX0 + 8)) return(0x007fffff^sx);
ix = ((unsigned int)((ix|0x00800000)<<8)>>(EX0 + 16 - ex));
if(sx<0) ix = -ix;
return(ix);
}

int makecoord(float x)
{
int ix;
int sx;
unsigned char ex;

ix = *((int *)&x);
sx = ix>>31;
ex = (unsigned char)(ix>>23);
if(ex<(EX0 - 16)) return(0);
if(ex>=(EX0 + 15)) return(0x7fffffff^sx);
ix = ((unsigned int)((ix|0x00800000)<<8)>>(EX0 + 15 - ex));
if(sx<0) ix = -ix;
return(ix);
}

int maketag(int reg)
{
int tag;

tag = (reg - 0x8000)/8;
return(tag);
}

