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

// abstraccion de la generacion y obtencion de numero aleatorio, devuelve un nro aleatorio del conjunto [li, ls)
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

	printf("Gramatica obtenida: \n - No terminales: %s\n - Terminales: %s\n - Producciones: %s\n - Axioma: %c\n", g->noTerminales, g->terminales, prodsString, g->axioma);
	free(prodsString);
	return 0;
}

int contieneNoTerminal(gramatica *g, char *palabra)
{
	if (palabra[strlen(palabra) - 1] == g->noTerminales[0] || palabra[strlen(palabra) - 1] == g->noTerminales[1])
	{
		return 0;
	}
	return 1;
}

void construirPalabra(char *palabra, char *palabraAux)
{
	for (int i = 0; i < strlen(palabraAux); i++)
	{
		palabra[strlen(palabra - 1)] = palabraAux[i];
	}
}

void obtenerNoTerminal(gramatica *g, char *palabra)
{
	int numRandom = obtenerNumeroRandomEntreRango(0, 2);
	char noTerminal = palabra[strlen(palabra) - 1];
	char palabraAux[MAX_LARGO_PRODUCCION] = "";
	if (noTerminal == g->producciones[numRandom][0])
	{
		int pos = 0;
		for (int i = 1; i < strlen(g->producciones[numRandom]); i++)
		{
			palabraAux[pos++] = g->producciones[numRandom][i];
		}
		construirPalabra(&palabra, palabraAux);
	}
}

void derivarGramatica(gramatica *g)
{
	char palabra[MAX_LARGO_PRODUCCION] = "";
	palabra[0] = g->axioma;
	while (1)
	{
		obtenerNoTerminal(g, palabra);
		if (contieneNoTerminal(g, palabra))
		{
			palabra[strlen(palabra)] = '\0';
			break;
		}
	}
	printf("Palabra: %s\n", palabra);
}

// Formato de los argumentos: noTerminales terminales producciones axioma
//                        EJ: (S,T; a,b; S-\>aT,T-\>a,T-\>bT; S)
// Cada argumento se separa con espacios y cada elemento de los mismos con comas. Hay que poner \ antes del > para que no quiera generar un archivo
int main(int argc, char *argv[])
{
	// obtener e interpretar los argumentos, verificando que sea GF - Hecho, pero hardcodeado
	gramatica g;
	if (generarGramatica(&g, argv[1], argv[2], argv[3], argv[4]))
		return 1;

	// Genera la palabra con los caracteres mostrados anteriormente
	derivarGramatica(&g);

	// generar cadenas pertenecientes a la gramatica - to do
	// int cantGramaticasGenerar = 10;
	// printf("%d\n", obtenerNumeroRandomEntreRango(0, 2));
	// printf("%d\n", obtenerNumeroRandomEntreRango(5, 10));
	// printf("%d\n", obtenerNumeroRandomEntreRango(10, 100));

	return 0;
}