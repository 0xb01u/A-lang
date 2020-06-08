/*
 * @Author: Manuel de Castro Caballero, María Ruiz Molina, Andrés Trigueros Vega
 * @Year: 2020
 */

#include "ast.h"
#include "symtab.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

extern int lineNum;
extern char programName[];

ast_t *newLeafString(unsigned tag, char *str)
{
	mallocCheck(ast_t *res, sizeof(ast_t));
	lnum(res) = (unsigned)lineNum;
	tag(res) = tag;
	sv(res) = str;
	return res;
}


ast_t *newLeafNum(unsigned tag, double dval)
{
	mallocCheck(ast_t *res, sizeof(ast_t));
	lnum(res) = (unsigned)lineNum;
	tag(res) = tag;
	dv(res) = dval;
	return res;
}

ast_t *newNode(unsigned tag, ast_t *l, ast_t *r)
{
	mallocCheck(ast_t *res, sizeof(ast_t));
	lnum(res) = (unsigned)lineNum;
	tag(res) = tag;
	left(res) = l;
	right(res) = r;
	return res;
}

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
	switch (tag(root)) {		
		case '?':			
			if (expr(left(root)) != 0.0)
			{				
				return expr(left(right(root)));
			}
			else
			{				
				return expr(right(right(root)));
			}		

		case EQ:
			if (expr(left(root)) == expr(right(root))) {
				return 1.0;
			}
			else
			{
				return 0.0;
			}

		case NE:
			if (expr(left(root)) != expr(right(root)))
			{
				return 1.0;
			}
			else
			{
				return 0.0;
			}

		case LT:
			if (expr(left(root)) < expr(right(root)))
			{
				return 1.0;
			}
			else
			{
				return 0.0;
			}

		case GT:
			if (expr(left(root)) > expr(right(root)))
			{
				return 1.0;
			}
			else
			{
				return 0.0;
			}

		case '+':
			return expr(left(root)) + expr(right(root));

		case '-':			
			if (left(root) == NULL)
			{
				return - expr(right(root));

			}
			else
			{
				return expr(left(root)) - expr(right(root));
			}

		case '*':
			return expr(left(root)) * expr(right(root));

		case '/':
			return expr(left(root)) / expr(right(root));

		case '%':
			return fmod(expr(left(root)), expr(right(root)));

		case DIV:
			return (int)(expr(left(root)) / expr(right(root))):

		case '^':
			return pow(expr(left(root)), expr(right(root)));

		
		case FLOAT:
			return dv(root);

		case ID:
			return read(sv(root));

		case SIN:
			return sin(expr(left(root)));

		case COS:
			return cos(expr(left(root)));

		case TAN:
			return tan(expr(left(root)));

		case LN:
			return log(expr(left(root)));
	   

		default:
			fprintf(stderr, "%s(%d): error -- Etiqueta desconocida en expresión AST %u\n", programName, lnum(root), tag(root));
			break;
	}
}



static void proc(ast_t *root)
{
	switch (tag(root))
	{
		case '=':
			insertModify(sv(left(root)), expr(right(root)));
			break;

		case PRINT:			
			if (left(root) == NULL)
			{
				printf("%g\n", expr(right(root)));
			
			}
			else if (right(root) == NULL)
			{
				puts(sv(left(root)));
			}
			else
			{
				printf("%s%g\n", sv(left(root)), expr(right(root)));
			}
			break;

		case READ:
			if (left(root) == NULL)
			{
				double rval;
		  	 	scanf("%lf", &rval);
		  	  
				insertModify(sv(right(root)), rval);
			}
			else
			{
				double rval;
				printf("%s", sv(left(root)));
				scanf("%lf", &rval);

				insertModify(sv(right(root)), rval);
			}
		break;
 	
	case WHILE:
		if (right(root) == NULL)
		{
			while (expr(left(root)));
		}
		else
		{
			while (expr(left(root)))
			{
				evaluate(right(root));	
			}	
		}
		break;

	case IF:
		
		if(expr(left(root)))
		{
			evaluate(right(root));
		}
	
		break;
	case ELSE:

		if(!(expr(left(root))))
		{
			evaluate(right(root));
		}
	
		break;

		default:

			fprintf(stderr, "%s(%d): error - Etiqueta desconocida en expresión AST %u\n", programName, lnum(root), tag(root));
			break;
	}
}


void evaluate(ast_t *root)
{
	while (root != NULL)
	{
		proc(left(root));
		root = right(root);
	}
}
