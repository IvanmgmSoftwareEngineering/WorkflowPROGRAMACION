/* TÍTULO: prueba.c
 * AUTOR:  Iván Martín Gómez
 * FECHA:  Thu Nov 26 21:51:28 CET 2020
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
//LIBRERÍAS ESTANDAR DE C (entre < >). YA PREDEFINIDAS EN EL SISTEMA. El compilador toma el código que hay en stdio.h y lo pega en el Fichero.c donde hayamos hecho el '#include'
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>// para Llamada al Sistema fork()
#include <signal.h>// para Llamada al Sistema signal()
#include <ctype.h>// para Función de Biblioteca toupper(); pasa un char de lower case to upper case
#include <pthread.h>// Para utilizar la API Pthread útil para la Creación y Sincronización de Hebras. Para Compilar haciendo uso de la Librería ptherad.h se debe incluir "-lpthread" en la compilación (Ej: gcc fila.c -pthread -o programa)
#include <semaphore.h>// Para el uso de Semáforos

//LIBRERÍAS PROPIAS (entre " ")
//#include "lireria.h"
//Cuando queremos agregar una Librería con extension .a utilizar manual INSTRUCCIONES_ECLIPSE.rtf situado en el Directorio WORKFLOWProgramacion


//ALGUNAS SECCIONES DEL MANUAL QUE ES CONVENIENTE SABER:
// Para consultar el Manual hacer: man 
//  	Sección (1)Mandatos,
//	Sección (2)Llamadas al Sistema,
//	Sección (3)Funciones de Biblioteca


// CABECERAS FUNCIONES UTILIZADAS DENTRO DE MAIN
	// CABECERA FUNCION 1
		char *nombre_funcion_1(char *argumento_in);
	// FIN CABECERA FUNCION 1
	//---------------------------------
	// CABECERA FUNCION 2
		//tipo_out nombreFuncion2(tipo_in1 Arg1_in, tipo_in2 Arg2_in,...,tipo_inn Argn_in);
	// FIN CABECERA FUNCION 2
	//---------------------------------
	// CABECERA FUNCION 3: Manejo señales Procesos
		//void manejador_padre(int signal);
	// FIN CABECERA FUNCION 3
	//---------------------------------
	// CABECERA FUNCION 4: Manejo señales Procesos
		//void manejador_hijo_1(int signal);
	// FIN CABECERA FUNCION 4
	//---------------------------------
	// CABECERA FUNCION 5: Manejo señales Procesos
		//void manejador_hijo_2(int signal);
	// FIN CABECERA FUNCION 5
	//---------------------------------
	// CABECERA FUNCION 6: Manejo Hebra
		//void hebra1(tipo_in1 Arg1_in, tipo_in2 Arg2_in, ..., tipo_inn Argn_in);
	// FIN CABECERA FUNCION 5
	//---------------------------------
	// CABECERA FUNCION N
		//tipo_out nombreFuncionN(tipo_in1 Arg1_in, tipo_in2 Arg2_in,..., tipo_inn Argn_in);

	// FIN CABECERA FUNCION N
// FIN CABECERAS FUNCIONES UTILIZADAS DENTRO DE MAIN
//---------------------------------

// ZONA DECLARACION VARIABLE GLOBALES (fuera de la función main())
	 //nota: La función main() aún siendo una función especial por ser la función de entrada y de salida del programa,
         //	 es una función más, por lo que las variables declaradas dentro de la función main() serán variables  
	 // 	 locales a la función main(), es decir, no serán accesibles desde funciones externas a la función main().
	 // 	 Si queremos tener acceso a una variable desde cualquier función, debemos declararla aquí.
	 //	 Este comentario es útil para:
	 //		- Funciones manejadoras de señales; utilizan pid_fork
	 //	 	- Funciones que devuelven una variable de tipo apuntador; se debe hacer copia antes de hacer return



pid_t *vector_PIDs; // Variable de tipo vector, Global para almacenar lo que devuelven las Llamadas al Sistema fork(2). Esto nos permitirá conocer los PIDs entre Procesos que son Hermanos. Se pone aquí fuera porque esta variable para que pueda ser consultada desde las funciones manejadoras() de señales que están fuera del main()
pid_t pid1_fork; // Variable global para tratar lo que devuelve la Llamada al Sistema fork(2). Se pone aquí fuera porque esta variable para que pueda ser consultada desde las funciones manejadoras() de señales que están fuera del main()
pid_t pid2_fork; // Variable global para tratar lo que devuelve la Llamada al Sistema fork(2). Se pone aquí fuera porque esta variable para que pueda ser consultada desde las funciones manejadoras() de señales que están fuera del main()
pid_t pid3_fork; // Variable global para tratar lo que devuelve la Llamada al Sistema fork(2). Se pone aquí fuera porque esta variable para que pueda ser consultada desde las funciones manejadoras() de señales que están fuera del main()

int fd_tuberia_del_padre_al_hijo[2]; // GLOBAL PORQUE SE GESTIONA EN función fuera del main() (manejadora de señales)

int fd_tuberia_del_hijo_al_padre[2]; // GLOBAL PORQUE SE GESTIONA EN función fuera del main() (manejadora de señales)

sem_t semaforo;// Variable de tipo Semáforo declarada de forma Global para que puede ser vista desde funciones externas que utiliza la hebra para su ejecución. Típicamente esta función externa que ejecuta la hebra llama a las funciones sem_wait(&semaforo) y sem_post(&semaforo) de la librería semaphore.h
pthread_mutex_t mutex; // Variable de tipo MUTEX(Mutual Exclusion)=SEMÁFORO BINARIO
pthread_cond_t cond_mutex; //Variable de tipo CONDICIÓN de un MUTEX

// FIN ZONA DECLARACION VARIABLE GLOBALES
//---------------------------------
// EMPIEZA FUNCION MAIN
int main (int argc, char *argv[], char *envp[]){

	//Notas :
	//	Nota 1: **argv=*argv[]=argv[][]
	// 	Nota 2: true=1, false =0
	//	Nota 3: NULL es una constante. var=NULL ==> if(!var) ==>0
	//  	Nota 4: Violaciones de Segmento típicas: una Violación de Segmento se produce cuando intentamos acceder a una zona de memoria a la que no tenemos derecho 
	//						 Tipo 1: intentar acceder a un Apuntador que esta apuntando a NULL
	//						 Tipo 2: intentar indexar un vector o array en una posición que no existe
	//  	Nota 5: Toda variable se deben DECLARAR y INICIALIZAR
	//		Destacamos la Declaración e Inicialización de las variables de tipo Apuntador (el uso de los Apuntadores es una de las característica más relevantes que distingue al Lenguaje de Programación C de otros Lenguajes de Programación de más alto nivel
	//				Declaración: (char *linea ó char linea[]) (Vector) ó (char **lista_lineas ó char lista_lineas[][]) (Matriz) 
	//				             (int *numeros ó int numeros[]) (Vector) ó (int **lista_numeros ó int lista_numeros[][]) (Matriz)
        //			                Inicialización: (3 posibles formas):
	//					- FORMA 1: de forma explícita. linea =['a' 'b' 'c' 'd'] 
	//					- FORMA 2: Asignación Memoria Estática. char *linea [1024]. Esta forma de hacerlo implica hacer la Inicialización en la misma línea que la Declaración 
	//					- FORMA 3: Asignación Memoria Estática. Mediante uso de malloc(3)
	//					  (nota): Cuando asignamos Memoria, ya sea de forma Estática o de forma Dinámica, estamos Inicializando la variable de tipo apuntador, esto se debe que en ese momento la variable apuntador empieza apuntar a una zona de memoria, hasta el momento de la Inicialización, la variable de tipo Apuntador no apuntaba a ninguna parte.

	//	Nota 6: No confundir Reserva de Memoria Dinámica con Reserva de Memoria Estática.

	//		--> Reserva de Memoria ESTÁTICA: se utiliza cuando SI conocemos el tamaño de memoria que vamos a reservar en tiempo de COMPILACIÓN
  //						Ejemplo: *char[argc] No es una asignación de Memoria Estática ya que el valor de argh no lo conocemos en tiempo de Compilación. Aunque C permita hacerlo esto está mal.
	//		--> Reserva de Memoria DINÁMICA: se utiliza cuando NO conocemos el tamaño de memoria que vamos a reservar en tiempo de COMPILACIÓN, pero SI lo conocemos en tiempo de EJECUCIÓN



	//	Nota 7:

		printf("------------------------------------------------------\n");
printf("EMPIEZA EL PROGRAMA\n");

// ZONA DE DECLARACION DE VARIABLES LOCALES FUNCIÓN MAIN()
		static int variable_statica = 0; // Imprescindible hacer la Inicialización en la misma línea que la para que la
						 // variable se comporte como 'estática', si hacemos la Inicialización después de la Declaración,
						 // entonces la variable se comportará como una variable que no es de tipo estática.
						 // Esto se debe a que al hacerlo todo en una misma línea la Inicialización sólo se realiza una vez,
						 // el resto de veces que volvamos a pasar por la mismo zona del código,
						 // no se tendrá en cuenta la Inicialización. Las variables de 
						 // tipo 'estáticas' tienen la característica que conservan su valor, es decir,
						 // si la variable está Declarada e Inicializada dentro de una función la que llamamos en repetidas ocasiones,
						 // la variable 'estática conservará su último valor. 
						 // Si esto mismo lo hiciésemos con una variable 'no estática', 
						 // entonces la Inicialización se tendría en cuenta cada vez que llamamos a la función,
						 // por lo que perderíamos el último valor.
		int i;
		char buf[1024]; // Esta variable de tipo apuntador está Declarada e Inicializada Mediante la Técnica de Reserva de Memoria Estática.
		char **lineasTexto;// Esta variable de tipo apuntador sólo esta Declarada, pero no está Inicializada
		char lista[100]; // Si quiero un array donde almacenar char's, debo inicializar así lista para después poder indexarla
		pid_t pid_fork;// Lo que devuelve la Llamada al Sistema fork() no es un int (aunque lo parezca), si no una variable de tipo pid_t. Lo que tiene de especial la variable de tipo pid_t, es que la variable pid_t cambia su valor dependiendo de cuál sea el Proceso desde el que la estamos consultando
		pthread_t TID;// Thread Identifier único de la Hebra


// FIN ZONA DE DECLARACION DE VARIABLES LOCALES FUNCIÓN MAIN()
//------------------------------------
// ZONA DE PARSEO (Parsear Mandato = Tokenizar Mandato)

//		- Un token es un elemento atómico

//		- Tokenizar consiste en dividir un String en
//		  varios Tokens separados por un delimitador

//		- Un tokens debe cumplir con una Gramática y para comprobar que
//		  un Token pertenece a un cierto Lenguaje es necesario utilizar un
//		  analizador Léxico (¿Quizás relacionado con AFD y AFN: Asignatura LenguajesFormales)?)

//		- Hay una Función de Biblioteca strtok(3) que es muy útil para tokenizar.
//			char *str;
//			char *token;
//			str = PATH; PATH=string_1:string_2:...:string_n
//			token=strtok(str,:);  Almacena en 'token' string_1
//			token=strtok(NULL,:); Almacena en 'token' string_2. Lo que pasa aquí es que 
//					       el Indicador de Posición se ha quedado en el primer ':' después 
//					       de llamar por primera vez a strtok(), y con NULL le indicamos
//					       que se fije donde está apuntando el Indicador de Posición.

//			if(token==NULL);       Si token=NULL significa que hemos llegado al final de lo que contiene str, es decir,
//					       al final de lo que contiene la variable PATH. No confundir este
//					       NULL que devuelve la Función strtok(), con el NULL de antes que le pasamos como
//					       Argumento de entrada a la función strtok().



		printf("------------------------------------------------------\n");
		printf("Tokenizamos el Mandato:\n");
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
			/*printf("usage: %s Argumento1 [Argumento2] ... [Argumento_n-1 [Argumento_n]]\n",argv[0]);
			return 1;*/
		}
		if(argc==2){ //Hemos recibido 1 Argumento:
			/*printf("Error %s: explicación error: %s\n",argv[0],argv[1]);
			return 2;	*/
		}
		if(argc==3){//Hemos recibido 2 Argumentos:
			/*printf("Error %s: explicación error: %s\n",argv[0],argv[1]);
			return 3;	*/
		}
		if(argc > 3){//Hemos recibido más de 3 Argumentos:

		}

