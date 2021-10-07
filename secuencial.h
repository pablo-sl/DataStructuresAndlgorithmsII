#ifdef __SECUENCIAL_H
#define __SECUENCIAL_H


#include <stdio.h>
#include "alumno.h"

int leeSecuencial(char *fichero);
int buscaReg(FILE *fSecuencial, tipoAlumno *reg,char *dni);
int insertaReg(char *fSecuencial, tipoAlumno *reg);
#endif
