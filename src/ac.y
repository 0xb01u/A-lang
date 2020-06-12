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

%nonassoc EQ NE LAND LOR LT GT

%left BAND BOR SL SR

%left '+' '-'
%left '*' '/' '%' DIV

%right '^'
%nonassoc UNARY BNOT

%token <s> ID
%term PRINT READ SIN COS TAN LN
%token <s> INT
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
	| '\n' DEL_BL_A SENTENCE_GROUP DEL_BL_C
	{
		$$ = $3;
	}
	;

 /* Grupo de sentencias */
SENTENCE_GROUP
	/* Un único elemento de un programa */
	: PROGRAM_ELEMENT
	{
		$$ = $1;
	}
	/* Un grupo de sentencias y un elemento del programa (recursivo) */
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
	/* Bucle while */
	| WHILE '(' EXPR ')' SENTENCE
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode(WHILE, $3.u.node, $5.u.node);
	}
	/* Condicional if-else */
	| IF '(' EXPR ')' SENTENCE ELSE SENTENCE
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode('g', NULL, newRoot('r', newRoot('r', NULL, newNode(IF, $3.u.node, $5.u.node)), newNode(ELSE, $3.u.node, $7.u.node)));
	}
	/* Condicional if */
	| IF '(' EXPR ')' SENTENCE
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode(IF, $3.u.node, $5.u.node);
	};

 /* Expresiones */
EXPR
	/* Igualdad lógica */
	: EXPR EQ EXPR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode(EQ, $1.u.node, $3.u.node);
	}
	/* Desigualdad lógica */
	| EXPR NE EXPR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode(NE, $1.u.node, $3.u.node);
	}
	/* And lógico */
	| EXPR LAND EXPR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode(LAND, $1.u.node, $3.u.node);
	}
	/* Or lógico */
	| EXPR LOR EXPR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode(LOR, $1.u.node, $3.u.node);
	}
	/* Menor que */
	| EXPR LT EXPR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode(LT, $1.u.node, $3.u.node);
	}
	/* Mayor que */
	| EXPR GT EXPR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode(GT, $1.u.node, $3.u.node);
	}
	/* Suma aritmética */
	| EXPR '+' EXPR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode('+', $1.u.node, $3.u.node);
	}
	/* Resta aritmética */
	| EXPR '-' EXPR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode('-', $1.u.node, $3.u.node);
	}
	/* Multiplicación aritmética */
	| EXPR '*' EXPR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode('*', $1.u.node, $3.u.node);
	}
	/* División aritmética */
	| EXPR '/' EXPR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode('/', $1.u.node, $3.u.node);
	}
	/* Módulo */
	| EXPR '%' EXPR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode('%', $1.u.node, $3.u.node);
	}
	/* Potencia */
	| EXPR '^' EXPR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode('^', $1.u.node, $3.u.node);
	}
	/* Operación cociente */
	| EXPR DIV EXPR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode(DIV, $1.u.node, $3.u.node);
	}
	/* Mantenimiento de signo */
	| '+' EXPR %prec UNARY
	{
		$$ = $2;
	}
	/* Cambio de signo */
	| '-' EXPR %prec UNARY
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode('-', NULL, $2.u.node);
	}
	| '(' EXPR ')'
	{
		$$ = $2;
	}
	/* Bit-and */
	| EXPR BAND EXPR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode(BAND, $1.u.node, $3.u.node);
	}
	/* Bit-or */
	| EXPR BOR EXPR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode(BOR, $1.u.node, $3.u.node);
	}
	/* Bit-not */
	| BNOT EXPR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode(BNOT, NULL, $2.u.node);
	}
	/* Shift left */
	| EXPR SL EXPR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode(SL, $1.u.node, $3.u.node);		
	}
	/* Shift right */
	| EXPR SR EXPR
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode(SR, $1.u.node, $3.u.node);		
	}
	/* Número real */
	| FLOAT
	{
		$$.type = AST_NODE_id;
		$$.u.node = newLeafNum(FLOAT, $1.u.real_value);
	}
	/* Número entero */
	| INT
	{
		$$.type = AST_NODE_id;
		$$.u.node = newLeafInt(INT, $1.u.int_value);
	}
	/* Variable */
	| ID
	{
		$$.type = AST_NODE_id;
		$$.u.node = newLeafString(ID, $1.u.string);
	}
	/* Seno */
	| SIN '(' EXPR ')'
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode(SIN, $3.u.node, NULL);
	}
	/* Coseno */
	| COS '(' EXPR ')'
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode(COS, $3.u.node, NULL);
	}
	/* Tangente */
	| TAN '(' EXPR ')'
	{
		$$.type = AST_NODE_id;
		$$.u.node = newNode(TAN, $3.u.node, NULL);
	}
	/* Logaritmo neperiano */
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
	/* Comprobación de corrección de argumentos */
	if (argc < 2 || argc > 3 || (argc == 3 && strcmp("-t", argv[2]) != 0))
	{
		printf("Uso: ./ac <fichero> [-t]\n");
		exit(FILE_ERROR);
	}

	/* Nombre del fichero */
	strcpy(programName, argv[1]);

	/* Lectura del fichero */
	yyin = fopen(programName, "rb");
	if (yyin == NULL)
	{
		fprintf(stderr, "Error intentando abrir el fichero %s\n", programName);
		exit(FILE_ERROR);
	}

	/* Parseo */
	if (yyparse() != PARSE_SUCCESS)
	{
		fclose(yyin);
		fprintf(stderr, "Compilacion abortada.\n");
		exit(SYNTAX_ERROR);
	}

	fclose(yyin);

	/* Recorrido del árbol */
	if (ast != NULL)
	{
		/* Impresión, si se ha utilizado la opción -t */
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
		/* Evaluación del árbol */
		process(ast);
	}
	else
	{
		fprintf(stderr, "No hay nada que ejecutar.\n");
	}

	return 0;
}
