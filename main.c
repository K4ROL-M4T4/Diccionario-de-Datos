//Vega Torres Marian
//Tovar Mata Karol

#include <stdio.h>
#include <stdlib.h>
#include "paciente.h"

typedef char cadena[50];

int muestraOpciones(cadena opciones[], int n)
{
    int opc;
    for (int i = 0; i < n; i++)
        printf("%s\n", opciones[i]);

    scanf("%d", &opc);
    return opc;
}

void submenu(FILE *archivo)
{
    int op;
    cadena opciones[3] = {
        "1- Capturar",
        "2- Consultar",
        "3- Cerrar"
    };

    do
    {
        op = muestraOpciones(opciones, 3);

        switch (op)
        {
        case 1:
            capturar(archivo);
            break;
        case 2:
            consultar(archivo);
            break;
        case 3:
            fclose(archivo);
            break;
        }
    } while (op != 3);
}

int main()
{
    FILE *archivo = NULL;
    int op;
    char nombre[100];

    cadena opciones[3] = {
        "1- Nuevo",
        "2- Abrir",
        "3- Salir"
    };

    do
    {
        op = muestraOpciones(opciones, 3);

        switch (op)
        {
        case 1:
            scanf("%s", nombre);
            archivo = fopen(nombre, "wb+");
            if (archivo != NULL)
                submenu(archivo);
            break;

        case 2:
            scanf("%s", nombre);
            archivo = fopen(nombre, "rb+");
            if (archivo != NULL)
                submenu(archivo);
            break;

        }
    } while (op != 3);

    return 0;
}
