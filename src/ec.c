#include "eh.h"
#include "vh.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void ep(char *s) {
    s += 5;
    tr(s);
    if (*s == '(') s++;
    
    while (*s && *s != ')') {
        if (*s == '"' || *s == '\'') {
            char q = *s++;
            while (*s != q && *s) putchar(*s++);
            if (*s == q) s++;
        } 
        else if (isalpha(*s) || *s == '_') {
            char n[50], *p = n;
            while (isalnum(*s) || *s == '_') *p++ = *s++;
            *p = '\0';
            int i = fv(n);
            if (i != -1) pv(&vs[i]);
            else printf("?%s?", n);
        }
        else if (isdigit(*s) || *s == '+' || *s == '-') {
            char expr[100];
            char *p = expr;
            while (*s && *s != ',' && *s != ')' && !isspace(*s)) *p++ = *s++;
            *p = '\0';
            float res = eval_expr(expr);
            printf("%g", res);
        }
        
        tr(s);
        if (*s == ',') { printf(" "); s++; tr(s); }
    }
    printf("\n");
}

bool eval_cond(char *expr) {
    float a, b;
    char op[3];
    if (sscanf(expr, "%f%2s%f", &a, op, &b) == 3) {
        if (strcmp(op, ">") == 0) return a > b;
        if (strcmp(op, "<") == 0) return a < b;
        if (strcmp(op, ">=") == 0) return a >= b;
        if (strcmp(op, "<=") == 0) return a <= b;
        if (strcmp(op, "==") == 0) return fabs(a - b) < 0.0001;
        if (strcmp(op, "!=") == 0) return fabs(a - b) > 0.0001;
    }
    return false;
}

void ea(char *s) {
    char n[50], *p = n;
    while (isalnum(*s) || *s == '_') *p++ = *s++;
    *p = '\0';
    
    while (*s != '=') s++;
    s++;
    tr(s);
    
    if (strstr(s, "+=") || strstr(s, "-=") || strstr(s, "*=") || strstr(s, "/=")) {
        char op = s[strlen(n)];
        float val = atof(s + strlen(n) + 2);
        int idx = fv(n);
        if (idx != -1) {
            if (vs[idx].t == I) {
                switch(op) {
                    case '+': vs[idx].v.i += (int)val; break;
                    case '-': vs[idx].v.i -= (int)val; break;
                    case '*': vs[idx].v.i *= (int)val; break;
                    case '/': vs[idx].v.i /= (int)val; break;
                }
            } else {
                switch(op) {
                    case '+': vs[idx].v.f += val; break;
                    case '-': vs[idx].v.f -= val; break;
                    case '*': vs[idx].v.f *= val; break;
                    case '/': vs[idx].v.f /= val; break;
                }
            }
        }
        return;
    }
    
    char *expr = s;
    if (strpbrk(expr, "+-*/%^")) {
        float res = eval_expr(expr);
        if (strchr(expr, '.')) {
            sv(n, F, &res);
        } else {
            int ires = (int)res;
            sv(n, I, &ires);
        }
        return;
    }
    
    char v[100];
    strncpy(v, s, 99);
    v[99] = '\0';
    
    T t = gt(v);
    if (t == (T)-1) { printf("Bad type\n"); return; }
    
    if (t == B) {
        bool b = !strcmp(v, "True");
        sv(n, B, &b);
    }
    else if (t == S) {
        char *p = v;
        if (*p == '"' || *p == '\'') p++;
        char *e = p + strlen(p) - 1;
        if (*e == '"' || *e == '\'') *e = '\0';
        sv(n, S, p);
    }
    else if (t == I) {
        int i = atoi(v);
        sv(n, I, &i);
    }
    else if (t == F) {
        float f = atof(v);
        sv(n, F, &f);
    }
}

void ev(char *l) {
    tr(l);
    if (!*l || *l == '#') return;
    
    if (strncmp(l, "print", 5) == 0) ep(l);
    else if (strstr(l, "==") || strstr(l, "!=") || strstr(l, ">=") || strstr(l, "<=") ||
             strstr(l, ">") || strstr(l, "<")) {
        printf(eval_cond(l) ? "True\n" : "False\n");
    }
    else if (isalpha(*l) || *l == '_') ea(l);
    else printf("Unknown cmd\n");
}