// FIN DE CONTROL ERRORES ARGUMENTOS
//---------------------------------
// EMPIEZA FUNCIONALIDAD PROGRAMA

		//Reserva Memoria Dinámica = Inicializamos la variable de tipo apuntador mediante la Técnica de Reserva Memoria Dinámica
		lineasTexto = (char**)malloc(N*sizeof(char*)); //N es un dato que recibimos como Argumento en *argv[].
		//Control Errores Reserva Memoria Dinámica
		if (lineasTexto == NULL) {
			fprintf(stderr, "Error reservando memoria dinámica con malloc()\n");
			return -3;
		}
		//Reserva Memoria Dinámica
		while(i<N){
			lineasTexto[i] = (char*)malloc(1024*sizeof(char));//Reserva de Memoria Dinámica para las columnas de cada una de las filas. 1024 coincide con el tamaño de la variable 'buf'. Notar que también podemos hacer una reserva más eficiente haciendo uso de strlen(argv[i]) para i desde 1 hasta argc-1
			//Control Errores Reserva Memoria Dinámica
			if (lineasTexto == NULL) {
				fprintf(stderr, "Error reservando memoria dinámica con malloc()\n");
				return -3;
			}
			i++;
		}
		//Ahora ya podemos tratar a lineasTexto como un Array de N filas por 1024 columnas

		//Incluir estructura básica para leer de teclado
		i=0;
		while(fgets(buf, 1024, stdin) != NULL){
			if(i<=N){
				strcpy(lineasTexto[i],buf);
				i++;
			}
		}

		//Liberar Memoria Dinámica
		for(z=0;z<N;z++){
			free(lineasTexto[z]);
		}
		free(lineasTexto); //Liberamos Memoria Dinámica
		//Apuntamos a la primera posición de stdin rewind()
		   rewind(stdin); // Colocamos el indicador de posición de fichero al principio del Fichero stdin


		//ZONA DE PROCESOS

			//CONCEPTOS PREVIOS:


