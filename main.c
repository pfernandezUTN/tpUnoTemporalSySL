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

//Inicializar gramatica
void inicializarGramatica (gramatica *g) {
	memset(g, 0, sizeof(gramatica));
}

// abstraccion de la generacion y obtencion de numero aleatorio, devuelve un nro aleatorio del conjunto [li, ls)
int obtenerNumeroRandomEntreRango(int li, int ls) {
    if (hayQueCrearSeed) {
        srand(time(NULL));
        hayQueCrearSeed = 0;
    }
    return li + (rand() % (ls - li));
}

int esTerminal(char terminal) {
	// Verifica que los caracteres representen un terminal
	return ((terminal >= 'a' && terminal <= 'z') || terminal == 'ß');
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
		if (!esTerminal(terminal)) {
			printf("Error: El caracter %c no representa un elemento terminal\n", terminal);
			return 1;
		}

		terminalesProcesados[contador] = terminal;
		contador++;
		terminales = strtok(NULL, ",");
	}

	terminalesProcesados[contador] = '\0';
	strcpy(g->terminales, terminalesProcesados);
	return 0;
}

int esNoTerminal(char noTerminal) {
	// Verifica que los caracteres representen un no terminal
	return (noTerminal >= 'A' && noTerminal <= 'Z');
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
		if (!esNoTerminal(noTerminal)) {
			printf("Error: El caracter %c no representa un elemento no terminal\n", noTerminal);
			return 1;
		}

		noTerminalesProcesados[contador] = noTerminal;
		contador++;
		noTerminales = strtok(NULL, ",");
	}

	noTerminalesProcesados[contador] = '\0';
	strcpy(g->noTerminales, noTerminalesProcesados);
	return 0;
}

int verificarErroresProduccion(gramatica *g, char *produccion) {
	//Verificar que los no terminales sean validos
	for (int i = 0; produccion[i] != '\0'; i++) {
		if (esNoTerminal(produccion[i])) {
			if (strchr(g->noTerminales, produccion[i]) == NULL) {
				printf("Error: El no terminal '%c' en la produccion '%s' no pertenece al conjunto definido '%s'\n", produccion[i], produccion, g->noTerminales);
				return 1;
			}
		}
	}
	
	//Verificar que los terminales sean validos
	for (int i = 0; produccion[i] != '\0'; i++) {
		if (esTerminal(produccion[i])) {
			if (strchr(g->terminales, produccion[i]) == NULL) {
				printf("Error: El terminal '%c' en la produccion '%s' no pertenece al conjunto definido '%s'\n", produccion[i], produccion, g->terminales);
				return 1;
			}
		}
	}

	//Verificar que exista la flecha
	if (strstr(produccion, "->") == NULL) {
		printf("Error: La produccion se ingreso sin flecha: '%s'\n", produccion);
		return 1;
	}

	//Verificar que la expresion del lado izquierdo sea valida
	if (!esNoTerminal(produccion[0])) {
		if (esTerminal(produccion[0])) {
			printf("Error: el primer elemento de '%s' es terminal\n", produccion);
			return 1;
		}
		printf("Error: Falta elemento no terminal en '%s' antes de la flecha\n", produccion);
		return 1;
	}

	if (produccion[1] != '-') {
		printf("Error: Hay mas de un elemento al lado izquierdo de la produccion '%s'\n", produccion);
		return 1;
	}

	//Verificar que la expresion del lado derecho sea valida
	int k = 3;
	int noTerminalContador = 0;

	while (produccion[k] != '\0') {
		if (esNoTerminal(produccion[k])) {
			noTerminalContador++;

			if (noTerminalContador > 1) {
				printf("Error: Hay mas de un no terminal al lado derecho de la produccion: '%s'\n", produccion);
				return 1;
			}
		}

		k++;
	}

	int j = 0;
	int terminalContador = 0;
	while (produccion[j] != '\0') {
		if (esTerminal(produccion[j])) {
			terminalContador++;

			if (terminalContador > 1) {
				printf("Error: Hay mas de un terminal en la produccion: '%s'\n", produccion);
				return 1;
			}
		}

		j++;
	}

	return 0;
}

int tieneProducciones(gramatica *g, char noTerminal) {
    for (int i = 0; i < MAX_CANT_PRODUCCIONES && g->producciones[i][0] != '\0'; i++) {
        if (g->producciones[i][0] == noTerminal) {
            return 1;
        }
    }
    return 0;
}

