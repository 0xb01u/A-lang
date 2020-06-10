/**
 * Compilador de A - Proyecto final GLF
 * Manuel de Castro Caballero, María Ruiz Molina, Andrés Trigueros Vega
 * Curso 2019-2020
 */

#ifndef __AST_H__
#define __AST_H__

/* Tipo "nodo del ast" */
typedef struct ast_s {
	unsigned tag;
	unsigned lineNum;
	union {
		struct {
			ast_t *left, *right;
		} child;
		char *str;
		double real;
	} u;
} ast_t;

#endif