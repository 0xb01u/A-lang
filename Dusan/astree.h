/**
 * @Author: Dušan Kolář
 * @Year:   2003-2018
 * Copyright (c) 2018
 * Licence: GLP 3.0
 */

#ifndef ___ASTREE_H___
#define ___ASTREE_H___

// Se define el tipo ast_t (tipo AST)
// como un alias para la estructura ast_s (estructura AST)
typedef struct ast_s {
    unsigned tag;		// Etiqueta del nodo
    unsigned lineno;	// Línea del nodo
    union {				// Solo 1 de entre 3 tipos de nodos:
        struct {
            struct ast_s *lft, *rgt;	// Nodo con dos hijos (derecho e izquierdo)
        } ptr;
        char   *sVal;	// Nodo con una string.
        double  dVal;	// Nodo con un número.
    } u;
} ast_t;

#define tag(x) (x->tag)
#define lnum(x) (x->lineno)
#define left(x) (x->u.ptr.lft)
#define right(x) (x->u.ptr.rgt)
#define sv(x) (x->u.sVal)
#define dv(x) (x->u.dVal)

// Crear hoja String
ast_t *mkSlf(unsigned tag, char *str);
// Crear hoja double (número)
ast_t *mkDlf(unsigned tag, double dval);
// Crear nodo (padre de l y r)
ast_t *mkNd(unsigned tag, ast_t *l, ast_t *r);
ast_t *appR(unsigned tag, ast_t *lst, ast_t *nd);

void evaluate(ast_t *root);

#endif
