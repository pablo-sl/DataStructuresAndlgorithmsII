#include "dispersion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




int buscaRegD(FILE *fHash, tipoAlumno *reg, char *dni){
		
	tipoCubo posCubo;
	int nCubo;
	int i,j;

	nCubo = atoi(dni) % CUBOS;
	fseek(fHash,nCubo * sizeof (tipoCubo),SEEK_SET);
	fread(&posCubo,sizeof(tipoCubo),1,fHash);
	
	for(i=0;i<posCubo.numRegAsignados;i++){
		if(0 == (strcmp(dni,posCubo.reg[i].dni))){
			*reg = posCubo.reg[i];
			return nCubo;
		}
	}
	fseek(fHash,CUBOS * sizeof(tipoCubo),SEEK_SET);

	for(i=CUBOS;i<CUBOS + CUBOSDESBORDE;i++){
		fread(&posCubo,sizeof(tipoCubo),1,fHash);
		for(j=0;j<C;j++){
			if(strcmp(dni,posCubo.reg[j].dni)==0){
				*reg = posCubo.reg[j];
				return i;						
			}
		}
	}
	return -1;
}



int insertarRegistro(FILE *fHash, tipoAlumno *regAlumno){

	tipoCubo posCubo;
	int nCubo;
	int i;

	nCubo = atoi(regAlumno->dni) % CUBOS;
	fseek(fHash,nCubo * sizeof(tipoCubo),SEEK_SET);
	fread(&posCubo,sizeof(tipoCubo),1,fHash);
	posCubo.numRegAsignados++; 
	fseek(fHash,-sizeof(tipoCubo),SEEK_CUR);

	if(posCubo.numRegAsignados<=C){
		posCubo.reg[(posCubo.numRegAsignados)-1] = *regAlumno;
		fwrite(&posCubo,sizeof(tipoCubo),1,fHash);
		return nCubo;
	}
	else{
		fwrite(&posCubo,sizeof(tipoCubo),1,fHash);
		fseek(fHash,CUBOS * sizeof(tipoCubo),SEEK_SET);
	
		for(i=CUBOS;i<CUBOS+CUBOSDESBORDE;i++){
			fread(&posCubo,sizeof(tipoCubo),1,fHash);
			(posCubo.numRegAsignados)++;
			fseek(fHash,-sizeof(tipoCubo),SEEK_CUR);

			if(posCubo.numRegAsignados<=C){
				posCubo.reg[(posCubo.numRegAsignados)-1] = *regAlumno;
				fwrite(&posCubo,sizeof(tipoCubo),1,fHash);
				return i;
			}
			fwrite(&posCubo,sizeof(tipoCubo),1,fHash);
		}
	}
	return -1;
}



void creaHvacio(char *fichHash)
{ FILE *fHash;
  tipoCubo cubo;
  int j;
  int numCubos =CUBOS+CUBOSDESBORDE;

  memset(&cubo,0,sizeof(cubo));
  fHash = fopen(fichHash,"wb");
  for (j=0;j<numCubos;j++) fwrite(&cubo,sizeof(cubo),1,fHash);
  fclose(fHash);
}



int leeHash(char *fichHash)
{ FILE *f;
  tipoCubo cubo;
  int j,i=0;
  size_t numLee;

   f = fopen(fichHash,"rb");
   rewind(f);
   fread(&cubo,sizeof(cubo),1,f);
   while (!feof(f)){
	for (j=0;j<C;j++) {
        if (j==0)    	printf("Cubo %2d (%2d reg. ASIGNADOS)",i,cubo.numRegAsignados);
        else            printf("\t\t\t");
	    printf("\t%s %s %s %s %s\n",
	    cubo.reg[j].dni,
	    cubo.reg[j].nombre,
	    cubo.reg[j].ape1,
	    cubo.reg[j].ape2,
  	    cubo.reg[j].provincia);
        }
       i++;
       fread(&cubo,sizeof(cubo),1,f);
   }
   fclose(f);
   return i;
}

int creaHash(char *fichEntrada,char *fichHash){

	FILE *entrada;
	FILE *salida;
	tipoAlumno regAlumno;
	tipoCubo posCubo;
	int nCubo;
	int idCubo;
	int i,desbordados=0;

	if((entrada = fopen(fichEntrada, "rb")) == NULL){
		fprintf(stderr, "ERROR: %s\n",fichEntrada);
		return -1;
	}
	if((salida = fopen(fichHash,"rb+")) == NULL){
		fprintf(stderr, "ERROR: %s\n",fichHash);
		return -2;
	}
	
	fread(&regAlumno,sizeof(tipoAlumno),1,entrada);
	while(!feof(entrada)){
		idCubo = insertarRegistro(salida,&regAlumno);
		if(idCubo>=CUBOS){
			desbordados++;
		}
		if(idCubo == -1){
			printf("Ya no entran mas registros en los cubos de desborde\n");
		}
		fread(&regAlumno,sizeof(tipoAlumno),1,entrada);	
	}
	fclose(entrada);
	fclose(salida);
	return desbordados;
}
