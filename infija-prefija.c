
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

// Prioridad de los operadores
int prioridad(char c) {
    if (c == '*' || c == '/') return 2;
    if (c == '+' || c == '-') return 1;
    return 0;
}

// Revisa si es operador
int esOperador(char c) {
    return c == '+' || c == '-' ||
           c == '*' || c == '/';
}

// Convierte de infija a prefija
void convertir(char infija[], char prefija[]) {
    char pila[MAX], salida[MAX], exp[MAX];
    int n = 0, j = 0, tope = -1;

    // Quita espacios
    for (int i = 0; infija[i]; i++)
        if (infija[i] != ' ' && infija[i] != '\n')
            exp[n++] = infija[i];

    // Invierte la expresion y cambia parentesis
    for (int i = 0; i < n / 2; i++) {
        char aux = exp[i];
        exp[i] = exp[n-1-i];
        exp[n-1-i] = aux;
    }

    for (int i = 0; i < n; i++) {
        if (exp[i] == '(') exp[i] = ')';
        else if (exp[i] == ')') exp[i] = '(';
    }

    // Convierte a postfija usando una pila
    for (int i = 0; i < n; i++) {
        char c = exp[i];

        if (isalnum(c))
            salida[j++] = c;

        else if (c == '(')
            pila[++tope] = c;

        else if (c == ')') {
            while (tope >= 0 && pila[tope] != '(')
                salida[j++] = pila[tope--];
            tope--;
        }

        else if (esOperador(c)) {
            while (tope >= 0 && pila[tope] != '(' &&
                   prioridad(pila[tope]) > prioridad(c))
                salida[j++] = pila[tope--];

            pila[++tope] = c;
        }
    }

    // Saca lo que queda en la pila
    while (tope >= 0)
        salida[j++] = pila[tope--];

    // Invierte para obtener prefija
    for (int i = 0; i < j; i++)
        prefija[i] = salida[j-1-i];

    prefija[j] = '\0';
}

int main() {
    FILE *archivo;
    char ruta[200], infija[MAX], prefija[MAX];

    printf("Ruta del archivo .txt: ");
    fgets(ruta, sizeof(ruta), stdin);
    ruta[strcspn(ruta, "\n")] = '\0';

    archivo = fopen(ruta, "r");

    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    // Lee y convierte cada expresion
    while (fgets(infija, MAX, archivo)) {
        printf("\nInfija: %s", infija);
        convertir(infija, prefija);
        printf("Prefija: %s\n", prefija);
    }

    fclose(archivo);
    return 0;
}
