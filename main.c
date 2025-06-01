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
{ // hardcodeado
	strcpy(g->terminales, "ab");
	return 0;
}

int generarNoTerminales(gramatica *g, char *strNoTerminales)
{ // hardcodeado
	strcpy(g->noTerminales, "ST");
	return 0;
}

int generarProducciones(gramatica *g, char *strProducciones, char *prodsString)
{ // hardcodeado
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

void construirPalabra(int posNT, char *palabra, char *palabraAux)
{
	int aux = posNT;
	for (int i = 1; i < strlen(palabraAux); i++)
	{
		palabra[aux] = palabraAux[i];
		aux++;
	}
}

int quedanNoTerminalParaDerivar(gramatica *g, char *palabra, int *posNT)
{ // terminada
	// recorro la palabra hasta llegar al NT
	for (int i = 0; i < strlen(palabra); i++)
	{
		printf("Caracter actual: \"%c\"\n", palabra[i]);
		char caracterActual = palabra[i];

		// valido q el caracter sea un No Terminal
		if (strchr(g->noTerminales, caracterActual) != NULL)
		{
			// seteo la posicion del No Terminal
			*posNT = i;
			printf("Posicion del NT \"%c\": %d \n", caracterActual, *posNT);
			return 1;
		}
		else
			printf("El caracter \"%c\" no es un No Terminal\n", caracterActual);
	}
	return 0;
}

void obtenerIndiceProducciones(gramatica *g, char *noTerminal, char *produccionesAux)
{
	int indice = 0;

	for (int i = 0; i < 3; i++) // harcodeado largo de producciones
	{
		// guardar en nuevo array los indices de las producciones del noTerminal
		if (g->producciones[i][0] == noTerminal)
		{
			produccionesAux[indice] = i;
			indice++;
		}
	}
}

void derivarGramatica(gramatica *g)
{
	char *palabra = malloc(sizeof(char));
	int produccionesAux[MAX_CANT_PRODUCCIONES];
	int numRandom;

	palabra[0] = g->axioma;

	int posNT; // posicion del NT de la palabra

	while (quedanNoTerminalParaDerivar(g, palabra, &posNT))
	{
		// obtengo el NT en cuestion
		char noTerminal = palabra[posNT];
		obtenerIndiceProducciones(g, noTerminal, produccionesAux);

		numRandom = obtenerNumeroRandomEntreRango(0, 1); // harcodeado largo de produccionesAux
		construirPalabra(posNT, palabra[posNT], g->producciones[produccionesAux[numRandom]]);
		printf("Palabra: %s\n", palabra);
	}
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

	// generar cadenas pertenecientes a la gramatica - to do
	// printf("%d\n", obtenerNumeroRandomEntreRango(0, 2));
	// printf("%d\n", obtenerNumeroRandomEntreRango(5, 10));
	// printf("%d\n", obtenerNumeroRandomEntreRango(10, 100));

	return 0;
}
