#include <conio.h>
#include <dir.h>

#define cls clrscr()
#define locate(y,x) gotoxy (x,y)
#define color(fore,back) textbackground(back); textcolor(fore)
#define pr(n) cprintf(" %-13s \r",dblk[n].ff_name)
#define beep putch(7)

int system(const char *command);
int strcmp(const char *s1, const char *s2);
void gettime(struct time *timep);
void getdate(struct date *datep);

void sort(void);
void driveselect(int drives);
void dispclock(void);

struct date {
  int   da_year;
  char  da_day;
  char  da_mon;
};
struct time {
  unsigned char  ti_min;
  unsigned char  ti_hour;
  unsigned char  ti_hund;
  unsigned char  ti_sec;
};
struct ffblk dblk[128];
struct ffblk ffblk;
int i=0,j=0;
	
int main(void) {
	int done;
	int drives;
	int top=1;
	int sp=1;
	char cmd1[120]="\0",cmd2[120]="\0",cmd3[120]="\0";
	int cctr=0;
	char path[MAXPATH];
	unsigned char c;
	start1:
	;
	_setcursortype(_NOCURSOR);
	cls;
	cprintf("浜様様様様様様様様�\r\n");
	for(i=1;i<21;i++) {
		cprintf("�                 �\r\n");
	}
	cprintf("藩様様様様様様様様�\r\n");
	locate(8,21);
	cprintf("浜様様様様� 浜様様様様様様様様様様様様様様様様様様様様様様融\r\n");
	for(i=1;i<15;i++) {
		locate(i+8,21);
		cprintf("�         � �                                              �\r\n");
	}
	locate(22,21);
	cprintf("藩様様様様� 藩様様様様様様様様様様様様様様様様様様様様様様夕\r\n");
	locate(1,20);
	cprintf("               PB-Shell              浜様様様様用様様様様様�");
	locate(2,20);
	cprintf("       written by Paul Brannan       �                     �");
	locate(3,20);
	cprintf("                                     藩様様様様溶様様様様様�");
	locate(4,20);
	cprintf(" 浜様様様様様様様様様様様様様様様様様様様様様様様様様様様様融\r\n");
	locate(5,20);
	cprintf(" �                                                          �\r\n");
	locate(6,20);
	cprintf(" �                                                          �\r\n");
	locate(7,20);
	cprintf(" 藩様様様様様様様様様様様様様様様様様様様様様様様様様様様様夕\r\n");
	start2:
	;
	sp=top=1;
	j=0;
	done=getdisk();
	drives=setdisk(done);
	window(23,9,29,22);
	for(i=1;(i<(drives+1)) & (i<14);i++) cprintf(" [-%c-] ",'A'+i-1);
	window(3,2,18,21);
	done = findfirst("*.*",&ffblk,16);
	while((!done)&(j<128)) {
		j++;
		dblk[j]=ffblk;
		done = findnext(&ffblk);
	}
	sort();
	begin:
	;
	cls;
	locate(1,1);
	for(i=1;(i<22) & ((i+top)<j+1);i++) {
		pr(i+top);
		if(i<20) cprintf("\n");
	}
	locate(1,1);
	do {
		if(sp+top>j) {
			sp=1;
			top=j-1;
			goto begin;
		}
		window(1,24,80,25);
		cls;
		getcurdir(0, path);
		c='A'+getdisk();
		cprintf("%c:\\%s> %s_",c,path,cmd1);
		window(23,5,79,6);
		cls;
		getcurdir(0, path);
		c='A'+getdisk();
		cprintf("Directory: %c:\\%s\r\n",c,path);
		cprintf("%-13s ",dblk[sp+top].ff_name);
		if(dblk[sp+top].ff_attrib==32) {
			cprintf("%ld         ",dblk[sp+top].ff_fsize);
		}
		else {
			cprintf("  <DIR>   ");
		}
		locate(2,25);
		cprintf("%2.2u-%2.2u-%2.2u",(dblk[sp+top].ff_fdate>>5)&31,dblk[sp+top].ff_fdate & 31,((dblk[sp+top].ff_fdate>>9)&127)+80);
		i=(dblk[sp+top].ff_ftime>>11)&31;
		if(i>12) {
			i=i-12;
			c='p';
		}
		else {
			c='a';
		}
		cprintf(" %2.2u:%u%c ",i,(dblk[sp+top].ff_ftime>>5)&63,c);
		window(3,2,18,21);
		locate(sp,1);
		color(0,7);
		pr(sp+top);
		color(7,0);
		while(!kbhit()) {
			dispclock();
		}
		window(3,2,18,21);
		c=getch();
		if(c==0) {
			c=getch();
			switch(c) {
				case	72:
					locate(sp,1);
					pr(sp+top);
					if(sp<2) {
						if(top>1) {
							locate(1,1);
							insline();
							locate(20,1);
							pr(top+21);
							top--;
							if(top<2) top=1;
						}
					}
					else {
						sp--;
					}
					break;
				case 80:
					locate(sp,1);
					pr(sp+top);
					if(sp>19) {
						if((sp+top)<j) {
							locate(1,1);
							delline();
							locate(1,1);
							pr(top+1);
							top++;
							if(top>(j-1)) top=j;
						}
					}
					else {
						if((sp+top)<j) sp++;
					}
					break;
				case 75:
陳陳陳陳陳陳陳陳陳陳陳陳陳陳� 団陳陳賃陳陳陳堕陳陳賃陳陳陳堕陳陳賃陳陳陳堕陳陳調 団陳陳津陳陳陳田陳陳津陳陳陳田陳陳津陳陳陳田陳陳調 �      �      �      �      �      �      �      � 団陳陳珍陳陳陳祖陳陳珍陳陳陳祖陳陳珍陳陳陳祖陳陳調 %s %d  %48s %s, %s %d, %d %s, %d. %s %d  %40s   %2d%c         %-48s *.CAL CAL r CALENDAR
 TERMINKALENDER
 
 CAL w CALENDAR
 %08ld*
 %08ld
 		%04d%.1000s
   ����          �L$W�{     Y$W       �m$W��     �m$W   
   �}$Wd                %04d    ����        