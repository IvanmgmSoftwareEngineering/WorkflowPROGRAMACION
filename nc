#!/bin/bash

#CONTROL ERRORES ARGUMENTOS DE ENTRADA
	if test $# -eq 0
	then
		echo "Error: por favor indique el nombre del fichero"
		exit 1
	fi

	if test $# -gt 1
	then
		echo "Error: el numero de argumentos es mayor que 1"
		exit 2
	fi

	for i in `ls ./`
	do
		if test "$1.c" = "$i" 
		then
			echo "Error: el fichero ya existe"
			exit 3
		fi
	done
	

#FIN CONTROL ERRORES ARGUMENTOS DE ENTRADA

touch ./$1.c
chmod 700 ./$1.c
echo "/* TÍTULO:" >> ./$1.c
echo " * AUTOR: Iván Martín Gómez" >> ./$1.c
echo " *" >> ./$1.c
echo " * Sintaxis: nombre_programa Argumento1 [Argumento2] ... [Argumento_n-1 [Argumento_n]]" >> ./$1.c
echo " *" >> ./$1.c
echo " * Argumentos Entrada: (los que están entre [] son opcionales)" >> ./$1.c
echo " * 		Argumento 1:    -->" >> ./$1.c
echo " * 		Argumento 2: [] -->" >> ./$1.c
echo " * 		Argumento n: [] -->" >> ./$1.c
echo " *" >> ./$1.c
echo " * Salida:" >> ./$1.c
echo " *" >> ./$1.c
echo " * Errores:" >> ./$1.c
echo " * 		Error 1:" >> ./$1.c
echo " * 		Error 2:" >> ./$1.c
echo " * 		Error n:" >> ./$1.c
echo " *" >> ./$1.c
echo " * Descripción:" >> ./$1.c
echo " *" >> ./$1.c
echo " *" >> ./$1.c
echo " */" >> ./$1.c
echo "//----------------------------------------------------------------------------" >> ./$1.c
echo "#include <stdio.h>" >> ./$1.c
echo "#include <stdlib.h>" >> ./$1.c
echo "#include <string.h>" >> ./$1.c
echo >> ./$1.c
echo >> ./$1.c
echo >> ./$1.c
echo "// EMPIEZA EL PROGRAMA" >> ./$1.c
echo >> ./$1.c
echo "int main (int argc, char *argv[], char *envp[]){" >> ./$1.c
echo >> ./$1.c
echo "		printf(\"EMPIEZA EL PROGRAMA\n\");" >> ./$1.c
echo >> ./$1.c
echo "// ZONA DE DECLARACION DE VARIABLES" >> ./$1.c
echo "		int i;" >> ./$1.c
echo >> ./$1.c
echo >> ./$1.c
echo >> ./$1.c
echo >> ./$1.c
echo >> ./$1.c
echo "// FIN ZONA DE DECLARACION DE VARIABLES" >> ./$1.c
echo "//------------------------------------" >> ./$1.c
echo "// ZONA DE PARSEO" >> ./$1.c
echo "		printf(\"Parseamos el Mandato:\n\");" >> ./$1.c
echo "		printf(\"--> Se han recibido %i Argumento/s:\n\",argc-1);" >> ./$1.c
echo "		for ( i=0; i<argc;i=i+1){" >> ./$1.c
echo "			if(i==0){" >> ./$1.c
echo "				printf(\"---- El nombre del mandato es: %s\n\",argv[0]);" >> ./$1.c
echo "			}else{" >> ./$1.c
echo "				printf(\"---- Argumento %i:             %s\n\",i,argv[i]);" >> ./$1.c
echo "			}" >> ./$1.c
echo "		}" >> ./$1.c
echo "		printf(\"------------------------------------------------------\n\");" >> ./$1.c
echo "		printf(\"\n\");" >> ./$1.c
echo "// FIN ZONA DE PARSEO" >> ./$1.c
echo "//----------------------------------" >> ./$1.c
echo "// ZONA DE CONTROL ERRORES ARGUMENTOS" >> ./$1.c
echo "		if(argc==1){ //Hemos recibido 0 Argumentos:" >> ./$1.c
echo "			/*printf(\"usage: nombre_programa Argumento1 [Argumento2] ... [Argumento_n-1 [Argumento_n]]\");" >> ./$1.c
echo "			return 1;*/" >> ./$1.c
echo "		}" >> ./$1.c
echo "		if(argc==2){ //Hemos recibido 1 Argumento:" >> ./$1.c
echo "			/*printf(\"Error nombre_programa: explicación error: %s\n\",argv[1]);" >> ./$1.c
echo "			return 2;	*/" >> ./$1.c
echo "		}" >> ./$1.c
echo "		if(argc==3){//Hemos recibido 2 Argumentos:" >> ./$1.c
echo "			/*printf(\"Error nombre_programa: explicación error: %s\n\",argv[1]);" >> ./$1.c
echo "			return 3;	*/" >> ./$1.c
echo "		}" >> ./$1.c
echo "		if(argc > 3){//Hemos recibido más de 3 Argumentos:" >> ./$1.c
echo >> ./$1.c
echo "		}" >> ./$1.c
echo >> ./$1.c
echo "// FIN DE CONTROL ERRORES ARGUMENTOS" >> ./$1.c
echo "//---------------------------------" >> ./$1.c
echo "// EMPIEZA FUNCIONALIDAD PROGRAMA" >> ./$1.c
echo >> ./$1.c
echo >> ./$1.c
echo >> ./$1.c
echo >> ./$1.c
echo >> ./$1.c
echo >> ./$1.c
echo >> ./$1.c
echo >> ./$1.c
echo >> ./$1.c
echo "//---------------------------------" >> ./$1.c
echo "}// FIN PROGRAMA" >> ./$1.c