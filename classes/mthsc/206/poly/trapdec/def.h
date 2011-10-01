/* COPYRIGHT - Dimitrios Gunopulos, Princeton University, 1994 */
#define MAXV 1000
#define LEFTMOST_EDGE -2
#define RIGHTMOST_EDGE -3
#define UPPER_SPACE -2
#define LOWER_SPACE -3
#define PLUSE 1e-12
#define pluse 0.000000001
#define MINUSE -1e-12
#define OOGL 1
#define DEFAULT 0

/* Definitions of structures */

/* Polygon stored as a list of vertices */

typedef struct POLYGON {

	int nverts;
	float verts[MAXV];

} polygon;

typedef polygon *Polygon;



/* Stored polygon with holes (or divided in regions) */

typedef struct CHARACTER {

   short movex, movey; 
   short llx, lly, urx, ury;
   int nloops;
   int *nverts;
   float *verts;

} character;

typedef character *Character;




typedef struct EDGE *Edge;

typedef struct EDGE_LIST *EdgeL;

typedef struct TRAPEZOID_LIST *TrapezoidL;




typedef struct VERTEX *Vertex;

/* A 2-D vertex, it points to 2 edges */

typedef struct VERTEX {

    float x, y;
    EdgeL e1, e2;

} vertex ;

typedef struct VERTEX_LIST *VertexL;

typedef struct VERTEX_LIST {

	vertex v;
	VertexL next;

} vertexL;




/* An edge points to 2 vertices. It can be in one of log*n phases */

typedef struct EDGE {

    Vertex vu, vd;	
    int used;
    int phase;
    int no;

} edge;

typedef struct EDGE_LIST {

    edge e;
    EdgeL next;
    TrapezoidL left, right;

} edgeL;




/* Two special vertices are created for each edge in the execution
   of the nlogn subroutine. */

typedef struct SPECIAL_VERTEX {

    float y;
    int up;
    EdgeL ed;

} svertex;

typedef svertex *SVertex;




/*  A trapezoid is defined by two edges, on the left and the right
    and two vertices on top and bottom. */

typedef struct TRAPEZOID {

    Vertex up, down;
    EdgeL left, right;

    int no; 			/* for debugging */

} trapezoid;

typedef trapezoid *Trapezoid;

typedef struct REGION *Region;




/* Pointers upl, upm, upr, downl, downm, downr point to the at most 6
   trapoezoids that can be next to the given trapezoid (from above
   or below - no across edge pointers). 
   To get the trapezoids that are in the interior of the polygon,
   only upl, upr, downl, downr are used.
   The pointers upm, downm, point to trapezoids across the boundary
   of the polygon.
   The other pointers are used if the trapezoid is in the upper or 
   lower limit of the region. 
   Finally each trapezoid forms a new region in the next phase of the 
   algorithm. */

typedef struct TRAPEZOID_LIST {

    trapezoid t;
    TrapezoidL upl, upm, upr, downl, downm, downr;
    TrapezoidL upnext, upprev, downnext, downprev;
    Region er;

} trapezoidL;




typedef struct REGION {

    TrapezoidL topleft, topright, bottomleft, bottomright;
    EdgeL edges;
    SVertex vertices;
    EdgeL leftedge, rightedge;
    Vertex topvertex, bottomvertex;

} region;




typedef int Font;




/* rbtree definitions */
/* A node is in the red-black tree, and also in a
   doubly linked list that keeps the elements in sorted order. */

typedef struct TREE_NODE {

	int left, right;     	/* in tree */
	int parent;   		/* in tree */
	int prev, next;		/* in list */
	int color;
	EdgeL keyptr;

} treenode;

typedef treenode *Treenode;




typedef struct TREE_ROOT {

	int cur;
	Treenode table;
	int root;

} treeroot;

typedef treeroot *Treeroot;

