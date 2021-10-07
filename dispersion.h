#include <stdio.h>
#define C 5	                // capacidad del cubo
#define CUBOS 20           // Número de cubos en el area prima
#define CUBOSDESBORDE  4   // Número de cubos area de desborde

#define HASH(elemento) ((elemento) % CUBOS)
#define TAM_CUBO (sizeof(tipoCubo))
#define INICIO_DESBORDE (CUBOS*sizeof(tipoCubo))
#define DESPLAZA_HASH(elemento) (HASH(elemento)*sizeof(tipoCubo))


#include "alumno.h"

typedef struct {
	tipoAlumno reg[C];
	int cuboDES;      // De momento no la vamos a utilizar
	int numRegAsignados;
	}tipoCubo;

// funciones proporcionadas
void creaHvacio(char *fichHash);
int leeHash(char *fichHash);
// funciones a codificar
int creaHash(char *fichEntrada,char *fichHash);
int buscaRegD(FILE *fHash, tipoAlumno *reg,char *dni);
int insertarRegistro(FILE *f, tipoAlumno *regAlumno);

