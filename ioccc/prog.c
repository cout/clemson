#include <stdio.h>
int o[]={0,0,0,1,0,0,0,1,0,1,1,0,0,0,1,1,0,1,0,1,1,1,1,1};int main(int entional,
char **zar){FILE *me;unsigned char b[512],*p,*s,g;unsigned int w,h,j,x,y,z,t,_[3
];int n,d,e;b[0]=0162;b[1]=0x0;me=fopen(zar[1],b);fgets(b,512,me);do{fgets(b,512
,me);}while(b[0]==0x23);for(p=s=b;*p!=32;p++);*p=0;w=atoi(s);p++;for(s=p;*p!=32;
p++);*p=0;h=atoi(s);fgets(b,01000,me);s=(char*)malloc(3*w*h);fread(s,1,3*w*h,me)
;fclose(me);printf("\e[2J");for(y=0;y<h;y+=8){for(x=0;x<w;x+=8){for(j=0;j<3;j++)
{for(t=0;t<010;t++){z=3*(y+t)*w+3*x+j;_[j]=s[z]+s[z+3]+s[z+6]+s[z+9]+s[z+12];_[j
]+=s[z+15]+s[z+18]+s[z+21];}_[j]>>=5;}g=32;if(_[0]+_[1]+_[2]&32)g=056;if(_[0]+_[
1]+_[2]&64)g=111;if(_[0]+_[1]+_[2]&0x80)g=0117;_[0]>>=5;_[1]>>=5;_[2]>>=5;d=100;
e=0;for(j=0;j<8;j++){n=(_[0]-o[j*3])*(_[0]-o[j*3])+(_[1]-o[j*3+1])*(_[1]-o[j*3+1
])+(_[2]-o[j*3+2])*(_[2]-o[j*3+2]);if(n<0)n=-n;if(n<d){d=n;e=j;}}printf("\e""[%"
"dm\e[%d;%dH%c",30+e,1+y/8,1+x/8,g);}}free(s);puts("");}