//------------>Creación de nuevos Procesos con la Llamada al Sistema fork(2)

				//nota 1): Cuando hacemos un fork() se crea un Proceso Hijo (Copia exacta del código del Padre)

				//nota 2): No confundir lo que devuelve la Llamada al Sistema fork() con el pid (Proccess Identifier -Único- para cada proceso) de un 
				//	  Proceso, aunque típicamente se tiende a llamar a la variable de tipo pid_t, que devuelve el fork(), 'pid', NO ES LO MISMO,
				//	  el pid único para cada Proceso se consulta utilizando la Llamada al Sistema getpid().  Por todo lo dicho anteriormente, 
				//        no llamo a lo que devuelve la Llamada al Sistema fork() 'pid', si no que lo llamo 'pid_fork'

				//nota 3): 


//------------>Recepción y manejo de señales con Función de Biblioteca signal(3)

				//nota 1): La Función de Biblioteca signal(3) se llama desde el código del Proceso que recibe la señal.

				//nota 2): Tiene un sintaxis 'fea': Ejemplo: signal(SIGINT,manejador). La función manejador recibe como Argumento de entrada un 'int' pero aquí 
				//	  no se especifica. Lo que esta pasando internamente es que la Función de Biblioteca signal(3) le está pasando
				//	  como Argumento la señal SIGINT (que es un entero) a la función, implementada por nosotros, void manejador(int signal)

				//nota 3): Hay dos formas de gestionar las señales:

				//	  -Forma 1: Tenemos una única función 'manejador()' que se encarga de gestionar las señales recibidas en el Padre y en el hijo
				//	  	   -Esta forma de hacerlo implica tener un código en la Función manejador() que nos permita saber si estamos en el Padre o en el Hijo
				//	  	   -Esta forma de hacerlo implica también que signal deberá hacerse desde una zona del código compartida por padre e hijo

				//	  -Forma 2:(ESTA ES LA MEJOR FORMA) Tenemos dos funciones manejadoras: manejador_padre() y manejador_hijo()
				//	  	   que se encarga de gestionar las señales recibidas en el Padre y en el hijo
				//	  	   -Esta forma de hacerlo implica que habrá dos signal: uno para escuchar las señales que recibe el Padre y
				//	   	    otro signal para escuchar las señales que recibe el Hijo

				//nota 4): Algunas señales útiles 

				// 	-Señal SIGINT: Ctrl +C
				// 	-Señal SIGUSR1: Para uso del Programador
				// 	-Señal SIGUSR2: Para uso del Programador

				//nota 5): Ctrl+D no es una señal. Esta secuencia de teclas es utilizada para informar de final de fichero. Ejemplo: si estamos leyendo líneas por teclado en la Entrada Estándar mediante la Función de Biblioteca fgets(3) y queremos que fgets devuelva NULL, pulsando ctrl+d lo conseguimos. 

				//nota 6): Hay dos 'Macros' útiles para la gestión de señales:  

				//	  WIFEXITED (status): distinto de cero si el hijo ha terminado

				//	  WESISTATUS (status): nos permite conocer el valor que ha devuelto el hijo mediante el exit(1). Sólo se puede consultar si WIFEXITED es distinta de cero, es decir, si el hijo ha terminado

				//nota 7): 

				//nota 8): Variable stderr, con un número de error asociado errno y la Función de Biblioteca strerror(3) para gestionar: errores de Mandatos, Llamadas al Sistema y Funciones de biblioteca 

