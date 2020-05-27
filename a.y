/**
 * Proyecto final GLF
 * Manuel de Castro Caballero, María Ruiz Molina, Andrés Trigueros Vega
 * Curso 2019-2020
 */

%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.c"

/* Salidas del programa. */
#define PARSE_SUCCESS 0
#define SYNTAX_ERROR -2
#define FILE_ERROR -3

extern int lineNum;

/* Árbol de sintaxis */
static ast_t ast = NULL;
%}

 /* Tipo/estructura de los tokens */
%union {
	struct syn_elem {
		unsigned char type;	/* Tipo de token */
		union {				/* Valor útil del token */
			double real_value;	/* Como número real */
			long int_value;		/* Como número entero */
			char *string;		/* Como cadena de caracteres */
			struct ast_s *node;
		} u;
	} s;
}

// TODO: Operadores lógicos.

%left '+' '-'
%left '*' '/'

%right '^'

%%

 /* Producciones de un programa */
PROGRAM
	/* Un único elemento de programa */
	: PROGRAM_ELEMENT
	{
		ast = newRoot('l', ast, $1.u.node);
	}
	/* Varios elementos de programa */
	| PROGRAM PROGRAM_ELEMENT
	{
		ast = newRoot('l', ast, $2.u.node);
	};

 /* Elementos de un programa */
PROGRAM_ELEMENT
	/* Sentencia */
	: SENTENCE '\n'
	{
		$$ = $1;
	}
	/* Sentencia vacía */
	| '\n'
	{
		$$.type = AST_NODE;
		$$.u.node = NULL;
	};

 /* Sentencias */
SENTENCIA
	/* Asignación */
	: ID '=' EXPR
	{
		$$.type = AST_NODE;
		$$.u.node = newNode(ASSIG, newLeafString(ID, $1.u.string), $3.u.node);
	}
	/* Impresión de un valor */
	| PRINT EXPR
	{
		$$.type = AST_NODE;
		$$.u.node = newNode(PRINT, NULL, $2.u.node);
	}
	/* Impresión de una string */
	| PRINT STR
	{
		$$.type = AST_NODE;
		$$.u.node = newNode(PRINT, newLeafString(STR, $2.u.string), NULL);
	}
	/* Impresión de una string y un valor */
	| PRINT STR EXPR
	{
		$$.type = AST_NODE;
		$$.u.node = newNode(PRINT, newLeafString(STR, $2.u.string), $3.u.node);
	}
	/* Lectura de un valor */
	| READ ID
	{
		$$.type = AST_NODE;
		$$.u.node = newNode(READ, NULL, newLeafString(ID, $2.u.string));
	}
	/* Lectura de un valor con mensaje de aviso */
	| READ STR ID
	{
		$$.type = AST_NODE;
		$$.u.node = newNode(READ, newLeafString(STR, $2.u.string), newLeafString(ID, $3.u.string))
	};
	/* TODO: IF ELSE WHILE FOR... */

EXPR
	:

%%

char programName[256] = "";

/* Gestión de errores */
int yyerror(char *str)
{
	printf("%s(%d): error %s\n", programName, lineNum, str);
	return 1;
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Uso: ./ac <fichero>");
		return SYNTAX_ERROR;
	}

	strcpy(programName, argv[1]);

	yyin = fopen(programName, "rb");
	if (yyin == NULL)
	{
		fprintf(stderr, "Error intentando abrir el fichero %s\n", programName);
		return FILE_ERROR;
	}

	if (yyparse() != PARSE_SUCCESS)
	{
		fclose(yyin);
		fprintf(stderr, "Compilación abortada.\n");
	}
	
	fclose(yyin);

	if (ast != NULL)
	{
		process(ast);
	}
	else
	{
		fprintf(stderr, "No hay nada que ejecutar.\n");
	}

	return 0;
}
