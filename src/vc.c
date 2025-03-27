#include "vh.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

V vs[MX_V];
int vc = 0;

void tr(char *s) {
    char *p = s, *e = s + strlen(s);
    while (isspace(*p)) p++;
    while (e > p && isspace(*(e-1))) e--;
    memmove(s, p, e - p);
    s[e - p] = '\0';
}

int fv(char *n) {
    for (int i = 0; i < vc; i++)
        if (!strcmp(vs[i].n, n)) return i;
    return -1;
}

float eval_expr(char *expr) {
    float a, b;
    char op;
    if (sscanf(expr, "%f%c%f", &a, &op, &b) != 3) {
        printf("ERROR: Bad expression '%s'\n", expr);
        return 0;
    }
    switch(op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return b != 0 ? a / b : 0;
        default: return 0;
    }
}

void sv(char *n, T t, void *v) {
    if (strpbrk(n, "+-*/%^")) {
        float res = eval_expr(n);
        if (strchr(n, '.')) {
            sv(n, F, &res);
        } else {
            int ires = (int)res;
            sv(n, I, &ires);
        }
        return;
    }
    
    int i = fv(n);
    if (i == -1) {
        if (vc >= MX_V) { printf("Too many vars\n"); return; }
        strcpy(vs[vc].n, n);
        i = vc++;
    }
    vs[i].t = t;
    switch (t) {
        case I: vs[i].v.i = *(int*)v; break;
        case F: vs[i].v.f = *(float*)v; break;
        case B: vs[i].v.b = *(bool*)v; break;
        case S: strncpy(vs[i].v.s, (char*)v, MX_S-1); break;
    }
}

T gt(char *s) {
    if (!strcmp(s, "True") || !strcmp(s, "False")) return B;
    if (*s == '"' || *s == '\'') return S;
    
    char *p = s;
    if (*p == '-' || *p == '+') p++;
    int d = 0;
    while (*p) {
        if (*p == '.') d++;
        else if (!isdigit(*p)) return (T)-1;
        p++;
    }
    return d ? F : I;
}

void pv(V *v) {
    switch (v->t) {
        case I: printf("%d", v->v.i); break;
        case F: printf("%g", v->v.f); break;
        case B: printf(v->v.b ? "True" : "False"); break;
        case S: printf("%s", v->v.s); break;
    }
}
