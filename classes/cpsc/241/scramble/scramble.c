#include <stdio.h>
#include <stdlib.h>

int primes[] = {2, 3, 5, 7, 11, 13, 19, 23, 29, 31, 37, 41, 43, 47, 53,
59, 61, 67, 71, 73, 79, 83, 89, 97, 101};

long long hash(char *s) {
	long long h = 1;
	char c;
	while(isalpha(*s)) {
		if(*s <= 'Z') c = *s - 'A'; else c = *s - 'a';
		h *= primes[c];
		s++;
	}
	return h;
}
 
int main(int argc, char *argv[]) {
	FILE *fp;
	long long h;

	if(argc != 2) {
		printf("Usage %s <%s>\n", argv[0], "scrambled word");
		exit(1);
	}

	if((fp = fopen("words", "r")) == NULL) {
		printf("Unable to open file %s for reading\n", "words");
		exit(1);
	}

	h = hash(argv[1]);

	while(!feof(fp)) {
		char s[2048];
		fgets(s, sizeof(s), fp);
		if(hash(s) == h) fputs(s, stdout);
	}

	fclose(fp);

	return 0;
}
