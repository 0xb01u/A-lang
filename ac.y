/**
 * Compilador de A - Proyecto final GLF
 * Manuel de Castro Caballero, María Ruiz Molina, Andrés Trigueros Vega
 * Curso 2019-2020
 */

%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "autils.h"
#include "ast.h"
#include "ac.syn.h"

int yylex();
int yyerror(char *error);
extern int yylineno;
extern FILE *yyin;

/* Árbol de sintaxis */
static ast_t *ast = NULL;

%}

 /* Tipo/estructura de los tokens */
%union {
	struct syn_elem {
		unsigned char type;		/* Tipo de token */
		union {					/* Valor útil del token */
			double real_value;	/* Como número real */
			long int_value;		/* Como número entero */
			char *string;		/* Como cadena de caracteres */
			struct ast_s *node;
		} u;
	} s;
}

%nonassoc EQ NE LT GT

%left '+' '-'
%left '*' '/' '%' DIV

%right '^'
%nonassoc UNARY

%token <s> ID
%term PRINT READ SIN COS TAN LN
%token <s> FLOAT
%token <s> STR
%term WHILE IF ELSE DEL_BL_A DEL_BL_C

%type <s> PROGRAM PROGRAM_ELEMENT BLOCK SENTENCE_GROUP SENTENCE EXPR

%%

 /* Producciones de un programa */
PROGRAM
	/* Un único bloque de programa */
	: PROGRAM_ELEMENT
	{
		ast = newRoot('r', ast, $1.u.node);
	}
	/* Varios bloques de programa */
	| PROGRAM PROGRAM_ELEMENT
	{
		ast = newRoot('r', ast, $2.u.node);
	}
	;

 /* Elemento del programa */
PROGRAM_ELEMENT
	/* Una sentencia */
	: SENTENCE '\n'
	{
		$$ = $1;
	}
	/* Sentencia vacía */
	| '\n'
	{
		$$.type = AST_NODE_id;
		$$.u.node = NULL;
	}
	;

 /* Bloque de sentencias */
BLOCK
	: DEL_BL_A SENTENCE_GROUP DEL_BL_C
	{
		$$ = $2;
	}
	;

 /* Grupo de sentencias */
SENTENCE_GROUP
	: PROGRAM_ELEMENT
	{
		$$ = $1;
	}
	| SENTENCE_GROUP PROGRAM_ELEMENT
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode('g', NULL, newRoot('r', $1.u.node, $2.u.node));
	}
	;

 /* Sentencias */
SENTENCE
	: BLOCK
	{
		$$ = $1;
	}
	/* Asignación */
	| ID '=' EXPR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode('=', newLeafString(ID, $1.u.string), $3.u.node);
	}
	/* Impresión de un valor */
	| PRINT EXPR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode(PRINT, NULL, $2.u.node);
	}
	/* Impresión de una string */
	| PRINT STR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode(PRINT, newLeafString(STR, $2.u.string), NULL);
	}
	/* Impresión de una string y un valor */
	| PRINT STR EXPR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode(PRINT, newLeafString(STR, $2.u.string), $3.u.node);
	}
	/* Lectura de un valor */
	| READ ID
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode(READ, NULL, newLeafString(ID, $2.u.string));
	}
	/* Lectura de un valor con mensaje de aviso */
	| READ STR ID
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode(READ, newLeafString(STR, $2.u.string), newLeafString(ID, $3.u.string));
	}
	| WHILE '(' EXPR ')' SENTENCE
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode(WHILE, $3.u.node, $5.u.node);
	}
	/* TODO: FOR? */
	| IF '(' EXPR ')' SENTENCE ELSE SENTENCE
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode(IF, $3.u.node, $5.u.node);
		$$.u.node = newNode(ELSE, $3.u.node, $7.u.node);
	}
	| IF '(' EXPR ')' SENTENCE
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode(IF, $3.u.node, $5.u.node);
	};

