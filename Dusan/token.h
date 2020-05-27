/**
 * @Author: Dušan Kolář
 * @Year:   2003-2018
 * Copyright (c) 2018
 * Licence: GLP 3.0
 */

#ifndef ___TOKEN_H___
#define ___TOKEN_H___

// Macros para obtener el correspondiente valor de yylval
#define yyFlag(x)    x.s.flag
#define yyFloat(x)   x.s.u.vFloat
#define yyStr(x)     x.s.u.vStr
#define yyAST(x)     x.s.u.ast

// Identificadores de tipo (para la flag)
#define fIDENT  1	// Identificador
#define fFLOAT  2	// Número
#define fSTR    3	// Cadena
#define fAST    4	// Nodo del AST

#endif
