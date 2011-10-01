#include <stdio.h>
main() {
        char s[81]; int j;
        for(j = !0;;j = !j) {
                fgets(s, sizeof(s), stdin);
                if(j) fputs(s, stdout);
        }
}


