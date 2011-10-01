#ifndef __RECORD_H

#define __RECORD_H

#define PKEYSIZE 32
#define FNAMESIZE 32
#define LNAMESIZE 32
#define ADDRSIZE 80
#define CITYSIZE 32
#define STATESIZE 3
#define ZIPSIZE 11
#define PHONESIZE 13

struct record {
	char pkey[PKEYSIZE];
	char fname[FNAMESIZE];
	char lname[LNAMESIZE];
	char addr1[ADDRSIZE];
	char addr2[ADDRSIZE];
	char city[CITYSIZE];
	char state[STATESIZE];
	char zip[ZIPSIZE];
	char phone[PHONESIZE];
};

#endif
