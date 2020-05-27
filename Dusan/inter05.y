/**
 * @Author: Dušan Kolář
 * @Year:   2003-2018
 * Copyright (c) 2018
 * Licence: GLP 3.0
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stduse.h"
#include "symtab.h"
#include "astree.h"
#include "token.h"

#define IN_PARSER
#include "inter05.h"


int yylex(void);
extern int yylineno;

FILE *fIn;

int yyerror(char *str);

static ast_t *astRoot = NULL; // Raíz del AST
%}
 /* Los elementos sintácticos leídos pueden son una estructura sStackType: */
%union {
    struct sStackType {
    /* Flag: identificador del tipo */
    unsigned char   flag;
        /* Unión con el valor del tipo correspondiente */
        union {
        double         vFloat;
        char          *vStr;
        struct ast_s  *ast;
        }   u;
    }   s;  /* Identificador del único campo de yylval **/
}

%left OR
%left AND

%nonassoc EQ NE
%nonassoc LE GE '<' '>'

%left '+' '-'
%left '*' '/'

%right '^'

%right '!'

%right UNOP

 /* Todos los tokens son del tipo de s (sStackType) */
%token <s> IDENT
%term LET PRINT SIN COS TAN READ
%token <s> FLOAT
%token <s> STR

%type <s> expr ternary statement  progelem prog

%%

 /* Un programa es */
prog
  : progelem  // Un elemento de un programa
    {
      astRoot = appR(';', astRoot, $1.u.ast); // Se añade el nodo de elemento en el AST
    }
  | prog progelem // O un programa seguido de un elemento de un programa (= varios elementos)
    {
      astRoot = appR(';', astRoot, $2.u.ast);
    }
  ;

 // Un elemento del programa es
progelem
  : statement '\n'  // Una sentencia con un salto de línea
    {
      $$ = $1;
    }
  | '\n'    // O un salto de línea (nada)
    {
      $$.flag = fAST;
      $$.u.ast = NULL;  // Nodo vacío en el AST (no se va a hacer nada con él)
    }
  ;

 // Una sentencia es
statement
  : LET IDENT '=' ternary // Una asignación
    {
      $$.flag = fAST;
      // Se añade el nodo con la asignación con hijos lado izquierdo-lado derecho
      $$.u.ast = mkNd('=', mkSlf(IDENT,$2.u.vStr), $4.u.ast);
    }
  | PRINT ternary // Una impresión de una expresión
    {
      $$.flag = fAST;
      $$.u.ast = mkNd(PRINT, NULL, $2.u.ast); // Nodo de impresión sin hijo izquierdo
    }
  | PRINT STR // Una impresión de una String
    {
      $$.flag = fAST;
      $$.u.ast = mkNd(PRINT, mkSlf(STR,$2.u.vStr), NULL); // Nodo impresión sin hijo derecho
    }
  | PRINT STR ternary // Una impresión de una String y una variable
    {
      $$.flag = fAST;
      $$.u.ast = mkNd(PRINT, mkSlf(STR,$2.u.vStr), $3.u.ast); // Nodo impresión con dos hijos
    }
  | READ IDENT  // Lectura de una variable
    {
      $$.flag = fAST;
      $$.u.ast = mkNd(READ, NULL, mkSlf(IDENT,$2.u.vStr));  // Nodo lectura sin hijo izquierdo
    }
  | READ STR IDENT  // Impresión y lectura de una variable
    {
      $$.flag = fAST;
      $$.u.ast = mkNd(READ, mkSlf(STR,$2.u.vStr), mkSlf(IDENT,$3.u.vStr));  // Nodo lectura con dos hijos
    }
  ;

 // Una expresión ternaria puede ser
ternary
  // Una expresión ternaria de varias expresiones ternarias
  : ternary '?' ternary ':' ternary
    {
      $$.flag = fAST;
      $$.u.ast = mkNd('?', $1.u.ast, mkNd(':', $3.u.ast, $5.u.ast) );
    }
  // O una expresión
  | expr
    /* $$ = $1 */
  ;

 // Expresiones. Simplemente se crean los nodos correspondientes
