/**
 * @Author: Dušan Kolář
 * @Year:   2003-2018
 * Copyright (c) 2018
 * Licence: GLP 3.0
 */

%{
#include <stdio.h>
#include <string.h>

#include "stduse.h"
#include "inter05.h"
#include "token.h"
#include "astree.h"

#define KWLEN  6  /* Número de palabras clave del lenguaje */
/* Palabras clave del lenguaje 
   (Ordenadas alfabéticamente) */
char *keywords[KWLEN] = {
    "cos",
    "let",
    "print",
    "read",
    "sin",
    "tan",
};

/* Identificadores de las palabras clave */
unsigned keycodes[KWLEN] = {
    COS,
    LET,
    PRINT,
    READ,
    SIN,
    TAN,
};

static void lower(char *s); /* Pasar todas las letras a minúsculas (definida abajo) */
static char *readStr(void); /* Leer y guardar String como "variable" (definida abajo) */

int yywrap(void) { return 1; }  /* Al terminar la lectura de un fichero, pista */
%}

LETTER    ([_a-zA-Z])

DIGIT     ([0-9])
DIGITS    ({DIGIT}+)

EXP       ([eE][-+]?{DIGITS})

FLOAT1    ({DIGITS}"."{DIGITS})
FLOAT2    ({DIGITS}{EXP})
FLOAT3    ({DIGITS}"."{DIGITS}{EXP})

IDENT     ({LETTER}({LETTER}|{DIGIT})*)

COMSEP    ([\n])

WSPC      ([ \t\f\r])
WSPCS     ({WSPC}+)

FLOAT     ({FLOAT1}|{FLOAT2}|{FLOAT3})

OP1       ([-+/*=<>?:()!^])
OP2       ("=="|"!="|"<="|">="|"&&"|"||")

STRSTART  (["])

%%

{WSPC}              ; /* nothing to do, white space */

{IDENT}               {
                        unsigned i = 0;
                        int r=-1;
                        char *res;
                        
                        lower(yytext);  /* Se pasa a minúsculas */

                        /* Se busca si es palabra una clave */
                        while (i<KWLEN && r<0) {
                            if ((r=strcmp(keywords[i],yytext))==0) return keycodes[i];
                            ++i;
                        }

                        /* Si no es una palabra clave */
                        yyStr(yylval)=sdup(yytext); /* Se copia el identificador leído */
                        yyFlag(yylval)=fIDENT;  /* Se apunta que es un identificador */

                        return IDENT;   /* Se devuelve que se ha leído un identificador */
                    }

{COMSEP}              {
                        /* Salto de línea: se aumenta el número de línea */
                        ++yylineno; /* yylineno se define aquí (en este fichero) */
                        return yytext[0]; /* Se devuelve el salto de línea */
                    }

{DIGITS}              {
                        /* Se lee el número entero */
                        long int li;
                        sscanf(yytext,"%ld",&li);
                        /* Se castea a double */
                        yyFloat(yylval) = (double)li;
                        yyFlag(yylval) = fFLOAT;
                        
                        return FLOAT;
                    }
                    
{FLOAT}               {
                        /* Se lee el número decimal */
                        sscanf(yytext,"%lf",&( yyFloat(yylval) ));
                        yyFlag(yylval) = fFLOAT;
                        
                        return FLOAT;
                    }
                    
{STRSTART}            {
                      /* Se lee la String */
                      yyStr(yylval) = readStr();
                      yyFlag(yylval) = fSTR;
                      
                      return STR;
                    }
                    
{OP1}                 return yytext[0]; /* Se lee el operador aritmético */

{OP2}                 {
                        switch (yytext[0]) {  /* Se lee el operador lógico */
                            case '=':
                                return EQ;
                            case '!':
                                return NE;
                            case '<':
                                return LE;
                            case '>':
                                return GE;
                            case '&':
                                return AND;
                            case '|':
                                return OR;
                        }
                    }                    
                    
.                   {
                        /* Cualquier otro caracter: error */
                        prError(yylineno,"Unexpected character in input: %c [%d]\n",yytext[0],yytext[0],NULL);
                    }

%%

/* Pasar la String s a minúsculas */
static void lower(char *s) {
    unsigned l = strlen(s);
    /* Vamos de atrás a adelante */
    while (l>0) {
        --l;
        /* Si es una letra mayúscula, pues se hace minúscula. Easy */
        if (s[l]>='A' && s[l]<='Z') s[l] = s[l]+'a'-'A';
    }
}

/* Añadir caracter c a la String s, acutalizando su tamaño */
static void addStr(char **s, unsigned long *len, char c) {
  char buf[2];
  buf[0] = c;
  buf[1] = '\0';

  /* Si la cadena actualmente tiene tantos caracteres (o más) */
  /* como undica su tamaño */
  if (strlen(*s) >= *len) {
    char *ss;
    /* Se alocan 1024 caracteres más (más el \0 final) */
    ss=xmalloc(*len+1025);
    /* Se copia */
    strcpy(ss,*s);
    /* Se libera la anterior */
    xfree(*s);
    *s=ss;
    /* Se actualiza el tamaño */
    *len = *len+1024;
  }
  
  /* Se concatena la cadena actual con el caracter + \0 */
  strcat(*s,buf);
}

/* Leer y almacenar una String */
static char *readStr(void) {
  int c;
  char *buff; /* Buffer donde se almacena la String */
  unsigned long len = 256;  /* Longitud máxima: 256 caracteres */

  buff=xmalloc(257);  /* Se aloca memoria para la String */
  buff[0]='\0';       /* Se pone el primer caracter como '\0' */

  /* Se van leyendo caracteres: */
  do {
    /* Se lee un caracter */
    c = input();

    /* Caracteres erróneos */
    if (c < ' ') prError(yylineno,"Unexpected symbol in string literal [%d]\n",c,NULL);
    /* Si se lee el terminador de cadena, se finaliza el bucle */
    if (c == '"') break;

    /* Las subcadenas '\\' y '\"' se leen como solo '\' y '"', respectivamente */
    if (c=='\\') {
      c == input();
      if (c!='\\' && c !='"') {
        unput(c);
        c = '\\';
      }
    }

    /* Se añade el caracter leído a la cadena */
    addStr(&buff,&len,c);
    
  } while (1);

  return buff;
}


/* -- EOF -- */