int tieneProduccionNoRecursiva(gramatica *g, char noTerminal) {
    for (int i = 0; i < MAX_CANT_PRODUCCIONES && g->producciones[i][0] != '\0'; i++) {
        if (g->producciones[i][0] == noTerminal) {
            char *prod = g->producciones[i];
            int len = strlen(prod);
            char terminalFinal = prod[len - 1];

            if (esTerminal(terminalFinal)) {
                return 1;
            }

            if (esNoTerminal(terminalFinal) && terminalFinal != noTerminal) {
                return 1;
            }
        }
    }
    return 0;
}

int produccionTieneContinuacion(gramatica *g, char *produccion) {
    int len = strlen(produccion);
    if (len < 2) return 0;

    char terminalFinal = produccion[len - 1];

    if (esTerminal(terminalFinal)) {
        return 1;

    } else if (esNoTerminal(terminalFinal)) {
        
        if (!tieneProducciones(g, terminalFinal)) {
            return 0;
        }
        
        if (!tieneProduccionNoRecursiva(g, terminalFinal)) {
            printf("La produccion %s no tiene salida del bucle\n", produccion);
            return 0;
        }
        return 1;
    }
    return 0;
}

int generarProducciones(gramatica *g, char *strProducciones, char *prodsString) { // hardcodeado

	int contador = 0;
	int cantidadProducciones = 0;
	int cantidadCaracProcesados = 0;
	int fila = 0;
	int columna = 0;
	char produccion[strlen(strProducciones)];

	while (strProducciones[contador] != '\0') {
		char caracter = strProducciones[contador];

		if (caracter == ',') {
			produccion[cantidadCaracProcesados] = '\0';

			if (verificarErroresProduccion(g, produccion)){
				return 1;
			}

			if (cantidadProducciones > 0) {
				strcat(prodsString, " | ");
			}
			strcat(prodsString, produccion);
			cantidadProducciones++;

			g->producciones[fila][columna] = '\0';

			cantidadCaracProcesados = 0;
			produccion[0] = '\0';
			fila++;
			columna = 0;
			contador++;
			continue;
		}

		if (esNoTerminal(caracter) || esTerminal(caracter) || caracter == '-' || caracter == '>') {
			produccion[cantidadCaracProcesados] = caracter;

			if (caracter != '-' && caracter != '>'){
				g->producciones[fila][columna] = caracter;
				columna++;
			}

			cantidadCaracProcesados++;
		}
		else {
			printf("Error: El caracter %c no pertenece a la contruccion de una produccion\n", caracter);
			return 1;
		}
		contador++;
	}

	if (cantidadCaracProcesados > 0) {
		produccion[cantidadCaracProcesados] = '\0';

		if (verificarErroresProduccion(g, produccion)){
			return 1;
		}

		if (cantidadProducciones > 0) {
			strcat(prodsString, " | ");
		}
		strcat(prodsString, produccion);
		cantidadProducciones++;

		// Cierre de la última producción
		g->producciones[fila][columna] = '\0';
	}

	for (int i = 0; i < MAX_CANT_PRODUCCIONES && g->producciones[i][0] != '\0'; i++) {
		if (!produccionTieneContinuacion(g, g->producciones[i])) {
			printf("Produccion '%s' termina en un no terminal sin continuacion\n", g->producciones[i]);
		}
	}

	return 0;
}

int generarAxioma(gramatica *g, char *strAxioma) {
	// Verfica que el axioma sea uno solo
	// Verfica que el axioma sea uno solo
	if (strlen(strAxioma) != 1) {
		printf("Error: Axioma invalido: \"%s\"", strAxioma);
		return 1;
	}

	// Verfica que el caracter represente un axioma
	if (!esNoTerminal(strAxioma[0])){
		printf("Error: El caracter %c no es axioma\n", strAxioma[0]);
		return 1;
	}

	g->axioma = strAxioma[0];
    	printf("%c\n", g->axioma);
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
    printf("--- Inicio de la derivacion ---\n");
	char *palabra = malloc(sizeof(char));
	int produccionesAux[MAX_CANT_PRODUCCIONES];
	int numRandom;

	palabra[0] = g->axioma;
    printf("Palabra (caso axioma): %s\n", palabra); // esto es para printear como arranca
	int posNT; // posicion del NT de la palabra
	
	int i = 0;

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
		printf("Palabra iteracion %d:   %s\n", i++, palabra);
	}
	printf("---  Fin de la derivacion   ---\n\nResultado: %s", palabra);
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
