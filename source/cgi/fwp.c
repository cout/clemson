#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>

/* Hostname information */
#define HOST "172.16.27.150"
#define PORT 80

int sock_gets(int sock, char *str, int len) {
	char c = 0;
	int count = 0;

	while(c != '\n') {
		if(read(sock, &c, 1) <= 0) return -1;
		if(count < len && c != '\n' && c != '\r') str[count++] = c;
	}

	if(count) str[count] = 0;

	return count;
}

int strncmpi(char *str1, char *str2, int n) {
	int j = 0;
	while(*str1 != 0 && *str2 != 0 && j < n) {
		if(toupper(*str1) != toupper(*str2)) return *str1 - *str2;
		j++; str1++; str2++;
	}
	return 0;
}

main(int argc, char *argv[]) {
	struct sockaddr_in sa;
	struct hostent *hp;
	int sock;
	char buf[2048];
	char c;
	char *addr = NULL, *query = NULL;
	int flag;

	/* Get the name of the requested file */
	addr = (char *)getenv("PATH_INFO");
	if(addr == NULL) addr = (char *)getenv("QUERY_STRING");
	else query = (char *)getenv("QUERY_STRING");

	/* If we didn't get an address, then redirect the request */
	if(addr == NULL || *addr == 0) {
		puts("Content-type: text/html\n");
		puts("<HTML>");
		printf("<META HTTP-EQUIV=\"Refresh\" CONTENT=\"10,./%s//\">\n",
			argv[0]);
		puts("<BODY>");
		puts("You have specified an illegal request.  Please");
		printf("click <a href=\"./%s/\">here</a> if your\n", argv[0]);
		puts("browser does not forward you within 10 seconds.");
		puts("</BODY></HTML>");
		return 0;
	}

	/* Get the host's address */
	if((hp = gethostbyname(HOST)) == NULL) exit(1);

	/* Set up the information necessary to connect */
	memset(&sa, 0, sizeof(sa));
	memcpy((char *)&sa.sin_addr.s_addr, hp->h_addr, hp->h_length);
	sa.sin_family = hp->h_addrtype;
	sa.sin_port = htons ((u_short)PORT);

	/* Connect to the server */
	if ((sock = socket(sa.sin_family, SOCK_STREAM, 0)) < 0) exit(1);
	if(connect(sock, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
		shutdown(sock, 2);
		exit(1);
	}

	/* Send the request */
	write(sock, "GET ", 4);
	write(sock, addr, strlen(addr));
	if(query) {
		write(sock, "?", 1);
		write(sock, query, strlen(query));
	}
	write(sock, " HTTP/1.0\r\n\r\n", 13);

	/* Get the content type from the server */
	flag = 0;
	while(sock_gets(sock, buf, sizeof(buf)) > 0) {
		if(!strncmpi(buf, "Content-type: ", 14)) {
			puts(buf); puts("");
			flag = 1;
		} else if(*buf == '\r' || *buf == '\n') {
			if(!flag) puts("Content-type: text/html\n");
			break;
		}
	}
			
	/* Return the server's response */
	while(read(sock, &c, 1) > 0) {
		fputc(c, stdout);
	}

	/* Close the connection and return */
	shutdown(sock, 2);
	return 0;
}
