#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "secuencial.h"
#include "alumno.h"

int leeSecuencial(char *fichero)
{
	int total = 0;
	FILE *f1;
	f1 = fopen(fichero, "rb");
	tipoAlumno alumno;
	if (fichero==NULL)
	{
	   perror("No se puede abrir alumnos.dat");
	   return -1;
	}
	while(!feof(f1)){
		fread(&alumno, sizeof(tipoAlumno), 1, f1);
		total++;
		printf("\t%d %s %s %s %s %s\n\n", total, alumno.dni, alumno.nombre, alumno.ape1, alumno.ape2, alumno.provincia);
	}
	fclose(f1);
	return total;
}
int buscaReg(FILE *fSecuencial, tipoAlumno *reg,char *dni)
{
	tipoAlumno alumno;
	int numero = 0;
	if(fSecuencial == NULL) return -1;
		while(!feof(fSecuencial)){
		fread(&alumno, sizeof(tipoAlumno), 1, fSecuencial);
		if(strcmp(dni, alumno.dni)==0){
		strcpy(reg->dni, alumno.dni);
		strcpy(reg->nombre, alumno.nombre);
		strcpy(reg->ape1, alumno.ape1);
		strcpy(reg->ape2, alumno.ape2);
		strcpy(reg->provincia, alumno.provincia);
		return ++numero;
		}
		
		else numero++;
	}
	
	return -2;
	
}
int insertaReg(char *fSecuencial, tipoAlumno *reg)
{
	int total = 0;
	FILE *f1;
	f1 = fopen(fSecuencial, "ab");
	tipoAlumno alumno;
	if (fSecuencial==NULL)
	{
	   perror("No se puede abrir alumnos.dat");
	   return -1;
	}
	total = ftell(f1)/sizeof(tipoAlumno)+1;
	printf("AÑADO EL REGISTRO.\n");
	fwrite(reg , 1 , sizeof(tipoAlumno) , f1 );
	
	fclose(f1);
	return total;
}

