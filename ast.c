/**
 * Compilador de A - Proyecto final GLF
 * Manuel de Castro Caballero, María Ruiz Molina, Andrés Trigueros Vega
 * Curso 2019-2020
 */

#include "ast.h"
#include "autils.h"
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

static symbol evaluateExpr(ast_t *root)
{	
	switch (root->tag)
	{
		case EQ:
			if ((evaluateExpr(root->u.child.left)).value.real == (evaluateExpr(root->u.child.right)).value.real)
			{
				symbol value;
				value.type = REAL_id;
				value.value.real = 1.0;
				return value;
			}
			else
			{
				symbol value;
				value.type = REAL_id;
				value.value.real = 0.0;
				return value;
			}

		case NE:
			if ((evaluateExpr(root->u.child.left)).value != (evaluateExpr(root->u.child.right)).value)
			{

				symbol value;
				value.type = REAL_id;
				value.value.real = 1.0;
				return value;
			}
			else
			{
				symbol value;
				value.type = REAL_id;
				value.value.real = 0.0;
				return value;
			}

		case LT:
			if ((evaluateExpr(root->u.child.left)).real.value < (evaluateExpr(root->u.child.right)).real.value)
			{
				symbol value;
				value.type = REAL_id;
				value.value.real = 1.0;
				return value;
			}
			else
			{
				symbol value;
				value.type = REAL_id;
				value.value.real = 0.0;
				return value;
			}

		case GT:
			if ((evaluateExpr(root->u.child.left)).real.value > (evaluateExpr(root->u.child.right)).real.value)
			{
				symbol value;
				value.type = REAL_id;
				value.value.real = 1.0;
				return value;
			}
			else
			{
				symbol value;
				value.type = REAL_id;
				value.value.real = 0.0;
				return value;
			}

		case '+':
			symbol value;
			value.type = REAL_id;
			value.value.real = (evaluateExpr(root->u.child.left)).value.real + (evaluateExpr(root->u.child.right)).value.real;
			return value;

		case '-':			
			if (root->u.child.left == NULL)
			{
				symbol value;
				value.type = REAL_id;
				value.value.real = - (evaluateExpr(root->u.child.right)).value.real;
				return value;
			}
			else
			{
				symbol value;
				value.type = REAL_id;
				value.value.real = (evaluateExpr(root->u.child.left)).value.real - (evaluateExpr(root->u.child.right)).value.real;
				return value;
			}

		case '*':
			symbol value;
			value.type = REAL_id;
			value.value.real = (evaluateExpr(root->u.child.left)).value.real * (evaluateExpr(root->u.child.right)).value.real;
			return value;	

		case '/':
			symbol value;
			value.type = REAL_id;
			value.value.real = (evaluateExpr(root->u.child.left)).value.real / (evaluateExpr(root->u.child.right)).value.real;
			return value;

		case '%':
			symbol value;
			value.type = REAL_id;
			value.value.real = fmod((evaluateExpr(root->u.child.left)).value.real, (evaluateExpr(root->u.child.right)).value.real);
			return value;

		case DIV:
			symbol value;
			value.type = REAL_id;
			value.value.real = (int)((evaluateExpr(root->u.child.left)).value.real / (evaluateExpr(root->u.child.right)).value.real);
			return value;

		case '^':
			symbol value;
			value.type = REAL_id;
			value.value.real = pow((evaluateExpr(root->u.child.left)).value.real, (evaluateExpr(root->u.child.right)).value.real);
			return value;

		case FLOAT:
			symbol value;
			value.type = REAL_id;
			value.value.real = root->u.real;
			return value;

		case ID:
			return get(root->u.str);

		case SIN:
			symbol value;
			value.type = REAL_id;
			value.value.real = sin(evaluateExpr(root->u.child.left).value.real);
			return value;

		case COS:
			symbol value;
			value.type = REAL_id;
			value.value.real = cos(evaluateExpr(root->u.child.left).value.real);
			return value;

		case TAN:
			symbol value;
			value.type = REAL_id;
			value.value.real = tan(evaluateExpr(root->u.child.left).value.real);
			return value;

		case LN:
			symbol value;
			value.type = REAL_id;
			value.value.real = log(evaluateExpr(root->u.child.left).value.real);
			return value;	   

		default:
			fprintf(stderr, "%s(%d): error -- Etiqueta desconocida en expresión AST %u\n", programName, lnum(root), root)->tag;
		break;
	}
}



static void evaluateNode(ast_t *root)
{
	switch (root->tag)
	{
		case '=':
			edit(root->u.child.left->u.str, (evaluateExpr(root->u.child.right)).value.real);
			break;

		case PRINT:			
			if (root->u.child.left == NULL)
			{
				printf("%g\n", (evaluateExpr(root->u.child.right)).value.real);
			
			}
			else if (root->u.child.right == NULL)
			{
				puts(root->u.child.left->u.str);
			}
			else
			{
				printf("%s%g\n", root->u.child.left->u.str, (evaluateExpr(root->u.child.right)).value.real);
			}
			break;

		case READ:
			if (root->u.child.left == NULL)
			{
				symbol number;
				number.type = REAL_id;
		  	 	scanf("%lf", &number.value.real);

				edit(root->u.child.right->u.str, number);
			}
			else
			{
				symbol number;
				number.type = REAL_id;
				printf("%s", root->u.child.left->u.str);
				scanf("%lf", &number.value.real);

				edit(root->u.child.right->u.str, number);
			}
		break;
 	
	case WHILE:
		if (root->u.child.right == NULL)
		{
			while ((evaluateExpr(root->u.child.left)).value.real);
		}
		else
		{
			while ((evaluateExpr(root->u.child.left)).value.real)
			{
				evaluateNode(root->u.child.right);	
			}	
		}
		break;

	case IF:
		if((evaluateExpr(root->u.child.left)).value.real)
		{
			evaluateNode(root->u.child.right);
		}	
		break;

	case ELSE:
		if(!((evaluateExpr(root->u.child.left)).value.real))
		{
			evaluateNode(root->u.child.right);
		}	
		break;

	default:
		fprintf(stderr, "%s(%d): error -- Etiqueta desconocida en expresión AST %u\n", programName, lnum(root), root)->tag;
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
