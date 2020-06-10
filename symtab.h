/**
 * Proyecto final GLF
 * Manuel de Castro Caballero, María Ruiz Molina, Andrés Trigueros Vega
 * Curso 2019-2020
 */

typedef struct symbol_s {
	int type;
	union {
		double real;
	} value;
} sym_value;

sym_value get(char *id);
void edit(char *id, sym_value value);
