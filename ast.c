/**
 * Compilador de A - Proyecto final GLF
 * Manuel de Castro Caballero, María Ruiz Molina, Andrés Trigueros Vega
 * Curso 2019-2020
 */

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ast.h"
#include "autils.h"
#include "ac.syn.h"
#include "symtab.h"

extern int yylineno;
extern char programName[];

ast_t *newLeafString(unsigned tag, char *str)
{
	ast_t *res;
	mallocCheck(res, sizeof(ast_t));
	res->lineNum = (unsigned)yylineno;
	res->tag = tag;
	res->u.str = str;
	return res;
}


ast_t *newLeafNum(unsigned tag, double dval)
{
	ast_t *res;
	mallocCheck(res, sizeof(ast_t));
	res->lineNum = (unsigned)yylineno;
	res->tag = tag;
	res->u.real = dval;
	return res;
}

ast_t *newNode(unsigned tag, ast_t *l, ast_t *r)
{
	ast_t *res;
	mallocCheck(res, sizeof(ast_t));
	res->lineNum = (unsigned)yylineno;
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
	tmp->u.child.right = newNode(tag, nd, NULL);

	return lst;
}

static symbol evaluateExpr(ast_t *node)
{
	symbol value;	/* Valor de la expresión */
	switch (node->tag)
	{
		case EQ:
			if ((evaluateExpr(node->u.child.left)).value.real == (evaluateExpr(node->u.child.right)).value.real)
			{
				value.type = REAL_id;
				value.value.real = 1.0;
				return value;
			}
			else
			{
				value.type = REAL_id;
				value.value.real = 0.0;
				return value;
			}

		case NE:
			if ((evaluateExpr(node->u.child.left)).value.real != (evaluateExpr(node->u.child.right)).value.real)
			{

				value.type = REAL_id;
				value.value.real = 1.0;
				return value;
			}
			else
			{
				value.type = REAL_id;
				value.value.real = 0.0;
				return value;
			}

		case LT:
			if ((evaluateExpr(node->u.child.left)).value.real < (evaluateExpr(node->u.child.right)).value.real)
			{
				value.type = REAL_id;
				value.value.real = 1.0;
				return value;
			}
			else
			{
				value.type = REAL_id;
				value.value.real = 0.0;
				return value;
			}

		case GT:
			if ((evaluateExpr(node->u.child.left)).value.real > (evaluateExpr(node->u.child.right)).value.real)
			{
				value.type = REAL_id;
				value.value.real = 1.0;
				return value;
			}
			else
			{
				value.type = REAL_id;
				value.value.real = 0.0;
				return value;
			}

		case '+':
			value.type = REAL_id;
			value.value.real = (evaluateExpr(node->u.child.left)).value.real + (evaluateExpr(node->u.child.right)).value.real;
			return value;

		case '-':			
			if (node->u.child.left == NULL)
			{
				value.type = REAL_id;
				value.value.real = - (evaluateExpr(node->u.child.right)).value.real;
				return value;
			}
			else
			{
				value.type = REAL_id;
				value.value.real = (evaluateExpr(node->u.child.left)).value.real - (evaluateExpr(node->u.child.right)).value.real;
				return value;
			}

		case '*':
			value.type = REAL_id;
			value.value.real = (evaluateExpr(node->u.child.left)).value.real * (evaluateExpr(node->u.child.right)).value.real;
			return value;	

		case '/':
			value.type = REAL_id;
			value.value.real = (evaluateExpr(node->u.child.left)).value.real / (evaluateExpr(node->u.child.right)).value.real;
			return value;

		case '%':
			value.type = REAL_id;
			value.value.real = fmod((evaluateExpr(node->u.child.left)).value.real, (evaluateExpr(node->u.child.right)).value.real);
			return value;

		case DIV:
			value.type = REAL_id;
			value.value.real = (int)((evaluateExpr(node->u.child.left)).value.real / (evaluateExpr(node->u.child.right)).value.real);
			return value;

		case '^':
			value.type = REAL_id;
			value.value.real = pow((evaluateExpr(node->u.child.left)).value.real, (evaluateExpr(node->u.child.right)).value.real);
			return value;

		case FLOAT:
			value.type = REAL_id;
			value.value.real = node->u.real;
			return value;

		case ID:
			return get(node->u.str);

		case SIN:
			value.type = REAL_id;
			value.value.real = sin(evaluateExpr(node->u.child.left).value.real);
			return value;

		case COS:
			value.type = REAL_id;
			value.value.real = cos(evaluateExpr(node->u.child.left).value.real);
			return value;

		case TAN:
			value.type = REAL_id;
			value.value.real = tan(evaluateExpr(node->u.child.left).value.real);
			return value;

		case LN:
			value.type = REAL_id;
			value.value.real = log(evaluateExpr(node->u.child.left).value.real);
			return value;	   

		default:
			fprintf(stderr, "%s(%d): error -- Etiqueta desconocida en expresion del AST %u\n", programName, node->lineNum, node->tag);
		break;
	}
}



