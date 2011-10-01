#ifdef WIN32
#include <windows.h>
#endif

#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#include <process.h>
#endif

#include "midi.h"

void playmidifile(const char *file) {
	char s[64000];
	char b[80];
#ifdef WIN32
	strcpy(s, "open ");
	strncat(s, file, sizeof(s));
	strncat(s, " alias music", sizeof(s));
	mciSendString((LPSTR)s, b, sizeof(b), NULL);
	mciSendString((LPSTR)"play music", b, 80, NULL);
#else
#ifdef SGI
	strcpy(s, "soundplayer ");
	strcat(s, file);
#else
	strcpy(s, "cat ");
	strcat(s, file);
	strcat(s, "> /dev/midi &");
#endif
	system(s);
#endif
}
