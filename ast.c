/**
 * Compilador de A - Proyecto final GLF
 * Manuel de Castro Caballero, María Ruiz Molina, Andrés Trigueros Vega
 * Curso 2019-2020
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
	res->lineN = (unsigned)lineNum;
	res->tag = tag;
	res->u.str = str;
	return res;
}


ast_t *newLeafNum(unsigned tag, double dval)
{
	mallocCheck(ast_t *res, sizeof(ast_t));
	res->lineN = (unsigned)lineNum;
	res->tag = tag;
	res->u.real = dval;
	return res;
}

ast_t *newNode(unsigned tag, ast_t *l, ast_t *r)
{
	mallocCheck(ast_t *res, sizeof(ast_t));
	res->lineN = (unsigned)lineNum;
	res->tag = tag;
	res->u.child.left = l;
	res->u.child.right = r;
	return res;
}

ast_t *newRoot(unsigned tag, ast_t *lst, ast_t *nd)
{
	if (lst == NULL) {
		if (nd == NULL) {
			return NULL;
		}
		return newNode(tag, nd, NULL);
	}
	if (nd == NULL) {
		return lst;
	}

	ast_t *tmp = lst;
	while (tmp->u.child.right != NULL) {
		tmp = tmp->u.child.right;
	}
	tmp->u.child.right = newNode(tag,nd,NULL);

	return lst;
}

static double evaluateExpr(ast_t *root)
{	
	switch (root->tag)
	{
		case EQ:
			if (evaluateExpr(root->u.child.left) == evaluateExpr(root->u.child.right)) {
				return 1.0;
			}
			else
			{
				return 0.0;
			}

		case NE:
			if (evaluateExpr(root->u.child.left) != evaluateExpr(root->u.child.right))
			{
				return 1.0;
			}
			else
			{
				return 0.0;
			}

		case LT:
			if (evaluateExpr(root->u.child.left) < evaluateExpr(root->u.child.right))
			{
				return 1.0;
			}
			else
			{
				return 0.0;
			}

		case GT:
			if (evaluateExpr(root->u.child.left) > evaluateExpr(root->u.child.right))
			{
				return 1.0;
			}
			else
			{
				return 0.0;
			}

		case '+':
			return evaluateExpr(root->u.child.left) + evaluateExpr(root->u.child.right);

		case '-':			
			if (root->u.child.left == NULL)
			{
				return - evaluateExpr(root->u.child.right);

			}
			else
			{
				return evaluateExpr(root->u.child.left) - evaluateExpr(root->u.child.right);
			}

		case '*':
			return evaluateExpr(root->u.child.left) * evaluateExpr(root->u.child.right);

		case '/':
			return evaluateExpr(root->u.child.left) / evaluateExpr(root->u.child.right);

		case '%':
			return fmod(evaluateExpr(root->u.child.left), evaluateExpr(root->u.child.right));

		case DIV:
			return (int)(evaluateExpr(root->u.child.left) / evaluateExpr(root->u.child.right)):

		case '^':
			return pow(evaluateExpr(root->u.child.left), evaluateExpr(root->u.child.right));

		
		case FLOAT:
			return root->u.real;

		case ID:
			return get(root->u.str);

		case SIN:
			return sin(evaluateExpr(root->u.child.left));

		case COS:
			return cos(evaluateExpr(root->u.child.left));

		case TAN:
			return tan(evaluateExpr(root->u.child.left));

		case LN:
			return log(evaluateExpr(root->u.child.left));
	   

		default:
			fprintf(stderr, "%s(%d): error -- Etiqueta desconocida en evaluateExpresión AST %u\n", programName, lnum(root), root)->tag;
		break;
	}
}



static void evaluateNode(ast_t *root)
{
	switch (root)->tag
	{
		case '=':
			edit(root->u.child.left->u.str, evaluateExpr(root->u.child.right));
			break;

		case PRINT:			
			if (root->u.child.left == NULL)
			{
				printf("%g\n", evaluateExpr(root->u.child.right));
			
			}
			else if (root->u.child.right == NULL)
			{
				puts(root->u.child.left->u.str);
			}
			else
			{
				printf("%s%g\n", root->u.child.left->u.str, evaluateExpr(root->u.child.right));
			}
			break;

		case READ:
			if (root->u.child.left == NULL)
			{
				double rval;
		  	 	scanf("%lf", &rval);
		  	  
				edit(root->u.child.right->u.str, rval);
			}
			else
			{
				double rval;
				printf("%s", root->u.child.left->u.str);
				scanf("%lf", &rval);

				edit(root->u.child.right->u.str, rval);
			}
		break;
 	
	case WHILE:
		if (root->u.child.right == NULL)
		{
			while (evaluateExpr(root->u.child.left));
		}
		else
		{
			while (evaluateExpr(root->u.child.left))
			{
				evaluateNode(root->u.child.right);	
			}	
		}
		break;

	case IF:
		if(evaluateExpr(root->u.child.left))
		{
			evaluateNode(root->u.child.right);
		}	
		break;

	case ELSE:
		if(!(evaluateExpr(root->u.child.left)))
		{
			evaluateNode(root->u.child.right);
		}	
		break;

	default:
		fprintf(stderr, "%s(%d): error -- Etiqueta desconocida en evaluateExpresión AST %u\n", programName, lnum(root), root)->tag;
	break;
	}
}


void evaluate(ast_t *root)
{
	while (root != NULL)
	{
		evaluateNode(root->u.child.left);
		root = root->u.child.right;
	}
}
