/******************************************************************************

							Online C Compiler.
				Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_TERMINALES 5
#define MAX_NO_TERMINALES 5
#define MAX_LARGO_PRODUCCION 20 // esto es el largo que puede tener como maximo una produccion, tecnicamente no deberia superar los 5 caracteres (no guardamos la flecha ->) porque solamente representamos GF
#define MAX_CANT_PRODUCCIONES 10

int hayQueCrearSeed = 1;

typedef struct gram
{
	char terminales[MAX_TERMINALES];
	char noTerminales[MAX_NO_TERMINALES];
	char producciones[MAX_CANT_PRODUCCIONES][MAX_LARGO_PRODUCCION];
	char axioma;
} gramatica;

// abstraccion de la generacion y obtencion de numero aleatorio, devuelve un nro aleatorio del conjunto [limiteInferior, limiteSuperior)
int obtenerNumeroRandomEntreRango(int li, int ls)
{
	if (hayQueCrearSeed)
	{
		srand(time(NULL));
		hayQueCrearSeed = 0;
	}
	return li + (rand() % (ls - li));
}

int generarTerminales(gramatica *g, char *strTerminales)
{	// hardcodeado
	strcpy(g->terminales, "ab");
	return 0;
}

int generarNoTerminales(gramatica *g, char *strNoTerminales)
{	// hardcodeado
	strcpy(g->noTerminales, "ST");
	return 0;
}

int generarProducciones(gramatica *g, char *strProducciones, char *prodsString)
{	// hardcodeado
	strcpy(g->producciones[0], "SaT");
	strcpy(g->producciones[1], "Ta");
	strcpy(g->producciones[2], "TbT");
	strcpy(prodsString, "S->aT | T->a | T->bT");
	return 0;
}

int generarAxioma(gramatica *g, char *strAxioma)
{
	if (strlen(strAxioma) != 1)
	{
		printf("Error: Axioma invalido: \"%s\"", strAxioma);
		return 1;
	}
	g->axioma = strAxioma[0];
	return 0;
}

int generarGramatica(gramatica *g, char *strNoTerminales, char *strTerminales, char *strProducciones, char *strAxioma)
{
	char *prodsString = malloc(sizeof(char) * (MAX_CANT_PRODUCCIONES * MAX_LARGO_PRODUCCION + (MAX_CANT_PRODUCCIONES * 3 - 1))); // lo ultimo es porque pongo " | " entre cada produccion

	if (generarTerminales(g, strTerminales) ||
	        generarNoTerminales(g, strNoTerminales) ||
	        generarProducciones(g, strProducciones, prodsString) ||
	        generarAxioma(g, strAxioma))
		return 1;

	printf("Gramatica obtenida: \n - No terminales: %s\n - Terminales: %s\n - Producciones: %s\n - Axioma: %c\n\n\n", g->noTerminales, g->terminales, prodsString, g->axioma);
	free(prodsString);
	return 0;
}



void construirPalabra(int posNT, char *palabra, char *parteDerechaProd)
{ // terminado
    // Como siempre son GR, el NT o esta al principio (GRI) o esta al final (GRD)
	// caso axioma
	if (strlen(palabra) == 1) {
		strcpy(palabra, parteDerechaProd);
		printf("Caso axioma, quedo: %s\n", palabra);
	}
	// caso GRI
	else if (posNT == 0) { // Tba
		// concateno la produccion con la palabra sin el NT
		char *buffer = malloc(sizeof(char));
		sprintf(buffer, "%s%s", parteDerechaProd, palabra + 1); // palabra + 1 es para que sacar el NT
		strcpy(palabra, buffer);
		free(buffer);
	}
	// caso GRD
	else {
		char *aux = malloc(sizeof(char));
		strncpy(aux, palabra, strlen(palabra) - 1); // le saco el ultimo caracter a la palabra (siendo este el NT)
		strcat(aux, parteDerechaProd);
		strcpy(palabra, aux);
		free(aux);
	}
}

int quedanNoTerminalParaDerivar(gramatica *g, char *palabra, int *posNT)
{	// terminada
	// recorro la palabra hasta llegar al NT
	for (int i = 0; i < strlen(palabra); i++)
	{
		//printf("Caracter actual: \"%c\"\n", palabra[i]);
		char caracterActual = palabra[i];

		// valido q el caracter sea un No Terminal
		if (strchr(g->noTerminales, caracterActual) != NULL)
		{
			// seteo la posicion del No Terminal
			*posNT = i;
			//printf("Posicion del NT \"%c\": %d \n", caracterActual, *posNT);
			return 1;
		}
		else 
		{ 
		    //printf("El caracter \"%c\" no es un No Terminal\n", caracterActual);
		}
	}
	return 0;
}

void obtenerParteDerechaProduccion(char *parteDerecha, char *produccion) { // terminada
	strncpy(parteDerecha, produccion + 1, 2);
}

int obtenerIndiceProducciones(gramatica *g, char noTerminal, int *produccionesAux)
{ // terminado
	int indice = 0;

	for (int i = 0; g->producciones[i][0] != '\0'; i++)
	{
		// guardar en nuevo array los indices de las producciones del noTerminal
		if (g->producciones[i][0] == noTerminal)
		{
			produccionesAux[indice] = i;
			indice++;
		}
	}
	
	return indice++; 
}

void derivarGramatica(gramatica *g)
{ // terminado
	char *palabra = malloc(sizeof(char));
	int produccionesAux[MAX_CANT_PRODUCCIONES];
	int numRandom;

	palabra[0] = g->axioma;

	int posNT; // posicion del NT de la palabra

	while (quedanNoTerminalParaDerivar(g, palabra, &posNT))
	{
		// obtengo el NT en cuestion
		char noTerminal = palabra[posNT];
		int cantProdUsables = obtenerIndiceProducciones(g, noTerminal, produccionesAux);
		
		numRandom = obtenerNumeroRandomEntreRango(0, cantProdUsables); // harcodeado largo de produccionesAux

		// el largo es 2 xq las opciones son T o NT + T (siendo ambos de 1 unico caracter)
		char parteDerechaProdElegida[2];
		obtenerParteDerechaProduccion(parteDerechaProdElegida, g->producciones[produccionesAux[numRandom]]);
		
		construirPalabra(posNT, palabra, parteDerechaProdElegida);
		printf("Palabra: %s\n", palabra);
	}
	printf("Fin\n");
	free(palabra);
}

// Formato de los argumentos: noTerminales terminales producciones axioma
//                        EJ: (S,T a,b S-\>aT,T-\>a,T-\>bT S)
// Cada argumento se separa con espacios y cada elemento de los mismos con comas. Hay que poner \ antes del > para que no quiera generar un archivo
int main(int argc, char *argv[])
{
	// obtener e interpretar los argumentos, verificando que sea GF - Hecho, pero hardcodeado
	gramatica g;

	if (generarGramatica(&g, argv[1], argv[2], argv[3], argv[4]))
		return 1;

	derivarGramatica(&g);
	
	return 0;
}
