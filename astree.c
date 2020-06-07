/*
 * @Author: Manuel de Castro Caballero, María Ruiz Molina, Andrés Trigueros Vega
 * @Year: 2020
 */

#include "astree.h"
#include "stduse.h"
#include "a.h"
#include "symtab.h" //hashtable

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

extern int yylineno;

// Hoja String
ast_t *newLeafString(unsigned tag, char *str) {
    ast_t *res = xmalloc(sizeof(ast_t));
    lnum(res) = (unsigned)yylineno;
    tag(res) = tag;
    sv(res) = str;
    return res;
}


// Hoja double
ast_t *newLeafNum(unsigned tag, double dval) {
    ast_t *res = xmalloc(sizeof(ast_t));
    lnum(res) = (unsigned)yylineno;
    tag(res) = tag;
    dv(res) = dval;
    return res;
}

// Nodo (padre de l y r)
ast_t *newNode(unsigned tag, ast_t *l, ast_t *r) {
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
ast_t *newRoot(unsigned tag, ast_t *lst, ast_t *nd) {
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
        // Operadores logicos
        
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
        // Operaciones aritmeticas
        case '+':
            return expr(left(root)) + expr(right(root));
        case '-':
            // Operador unario, cambio de signo
            if (left(root) == NULL) {
                return - expr(right(root));
            // Operador binario, resta
            } else {
                return expr(left(root)) - expr(right(root));
            }
        case '*':
            return expr(left(root)) * expr(right(root));
        case '/':
            return expr(left(root)) / expr(right(root));
        case '^':
            return pow( expr(left(root)), expr(right(root)) );
        
        // Terminales, devuelve su valor
        case FLOAT:
            return dv(root);
        // Variables, lee de la tabla de simbolos
        case ID:
            return read( sv(root) );
        // Funciones predefinidas
        case SIN:
            return sin( expr(left(root)) );
        case COS:
            return cos( expr(left(root)) );
        case TAN:
            return tan( expr(left(root)) );
       

        default:
            prError((unsigned short)lnum(root),"Etiqueta desconocida en expresion AST %u\n",tag(root),NULL);
            break;
    }
}


// Procesar el árbol (un nodo de sentencia)
static void proc(ast_t *root) {
    switch (tag(root)) {
        case '=':
            // Insertar en la tabla de simbolos el id (izquierda)
            // y su valor (derecha) (value en la tabla hash)
            // sv = valor de String del nodo (nombre de la variable) (key en la tabla hash)
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
              	  // Cambia el valor del simbolo en la tabla
                	insertModify(sv(right(root)), rval);
            	} else {  // Hay que imprimir (nodo izquierdo)
             	  	 double rval;
               		 printf("%s", sv(left(root)));
               		 scanf("%lf", &rval);
               		 // Cambia el valor del simbolo en la tabla
                	 insertModify( sv(right(root)), rval);
            }
		break;
 	//bloque while
	case WHILE:
		if (right(root) == NULL) {
			while (expr(left(root)));
		}else {
			while (expr(left(root))){
				evaluate(right(root));	
			}	
		}
	break;
//IF SIN ACABAR
	case IF:
		if (right(root) == NULL) {
		}
		else{
		if(left(root)){
			right(root);
		}
	}
	break;

        default:
           
            prError((unsigned short)lnum(root),"Etiqueta desconocida en expresion AST %u\n",tag(root),NULL);
            break;
    }
}

// Procesa el arbol entero
void evaluate(ast_t *root) {
    while (root != NULL) {
        // Ejecutar el subarbol izquierdo
        proc(left(root));
        // Pasar al subarbol derecho
        root = right(root);
    }
}