//------------>Envío de señales con Mandato Kill(1)

				//nota 1): No hay que malinterpretar la palabra 'Kill'. Este mandato NO se utiliza para matar un Proceso, se utiliza para enviar una señal desde un Proceso 
				//nota 2): El mandato kill(1) se ejecutará desde el código del Proceso que desea enviar la señal. 

				//nota 3): Será útil la Llamada al Sistema getppid(2) para conseguir el pid único del Proceso Padre

				//nota 4): 

//------------>Creación y uso de Tuberías con Comando de Administración de Sistema pipe(8)

				//nota 1): int fd[2]. Este array de 2 enteros se le pasará cómo argumento a pipe()

				//nota 2): La comunicación de una Tubería (una llamada a pipe()) es unidireccional, es decir, si queremos una comunicación bidireccional, entonces se necesitan dos Tuberías (dos llamadas a pipe())

				//nota 3): TODOS LOS PIPES SE CREAN ANTES DEL fork().

				//nota 4): CUIDADO AHORA!! ESTE CONCEPTO ES ESCURRIDIZO:
				//	  CONCEPTO DE HERENCIA DE PIPES: un pipe creado antes del fork() por un padre, es heredado por su hijo. Entonces, cuando utilizamos fd[0] y fd[1], debemos tener cuidado desde que proceso lo estamos haciendo.

				//	  TUBERÍA 1: envío de datos desde el proceso PADRE --> HIJO

				//	  	fd_tuberia_padre_hijo[1]; El padre escribe aquí, estando en el proceso padre

				//	  	fd_tuberia_padre_hijo[0]; El hijo lee aqui, estando en el proceso hijo

				//	  Debemos cerrar por no usarlos:

				//	  	fd_tuberia_padre_hijo[0]; El Padre sólo Envía (escribe), el padre no Recibe (no lee) ==> estando en el padre hacemos close (fd_tuberia_padre_hijo[0])

				//	  	fd_tuberia_padre_hijo[1]; El Hijo sólo Recibe (lee), el Hijo no Envía (no escribe) ==> estando en el hijo hacemos close (fd_tuberia_padre_hijo[1])

				//	  TUBERÍA 2: envío de datos desde el proceso HIJO --> PADRE

				//	  	fd_tuberia_hijo_padre[1]; El Hijo escribe aquí, estando en el proceso hijo

				//	  	fd_tuberia_hijo_padre[0]; El Padre lee aquí, estando en el proceso padre

				//	  Debemos cerrar por no usarlos:

				//	  	fd_tuberia_hijo_padre[0]; El Hijo sólo Envía (escribe), el hijo no Recibe (no lee) ==> estando en el hijo hacemos close (fd_tuberia_hijo_padre[0])

				//	  	fd_tuberia_hijo_padre[1]; El Padre sólo Recibe (lee), el padre no Envía (no escribe) ==> estando en el padre hacemos close (fd_tuberia_hijo_padre[1])

				//nota 5): Las Tuberías se pueden gestionar de dos formas: 

//				- Forma 1: Mediante el Uso de Descriptores de Fichero 
//					   Utilizaremos Llamadas al sistema (2) cómo: write(), read() y close()

//				- Forma 2: Mediante el Uso de File * 
//					   Utilizaremos Funciones de Biblioteca (3) cómo: fopen(), fputs(), fgets() y fclose()

//			nota 6): La lectura del pipe SI es bloqueante. La escritura del pipe NO es bloquearte

//			nota 7): RECORDAR QUE SIMPRE: hay que cerrar los pipes antes salir del programa. Es algo similar a liberar memoria dinámica.

//			nota 8):


//------------>Matar a un Proceso: Utilizamos Mandato exit(1): esta es la única manera de poner un proceso en estado Terminado (muerto)

			  //			Recordar siempre: "Todo buen padre espera por sus hijos". Esta frase es importante tenerla en cuanta para

			  //					   que ningún hijo se quede en esta 'Zombie' \<defunct\>, es decir, si un padre muere antes que su hijo, el hijo se queda en estado 'Zombie'.

//------------>Diferencias entre: wait(1), waitpid(2), pause(3) y sleep(1) 

			  //	- wait(1): wait(NULL) pone en estado de pausa el proceso que la llama hasta que uno (cualquiera) de sus procesos hijos muere

			  //	- waitpid(2): pone en estado de pausa el proceso que la llama hasta que EL (uno en concreto) proceso hijo muere

			  //	- pause(3): no recibe ningún Argumento de entrada y el Proceso que llama se queda en estado de pausa hasta que recibe una señal

			  //	- sleep(1): recibe cómo Argumento de entrada el tiempo que el Proceso debe quedarse en estado de pausa.


//------------>Ejecución de mandatos mediante execvp(3)


//				nota 0): Cuidado aquí entre el concepto de Tubería que creamos mediante pipe(2) para que dos Procesos se comuniquen y el concepto de pipe '|' que utilizamos para redireccionar la Salida Estándar de un Mandato a la Entrada Estándar de otro mandato.
//				nota 1): Siempre que hagamos uso de execvp(3), deberemos crear un Proceso Hijo, y será el Hijo quien haga uso de execvp(3)
//				nota 2): Para comunicar al Proceso Padre y al Proceso Hijo haremos uso de Tuberias (pipe(2))

