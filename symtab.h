/**
 * Proyecto final GLF
 * Manuel de Castro Caballero, María Ruiz Molina, Andrés Trigueros Vega
 * Curso 2019-2020
 */

enum symbolType = { VOID, REAL };	// TODO: tipos en función de los lexemas.
typedef enum symbolType symbol_t;

typedef struct symbol_s {
	symbol_t type;
	union {
		double real;
	} value;
} sym_value;
