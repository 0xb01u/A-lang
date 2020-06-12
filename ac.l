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
	"cos",
	"div",
	"else",
	"if",
	"ln",
	"print",
	"read",
	"sin",
	"tan",
	"while",
};
unsigned keyCodes[KWORDSN] = {
	COS,
	DIV,
	ELSE,
	IF,
	LN,
	PRINT,
	READ,
	SIN,
	TAN,
	WHILE,
};

int yywrap(void) { return 1; }

extern char programName[];

static char *readStr();
static void addStr(char **s, unsigned long *len, char c);
%}

LETTER		([_a-zA-Z])

DIGIT		([0-9])
NUMBER		({DIGIT}+)

HEX_DIGIT	([0-9a-fA-F])
HEX			(0x{HEX_DIGIT}+)

FLOAT		(({NUMBER}"."{DIGIT}*)|({DIGIT}*"."{NUMBER}))

ID			({LETTER}({LETTER}|{DIGIT})*)

DEL_BL_A	(\{)
DEL_BL_C	(\})

WSPC		([ \t\f\r])
WSCPS		({WSPC}+)

OP_AR		([+*-/%=()^])

OP_LOG		("=="|"!="|"&&"|"||"|"<"|">")

OP_BIT		("&"|"|"|"~"|"<<"|">>")

STR_START	(["])

LINE_COMM	("//".*\n)

COMM ("/*"(.|\n)*"*/")

%%

[\n]	{	/*Salto de linea*/
			yylineno++;
			return yytext[0];
		}

{DEL_BL_A}	return DEL_BL_A;

{DEL_BL_C}	return DEL_BL_C;

{WSPC}	;

{LINE_COMM}	{
			yylineno++;
			return '\n';
		}

{COMM}	{	/*Cuenta las líneas que ocupa el comentario*/
			for (int i = 0; i < strlen(yytext); i++)
			{
				if (yytext[0] == '\n') yylineno++;
			}

			return '\n';
		}

{ID}	{
			unsigned i = 0;
			int r = -1;
			
			/*Recorre el listado de palabras clave para ver si la introducida lo es*/
			while (i<KWORDSN && r<0)
			{
				if ((r = strcmp(keyWords[i], yytext)) == 0) return keyCodes[i];
				++i;
			}
			/*Si no es una palabra clave y lo devuelve como identificador*/
			mallocCheck(yylval.s.u.string, strlen(yytext) + 1);
			strcpy(yylval.s.u.string, yytext);
			yylval.s.type = ID_id;

			return ID;
		}

{NUMBER}	{	/*Se lee un numero entero*/
			sscanf(yytext, "%ld", &yylval.s.u.int_value);
			yylval.s.type = INT_id;

			return INT;
		}

{HEX}	{
			sscanf(&yytext[2], "%lx", &yylval.s.u.int_value);
			yylval.s.type = INT_id;

			return INT;
		}

{FLOAT}	{	/*Se lee un numero con decimales*/
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

{OP_BIT}	{
			switch (yytext[0])
			{
				case '&':
					return BAND;
				case '|':
					return BOR;
				case '~':
					return BNOT;
				case '<':
					return SL;
				case '>':
					return SR;
			}
	}
	
{OP_LOG}	{
			switch (yytext[0])
			{
				case '=':
					return EQ;
				case '!':
					return NE;
				case '&':
					return LAND;
				case '|':
					return LOR;
				case '<':
					return LT;
				case '>':
					return GT;
			}
		}
		
.		{
			fprintf(stderr, "%s(%d): error -- Caracter inesperado %c\n", programName, yylineno, yytext[0]);
			exit(LEXICAL_ERROR);
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
	char *buff;
	mallocCheck(buff, sizeof(char) * 257);
	unsigned long len = 256;
	
	buff[0] = '\0';
	
	do {
		c = input();
		if (c < ' ')
		{
			fprintf(stderr, "%s(%d): error -- Simbolo no esperado en cadena de caracteres: %d\n", programName, yylineno, c);
			exit(LEXICAL_ERROR);
		}
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
