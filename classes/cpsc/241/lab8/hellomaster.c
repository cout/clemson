/* hellomaster.c
 * Paul Brannan
 * 3/9/98
 */

#include <stdio.h>
#include <stdlib.h>
#include "hello.h"

#define PROCS 4

/* Global variables */
int tids[MAX_PROC], mytid, node;

main() {
	int j;

	/* enroll in pvm */
	mytid = pvm_mytid();

	/* Initialize output */
	pvm_catchout(stdout);

	/* start slave tasks */
	pvm_spawn("helloslave", (char **)0, PvmTaskDefault, "", PROCS, tids);

	/* Start main loop */
	for(j = 0; j < PROCS; j++) {
		/* Initialize send */
		pvm_initsend(PvmDataDefault);

		/* send GO signal */
		pvm_pkint(&j, 1, 1);
		pvm_send(tids[j], GO);

		/* wait for result */
		pvm_recv(tids[j], DONE);
		pvm_upkint(&node, 1, 1);
		if(node != j) {
			fprintf(stderr, "Houston, we have a problem.\n");
			fprintf(stderr, "Expecting node %d, ", j);
			fprintf(stderr, "got node %d\n", node);
			pvm_exit();
			exit(1);
		}

		/* flush stdout */
		fflush(stdout);
	}

	/* Clean up */
	pvm_exit();

	return 0;
}
