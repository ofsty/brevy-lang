#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vh.h"
#include "eh.h"

int main(int argc, char **argv) {
    if (argc != 2) { 
        printf("Usage: %s script_file\n", argv[0]); 
        return 1; 
    }
    
    FILE *f = fopen(argv[1], "r");
    if (!f) { 
        printf("Error: Cannot open file '%s'\n", argv[1]); 
        return 1; 
    }
    
    char l[256];
    while (fgets(l, sizeof(l), f) != NULL) {
        char *c = strchr(l, '#');
        if (c) *c = '\0';
        ev(l);
    }
    
    if (ferror(f)) {
        printf("Error reading file\n");
        fclose(f);
        return 1;
    }
    
    fclose(f);
    return 0;
}
