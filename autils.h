/**
 * Compilador de A - Proyecto final GLF
 * Manuel de Castro Caballero, María Ruiz Molina, Andrés Trigueros Vega
 * Curso 2019-2020
 */

/* Identificadores de tokens */
#define ID_id		1
#define INT_id		2
#define REAL_id		3
#define STR_id		4
#define AST_NODE_id	5

/* Salidas del programa. */
#define PARSE_SUCCESS		 0
#define LEXICAL_ERROR		-1
#define SYNTAX_ERROR		-2
#define FILE_ERROR			-3
#define SYMTAB_NOT_FOUND	-4
#define MEMORY_ERROR		-5
#define ILEGAL_OPERATION	-6

#define mallocCheck(ptr, size) { \
	if ((ptr = malloc(size)) == NULL) \
	{ \
		fprintf(stderr, "Fallo de alocación de memoria.\n"); \
		exit(MEMORY_ERROR); \
	} \
}
