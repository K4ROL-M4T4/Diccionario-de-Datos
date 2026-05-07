#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 50
typedef char cadena [30];

typedef struct
{
    long atr;
    long sig;
    cadena nombre;
    long data;
}Entidad;

class CDiccionario
{
private:
    FILE *archivo;
    Entidad activa;
    long diractiva;
    long tambloque;
    int nAtributos;

    char nombreArchivo[50];

public:
    CDiccionario();

    void menuPrincipal();

    void nuevoDiccionario();
    void abrirDiccionario();
    void MenuEntidades();

    void altaEntidad();
    void consultarEntidades();
    void bajaEntidad();
    void modificaEntidad();

    long getCabEntidades();
    void escribeCabEntidades(long cab);
    Entidad capturaEntidad();
    long buscaEntidad(Entidad ent);
    Entidad leeEntidad(long dir);
    long escribeEntidad(Entidad ent);
    void reescribeEntidad(long dir, Entidad ent);
    void insertaEntidad(Entidad nvo, long dir);
    long eliminaEntidad(cadena nom);

    void menuAtributos();

    void nuevoAtributo();
    void consultarAtributo();
    void eliminaAtributo();
    void modificaAtributo();

    void menuDatos();

    void nuevoRegistro();
    void consultarRegistro();
    void eliminaRegistro();
    void modificaRegistro();
};