//				nota 3): La función de Biblioteca execvp(3), sólo admite UN ÚNICO mandato (junto con sus argumentos) en sus Argumentos. No admite redirecciones ni pipes
//				nota 4): Diferenciar entre Redirecciones '>' ó '<' y Pipe '|'
//					(nota: las redireción de salida tiene la siguiente sintaxis mandato>fichero)
//					(nota: las redireción de entrada tiene la siguiente sintaxis fichero<mandato)
//					(nota: los pipes tienen la siguiente sintaxis mandato1|mandato2|...|mandaton)
//					(nota: Con los pies hay que tener cuidado de que mandatos ponemos en cada posición (Concepto de mandatos de tipo Filtro, concepto de que un mandato reciba información a través de Argumentos o a través de la Entrada Estándar.)
//					(nota: Mezcla Redirecciones y pipes: fichero_in < mandato1|mandato2|...|mandaton > fichero_out) En este caso, únicamente la Entrada Estándar del primer mandato que aparece en la cadena de pipes será redirigida a fichero_in, de igual forma, únicamente la Salida Estándar del último mandato que aparece en la cadena de pipes será redirigida a fichero_out 
//					-Redirecciones:
//							Redireccion de la Entrada Estándar: fichero 0< mandato == fichero < mandato. Por defecto, si no especificamos ninguna redirección de entrada, el mandato tiene asociada la Entrada Estándar con el fichero stdin (TECLADO)  

//							Redireccion de la Salida Estándar: mandato 1> fichero == fichero > mandato. Por defecto, si no especificamos ninguna redirección de salida, el mandato tiene asociada la Salida Estándar con el fichero stdout (PANTALLA)  

	//						Redireccion de la Salida Estándar Error: mandato 2> fichero. Por defecto, si no especificamos ninguna redirección de salida, el mandato tiene asociada la Salida Estándar Error con el fichero stdout (PANTALLA)  


//				nota 5): Cuando dentro de un proceso Hijo hacemos un execvp(3), lo que se hace es sustituir el código del Hijo por el Código del mandato que le hemos pasado como argumento a execvp(3). En caso de todo ir bien y no producirse ningún error en la ejecución del mandato que hemos pasado cómo argumento a execvp(3), se hará un exit(0) lo que hará que el Hijo muera. En caso de que algo haya ido mal en el execvp(3), se almacenará en la variable 'errno' el código de error y se seguirá con la siguiente línea de código que el Hijo tenga a continuación de execvp(3). 


//				nota 6): Por la nota1) Concluimos que execvp(3) deber recibir el mandato LIMPIO (mandato + argumentos, nada más, ni redirecciones ni pipes)
//				nota 7): Por la nota 4), si queremos ejecutar una línea que contenga Redirecciones debemos:
//						-Paso 1: Limpiar la línea y quedarnos sólo con el mandato y con sus argumentos Limpios 
//						-Paso 2: Antes de hacer execvp(3) debemos hacer la redirección utilizando dup(2) ó dup2(2)
//						(nota: dup() hace una copia del Descriptor de fichero que recibe como argumento y a la copia le asigna el número de Descriptor de Fichero más bajo disponible.
//						(nota: dup2()=close(1)+dup() hace una copia del Descriptor de fichero que recibe como argumento y a la copia le asigna el número de Descriptor de Fichero que le hemos indicado como Argumento.)
//						(nota: Por qué dup2()=close(1)+dup()? Porque haciendo close(1) dejamos libre el número de Descriptor de fichero 1 y a continuación dup() cómo toma el primer número de Descriptor de Fichero más bajo disponible, sabemos que tomará el 1)


//				nota 8): Por la nota 4), si queremos ejecutar una línea que contenga varios mandatos unidos por pipes '|' (mandato1|mandato2|mandato3|...) debemos:
//						-Paso 1: Parsear la línea: y quedarnos con cada mandato (junto con sus argumentos) de forma individual
//						-Paso 2: Crear Tuberías desde el Padre a cada uno de sus hijos para enviarle un único mandato (junto con sus argumentos)
//						-Paso 3: Creamos tantos Procesos Hijos como mandatos tenga la línea (número de mandatos= número de apariciones símbolo '|' -1). Notar que debemos hacer los fork() de forma controlada para que siempre se hagan en el padre y así asegurarnos que todos los Hijos son Hermanos entre sí.
//						-Paso 4: Para hacer posible la Transferencia de información entre los hermanos, notar que los hermanos deben conocer los números únicos de PID's de sus otros hermanos, para esto nos ayudamos de pid_t *vector_PIDs definido de forma GLOBAL
//						-Paso 5: Debemos crear Tuberías para que los hermanos puedan transferirse información entre ellos.
//						-Paso 6: Haciendo uso de dup() o dup2(). Cada Hijo deberá, antes de utilizar execvp(3), redirigir la Salida Estándar hacia el pipe de escritura 
//						-Paso 7: Haciendo uso de dup() o dup2(). ¿Cada Hijo deberá, antes de utilizar execvp(3), redirigir la Entrada Estándar hacia el pipe de lectura? 
//						-Paso 8: Haciendo uso de dup() o dup2(). Cada Hijo deberá, antes de utilizar execvp(3), redirigir la Salida Estándar Error hacia el pipe de escritura 
//						-Paso 9: Cada hermano ejecutará un único mandato (junto con sus argumentos) haciendo uso de execvp(3)

