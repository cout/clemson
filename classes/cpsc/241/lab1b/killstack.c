#include <stdio.h>

#define PIGS_DONT_FLY 1

int main() {
	FILE *fp;

	fp = popen("./a.out", "w");

	while(PIGS_DONT_FLY) {
		fputs("65\n", fp);
	}
}
