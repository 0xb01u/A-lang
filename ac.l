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
#include "ac.syn.h"

#define KWORDSN 10

char *keyWords[KWORDSN] = {
	"read"
	"print"
	"if"
	"else"
	"while"
	"sin"
	"cos"
	"tan"
	"ln"
	"div"
};
unsigned keyCodes[KWORDSN] = {
	READ,
	PRINT,
	IF,
	ELSE,
	WHILE,
	SIN,
	COS,
	TAN,
	LN,
	DIV,
};

int yywrap(void) { return 1; }

extern char programName[];

static char *readStr();
static void addStr(char **s, unsigned long *len, char c);
%}

LETTER		([_a-zA-Z])

DIGIT		([0-9])
NUMBER		({DIGIT}+)

FLOAT		(({NUMBER}"."{DIGIT}*)|({DIGIT}*"."{NUMBER}))

ID			({LETTER}({LETTER}|{DIGIT})*)

DEL_BL_A	([{{WSPC}*\n])
DEL_BL_C	([}{WSPC}*\n])

WSPC		([ \t\f\r])
WSCPS		({WSPC}+)

OP_AR		([+*-/%=()^])

OP_LOG		("=="|"!="|"<"|">")

STR_START	(["])

LINE_COMM	("//".*\n)

COMM ("/*"[.|\n]*"*/")

%%

[\n]	{	/*Salto de linea*/
			++yylineno;
			return yytext[0];
		}

{WSPC}|{LINE_COMM}	;

{COMM}	{	/*Cuenta las líneas que ocupa el comentario*/
			for (int i = 0; i < strlen(yytext); i++)
			{
				if (yytext[0] == '\n') yylineno++;
			}
		}

{ID}	{
			unsigned i = 0;
			int r = -1;
			char *res;
			
			/*Recorre el listado de palabras clave para ver si la introducida lo es*/
			while (i<KWORDSN && r<0) {
				if ((r = strcmp(keyWords[i],yytext)) == 0) return keyCodes[i];
				++i;
			}
			/*Si no es una palabra clave y lo devuelve como identificador*/
			mallocCheck(yylval.s.u.string, strlen(yytext) + 1);
			strcpy(yylval.s.u.string, yytext);
			yylval.s.type = ID_id;

			return ID;
		}

{NUMBER}	{	/*Se lee el numero entero y se castea a double*/
			long li;
			sscanf(yytext, "%ld", &li);
			yylval.s.u.real_value = (double)li;
			yylval.s.type = REAL_id;

			return FLOAT;
		}

{FLOAT}	{	/*Se lee el numero decimal*/
			sscanf(yytext, "%lf", &yylval.s.u.real_value);
			yylval.s.type = REAL_id;

			return FLOAT;

		}

{STR_START}	{
			yylval.s.u.string = readStr();
			yylval.s.type = STR_id;
			return STR;
		}


{OP_AR}	{
			return yytext[0];
		}
	
{OP_LOG}	{
			switch(yytext[0]) {
				case '=':
					return EQ;
				case '!':
					return NE;
				case '<':
					return LT;
				case '>':
					return GT;
			}
		}
		
.		{
			fprintf(stderr, "%s(%d): error -- Caracter inesperado %c\n", programName, yylineno, yytext[0]);
		}

%%

static void addStr(char **s, unsigned long *len, char c)
{
	char buf[2];
	buf[0] = c;
	buf[1] = '\0';

	if (strlen(*s) >= *len) {
		char *ss = (char *)malloc(sizeof(char) * (*len + 1025));
		strcpy(ss, *s);
		*s = ss;
		*len += 1024;
	}
	strcat(*s, buf);
}


static char *readStr()
{
	int c;
	char *buff = (char *)malloc(sizeof(char) * 257);
	unsigned long len = 256;
	
	buff[0] = '\0';
	
	do {
		c == input();
		if (c < ' ')
			fprintf(stderr, "%s(%d): error -- Simbolo no esperado en cadena de caracteres: %c\n", programName, yylineno, c);
		if (c == '"') break;
		if (c == '\\')
		{
			c = input();
			if(c != '\\' && c != '"')
			{
				unput(c);
				c = '\\';
			}
		}
		addStr(&buff, &len, c);	
	} while(1);
	return buff;
}