//------------>Estructura básica para saber en que proceso estamos


				pipe(fd_tuberia_del_padre_al_hijo);
				pipe(fd_tuberia_del_hijo_al_padre);
				pid1_fork=fork();

				if(pid_fork<0){ // ERROR AL HACER EL fork()
					printf("Error: al hacer el fork()");
				}else if(pid_fork==0){// ESTAMOS EN EL HIJO
					signal(SIGUSR1,manejador_hijo);
					signal(SIGUSR2,manejador_hijo);
					close(fd_tuberia_del_padre_al_hijo[1]);// A través de este Pipe con flujo de datos Padre---->>Hijo el hijo solo lee, no escribe. Cierro fd_tph[1].
					close(fd_tuberia_del_hijo_al_padre[0]);// A través de este Pipe con flujo de datos Padre<<----Hijo el hijo solo escribe, no lee. Cierro fd_thp[1].
					pause();//Sale del pause cuando se recibe alguna señal
					exit(0)

				}else if (pid_fork>0){ // ESTAMOS EN EL PADRE
					signal(SIGUSR2,manejador_padre);
					close(fd_tuberia_del_padre_al_hijo[0]);// A través de este Pipe con flujo de datos Padre---->>Hijo el padre solo escribe, no lee. Cierro fd_tph[0].
					close(fd_tuberia_del_hijo_al_padre[1]);// A través de este Pipe con flujo de datos Padre<<----Hijo el padre solo lee, no escribe. Cierro fd_thp[0].
					kill(pid_fork,SIGUSR1);//Enviamos señal al hijo para que despierte
					waitpid(pid_fork,&status,0);
					if(WIFEXITED(status) != 0){//La Macro WIFEXITED de "sintaxis fea" séra distinta de cero si el Hijo ha terminado
						if(WEXITSTATUS(status) != 0){//La Macro WEXITSTATUS de "sintaxis fea" sólo será accesible si el Hijo ha terminado, es decir, si la Macro WIFEXITED es distinta de cero 
							exit_status= WEXITSTATUS(status);
							printf(El Hijo1 no terminó de forma correctan);
						}
					}
					exit(0)//Matamos al Padre
				}


		//ZONA DE HEBRAS ("THREADS"="PROCESOS LIGEROS")
			// -Identificador Único de una Hebra: pthread_t TID (Thread Identifier)
			// -Creación de Hebras:		      pthread_create (pthread_t *tid, pthread_attr_t *attr, void *funcion, void *param)
			// 	-pthread_t *tid == Identificador Único de la Hebra. Hay que usar '&'.
			// 	-pthread_attr_t **attr == NULL
			// 	-void *funcion == Función que se llamará y será el código que ejecute la hebra. Está Función deberá ser implementada fuera de la función main
			// 	-void *param == Argumentos que recibe la función anterior
			// -Sincronización de Hebras:	      pthread_join (pthread_t tid, int **res); La Hebra que llama a esta función, espera a que finalice la hebra llamada (indicada en el primer argumento de esta función), antes de continuar
			// 	-pthread_t tid == Identificador Único de la Hebra
			// 	-int **res == devuelve el valor de retorno ¿de la función que se ejecuta en la hebra llamada?
			// -Finalización de una Hebra: 	      pthread_exit(void *res). Toda Hebra debe finalizar su ejecución con esta función.
			// 	-void *res = ¿¿Por qué aquí es apuntador simple y en join es apuntador doble??
			// -COMENTARIOS SINCRONIZACIÓN:
			// 	(Ejemplo: Productor- Consumidor)
			// 	- CONDICIÓN DE CARRERA: Cuando varios procesos (dos o más) acceden a los mismos datos de forma concurrente y el resultado de la ejecución depende del orden concreto en que se realizasen los accesos, se dice que hay una Condición de Carrera.
			// -SECCIÓN CRÍTICA: Cada hebra tiene una zona de código llamada Sección Crítica donde se accede a variables y recursos compartidos. Cuando una Hebra está ejecutando su Sección Crítica, ninguna otra Hebra puede ejecutar su correspondiente Sección Crítica. El problema de la Sección Crítica consiste en diseñar un Protocolo que permita cooperar. Cualquier diseño del Protocolo para solucionar el Problema de la Sección Crítica debe cumplir:
			// 	-EXCLUSIÓN MUTUA: Si una Hebra está ejecutando su Sección Crítica, ninguna otra Hebra puede ejecutar su correspondiente Sección Critica
			// 	-PROGRESO: Si ninguna Hebra está ejecutando su Sección Crítica, y hay varias Hebras que quieren entrar en su Sección Crítica (están esperando), sólo las Hebras que están en su sección restante (no están esperando para entrar en su Sección Crítica), piden participar en la decisión de quién entra en la Sección Crítica, y esta decisión no puede posponerse indefinidamente.
			// 	-ESPERA LIMITADA: Existe un límite de veces que se permite a otras Hebras entrar en la Sección Crítica después de que otra Hebra lo haya solicitado y antes de que se le conceda.

			// -SEMÁFOROS:
			// 	-Definición: Un Semáforo es una variable entera a la que solamente se accede mediante dos operaciones Atómicas: wait y signal 
			//	-Declaración Global Semáforo: 		sem_t sem
			//	-Inicialización de un Semáforo : 	sem_init (sem_t *sem, int shared, int valor)
			//		-sem_t *sem== Apuntador a una variable de tipo sem_t (hay que utilizar operador &)
			//		-int shared== 0
			//		-int valor== valor inicial
			//	-Destrucción de un Semáforo: 		int sem_destroy(sem_t *sem)
			//	-Operación Atómica wait: 		sem_wait(sem_t *sem)
			//	-Operación Atómica signal: 		sem_post(sem_t *sem)
			//	-Los Semáforos no son parte del estándar Pthread, pero se pueden utilizar de forma conjunta.
			// 	-Operación Atómica: es una operación que garantiza que se ejecutará sin que ningún otro proceso la interrumpa de forma concurrente (Ejemplo: recordar que cuando un proceso 1 varios printf11(), printf12(),... y otro proceso está ejecutando varios printf21(), printf22(),... de forma concurrente, se alternarán los printf's() de ambos procesos por estarse ejecutando de forma concurrente. Pero lo que si sabemos es que un printf() es una operación atómica ya que hasta que no se ejecute el printf() de un proceso, no podrá imprimirse el printf() de otro proceso. 
			// 	-Las operaciones wait y signal deben ser atómicas, por lo que internamente utilizarán inhibidos de Interrupciones (TestAndSet, Swap, etc)
			// 			wait (S){// La librería semaphore.h tiene una implementación de esta función llamada sem_wait(*sem)
			// 				S--
			// 				if(S<0){
			// 					//añadir el proceso a la lista de bloqueados
			// 					block();
			// 				}
			//			}

			// 			signal(S){// La librería semaphore.h tiene una implementación de esta función llamada sem_post(*sem)
			// 				S++
			// 				if(S<=0){
			// 					//sacar el proceso P de la lista
			// 					wakeup();
			// 				}
			// 			}

			// 	-Dos tipos de Semáforos:
			// 		-SEMÁFORO BINARIO (MUTEX = Mutual Exclusion)
			// 			-Sólo puede tomar los valores 0 o 1
			// 			-Permiten resolver el Problema de la Sección crítica
			// 			-Permiten resolver otros Problemas de Sincronización: Ejemplo: garantizar que un proceso se ejecuta antes que otro d
			// 			-INTERBLOQUEOS: dos o más Hebras están esperando indefinidamente por un evento que sólo puede generar uno de las Hebras blóquelas. Ejemplo: 2 Semáforos S1 y S2 inicializados a 1, y 2 Hebras ejecutando H0 y H1-
			// 			-El MUTEX es un mecanismo liviano ideal para Hebras(Procesos Ligeros)
			// 			-Un MUTEX es Semáforo Binario (0 o 1 valores posibles) sin memoria
			// 				-Cerradura cerrada, llave puesta
			// 				-Abro y cierro y me llevo la llave
			// 			-Dos Acciones Atómicas: mutex_lock(m) y mutex_unlock(m)
			// 				mutex_lock(m)
			// 					//Código Sección Critica
			// 				mutex_unlock(m)
			// 				-No se puede hacer un mutex_unlock(m) sin haber hecho previamente un mutex_lock(m). Esto pasa igual que con Tuberías. Si hacemos un close() de una Tubería de la cual no hemos hecho antes un pipe() es un error que es difícil de depurar.
			// 			-MUTEX CON CONDICIÓN:
			// 				-A veces la Hebra que está en la Sección Crítica, no puede continuar, porque no se cumple cierta CONDICIÓN que sólo podría cambiar otra Hebra desde dentro de la Sección Crítica. Para solucionar esto, es preciso:
			// 					-Liberar temporalmente el MUTEX que protege la Sección Crítica mientras se espera a que la CONDICIÓN se cumpla.
			// 					-Sin abandonar la Sección Crítica
			// 					-De forma Atómica
			// 				-La implementación de un MUTEX (==SEMÁFORO BINARIO) no se hace utilizando funciones de la Librería semaphore.h, si no que se implementa haciendo uso de funciones de la Librería pthread.h
			// 				-Declaración MUTEX: 			pthread_mutex_t mutex
			// 				-Inicialización de un MUTEX:		int pthread_mutex_init(pthread_mutex_t *mutex, pthread_mutexattr_t *attr)
			// 					-pthread_mutex_t *mutex=Apuntador a una variable de tipo pthread_mutex_t (debemos utilizar &)
			// 					-pthread_mutexattr_t *attr= atributos MUTEX
			// 				-Destrucción de una MUTEX:		int pthread_mutex_destroy(pthread_mutex_t *mutex)
			// 				-Operaciones básicas con MUTEX:-
			// 					-Competir por coger el MUTEX:	int pthread_mutex_lock(pthread_mutex_t *mutex)
			// 					-Devolver el MUTEX:		int pthread_mutex_unlock(pthread_mutex_t *mutex)

			// 				-Declaración CONDICIÓN MUTEX: 		pthread_cond_t cond_mutex
			// 				-Inicialización CONDICIÓN de un MUTEX:	int pthread_cond_init(pthread_cond_t *cond_mutex, pthread_condattr_t *attr)
			// 					-pthread_cond_t *cond_mutex= apuntador a una variable de tipo condicion_mutex (se debe utilizar operador &)
			// 					-pthread_condattr_t *attr= atributos condición
			// 				-Destrucción de CONDICIÓN de un MUTEX:	int pthread_cond_destroy(pthread_cond_t *cond_mutex)
			// 				-Operaciones básicas con CONDICIONES de MUTEX
			// 					-Operación 1:Sin salir de la Sección Crítica, libera temporalmente el MUTEX que la protege, para esperar a que se cumpla la CONDICIÓN
			// 						int pthread_cond_wait(pthread_cond_t *cond_mutex, pthread_mutex_t *mutex)
			// 					-Operación 2: Señalar un cambio que permita continuar a una las Hebras que espera en la condición
			// 						int pthread_cond_signal(pthread_cond_t *cond_mutex)
			// 					-Operación 3: Señalar un cambio que permita continuar a todos las Hebras que esperan en la CONDICIÓN
			// 						int pthread_cond_broadcast(pthread_cond_t *cond_mutex)

			// 		-SEMÁFORO CONTADOR
			// 			-Pueden tomar valores dentro de un rango
			// 			-Se pueden utilizar para resolver el Problema de control de acceso a un determinado recurso formado por un número finito de instancias (recursos limitados) Ejemplo: Control de acceso a un Parking en base a plazas disponibles

			// 			-Los Semáforos Contadores son mecanismos más pesados, más adecuados para Procesos
			// 	-PROBLEMA PRODUCTOR-CONSUMIDOR: Tres semáforos compartidos por las dos Hebras (Hp=HebraProductor y Hc=HebraConsumidor), S1(Semáforo 1: llamado 'sección' inicializado a 1), S2(Semáforo 2: llamado 'full' inicializado a 0) y S3(Semáforo 3: llamado 'empty' inicializado a N

