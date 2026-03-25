#include <stdio.h>
#include <stdlib.h>
#include "paciente.h"

#define MAX_DIR 5000

int validarDireccion(int dir)
{
    return (dir >= 0 && dir <= MAX_DIR);
}

void capturar(FILE *archivo)
{
    int dir;

    void *ptr = malloc(sizeof(Paciente));

    if (ptr == NULL)
        return;

    Paciente *p = (Paciente *)ptr;

    scanf("%49s", p->nombre);
    scanf("%d", &p->edad);
    scanf("%f", &p->peso);
    scanf("%lf", &p->estatura);
    scanf("%ld", &p->idPaciente);

    scanf("%d", &dir);

    if (!validarDireccion(dir))
    {
        free(ptr);
        return;
    }

    fseek(archivo, dir, SEEK_SET);
    fwrite(ptr, sizeof(Paciente), 1, archivo);

    free(ptr);
}

void consultar(FILE *archivo)
{
    int dir;
    Paciente p;

    scanf("%d", &dir);

    if (!validarDireccion(dir))
        return;

    fseek(archivo, dir, SEEK_SET);

    if (fread(&p, sizeof(Paciente), 1, archivo) != 1)
        return;

    printf("%s\n", p.nombre);
    printf("%d\n", p.edad);
    printf("%.2f\n", p.peso);
    printf("%.2lf\n", p.estatura);
    printf("%ld\n", p.idPaciente);
}