static void evaluateNode(ast_t *node)
{
	switch (node->tag)
	{
		case '=':
			edit(node->u.child.left->u.str, evaluateExpr(node->u.child.right));
			break;

		case PRINT:			
			if (node->u.child.left == NULL)
			{
				printf("%g\n", (evaluateExpr(node->u.child.right)).value.real);
			}
			else if (node->u.child.right == NULL)
			{
				printf("%s\n", node->u.child.left->u.str);
			}
			else
			{
				printf("%s%g\n", node->u.child.left->u.str, (evaluateExpr(node->u.child.right)).value.real);
			}
			break;

		case READ:
			if (node->u.child.left == NULL)
			{
				symbol number;
				number.type = REAL_id;
		  	 	if (scanf("%lf", &number.value.real) != 1)
		  	 	{
		  	 		printf("Advertencia: input erroneo. El programa intentara continuar.\n");
		  	 	}

				edit(node->u.child.right->u.str, number);
			}
			else
			{
				symbol number;
				number.type = REAL_id;
				printf("%s", node->u.child.left->u.str);
				if (scanf("%lf", &number.value.real) != 1)
		  	 	{
		  	 		printf("Advertencia: input erroneo. El programa intentara continuar.\n");
		  	 	}

				edit(node->u.child.right->u.str, number);
			}
		break;
 	
	case WHILE:
		if (node->u.child.right == NULL)
		{
			while ((evaluateExpr(node->u.child.left)).value.real);
		}
		else
		{
			while ((evaluateExpr(node->u.child.left)).value.real)
			{
				evaluateNode(node->u.child.right);	
			}	
		}
		break;

	case IF:
		if((evaluateExpr(node->u.child.left)).value.real)
		{
			evaluateNode(node->u.child.right);
		}	
		break;

	case ELSE:
		if(!((evaluateExpr(node->u.child.left)).value.real))
		{
			evaluateNode(node->u.child.right);
		}	
		break;

	case 'g':
		process(node->u.child.right);
		break;

	default:
		fprintf(stderr, "%s(%d): error -- Etiqueta desconocida en nodo del AST %u\n", programName, node->lineNum, node->tag);
	break;
	}
}


void process(ast_t *root)
{
	while (root != NULL)
	{
		if (root->u.child.left != NULL)
			evaluateNode(root->u.child.left);
		root = root->u.child.right;
	}
}

static char *translate(ast_t *node)
{
	char *leaf;
	mallocCheck(leaf, 256);
	switch (node->tag)
	{
		case EQ: return "EQ";
		case NE: return "NE";
		case LT: return "LT";
		case GT: return "GT";
		case '+': return "+";
		case '-': return "-";
		case '*': return "*";
		case '/': return "/";
		case '%': return "%";
		case '^': return "^";
		case '=': return "=";
		case DIV: return "DIV";
		case PRINT: return "PRINT";
		case READ: return "READ";
		case SIN: return "SIN";
		case COS: return "COS";
		case TAN: return "TAN";
		case LN: return "LN";
		case FLOAT: ;
			char *number;
			mallocCheck(number, sizeof(char) * 5)
			return strcat(strcpy(leaf, "NUM: "), gcvt(node->u.real, 2, number));
		case STR: return strcat(strcpy(leaf, "STR: "), node->u.str);
		case WHILE: return "WHILE";
		case IF: return "IF";
		case ELSE: return "ELSE";
		case ID: return strcat(strcpy(leaf, "VAR: "), node->u.str);
		case 'r': return "ST";
		case 'g': return "BLCK";
	}
}

void print_tree(FILE *f, ast_t *node, int space)
{
	if (node->u.child.left != NULL && node->tag != STR && node->tag != FLOAT && node->tag != ID)
		print_tree(f, node->u.child.left, space + 8);

	fprintf(f, "\n");
	for (int i = 0; i < space; i++)
		fprintf(f, " ");
	fprintf(f, "%s\n", translate(node));

	if (node->u.child.right != NULL && node->tag != STR && node->tag != FLOAT && node->tag != ID)
		print_tree(f, node->u.child.right, space + 8);
}
