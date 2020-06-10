/**
 * @Autores: Manuel de Castro Caballero, María Ruiz Molina, Andrés Trigueros Vega
 * @Curso: 2019 - 2020
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KWORDSN = 10

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

%}

 /*Letras del alfabeto, minusculas y mayusculas*/
LETTER		([_a-zA-Z])

 /*Digito y numeros*/
DIGIT		([0-9])
NUMBER		({DIGIT}+)

 /*Representacion tipo float*/
FLOAT     	(({NUMBER}"."{DIGIT}*)({DIGIT}*"."{NUMBER})

 /*Identificador*/
ID		({LETTER}({LETTER}|{DIGIT})*)

 /*Delimitador de bloques*/	
DEL_BL_A		(["{"{WSPC}*"\n"])
DEL_BL_C		(["}"{WSPC}*"\n"])
 /* DEL_BL_C_2????? */	

 /*Espacios en blanco*/
WSPC		([ \t\f\r])
WSCPS		({WSPC}+)

/*Operadores aritmeticos*/
OP_AR		([+*-/%=()^])

 /*Operadores logicos*/
OP_LOG		("=="|"!="|"<"|">")

 /*Comienzo de cadena de caracteres*/
STR_START	(["]) /*"/**/

%%

WSPC		;


ID		{	
			unsigned i = 0;
			int r = -1;
			char *res;
			
			lower(yytext);
			
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

SEPAR		{	/*Salto de linea*/
			++lineNum;
			return yytext[0];
		}

NUMBER	{		/*Se lee el numero entero y se castea a double*/
			long int li;
			sscanf(yytext, "%d", &li);
			yylval.s.u.real_value = (double)li;
			yylval.s.type = REAL_id;

			return FLOAT;
		}

FLOAT		{	/*Se lee el numero decimal*/
			sscanf(yytext, "%lf", &yylval.s.u.real_value);
			yylval.s.type = REAL_id;

			return FLOAT;

		}

STR_START	{
			yylval.s.u.string = readStr();
			yylval.s.type = STR_id;
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
		
.		{
			fprintf(stderr, "%s(%d): error -- Caracter inesperado %c\n", programName, lineNum, yytext[0]);
		}

%%

static void lower(char *s)
{
	unsigned l = strlen(s);
	
	while(l>0) {
		--l;
		if (s[l] >= 'A' && s[l] <= 'Z') s[l] += ('a' - 'A');
	}
}

static void addStr(char **s, unsigned long *len, char c)
{
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


static char *readStr(void)
{
	int c;
	char buff[257];
	unsigned long len = 256;
	
	buff[0] = '\0';
	
	do {
		c == input();
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
