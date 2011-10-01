#include <stdio.h>
#include <stdlib.h>

int o[]={0,0,0, 1,0,0, 0,1,0, 1,1,0, 0,0,1, 1,0,1, 0,1,1, 1,1,1};

int main(int argc, char *argv[]) {
	FILE *fp;
	unsigned char b[512];
	unsigned char *p,*s;
	unsigned int w,h,j,x,y,z,t;
	unsigned int c[3];
	int n,d,e;
	char g;

	fp = fopen(argv[1], "r");
	fgets(b,512,fp);do{fgets(b,512,fp);}while(b[0]=='#');
	for(p=s=b;*p!=' ';p++);*p=0;w=atoi(s);
	p++;for(s=p;*p!='\n';p++);*p=0;h=atoi(s);
	fgets(b,512,fp);
	s=malloc(3*w*h);
	fread(s,1,3*w*h,fp);
	fclose(fp);

	printf("\e[2J");
	for(y=0;y<h&&y<200;y+=8) {
		for(x=0;x<w&&x<640;x+=8) {
			for(j=0; j<3;j++) {
				for(t=0;t<8;t++) {
					z=3*(y+t)*w+3*x+j;
					c[j]=s[z]+s[z+3]+s[z+6]+s[z+9]+s[z+12];
					c[j]+=s[z+15]+s[z+18]+s[z+21];
				}
				c[j]>>=5;
			}
			g = ' ';
			if(c[0]+c[1]+c[2] & 0x20) g = '.';
			if(c[0]+c[1]+c[2] & 0x40) g = 'o';
			if(c[0]+c[1]+c[2] & 0x80) g = 'O';
			c[0]>>=5;c[1]>>=5;c[2]>>=5;
			d=100;e=0;
			for(j=0;j<8;j++) {
				n=(c[0]-o[j*3])*(c[0]-o[j*3])
				+(c[1]-o[j*3+1])*(c[1]-o[j*3+1])
				+(c[2]-o[j*3+2])*(c[2]-o[j*3+2]);
				if(n<0)n=-n;
				if(n<d){d=n;e=j;}
			}
			printf("\e[%dm\e[%d;%dH%c\n",30+e,1+y/8,1+x/8,g);
		}
	}	

	free(s);
}
