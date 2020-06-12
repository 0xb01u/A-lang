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
    LAND = 260,
    LOR = 261,
    LT = 262,
    GT = 263,
    BAND = 264,
    BOR = 265,
    SL = 266,
    SR = 267,
    DIV = 268,
    UNARY = 269,
    BNOT = 270,
    ID = 271,
    PRINT = 272,
    READ = 273,
    SIN = 274,
    COS = 275,
    TAN = 276,
    LN = 277,
    INT = 278,
    FLOAT = 279,
    STR = 280,
    WHILE = 281,
    IF = 282,
    ELSE = 283,
    DEL_BL_A = 284,
    DEL_BL_C = 285
  };
#endif
/* Tokens.  */
#define EQ 258
#define NE 259
#define LAND 260
#define LOR 261
#define LT 262
#define GT 263
#define BAND 264
#define BOR 265
#define SL 266
#define SR 267
#define DIV 268
#define UNARY 269
#define BNOT 270
#define ID 271
#define PRINT 272
#define READ 273
#define SIN 274
#define COS 275
#define TAN 276
#define LN 277
#define INT 278
#define FLOAT 279
#define STR 280
#define WHILE 281
#define IF 282
#define ELSE 283
#define DEL_BL_A 284
#define DEL_BL_C 285

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 28 "ac.y" /* yacc.c:1909  */

	struct syn_elem {
		unsigned char type;		/* Tipo de token */
		union {					/* Valor útil del token */
			double real_value;	/* Como número real */
			long int_value;		/* Como número entero */
			char *string;		/* Como cadena de caracteres */
			struct ast_s *node;
		} u;
	} s;

#line 126 "ac.syn.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_AC_SYN_H_INCLUDED  */
