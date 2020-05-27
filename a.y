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

%union {
	struct syn_elem {
		unsigned char type;
		union {
			double real_value;
			int int_value;
			char *string;
			struct ast_s *node;
		} u;
	} s;
}

// Operadores lógicos.

%left '+' '-'
%left '*' '/'

%right '^'

%%



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
