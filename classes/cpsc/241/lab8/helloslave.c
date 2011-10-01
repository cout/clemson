/* helloslave.c
 * Paul Brannan
 * 3/9/98
 */

#include <stdio.h>
#include <stdlib.h>
#include "hello.h"

/* Global variables */
int mytid, master, node;

main() {
	
	/* enroll in pvm */
	mytid = pvm_mytid();
	master = pvm_parent();

	/* wait for data */
	pvm_recv(master, GO);
	pvm_upkint(&node, 1, 1);

	/* Print our message */
	printf("Hello, from node %d, to the world!\n", node);
	
	/* Tell master we are done */
	pvm_initsend(PvmDataDefault);
	pvm_pkint(&node, 1, 1);
	pvm_send(master, DONE);

	/* Clean up */
	pvm_exit();

	return 0;
}
