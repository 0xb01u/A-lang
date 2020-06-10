/**
 * Compilador de A - Proyecto final GLF
 * Manuel de Castro Caballero, María Ruiz Molina, Andrés Trigueros Vega
 * Curso 2019-2020
 */

/*
 * TABLA DE SÍMBOLOS PARA EL INTÉRPRETE
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "symtab.h"
#include "autils.h"

extern int yylineno;
extern char programName[];

static int size = 256;	/* Tamaño de la tabla */
static int symbols = 0;	/* Número de entradas llenas */

typedef struct {
	bool used;
	char *id;
	symbol value;
} entry;

static entry *symTab = NULL;

/* Rellenar la tabla con entradas vacías */
static void init()
{
	mallocCheck(symTab, sizeof(entry) * size);
	memset(symTab, 0, sizeof(entry) * (size_t)size);
}

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
		step(id, &pos);
	}

	return pos;
}

/* Función para redimensionar la tabla al insertar un símbolo */
static bool resize()
{
	if (size/symbols >= 2) return false;

	size *= 2;
	entry *tmp = symTab;
	init();

	for (int i = 0; i < size/2; i++)
	{
		if (tmp[i].used)
		{
			int index = freePos(tmp[i].id);
			symTab[index].used = true;
			strcpy(symTab[index].id, tmp[i].id);
			symTab[index].value = tmp[i].value;
		}
	}

	free(tmp);

	return true;
}

/* Función para obtener el valor de un símbolo de la tabla */
symbol get(char *id)
{
	int index = pos(id);
	if (index == SYMTAB_NOT_FOUND)
	{
		fprintf(stderr, "%s(%d) error -- identificador no encotrado: %s\n", programName, yylineno, id);
		exit(SYMTAB_NOT_FOUND);
	}
	return symTab[index].value;
}

/* Función para cambiar una entrada de la tabla de símbolos */
void edit(char *id, symbol value)
{
	if (symbols == 0) init();

	int index = pos(id);
	if (index == SYMTAB_NOT_FOUND)
	{
		symbols++;
		resize();

		index = freePos(id);
		symTab[index].used = true;
		strcpy(symTab[index].id, id);
	}

	symTab[index].value = value;
}
