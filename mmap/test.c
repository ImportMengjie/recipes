#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(){
    int len = 1024;
    char* str = malloc(len+1);
    printf("str=> %u\n", str);
    str[len] = '\0';
    for(int i=0;i<len;i++)
        str[i] = '0' + (i%10);
    printf("%s\n", str);

    int nnums = 10;
    char* cmalloc_p = calloc(len, nnums);
    for(int i=0;i<len*nnums;i++){
        assert(cmalloc_p[i]==0);
    }

    str = realloc(str, len*10);
    printf("realloc %s\n",str);

    free(str);

    return 0;
}