//---------------------------------
}// FIN FUNCIÓN MAIN
//---------------------------------
// IMPLEMENTACIONES FUNCIONES CUYAS CABECERAS ESTAN DEFINIDAS AL PRINCIPIO DE ESTE DOCUMENTO Y QUE SON UTILIZADAS DENTRO DE MAIN


	//IMPLEMENTACION FUNCION 1
	char * nombre_funcion_1(char *cmd){
		// ZONA DE DECLARACION DE VARIABLES LOCALES A LA FUNCIÓN 1
			//int i;
			char *buf;
		// FIN ZONA DE DECLARACION DE VARIABLES LOCALES A LA FUNCIÓN 1
		//---------------------------------
		// ZONA DE CONTROL ERRORES ARGUMENTOS

		// FIN DE CONTROL ERRORES ARGUMENTOS
		//---------------------------------
		// ZONA DE FUNCIONALIDAD FUNCION

		// FIN ZONA DE FUNCIONALIDAD FUNCION

		//IMPORTANTE: 	   C es un Lenguaje que trabaja con apuntadores (bajo nivel). Cuando queremos que una función
		//		   devuelva una variable de tipo apuntador hay dos opciones:
		//		       OPCIÓN 1: Hacer que la función devuelva un char * y usar strdup(3) en el return
		//				 char * funcion(char *Arg1, char *Arg2)
		//				 En está opción será necesario: 
		//					- Devolver mediante el return, una copia de alguna variable Local a la función, mediante el uso de strdup(3), es decir, return strdup(nombre_var_local_funcion_de_tipo_apuntador). Nunca devolver una variable Local.
	
		//		       OPCIÓN 2: void funcion(int *Arg1, int *Arg2). Hacer que la función modifique algunos de sus Argumentos de entrada y sea en ese argumento donde recogemos el resultado de lo que hace la función. Notar que cuando trabajamos con variables de tipo apuntador int, está será la única opción disponible, por no existir ninguna Función de Biblioteca equivalente a strdup(3) para enteros. Por ejemplo, será en Arg1 donde debemos recoger el resultado de los que hace la función.
	
		//IMPORTANTE: 	   No debemos devolver directamente el apuntador *buf, ya que *buf
		// 		   es una variable Local, y las variables locales se eliminan al acabar la función.
		// 		   Haciendo strdup() lo que hacemos es crear una copia
		// 		   de lo que hay donde apunta *buf, es decir, se crea una nueva zona de memoria,
		// 		   y devolvemos la copia. Este problema surge porque C puede trabajar a muy bajo
		// 		   nivel y esto es bueno para algunas cosas pero muy peligrosas para otras, este es
		// 		   un claro ejemplo del cuidado que hay que tener. En otros lenguajes de más alto nivel, al no utilizar apuntadores, 
		// 		   esta copia no es necesaria hacerla.
		return strdup(buf);
	}
	//FIN IMPLEMENTACION FUNCION 1
	//---------------------------------
	//IMPLEMENTACION FUNCION 2
		//tipo_out nombreFuncion2(tipo_in Arg1_in, Arg2_in,..., Argn_in){
			//...
			//return;(si tipo_out =void) ó return algo del mismo tipo_out; (nota: recordar IMPORTANTE)
		//}
	//FIN IMPLEMENTACION FUNCION 2
	//---------------------------------
	//IMPLEMENTACION FUNCION 3
		void manejador_padre(int signal){
			//Señal SIGINT: Ctrl +C
			//Señal SIGUSR1: Para uso del Programador
			//Señal SIGUSR2: Para uso del Programador
			printf("Padre: Recibida señal. %d\n", signal);
			if(signal == SIGINT){


			}else if(signal ==SIGUSR1){


			}else if(signal ==SIGUSR2){


			}
		}
	//FIN IMPLEMENTACION FUNCION 3
	//---------------------------------
	//IMPLEMENTACION FUNCION 4
		void manejador_hijo_1(int signal){
			//Señal SIGINT: Ctrl +C
			//Señal SIGUSR1: Para uso del Programador
			//Señal SIGUSR2: Para uso del Programador
			printf("Hijo_1: Recibida señal. %d\n", signal);
			if(signal == SIGINT){


			}else if(signal ==SIGUSR1){


			}else if(signal ==SIGUSR2){


			}
		}
	//FIN IMPLEMENTACION FUNCION 4
	//---------------------------------
	//IMPLEMENTACION FUNCION 5
		void manejador_hijo_2(int signal){
			//Señal SIGINT: Ctrl +C
			//Señal SIGUSR1: Para uso del Programador
			//Señal SIGUSR2: Para uso del Programador
			printf("Hijo_2: Recibida señal. %d\n", signal);
			if(signal == SIGINT){


			}else if(signal ==SIGUSR1){


			}else if(signal ==SIGUSR2){


			}
		}
	//FIN IMPLEMENTACION FUNCION 5
	//---------------------------------
	//IMPLEMENTACION FUNCION 6
		//void hebra1(tipo_in1 Arg1_in, tipo_in2 Arg2_in, ..., tipo_inn Argn_in){


		//}
	//FIN IMPLEMENTACION FUNCION N
	//---------------------------------
	//IMPLEMENTACION FUNCION N
		//tipo_out nombreFuncionN(tipo_in Arg1_in, Arg2_in,..., Argn_in){
			//...
			//return;(si tipo_out =void) ó return algo del mismo tipo_out; (nota: recordar IMPORTANTE)
		//}
	//FIN IMPLEMENTACION FUNCION N


// FIN IMPLEMENTACIONES FUNCIONES CUYAS CABECERAS ESTAN DEFINIDAS AL PRINCIPIO DE ESTE DOCUMENTO UTILIZADAS DENTRO DE MAIN
