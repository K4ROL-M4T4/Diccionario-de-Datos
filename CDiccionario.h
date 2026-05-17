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

typedef struct
{
    cadena nombre;
    int tipo;
    int tamano;
    char lskp;
    char nulo;
    char descripcion[50];
    long sig;
}Atributo;

class CDiccionario
{
private:
    FILE *archivo;
    Entidad activa;
    long diractiva;
    long tambloque;
    int nAtributos;
    Atributo arrAtributo[50];

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

    // Funciones publicas de atributos
    bool pideEntidad();
    Atributo capturaAtributo();
    long buscaAtributo(char *atr);
    void insertaAtributo(Atributo nvo, long dir);
    void nuevoAtributo();
    void consultarAtributo();
    long eliminaAtributo(char cab[30]);
    void modificaAtributo();
    Atributo leeAtributo(long dir);
    long escribeAtributo(Atributo atr);
    void reescribeAtributo(long dir, Atributo atr);
    void bajaAtributo();
    bool cargaAtributos();

    //Funciones Bloques
    void menuBloques();
    void* CapturaBloque();
    float comparaBloques(void *b1, void *b2);
    long buscaBloque(void *buscado);
    void altaBloque();
    void* LeeBloque(long dir);
    long EscribeBloque(void *bloque);
    void ReescribeBloque(long dir, void *bloque);
    void InsertaBloque(void *nuevo, long dirnvo);
    void ConsultaBloque();
};
