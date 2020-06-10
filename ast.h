
#ifndef __AST_H__
#define __AST_H__

typedef struct ast_s {
	unsigned tag;
	unsigned lineN;
	union {
		struct {
			ast_s *left, *right;
		} child;
		char *str;
		double real;
	} u;
} ast_t;

#endif