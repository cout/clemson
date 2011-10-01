main(){int x,y,r;for(x=y=0;;r=rand()%3){if(!r)x+=96;if(r>1)x+=48,y+=24;printf("\e[%d;%dH*",y/=2,x/=2);}}
