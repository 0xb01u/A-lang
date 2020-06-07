/**
 * Proyecto final GLF
 * Manuel de Castro Caballero, María Ruiz Molina, Andrés Trigueros Vega
 * Curso 2019-2020
 */
/*
 * TABLA DE SÍMBOLOS PARA EL INTÉRPRETE
 *
 */
#include "symtab.h"
#include "exits.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

extern int lineNum;
extern char programName[];

static int size = 256;	/* Tamaño de la tabla */
static int symbols = 0;	/* Número de entradas llenas */

typedef struct {
	bool used;
	char *id;
	sym_value value;
} symbol_t;

static symbol_t *symTab = NULL;

/* Función de dispersión para un identificador (String) */
static int hash(char *s)
{
	int h = 0;
	int i = 0;
	while (s[i] != 0) h = 31*h + s[i++];
	return abs(h);
}

/* Función para hallar la primera posición potencial de un identificador */
static int first(char *s)
{
	return hash(s) % size;
}

/* Función para hallar el "salto" para buscar un identificador en la tabla. */
static void step(char *s, int *pos)
{
	int step = (int)(hash(s) / size);
	if (step % 2 == 0) step++;
	*pos = (*pos + step) % size;
}

/* Función para obtener la posición de un identificador en la tabla */
static int pos(char *id)
{
	int pos = first(id);
	while (strcmp(id, symTab[pos].id) != 0)
	{
		if (symTab[pos].used)
		{
			return SYMTAB_NOT_FOUND;
		}
		step(id, &pos);
	}

	return pos;
}

/* Función que devuelve una posición vacía de la tabla 
 * en la que iría un identificador */
static int freePos(char *id)
{
	int pos = first(id);
	while (strcmp(id, symTab[pos].id) != 0 && !symTab[pos].used)
	{
		step(id. &pos);
	}

	return pos;
}

/* Función para redimensionar la tabla al insertar un símbolo */
static bool resize()
{
	if (size/symbols >= 2) return false;

	size *= 2;
	symbol_t *tmp = symTab;
	symTab = (symbol_t *)malloc(sizeof(symbol_t) * (size_t)size);
	memset(symTab, 0, sizeof(symbol_t) * (size_t)size);

	for (int i = 0; i < size/2; i++)
	{
		if (tmp[i].used)
		{
			int pos = freePos(tmp[i].id);
			symTab[pos].used = true;
			strcpy(&symTab[pos].id, tmp[i].id);
			symTab[pos].value = tmp[i].value;
		}
	}

	free(tmp);

	return true;
}

/* Función para obtener el valor de un símbolo de la tabla */
sym_value get(char *id)
{
	int pos = pos(id);
	if (pos == SYMTAB_NOT_FOUND)
	{
		fprintf(stderr, "%s(%d) error -- identificador no encotrado: %s\n", programName, lineNum, id);
		exit(SYMTAB_NOT_FOUND);
	}
	return symTab[pos].value;
}

/* Función para cambiar una entrada de la tabla de símbolos */
void edit(char *id, sym_value value)
{
	if (symbols == 0) init();

	int pos = pos(id);
	if (pos == SYMTAB_NOT_FOUND)
	{
		symbols++;
		resize();

		pos = freePos(id);
		symTab[pos].used = true;
		strcpy(&symTab[pos].id, id);
	}

	symTab[pos].value = value;
}

/* Rellenar la tabla con entradas vacías */
static void init()
{
	symTab = (symbol_t *)malloc(sizeof(symbol_t) * size);
	memset(symTab, 0, sizeof(symbol_t) * (size_t)size);
}
