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

typedef struct gram {
	char terminales[MAX_TERMINALES];
	char noTerminales[MAX_NO_TERMINALES];
	char producciones[MAX_CANT_PRODUCCIONES][MAX_LARGO_PRODUCCION];
	char axioma;
} gramatica;

// abstraccion de la generacion y obtencion de numero aleatorio, devuelve un nro aleatorio del conjunto [li, ls)
int obtenerNumeroRandomEntreRango(int li, int ls) {
    if (hayQueCrearSeed) {
        srand(time(NULL));
        hayQueCrearSeed = 0;
    }
    return li + (rand() % (ls - li));
}

int generarTerminales(gramatica *g, char *strTerminales) {
	char *terminales = strtok(strTerminales, ",");
	char terminalesProcesados[strlen(terminales)];
	int contador = 0;

    while (terminales != NULL) {
		char terminal = terminales[0];

		// Verfica la existencia de unicidad de los terminales
        if (strchr(terminalesProcesados, terminal) != NULL) {
			printf("Error: Se repite el elemento %c\n", terminal);
			return 1;
		}

        // Verifica que los caracteres representen un terminal
		if (!((terminal >= 97 && terminal <= 122) || (unsigned char)terminal == 195)){
			printf("Error: El caracter %c no representa un elemento terminal\n", terminal);
			return 1;
		}

		terminalesProcesados[contador] = terminal;
		contador++;
        terminales = strtok(NULL, ",");
    }

	strcpy(g->terminales, terminalesProcesados);
	printf("%s\n", g->terminales);
	return 0;
}

int generarNoTerminales(gramatica *g, char *strNoTerminales) { // hardcodeado
	char *noTerminales = strtok(strNoTerminales, ",");
	char noTerminalesProcesados[strlen(noTerminales)];
	int contador = 0;

    while (noTerminales != NULL) {
		char noTerminal = noTerminales[0];

		// Verfica la existencia de unicidad de los no terminales
        if (strchr(noTerminalesProcesados, noTerminal) != NULL) {
			printf("Error: Se repite el elemento %c\n", noTerminal);
			return 1;
		}

        // Verifica que los caracteres representen un no terminal
		if (!(noTerminal >= 65 && noTerminal <= 90)){
			printf("Error: El caracter %c no representa un elemento no terminal\n", noTerminal);
			return 1;
		}

		noTerminalesProcesados[contador] = noTerminal;
		contador++;
        noTerminales = strtok(NULL, ",");
    }

	strcpy(g->noTerminales, noTerminalesProcesados);
	printf("%s\n", g->noTerminales);
	return 0;
}

int generarProducciones(gramatica *g, char *strProducciones, char *prodsString) { // hardcodeado
	strcpy(g->producciones[0], "SaS");
	strcpy(g->producciones[1], "Ta");
	strcpy(g->producciones[2], "TbT");
	strcpy(prodsString, "S->aT | T->a | T->bT");
	return 0;
}

int generarAxioma(gramatica *g, char *strAxioma) {
	// Verfica que el axioma sea uno solo
	if (strlen(strAxioma) != 1) {
		printf("Error: Axioma invalido: \"%s\"", strAxioma);
		return 1;
	}
	
	// Verfica que el caracter represente un axioma
	if (!(strAxioma[0] >= 65 && strAxioma[0] <= 90)){
		printf("Error: El caracter %c no es axioma\n", strAxioma[0]);
		return 1;
	}

	g->axioma = strAxioma[0];
    printf("%c\n", g->axioma);
	return 0;
}

int generarGramatica(gramatica *g, char *strNoTerminales, char *strTerminales, char *strProducciones, char *strAxioma) {
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


// Formato de los argumentos: terminales noTerminales producciones axioma
//                        EJ: S,T a,b S-\>aS,T-\>a,T-\>bT S
// Cada argumento se separa con espacios y cada elemento de los mismos con comas. Hay que poner \ antes del > para que no quiera generar un archivo
int main(int argc, char *argv[])
{
	// obtener e interpretar los argumentos, verificando que sea GF - Hecho, pero hardcodeado
	gramatica *g;
	char terminales[] = "a,b,a";
	generarTerminales (g, terminales);
	// if (generarGramatica(&g, argv[1], argv[2], argv[3], argv[4]))
    // 	return 1;
	// generar cadenas pertenecientes a la gramatica - to do 
	// int cantGramaticasGenerar = 10;
    // printf("%d\n", obtenerNumeroRandomEntreRango(0,2));
    // printf("%d\n", obtenerNumeroRandomEntreRango(5,10));
    // printf("%d\n", obtenerNumeroRandomEntreRango(10,100));

	return 0;
}