expr
  : expr OR expr
    {
      $$.flag = fAST;
      $$.u.ast = mkNd(OR, $1.u.ast, $3.u.ast);
    }
  | expr AND expr
    {
      $$.flag = fAST;
      $$.u.ast = mkNd(AND, $1.u.ast, $3.u.ast);
    }
  | expr EQ expr
    {
      $$.flag = fAST;
      $$.u.ast = mkNd(EQ, $1.u.ast, $3.u.ast);
    }
  | expr NE expr
    {
      $$.flag = fAST;
      $$.u.ast = mkNd(NE, $1.u.ast, $3.u.ast);
    }
  | expr LE expr
    {
      $$.flag = fAST;
      $$.u.ast = mkNd(LE, $1.u.ast, $3.u.ast);
    }
  | expr GE expr
    {
      $$.flag = fAST;
      $$.u.ast = mkNd(GE, $1.u.ast, $3.u.ast);
    }
  | expr '<' expr
    {
      $$.flag = fAST;
      $$.u.ast = mkNd('<', $1.u.ast, $3.u.ast);
    }
  | expr '>' expr
    {
      $$.flag = fAST;
      $$.u.ast = mkNd('>', $1.u.ast, $3.u.ast);
    }
  | expr '+' expr
    {
      $$.flag = fAST;
      $$.u.ast = mkNd('+', $1.u.ast, $3.u.ast);
    }
  | expr '-' expr
    {
      $$.flag = fAST;
      $$.u.ast = mkNd('-', $1.u.ast, $3.u.ast);
    }
  | expr '*' expr
    {
      $$.flag = fAST;
      $$.u.ast = mkNd('*', $1.u.ast, $3.u.ast);
    }
  | expr '/' expr
    {
      $$.flag = fAST;
      $$.u.ast = mkNd('/', $1.u.ast, $3.u.ast);
    }
  | expr '^' expr
    {
      $$.flag = fAST;
      $$.u.ast = mkNd('^', $1.u.ast, $3.u.ast);
    }
  | '!' expr
    {
      $$.flag = fAST;
      $$.u.ast = mkNd('!',NULL,$2.u.ast);
    }
  | '+' expr %prec UNOP
    {
      $$ = $2;
    }
  | '-' expr %prec UNOP
    {
      $$.flag = fAST;
      $$.u.ast = mkNd('-',NULL,$2.u.ast);
    }
  | '(' ternary ')'
    {
      $$ = $2;
    }
  | FLOAT
    {
      $$.flag = fAST;
      $$.u.ast = mkDlf(FLOAT,$1.u.vFloat);
    }
  | IDENT
    {
      $$.flag = fAST;
      $$.u.ast = mkSlf(IDENT,$1.u.vStr);
    }
  | SIN '(' ternary ')'
    {
      $$.flag = fAST;
      $$.u.ast = mkNd(SIN,$3.u.ast,NULL);
    }
  | COS '(' ternary ')'
    {
      $$.flag = fAST;
      $$.u.ast = mkNd(COS,$3.u.ast,NULL);
    }
  | TAN '(' ternary ')'
    {
      $$.flag = fAST;
      $$.u.ast = mkNd(TAN,$3.u.ast,NULL);
    }
  ;


%%

// Error sintáctico: impresión del error
int yyerror(char *str) {
  prError(yylineno,"%s\n",str,NULL);
  return 1;
}

extern FILE *yyin;

// Programa principal (lanzador)
int main(int argc, char *argv[]) {
  exitOnError();  // Se indica que si hay errores se finaliza la ejecución

  // Impresión de funcionamiento
  if (argc!=2) {
    puts("\nUsage: demo <filename>\n");
    fflush(stdout);
    return 1;
  }

  // Lectura del fichero (programa)
  if ((fIn=fopen(argv[1],"rb"))==NULL) {
    fprintf(stderr,"\nCannot open file: %s\n\n",argv[1]);
    fflush(stderr);
    return 1;
  }

  yyin = fIn;

  // Asignación de nombre al programa
  setFilename( argv[1] );

  // Parseo del fichero (texto -> AST)
  if (yyparse() != 0) {
    fclose(fIn);
    prError(yylineno,"Parsing aborted due to errors in input\n",NULL);
  }

  fclose(fIn);  // Cierre del fichero

  // Si hay raiz en el AST
  if (astRoot != NULL) { 
    evaluate(astRoot);  // Interpretación del AST
  } else {
    prError(yylineno,"No parse output provided, aborting evaluation\n",NULL);
  }

  return 0;
}

/* -- EOF -- */

