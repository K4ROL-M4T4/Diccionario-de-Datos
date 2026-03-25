#ifndef PACIENTE_H
#define PACIENTE_H

#include <stdio.h>

typedef struct {
    char nombre[50];
    int edad;
    float peso;
    double estatura;
    long idPaciente;
} Paciente;

void capturar(FILE *archivo);
void consultar(FILE *archivo);

#endif
