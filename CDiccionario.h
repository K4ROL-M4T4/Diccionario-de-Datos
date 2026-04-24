#ifndef CDICCIONARIO_H
#define CDICCIONARIO_H

#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

typedef char cadena[30];

struct Entidad {
    cadena nombre;
    long atr;   // Dirección al primer atributo (-1 si no hay)
    long data;  // Dirección a los datos (-1 si no hay)
    long sig;   // Dirección a la siguiente entidad (-1 si es la última)
};

class CDiccionario {
public:
    FILE *arch;
    long cab;
    long dir;

    // Métodos de gestión de Entidades
    Entidad capturaEntidad();
    void altaEntidad();
    void bajaEntidad();
    void modificaEntidad();
    void consultaEntidades();

    // Métodos de bajo nivel (archivos)
    long buscaEntidad(char nombre[30]);
    long getCabeceraEntidades();
    Entidad leeEntidad(long dir);
    void escribeCabEntidades(long cab);
    long escribeEntidad(Entidad ent);
    void insertaEntidad(Entidad nvo, long dir);
    void reescribeEntidad(Entidad ent, long dir);
    long eliminaEntidad(char nombre[30]); // Lógica de desconexión
};

#endif
