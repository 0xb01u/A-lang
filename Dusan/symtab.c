/**
 * @Author: Dušan Kolář
 * @Year:   2003-2018
 * Copyright (c) 2018
 * Licence: GLP 3.0
 */

#include "symtab.h"
#include "stduse.h"

#include <stdlib.h>
#include <string.h>

extern int yylineno;

// El tipo symtab_t es un alias para la estructura symtab
// symtab es una lista doblemente enlazada con pares (nombre, valor)
typedef struct symtab {
    char   *name;   // Nombre de la variable
    double  value;  // Valor de la variable
    struct symtab *left, *right;    // Variables a la "izquierda/derecha" en la tabla
                                    // (En realidad es un árbol, así que serían sus hijos.
                                    //  La cosa es que indican orden)
} symtab_t;

static symtab_t *root = NULL;   // Asignamos una "raíz" (entrada inicial) a la tabla

static char *wName;
static double wValue;

// La tabla está "ordenada alfabéticamente" (en realidad no, porque es un árbol, pero casi)

// ----------------------------------------------------------------------------

// Buscar entrada con respecto a la entrada nd
// (para modificarla)
static void inmod(symtab_t *nd) {
    int res = strcmp(wName,nd->name);
    // Si se encuentra una entrada para ese nombre
    if (res == 0) { // the same string // Único comentario de Dusan lol
        // Se modifica el valor de la entrada y ya
        nd->value = wValue;
        return;
    }
    // Si lo que se busca es anterior a donde estamos,
    // se busca por la izquierda
    if (res < 0) {
        // Si hay entrada a la izquierda
        if (nd->left != NULL) {
            // Se busca con respecto a la entrada a la izquierda
            inmod(nd->left);
        // Si no hay entrada a la izquierda
        } else {
            // Se aloca espacio para una entrada
            symtab_t *newr = xmalloc(sizeof(symtab_t));
            // Se asigna a la entrada el nombre de la variable y su valor
            newr->value = wValue;
            newr->name  = sdup(wName);
            // Se indica que no tiene entradas "vecinas"
            newr->left  = NULL;
            newr->right = NULL;
            // Se añade esta entrada como vecina a la izquierda
            // de la entrada en la que estábamos buscando
            nd->left    = newr;
        }
    // Si lo que se busca es posterior a donde estamos,
    // se busca por la derecha
    } else {
        // Si hay entrada a la derecha
        if (nd->right != NULL) {
            // Se busca con respecto a la anetrada a la derecha
            inmod(nd->right);
        // Si no hay entrada a la derecha
        } else {
            // Se aloca espacio para una entrada
            symtab_t *newr = xmalloc(sizeof(symtab_t));
            // Se asigna a la entrada el nombre de la variable y su valor
            newr->value = wValue;
            newr->name  = sdup(wName);
            // Se indica que no tiene entradas "vecinas"
            newr->left  = NULL;
            newr->right = NULL;
            // Se añade esta entrada como vecina a la derecha
            // de la entrada en la que estábamos buscando
            nd->right   = newr;
        }
    }
}

// ----------------------------------------------------------------------------

// Insertar o modificar una entrada
void insertModify(char *s, double val) {
    // Si no hay entradas
    if (root == NULL) {
        // Se aloca espacio para una entrada
        root = xmalloc(sizeof(symtab_t));
        // Se añade el valor y nombre de la entrada
        root->value = val;
        root->name  = sdup(s);
        // Se indica que no tiene entradas "vecinas"
        root->left = NULL;
        root->right = NULL;
        return;
    }
    // Si hay alguna entrada
    // Se copian los valores
    wName = s;
    wValue = val;
    // Se intenta buscar empezando en el root
    inmod(root);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

// Buscar entrada con respecto a la entrada nd
// (para obtener su valor)
static double get(symtab_t *nd) {
    // Si hemos llegado a un nodo que no existe, pues error
    if (nd == NULL) {
        prError(yylineno,"Undefined variable %s!\n",wName);
    }
    
    // Comparamos con la entrada actual
    int res = strcmp(wName,nd->name);
    // Si es igual, devolvemos el valor
    if (res == 0) { // the same string // Único comentario de Dusan lol x2
        return nd->value;
    }
    
    // Si es anterior/posterior, nos movemos
    if (res < 0) {
        return get(nd->left);
    } else {
        return get(nd->right);
    }
}

// ----------------------------------------------------------------------------

// Leer la variable s
double read(char *s) {
    // Pues se copia el valor
    wName = s;
    // Y se busca con respecto a la raíz
    return get(root);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// EOF
