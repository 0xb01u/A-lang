/**
 * Compilador de A - Proyecto final GLF
 * Manuel de Castro Caballero, María Ruiz Molina, Andrés Trigueros Vega
 * Curso 2019-2020
 */

/* Tipo "símbolo" para la tabla de símbolos 
   (Par tipo de dato - valor del dato) */
typedef struct symbol_s {
	int type;
	union {
		double real;
	} value;
} symbol;

/* Declaración de funciones de symtab.c */
symbol get(char *id);
void edit(char *id, symbol value);
