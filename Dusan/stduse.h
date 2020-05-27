/**
 * @Author: Dušan Kolář
 * @Year:   2003-2018
 * Copyright (c) 2018
 * Licence: GLP 3.0
 */

#ifndef ___STDUSE_H___
#define ___STDUSE_H___

void exitOnError(void);

void *xmalloc(unsigned short size);
void xfree(void *ptr);

void setFilename(const char *str);
void prError(unsigned short lineno, char *format, ...);
void prWarning(unsigned short lineno, char *format, ...);
int wasError(void);

char *sdup(const char *s);

#endif
