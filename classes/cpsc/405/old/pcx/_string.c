#include <ctype.h>
#include "string.h"

#ifndef WIN32
int strcmpi(const char *s1, const char *s2) {
	for(; *s1 != 0 && *s2 != 0; s1++, s2++) {
		if(isalpha(*s1) && isalpha(*s2)) {
			if(tolower(*s1) > tolower(*s2)) return 1;
			if(tolower(*s1) < tolower(*s2)) return -1;
		} else {
			if(*s1 > *s2) return 1;
			if(*s1 < *s2) return -1;
		}
	}
	if(*s1 > *s2) return 1;
	if(*s1 < *s2) return -1;
	return 0;
}
#endif
