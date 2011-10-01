#include <GL/glut.h>
#include "npr_draw.h"

/* This is the model used at the U of Utah for generating edge lines */
void npr_draw_model(md2_model_t model, int currentFrame) {
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	/* This is a hack, but it affects speed */
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/* Draw the model */
	glCallList(model.framestart + currentFrame);

	/* Set up for drawing edges only */
	glEnable(GL_STENCIL_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.0, 0.0, 0.0);
	glScalef(1.005, 1.005, 1.005);
	glDisable(GL_LIGHTING);
	glLineWidth(2.0);

	/* Draw the edges for the front-facing polys into the stencil
	 * buffer */
	glDrawBuffer(GL_NONE);
	glStencilFunc(GL_ALWAYS, 0x1, 0x1);
	glStencilOp(GL_REPLACE, GL_KEEP, GL_REPLACE);
	glCullFace(GL_FRONT);
	glCallList(model.framestart + currentFrame);

	/* Draw the edges for the back-facing polys only if the stencil
	 * buffer is set already */
	glDrawBuffer(GL_BACK);
	glStencilFunc(GL_LEQUAL, 0x1, 0x1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glCullFace(GL_BACK);
	glCallList(model.framestart + currentFrame);

	/* Now, make another pass so we can handle one-sided polys */
	/* Why doesn't this work? */
/*	glDrawBuffer(GL_NONE);
	glClear(GL_STENCIL_BUFFER_BIT);
	glStencilFunc(GL_ALWAYS, 0x1, 0x1);
	glStencilOp(GL_REPLACE, GL_KEEP, GL_REPLACE);
	glCallList(model.framestart + currentFrame);

	glDrawBuffer(GL_BACK);
	glStencilFunc(GL_LEQUAL, 0x1, 0x1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glCallList(model.framestart + currentFrame);*/
	
	glPopAttrib();
}

