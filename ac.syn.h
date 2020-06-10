/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_AC_SYN_H_INCLUDED
# define YY_YY_AC_SYN_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    EQ = 258,
    NE = 259,
    LT = 260,
    GT = 261,
    DIV = 262,
    UNARY = 263,
    ID = 264,
    PRINT = 265,
    READ = 266,
    SIN = 267,
    COS = 268,
    TAN = 269,
    LN = 270,
    FLOAT = 271,
    STR = 272,
    WHILE = 273,
    IF = 274,
    ELSE = 275,
    DEL_BL_A = 276,
    DEL_BL_C = 277
  };
#endif
/* Tokens.  */
#define EQ 258
#define NE 259
#define LT 260
#define GT 261
#define DIV 262
#define UNARY 263
#define ID 264
#define PRINT 265
#define READ 266
#define SIN 267
#define COS 268
#define TAN 269
#define LN 270
#define FLOAT 271
#define STR 272
#define WHILE 273
#define IF 274
#define ELSE 275
#define DEL_BL_A 276
#define DEL_BL_C 277

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 25 "ac.y" /* yacc.c:1909  */

	struct syn_elem {
		unsigned char type;		/* Tipo de token */
		union {					/* Valor útil del token */
			double real_value;	/* Como número real */
			long int_value;		/* Como número entero */
			char *string;		/* Como cadena de caracteres */
			struct ast_t *node;
		} u;
	} s;

#line 110 "ac.syn.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_AC_SYN_H_INCLUDED  */
