/**
 * @Author: Dušan Kolář
 * @Year:   2003-2018
 * Copyright (c) 2018
 * Licence: GLP 3.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "stduse.h"

static char fName[10240] = "";

static unsigned char wasE = 0;
static unsigned char exitOnE = 0;

/* --------------------------------------------------------------------- */

// Nombre para el programa
void setFilename(const char *str) {
  strcpy(fName,str);
}

/* --------------------------------------------------------------------- */

// Devolución de errores
int wasError(void) { return wasE; }

/* --------------------------------------------------------------------- */

// Apunta que los errores causan la finalización del programa
void exitOnError(void) { exitOnE = 1; }

/* --------------------------------------------------------------------- */

// Apunta que se ha producido un error
// Format es el error con formato C, tipo "Variable %s con valor %d"
// ... son las variables a sustituir en la String format: "x", x
void prError(unsigned short lineno, char *format, ...) {
  va_list ap;
  char *arg;

  wasE = 1;

  // va = variable arguments
  // va_start asigna a ap los argumentos que van después de format
  va_start(ap,format);
  // Imprimir el nombre del fichero y el número de línea del error
  fprintf(stderr,"%s (%u): ",fName,lineno);
  // Imprimir el error en sí (format y sus variables)
  vfprintf(stderr,format,ap);
  fflush(stderr);
  // Limpiar ap
  va_end(ap);

  if (exitOnE) exit(1);
}

/* --------------------------------------------------------------------- */

// Apunta que se ha producido un aviso
void prWarning(unsigned short lineno, char *format, ...) {
  va_list ap;
  char *arg;

  // va = variable arguments
  // va_start asigna a ap los argumentos que van después de format
  va_start(ap,format);
  // Imprimir el nombre del fichero y el número de línea del error
  fprintf(stderr,"%s (%u): ",fName,lineno);
  // Imprimir el error en sí (format y sus variables)
  vfprintf(stderr,format,ap);
  fflush(stderr);
  // Limpiar ap
  va_end(ap);

}

/* --------------------------------------------------------------------- */
/* --------------------------------------------------------------------- */

// Un malloc(), pero fancy
void *xmalloc(unsigned short size) {
  void *tmp;

  if ((tmp=malloc(size))==NULL) {
    fprintf(stderr,"Fatal error: Out of memory\n");
    fflush(stderr);
    exit(2);
  }

  return tmp;
}

/* --------------------------------------------------------------------- */

// El free() para nuestro malloc() fancy
void xfree(void *ptr) {
  free(ptr);
}

/* --------------------------------------------------------------------- */
/* --------------------------------------------------------------------- */

// Alocar/copiar una String en memoria
char *sdup(const char *s) {

  char *res = xmalloc(strlen(s)+1);

  strcpy(res,s);

  return res;
}

/* --------------------------------------------------------------------- */
/* --------------------------------------------------------------------- */