EXPR
	: EXPR EQ EXPR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode(EQ, $1.u.node, $3.u.node);
	}
	| EXPR NE EXPR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode(NE, $1.u.node, $3.u.node);
	}
	| EXPR LT EXPR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode(LT, $1.u.node, $3.u.node);
	}
	| EXPR GT EXPR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode(GT, $1.u.node, $3.u.node);
	}
	| EXPR '+' EXPR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode('+', $1.u.node, $3.u.node);
	}
	| EXPR '-' EXPR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode('-', $1.u.node, $3.u.node);
	}
	| EXPR '*' EXPR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode('*', $1.u.node, $3.u.node);
	}
	| EXPR '/' EXPR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode('/', $1.u.node, $3.u.node);
	}
	| EXPR '%' EXPR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode('%', $1.u.node, $3.u.node);
	}
	| EXPR '^' EXPR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode('^', $1.u.node, $3.u.node);
	}
	| EXPR DIV EXPR
	{
	  	$$.type = AST_NODE_id;
	  	$$.u.node = newNode(DIV, $1.u.node, $3.u.node);
   	}
	| '+' EXPR %prec UNARY
	{
		$$ = $2;
	}
	| '-' EXPR %prec UNARY
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode('-', NULL, $2.u.node);
	}
	| '(' EXPR ')'
	{
		$$ = $2;
	}
	| FLOAT
	{
	  	$$.type = AST_NODE_id;
	  	$$.u.node = newLeafNum(FLOAT, $1.u.real_value);
	}
	| ID
	{
	  	$$.type = AST_NODE_id;
	  	$$.u.node = newLeafString(ID, $1.u.string);
	}
	| SIN '(' EXPR ')'
	{
	  	$$.type = AST_NODE_id;
	  	$$.u.node = newNode(SIN, $3.u.node, NULL);
	}
	| COS '(' EXPR ')'
	{
	  	$$.type = AST_NODE_id;
	  	$$.u.node = newNode(COS, $3.u.node, NULL);
	}
	| TAN '(' EXPR ')'
	{
	  	$$.type = AST_NODE_id;
	  	$$.u.node = newNode(TAN, $3.u.node, NULL);
	}
	| LN '(' EXPR ')'
	{
	  	$$.type = AST_NODE_id;
	  	$$.u.node = newNode(LN, $3.u.node, NULL);
	}
	;

%%

char programName[256] = "";

/* Gestión de errores */
int yyerror(char *error)
{
	fprintf(stderr, "%s(%d): error -- %s\n", programName, yylineno, error);
	return 1;
}

int main(int argc, char *argv[])
{
	if (argc < 2 || argc > 3 || (argc == 3 && strcmp("-t", argv[2]) != 0))
	{
		printf("Uso: ./ac <fichero> [-t]");
		exit(FILE_ERROR);
	}

	strcpy(programName, argv[1]);

	yyin = fopen(programName, "rb");
	if (yyin == NULL)
	{
		fprintf(stderr, "Error intentando abrir el fichero %s\n", programName);
		exit(FILE_ERROR);
	}

	if (yyparse() != PARSE_SUCCESS)
	{
		fclose(yyin);
		fprintf(stderr, "Compilacion abortada.\n");
		exit(SYNTAX_ERROR);
	}

	fclose(yyin);

	if (ast != NULL)
	{
		if (argc == 3 && strcmp("-t", argv[2]) == 0)
		{
			char *treeFilename;
			mallocCheck(treeFilename, sizeof(char) * strlen(programName) + 6);
			strcpy(treeFilename, programName);
			treeFilename[strlen(programName)] = '.';
			treeFilename[strlen(programName) + 1] = 't';
			treeFilename[strlen(programName) + 2] = 'r';
			treeFilename[strlen(programName) + 3] = 'e';
			treeFilename[strlen(programName) + 4] = 'e';
			treeFilename[strlen(programName) + 5] = '\0';
			FILE *treeFile = fopen(treeFilename, "w");
			if (treeFile == NULL)
			{
				fprintf(stderr, "Error al exportar el AST.\n");
			}
			else
			{
				print_tree(treeFile, ast, 0);
			}
			fclose(treeFile);
		}
		process(ast);
	}
	else
	{
		fprintf(stderr, "No hay nada que ejecutar.\n");
	}

	return 0;
}
