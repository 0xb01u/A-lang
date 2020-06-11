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
			struct ast_s *left, *right;
		} child;
		char *str;
		double real;
	} u;
} ast_t;

ast_t *newLeafString(unsigned tag, char *str);
ast_t *newLeafNum(unsigned tag, double dval);
ast_t *newNode(unsigned tag, ast_t *l, ast_t *r);
ast_t *newRoot(unsigned tag, ast_t *lst, ast_t *nd);

void process(ast_t *root);

#endif