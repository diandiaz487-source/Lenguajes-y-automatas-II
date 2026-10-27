
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char vars[100][50], tipos[100][10], *tipo = NULL;
int total = 0;

char *datos[] = {"int", "float", "char"};
char *palabras[] = {"if", "else", "for", "while", "return"};

// Busca una palabra en una lista
int buscar_lista(char *t, char *lista[], int n) {
    for (int i = 0; i < n; i++)
        if (!strcmp(t, lista[i])) return i;
    return -1;
}

// Busca una variable
int buscar(char *t) {
    for (int i = 0; i < total; i++)
        if (!strcmp(t, vars[i])) return i;
    return -1;
}

// Analiza cada elemento
void analizar(char *t) {
    printf("%-12s -> ", t);

    int d = buscar_lista(t, datos, 3);
    int r = buscar_lista(t, palabras, 5);
    int v = buscar(t);

    if (d != -1) {
        tipo = datos[d];
        puts("TIPO DE DATO");
    }
    else if (r != -1)
        printf("INSTRUCCION %s\n",
            r == 0 ? "CONDICIONAL" :
            r == 1 ? "ALTERNATIVA" :
            r == 4 ? "RETORNO" : "CICLO REPETITIVO");
    else if (!strcmp(t, "="))
        puts("ASIGNACION");
    else if (!strcmp(t, ";")) {
        puts("FIN DE INSTRUCCION");
        tipo = NULL;
    }
    else if (isdigit(t[0]))
        puts(strchr(t, '.') ? "NUMERO FLOTANTE" : "NUMERO ENTERO");
    else if (t[0] == '\'')
        puts("CARACTER");
    else if (strlen(t) == 1 && strchr("+-*/", t[0]))
        puts("OPERADOR");
    else if (strchr("><!", t[0]) || !strcmp(t, "=="))
        puts("OPERADOR DE COMPARACION");
    else if (strlen(t) == 1 && strchr("(){}", t[0]))
        puts("DELIMITADOR");
    else if (v != -1)
        printf("VARIABLE DE TIPO %s\n", tipos[v]);
    else if (tipo && total < 100) {
        strcpy(vars[total], t);
        strcpy(tipos[total++], tipo);
        printf("DECLARACION DE VARIABLE (%s)\n", tipo);
        tipo = NULL;
    }
    else
        puts("IDENTIFICADOR NO DECLARADO");
}

// Programa principal
int main() {
    char nombre[100], linea[500], t[100];

    printf("Nombre del archivo: ");
    fgets(nombre, 100, stdin);
    nombre[strcspn(nombre, "\n")] = 0;

    FILE *f = fopen(nombre, "r");
    if (!f) {
        puts("Error al abrir el archivo");
        return 1;
    }

    while (fgets(linea, 500, f)) {
        int i = 0;

        while (linea[i]) {
            if (isspace(linea[i])) {
                i++;
                continue;
            }

            int j = 0;

            if (isalnum(linea[i]) || linea[i] == '_') {
                while (isalnum(linea[i]) || linea[i] == '_'
                    || (linea[i] == '.' && isdigit(linea[i+1])))
                    t[j++] = linea[i++];
            }
            else if (linea[i] == '\'') {
                t[j++] = linea[i++];
                while (linea[i] && linea[i] != '\'')
                    t[j++] = linea[i++];
                if (linea[i]) t[j++] = linea[i++];
            }
            else if (strchr("=!><", linea[i]) && linea[i+1] == '=') {
                t[j++] = linea[i++];
                t[j++] = linea[i++];
            }
            else
                t[j++] = linea[i++];

            t[j] = 0;
            analizar(t);
        }
    }

    fclose(f);
    return 0;
}
