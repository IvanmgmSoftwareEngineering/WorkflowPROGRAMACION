/* TÍTULO:
 * AUTOR: Iván Martín Gómez
 *
 * Sintaxis: nombre_programa Argumento1 [Argumento2] ... [Argumento_n-1 [Argumento_n]]
 *
 * Argumentos Entrada: (los que están entre [] son opcionales)
 * 		Argumento 1:    -->
 * 		Argumento 2: [] -->
 * 		Argumento n: [] -->
 *
 * Salida:
 *
 * Errores:
 * 		Error 1:
 * 		Error 2:
 * 		Error n:
 *
 * Descripción:
 *
 *
 */
//----------------------------------------------------------------------------
//LIBRERÍAS SISTEMA (entre < >)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//LIBRERÍAS PROPIAS (entre " ")
//#include "lireria.h"



// EMPIEZA EL PROGRAMA

int main (int argc, char *argv[], char *envp[]){

		printf("EMPIEZA EL PROGRAMA\n");

// ZONA DE DECLARACION DE VARIABLES
		int i;





// FIN ZONA DE DECLARACION DE VARIABLES
//------------------------------------
// ZONA DE PARSEO
		printf("Parseamos el Mandato:\n");
		printf("--> Se han recibido %i Argumento/s:\n",argc-1);
		for ( i=0; i<argc;i=i+1){
			if(i==0){
				printf("---- El nombre del mandato es: %s\n",argv[0]);
			}else{
				printf("---- Argumento %i:             %s\n",i,argv[i]);
			}
		}
		printf("------------------------------------------------------\n");
		printf("\n");
// FIN ZONA DE PARSEO
//----------------------------------
// ZONA DE CONTROL ERRORES ARGUMENTOS
		if(argc==1){ //Hemos recibido 0 Argumentos:
			/*printf("usage: nombre_programa Argumento1 [Argumento2] ... [Argumento_n-1 [Argumento_n]]");
			return 1;*/
		}
		if(argc==2){ //Hemos recibido 1 Argumento:
			/*printf("Error nombre_programa: explicación error: %s\n",argv[1]);
			return 2;	*/
		}
		if(argc==3){//Hemos recibido 2 Argumentos:
			/*printf("Error nombre_programa: explicación error: %s\n",argv[1]);
			return 3;	*/
		}
		if(argc > 3){//Hemos recibido más de 3 Argumentos:

		}

// FIN DE CONTROL ERRORES ARGUMENTOS
//---------------------------------
// EMPIEZA FUNCIONALIDAD PROGRAMA









//---------------------------------
}// FIN PROGRAMA
