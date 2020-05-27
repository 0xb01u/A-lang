/**
 * @Author: Dušan Kolář
 * @Year:   2003-2018
 * Copyright (c) 2018
 * Licence: GLP 3.0
 */

#include "astree.h"
#include "stduse.h"
#include "inter05.h"
#include "symtab.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

extern int yylineno;

// Crear hoja String
ast_t *mkSlf(unsigned tag, char *str) {
    ast_t *res = xmalloc(sizeof(ast_t));
    lnum(res) = (unsigned)yylineno;
    tag(res) = tag;
    sv(res) = str;
    return res;
}

// Crear hoja double (número)
ast_t *mkDlf(unsigned tag, double dval) {
    ast_t *res = xmalloc(sizeof(ast_t));
    lnum(res) = (unsigned)yylineno;
    tag(res) = tag;
    dv(res) = dval;
    return res;
}

// Crear nodo (padre de l y r)
ast_t *mkNd(unsigned tag, ast_t *l, ast_t *r) {
    ast_t *res = xmalloc(sizeof(ast_t));
    lnum(res) = (unsigned)yylineno;
    tag(res) = tag;
    left(res) = l;
    right(res) = r;
    return res;
}

// Añadir nodo nd como hijo de lst
// Se busca por la derecha hasta que no se encuentra una nada
// Se añade como hijo derecho de la hoja (último nodo en el que se ha buscado)
// un nodo con nd como hijo izquierdo
ast_t *appR(unsigned tag, ast_t *lst, ast_t *nd) {
    if (lst == NULL) {
        if (nd == NULL) {
            return NULL;
        }
        return mkNd(tag,nd,NULL);
    }
    if (nd == NULL) {
        return lst;
    }

    ast_t *tmp = lst;
    while (right(tmp) != NULL) {
        tmp = right(tmp);
    }
    right(tmp) = mkNd(tag,nd,NULL);

    return lst;
}

// ----------------------------------------------------------------------------

static double expr(ast_t *root) {
    // Procesar un nodo terminal
    switch (tag(root)) {
        // Operador ternario (lo más complicado aquí)
        case '?':
            // Interpretar la expresión izquierda
            if (expr(left(root)) != 0.0) {
                // Es la expresión izquierda
                return expr( left(right(root)) );
            } else {
                // Es la expresión derecha
                return expr( right(right(root)) );
            }
        // Los operadores lógicos es símplemente ejecutarlos y ya
        case OR:
            if (expr(left(root)) != 0.0 || expr(right(root)) != 0.0) {
                return 1.0;
            } else {
                return 0.0;
            }
        case AND:
            if (expr(left(root)) != 0.0 && expr(right(root)) != 0.0) {
                return 1.0;
            } else {
                return 0.0;
            }
        case EQ:
            if (expr(left(root)) == expr(right(root))) {
                return 1.0;
            } else {
                return 0.0;
            }
        case NE:
            if (expr(left(root)) != expr(right(root))) {
                return 1.0;
            } else {
                return 0.0;
            }
        case LE:
            if (expr(left(root)) <= expr(right(root))) {
                return 1.0;
            } else {
                return 0.0;
            }
        case GE:
            if (expr(left(root)) >= expr(right(root))) {
                return 1.0;
            } else {
                return 0.0;
            }
        case '<':
            if (expr(left(root)) < expr(right(root))) {
                return 1.0;
            } else {
                return 0.0;
            }
        case '>':
            if (expr(left(root)) > expr(right(root))) {
                return 1.0;
            } else {
                return 0.0;
            }
        // Las operaciones aritméticas, más de lo mismo: ejecutarlas
        case '+':
            return expr(left(root)) + expr(right(root));
        case '-':
            // Como operador unario (cambio de signo)
            if (left(root) == NULL) {
                return - expr(right(root));
            // Como operador binario (resta)
            } else {
                return expr(left(root)) - expr(right(root));
            }
        case '*':
            return expr(left(root)) * expr(right(root));
        case '/':
            return expr(left(root)) / expr(right(root));
        case '^':
            return pow( expr(left(root)), expr(right(root)) );
        case '!':
            if (expr( right(root) ) == 0.0) {
                return 1.0;
            } else {
                return 0.0;
            }
        // Los terminales: devolver su valor correcto
        case FLOAT:
            return dv(root);
        // Variables: leerlas de la tabla de símbolos
        case IDENT:
            return read( sv(root) );
        // Funciones predefinidas: ejecutarlas
        case SIN:
            return sin( expr(left(root)) );
        case COS:
            return cos( expr(left(root)) );
        case TAN:
            return tan( expr(left(root)) );
        // Strange coisas
        default:
            prError((unsigned short)lnum(root),"Unknown tag in expr AST %u\n",tag(root),NULL);
            break;
    }
}

// Procesar el árbol (un nodo de sentencia)
static void proc(ast_t *root) {
    switch (tag(root)) {
        case '=':
            // Insertar en la table de símbolos el identificador (izquierda)
            // y su valor (derecha)
            // sv = valor de String del nodo (nombre de la variable)
            insertModify( sv(left(root)), expr(right(root)) );
            break;
        case PRINT:
            // Si no hay nodo izquierdo (String)
            // imprimir solo la variable (nodo derecho)
            if (left(root) == NULL) {
                printf("%g\n", expr(right(root)) );
            // Si no hay nodo derecho (variable)
            // imprimir solo la String (nodo izquierdo)
            } else if (right(root) == NULL) {
                puts( sv(left(root)) );
            // Si hay nodo derecho e izquierdo
            // imprimir la String seguido de la variable
            } else {
                printf("%s%g\n", sv(left(root)), expr(right(root)) );
            }
            break;
        case READ:
            // No hay que imprimir
            if (left(root) == NULL) {
                double rval;
                scanf("%lf",&rval);
                // Cambiar el valor del símbolo en la tabla
                insertModify(sv(right(root)), rval);
            // Hay que imprimir (nodo izquierdo)
            } else {
                double rval;
                printf("%s", sv(left(root)));
                scanf("%lf", &rval);
                // Cambiar el valor del símbolo en la tabla
                insertModify( sv(right(root)), rval);
            }
            break;
        default:
            // Bro momento
            prError((unsigned short)lnum(root),"Unknown tag in statement AST %u\n",tag(root),NULL);
            break;
    }
}

// Procesar el árbol entero (ejecutar el programa)
void evaluate(ast_t *root) {
    while (root != NULL) {
        // Ejecutar el subárbol izquierdo
        proc(left(root));
        // Pasar al subárbol derecho
        root = right(root);
    }
}


// ----- EOF ------
