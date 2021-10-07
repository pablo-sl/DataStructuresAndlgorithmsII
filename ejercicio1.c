#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "secuencial.h"
#include "dispersion.h"
#include "indice.h"


int generaIndice(char *ficheroDatos, char *ficheroIndice){
	int total = 0;
	FILE *f1, *f2;
	
	f1 = fopen(ficheroDatos, "rb");
	f2 = fopen(ficheroIndice, "wb");
	tipoIndice i;
	tipoAlumno alumno;
	if (ficheroDatos==NULL)
	{
	   perror("No se puede abrir alumnos.dat");
	   return -1;
	}
	fread(&alumno, sizeof(tipoAlumno), 1, f1);
	while(!feof(f1)){
		if(!feof(f1)){
			
			i.NRR = ++total;
			printf("\t%-3d %-10s %-10s %-10s %-10s %-10s\n", total, alumno.dni, alumno.nombre, alumno.ape1, alumno.ape2, alumno.provincia);
			strcpy(i.dni, alumno.dni);
			fwrite(&i , 1 , sizeof(tipoIndice) , f2 );
			fread(&alumno, sizeof(tipoAlumno), 1, f1);
		}
	}
	fclose(f1);
	fclose(f2);
	return total;



}


int busquedaIndice(char *dni, char *fDatos, char *fIndice, tipoAlumno *alumno){

	
	tipoIndice i;
	FILE *f1, *f2;
	f1 = fopen(fDatos, "rb");
	f2 = fopen(fIndice, "rb");
	fseek(f2, 0, SEEK_SET);
	int numero = 0;
	
	if(fDatos == NULL) return -2;
	if(fIndice == NULL) return -3;
	
		while(!feof(f2)){
		fread(&i, sizeof(tipoIndice), 1, f2);
		//printf("ENCONTRADO");
		if(strcmp(dni, i.dni)==0){
		
		 fseek(f1, 0, SEEK_SET);

		fseek(f1, numero*sizeof(tipoAlumno), SEEK_SET);
		
		fread(alumno, sizeof(tipoAlumno), 1, f1);
		fclose(f1);
		fclose(f2);
		return ++numero;
		}
		
		else numero++;
	}
	
	return -1;



}



tipoAlumno *busquedaHash(FILE *f, char *dni, int *nCubo, int *nCuboDes, int *posReg, int *error){
	tipoAlumno *alumno;
	tipoCubo c;
	int i, j;
	
	if((alumno = (tipoAlumno*) malloc(sizeof(tipoAlumno))) == NULL){
		*error = 4;
		return NULL;
	}
	
	if(f == NULL){
		*error = 2;
		return NULL;
	}
	
	fseek(f, DESPLAZA_HASH(atoi(dni)), SEEK_SET);
	fread(&c, sizeof(tipoCubo), 1, f);
	for(i = 0; i < c.numRegAsignados; ++i){
		if(!strcmp(c.reg[i].dni, dni)){
			*alumno = c.reg[i];
			*nCubo =HASH(atoi(dni)) ;
			*posReg = i;
			*nCuboDes = -1;
			return alumno;
		}
	}
		if(c.numRegAsignados < C || c.numRegAsignados == 0){
			*error = -1;
			return NULL;
		}
		
		fseek(f, INICIO_DESBORDE, SEEK_SET);
		j = 0;
		while (0 < fread(&c, TAM_CUBO, 1, f)){
			for(j = 0; j < c.numRegAsignados; ++j){
				if(!strcmp(c.reg[j].dni, dni)){
					*alumno = c.reg[j];
					*nCubo = HASH(atoi(dni));
					*posReg = j;
					*nCuboDes = CUBOS+j;
					return alumno;
				}
			}
			if(j<CUBOSDESBORDE){
				j++;
			}
		}
	}
		
	
int modificarReg(char *fichero, char *dni, char *provincia){
	
	tipoAlumno *alumno;
	tipoAlumno *aux;
	tipoCubo c;
	int error, posReg, nCubo, nCuboDes;
	int i;
	FILE *fHash;
	
	if(NULL == (fHash =  fopen(fichero, "r+b")))
		return -2;
	alumno = busquedaHash(fHash, dni, &nCubo, &nCuboDes, &posReg, &error);
	
	if(error == -1)
		return error;
	if(error == -2)
		return error;
	if(error == -4)
		return error;
	
	strcpy(alumno->provincia, provincia);
	fseek(fHash, (ftell(fHash)-TAM_CUBO), SEEK_SET);
	fread(&c, TAM_CUBO, 1, fHash);
	c.reg[posReg] = *alumno;
	fseek(fHash, (ftell(fHash)-TAM_CUBO), SEEK_SET);
	fwrite(&c, TAM_CUBO, 1, fHash);
	fclose(fHash);
	
	if(nCuboDes != -1)
		return nCuboDes;
	else
		return nCubo;
	
	
}

	

int main(){
	FILE *f;
	tipoAlumno reg, new;
	char dni[10];
	int total = 0;
	tipoIndice i;

	int numReg=generaIndice("alumnos.dat", "alumnos.idx");
	printf("Hay %d registros\n", numReg);
	
	
	int numBus=busquedaIndice("7152638", "alumnos.dat", "alumnos.idx", &reg);
	
	printf("EL REGISTRO BUSCADO ES EL %d: %s %s %s %s %s\n\n", numBus, reg.dni, reg.nombre, reg.ape1, reg.ape2, reg.provincia);
	
	
	
	printf("\n\n EJERCICIO 2:\n-----------------------------------\n");
	printf("Creo el fichero");
	
	creaHvacio("alumnos.hash");
	int j = creaHash("alumnos.dat", "alumnos.hash");
	
	printf("Numero de desbordados: %d\n", j);
	
	printf("\n\nBuscamos dni 7152638. \n\n");
	FILE *e2 = fopen("alumnos.hash", "rb");
	int nC, nCubDes, pReg, err=0;
	tipoAlumno nuevo = *busquedaHash(e2, "333333", &nC, &nCubDes, &pReg, &err);
	if(&nuevo != NULL){
	printf("EL REGISTRO BUSCADO está en cubo %d (desborde=%d), posicion=%d, error = %d\n", nC, nCubDes, pReg, err );
	printf("EL REGISTRO BUSCADO está en %s %s %s %s %s\n\n", nuevo.dni, nuevo.nombre, nuevo.ape1, nuevo.ape2, nuevo.provincia);
	}
	char prov[11];
	strcpy(prov, "CADIZ");
	printf("Cambios la provincia al registro anterior por %s: \n", prov);
	modificarReg("alumnos.hash", "3333333", prov);
	
	
	return 0;
	
}
