#ifndef VH_H
#define VH_H

#include <stdbool.h>
#include <string.h>
#include <math.h>

#define MX_V 100
#define MX_S 100

typedef enum { I, F, B, S } T;

typedef struct {
    char n[50];
    T t;
    union {
        int i;
        float f;
        bool b;
        char s[MX_S];
    } v;
} V;

extern V vs[MX_V];
extern int vc;

void tr(char *s);
int fv(char *n);
void sv(char *n, T t, void *v);
T gt(char *s);
void pv(V *v);
float eval_expr(char *expr);

#endif
