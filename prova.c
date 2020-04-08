#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main() {
    char* s2 = malloc(20*sizeof(char));
    fgets(s2, sizeof(s2), stdin);
    printf("%s", s2);
}