#define CUBOS 40
#define C 5
#define CUBOS_DESBORDE 4
#define CUBOS_DESBORDE2 8
#define CUBOS2 40
/*
typedef struct {
	char nombre [21];
	char DNI [15];
}tipoAlumno;
*/

typedef struct {
	tipoAlumno registros[C];
	int num_registros;
}tipoCubo;


//Insetar registro
// -1 error
//cubo

int insertarReg(FILE *f,tipoAlumno *reg){
	int numCubo;
	int i;
	tipoCubo leido;
	
	
	numCubo= atol(reg->DNI) %CUBOS2; // valor 0-39
	fseek(f,numCubo*sizeof(tipoCubo),SEEK_SET); //coloca puntero en la posicion que le corresponde
	fread(&leido,sizeof(tipoCubo),1,f);

	for(i=0;i<C;i++){
		if(strcmp(leido.registros[i].dni, "")== 0){
			leido.registros[i] = *reg;
			leido.num_registros++;
			fseek(f,-sizeof(tipoCubo),SEEK_CUR);
			fwrite(&leido,sizeof(tipoCubo),1,f);
			return numCubo;
		}
	}
	leido.num_registros++;
	fseek(f,-sizeof(tipoCubo),SEEK_CUR);
	fwrite(&leido,sizeof(tipoCubo),1,f);
	fseek(f,CUBOS2%sizeof(tipoCubo),SEEK_SET);
	
	for(numCubo = CUBOS2;numCUBO<CUBOS2+ CUBOS_ DESBORDE2;numCubo++){
		fread(&leido,sizeof(tipoCubo),1,f);
		for(i=0;i<c;i++){
			if(strcmp(leido.registros[i].DNI, " ") == 0){
				leido.registros[i]= *reg;
				leido.numRegistros++;
				fseek(f,-sizeof(tipoCubo),SEEK_CUR);
				fwrite(&leido,sizeof(tipoCubo),1,f);
				return numCubo;
			}
		}
	}

	// Si continua esta todo lleno 
	return -1;
}


int creaficheroVacio (char *nombre,int numCubos, int numCubosDesborde){

	tipoCubo cubo;
	int i; 
	FILE *f;
	
	f = fopen (nombre, "wb");
	if(f == NULL){
	printf("\nError al abrir el fichero\n");
	return -1;
	}
	
	for(i=0;i<C;i++){
		strcpy(cubo.registros[i].DNI, " ");
	}
	
	cubo.numRegistros = 0;
	
	for(i=0;i<numCubos+numCubosDesborde;i++){
		fwrite(&cubo,sizeof(tipoCubo),1,f);
	}
	fclose(f);
	return 0;
}


int expansionTotal (char *ficheroEntrada, char *ficheroSalida){
	int cont; 
	FILE *fent ;
	FILE *fsal;
	tipoCubo cubo;
	int i;
	int numCubos;
	
	fent = fopen(ficheroEntrada, "rb");
	if(fent == NULL){
	printf("\nError al abrir el fichero de entrada\n");
	return -1;
	}

	if(crearficheroVacio(ficheroSalida, CUBOS2, CUBOS_DESBORDE2)== -1){
		printf("No se puede crear fichero vacio\n ");
		fclose(fent);
		return -1;
	}
	
	fsal = fopen (ficheroSalida, "r+b");
	if(fsal == NULL){
		printf("\nNo se puede crear el fichero de salida\n");
		fclose (fent);
		return -1;
	}
	
	cont = 0;
	
	for(numCubos = 0 ; numCubos < CUBOS+CUBOS_DESBORDE; numCubos++){
		fread ( &cubo , sizeof (tipoCubo),1,fent);
		for(i=0;i<C;i++){
			if(strcmp(cubo.registros[i].DNI, " ")!=0){
				if(insetarReg(fsal,&cubo.registros[i]) >= CUBOS2){
					cont++;
				}
			}
		}
	}

	fclose(fent);
	fclose(fsal);
	return cont;
}
