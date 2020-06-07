/**
 * @Autores: 
 * @Curso: 2019 - 2020
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KWORDSN = 10

char *keyWords[KWORDSL] = {
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
unsigned keyCodes[KWORDSL] = {
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

%}

 /*Letras del alfabeto, minusculas y mayusculas*/
LETTER		([_a-zA-Z])

 /*Digito y numeros*/
DIGIT		([0-9])
NUMBER		({DIGIT}+)

 /*Exponente de float*/
EXP		([eE][-+]?{NUMBER})

 /*Distintas formas de representacion de constantes tipo float*/
FLOAT_DOT	({NUMBER}"."{NUMBER})
FLOAT_EXP	({NUMBER}{EXP})
FLOAT_DE		({NUMBER}"."{NUMBER}{EXP})	
 
 /*Representacion tipo float*/
FLOAT     ({FLOAT_DOT}|{FLOAT_EXP}|{FLOAT_DE})

 /*Identificador*/
ID		({LETTER}({LETTER}|{DIGIT})*)	 

 /*Separador de sentencias*/
SEPAR		([\n])	

 /*Delimitador de bloques*/	
DEL_BL		([{}])			

 /*Espacios en blanco*/
WSPC		([ \t\f\r])
WSCPS		({WSPC}+)

/*Operadores aritmeticos*/
OP_AR		([-+/*%=()^])

 /*Operadores logicos*/
OP_LOG		("=="|"!="|"<"|">")

 /*Comienzo de cadena de caracteres*/
STR_START	(["])/*"/**/

%%

WSPC		;


IDENT		{	
			unsigned i = 0;
			int r = -1;
			char *res;
			
			lower(yytext);
			
			/*Recorre el listado de palabras clave para ver si la introducida lo es*/
			while (i<KWORDSL && r<0) {
				if ((r=strcmp(keyWords[i],yytext))==0) return keyCodes[i];
				++i;
			}
			/*Si no es una palabra clave y lo devuelve como identificador*/
			yystr(yylval)=sdup(yytext);
			yyFlag(yylval)=fIDENT;
			
			return IDENT;
		}

SEPAR		{	/*Salto de linea*/
			++lineNum;
			return yytext[0];		
		}

NUMBER	{		/*Se lee el numero entero y se castea a double*/
			long int li;
			sscanf(yytext, "%d", &li);
			yyFLoat(yylval) = (double)li;
			yyFlag(yylval) = fFLOAT;

			return FLOAT;
		}

FLOAT		{	/*Se lee el numero decimal*/
			sscanf(yytext, "%lf", &(yyFloat(yylval)));
			yyFlag(yylval) = fFLOAT;

			return FLOAT;

		}

STR_START	{
			yyStr(yylval) = readStr();
			yyFlag(yylval) = fSTR;
			return STR;
		}


OP_AR		{
			return yytext[0];
		}
	
OP_LOG		{
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
		
		{
			printf("%s(%d): Caracter inesperado %c\n", programName, lineNum, yytext[0]);
		}

%%

static void lower(char *s) {
	unsigned l = strlen(s);
	
	while(l>0) {
		--l;
		if (s[l]>='A' && s[l]<='Z') s[l] = s[l]+'a' - 'A';
	}
}

static void addStr(char **s, unsigned long *len, char c) {
	char buf[2];
	buf[0] = c;
	buf[1] = '\0';

	if (strlen(*s) >= *len) {
		char ss[len + 1025];
		strcpy(ss, *s);
		*s = ss;
		*len += 1024;
	}
	strcat(*s, buf);
}


static char *readStr(void) {
	int c;
	char buff[257];
	unsigned long len = 256;
	
	buff[0] = '\0';
	
	do {
		c = input();
		if (c < ' ')
			fprintf(stderr, "%s(%d): error -- Simbolo no esperado en cadena de caracteres: %c\n", programName, lineNum, c);
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

/*
 ______
<      \
<.     .\ O
|  \______
<\O____/
 /     \
>       <


TODO
Donehre ASCII


*/



