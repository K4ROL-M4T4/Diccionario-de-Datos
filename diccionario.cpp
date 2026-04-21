//Vega Torres Marian
//Tovar Mata Karol
#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

struct Entidad {
    char nombre[50];
    long atr;
    long data;
    long sig;
};

class CDiccionario {
private:
    FILE *arch; 
    long cab;   
    long dir;   

public:
    CDiccionario() {
        arch = NULL;
        cab = -1;
        dir = -1;
    }

    void menuPrincipal();
    void nuevoDiccionario();
    void abrirDiccionario();
    void menuEntidades();
    void menuAtributos();
    void menuDatos();

    Entidad capturaEntidad();
    void altaEntidad();
    long buscaEntidad(Entidad ent);
    long getCabeceraEntidades();
    Entidad leeEntidad(long direccion);
    void escribeCabEntidades(long cabecera);
    long escribeEntidad(Entidad ent);
    void insertaEntidad(Entidad nvo, long dirNuevaEntidad);
    void reescribeEntidad(Entidad ent, long direccion);
};

void CDiccionario::nuevoDiccionario() {
    char nombreArchivo[100];
    cout << "\n[Nuevo] Ingresa el nombre del diccionario: ";
    cin >> nombreArchivo;
    arch = fopen(nombreArchivo, "rb");
    if (arch != NULL) {
        cout << "Error: El archivo ya existe." << endl;
        fclose(arch);
        arch = NULL;
        return; 
    }

    arch = fopen(nombreArchivo, "wb+");
    if (arch == NULL) {
        cout << "Error al crear el archivo." << endl;
        return;
    }
    escribeCabEntidades(-1);
    cout << "Archivo creado. Cabecera (-1) escrita." << endl;
    menuEntidades();
    if (arch != NULL) {
        fclose(arch);
        arch = NULL;
        cout << "Archivo cerrado de forma segura." << endl;
    }
}

void CDiccionario::abrirDiccionario() {
    char nombreArchivo[100];
    cout << "\nIngresa el nombre del diccionario: ";
    cin >> nombreArchivo;

    arch = fopen(nombreArchivo, "rb+");
    if (arch != NULL) {
        cout << "Archivo abierto exitosamente." << endl;
        menuEntidades();

        // Al regresar cerrar el archivo
        if (arch != NULL) {
            fclose(arch);
            arch = NULL;
            cout << "Archivo cerrado de forma segura." << endl;
        }
    } else {
        cout << "Error: El archivo no existe." << endl;
    }
}

void CDiccionario::menuDatos(){
    int opc;
    do{
        cout << "\nMENU DE DATOS" << endl;
        cout << "1. Nuevo registro" << endl;
        cout << "2. Consultar registros" << endl;
        cout << "3. Eliminar registro" << endl;
        cout << "4. Modificar registro" << endl;
        cout << "5. Regresar al menu anterior" << endl;
        cout << "Elige una opcion: ";
        cin >> opc;
        switch (opc) {
            case 1: cout << "[Ejecutando funcion Nuevo registro...]" << endl; break;
            case 2: cout << "[Ejecutando funcion Consultar registros...]" << endl; break;
            case 3: cout << "[Ejecutando funcion Eliminar registro...]" << endl; break;
            case 4: cout << "[Ejecutando funcion Modificar registro...]" << endl; break;
            case 5: break;
            default: cout << "Opcion invalida." << endl;
        }
    } while (opc != 5);
}

void CDiccionario::menuAtributos(){
    int opc;
    do {
        cout << "\nMENU ATRIBUTOS" << endl;
        cout << "1. Nuevo Atributo" << endl;
        cout << "2. Consultar Atributo" << endl;
        cout << "3. Eliminar Atributo" << endl;
        cout << "4. Modificar Atributo" << endl;
        cout << "5. Regresar al menu anterior" << endl;
        cout << "Elige una opcion: ";
        cin >> opc;
        switch (opc) {
            case 1: cout << "Ejecutando funcion Nuevo Atributo..." << endl; break;
            case 2: cout << "Ejecutando funcion Consultar Atributo..." << endl; break;
            case 3: cout << "Ejecutando funcion Eliminar Atributo..." << endl; break;
            case 4: cout << "Ejecutando funcion Modificar Atributo..." << endl; break;
            case 5: break;
            default: cout << "Opcion invalida." << endl;
        }
    } while (opc != 5);
}

void CDiccionario::menuEntidades() {
    int opc;
    do {
        cout << "\nMENU ENTIDADES" << endl;
        cout << "1. Nueva Entidad" << endl;
        cout << "2. Consultar Entidades" << endl;
        cout << "3. Eliminar Entidad" << endl;
        cout << "4. Modificar Entidad" << endl;
        cout << "5. Menu Atributos" << endl;
        cout << "6. Menu Datos" << endl;
        cout << "7. Regresar al menu anterior" << endl;
        cout << "Elige una opcion: ";
        cin >> opc;
        switch (opc) {
            case 1: cout << "Ejecutando funcion Nueva Entidad..." << endl; break;
            case 2: cout << "Ejecutando funcion Consultar Entidades..." << endl; break;
            case 3: cout << "Ejecutando funcion Eliminar Entidad..." << endl; break;
            case 4: cout << "Ejecutando funcion Modificar Entidad..." << endl; break;
            case 5: menuAtributos(); break; 
            case 6: menuDatos(); break;     
            case 7: break; 
            default: cout << "Opcion invalida." << endl;
        }
    } while (opc != 7);
}

void CDiccionario::menuPrincipal(){
    int opc;
    do{
        cout << "\nGESTOR DE DICCIONARIO DE DATOS" << endl;
        cout << "1. Nuevo Diccionario" << endl;
        cout << "2. Abrir Diccionario" << endl;
        cout << "3. Salir" << endl;
        cout << "Elige una opcion: ";
        cin >> opc;
        switch (opc) {
            case 1: nuevoDiccionario(); break;
            case 2: abrirDiccionario(); break;
            case 3: cout << "Saliendo..." << endl; break;
            default: cout << "Opcion invalida." << endl;
        }
    } while (opc != 3);
}

Entidad CDiccionario::capturaEntidad(){
    Entidad nueva;
    cout << "Ingrese el nombre de la entidad: ";
    cin >> nueva.nombre;
    nueva.atr= -1;
    nueva.data= -1;
    nueva.sig= -1;
    return nueva;
}

void CDiccionario::altaEntidad(){
    Entidad nueva = capturaEntidad();
    if (buscaEntidad(nueva) == -1) {
        long dirNueva = escribeEntidad(nueva);
        insertaEntidad(nueva, dirNueva);
    }else{
        cout << "Error: La entidad ya existe" << endl;
    }
}

long CDiccionario::buscaEntidad(Entidad ent) {
    long cabecera = getCabeceraEntidades();
    Entidad actual;
    while (cabecera != -1) {
        actual = leeEntidad(cabecera);
        if (strcmp(actual.nombre, ent.nombre) == 0) {
            return cabecera;
        }
        cabecera= actual.sig;
    }
    return -1;
}

long CDiccionario::getCabeceraEntidades(){
    long cabecera_local;
    fseek(arch, 0, SEEK_SET);
    fread(&cabecera_local, sizeof(long), 1, arch);
    return cabecera_local;
}

Entidad CDiccionario::leeEntidad(long direccion) {
    Entidad nvo;
    fseek(arch, direccion, SEEK_SET);
    fread(&nvo, sizeof(Entidad), 1, arch);
    return nvo;
}

void CDiccionario::escribeCabEntidades(long cabecera) {
    fseek(arch, 0, SEEK_SET);
    fwrite(&cabecera, sizeof(long), 1, arch);
}

long CDiccionario::escribeEntidad(Entidad ent){
    fseek(arch, 0, SEEK_END);
    long direccion = ftell(arch);
    fwrite(&ent, sizeof(Entidad), 1, arch);
    return direccion;
}

void CDiccionario::insertaEntidad(Entidad nvo, long dirNuevaEntidad) {
    long cabecera = getCabeceraEntidades();
    if (cabecera == -1) {
        escribeCabEntidades(dirNuevaEntidad);
    } else {
        Entidad actual = leeEntidad(cabecera);
        if (strcmp(actual.nombre, nvo.nombre) > 0) {
            nvo.sig = cabecera;
            reescribeEntidad(nvo, dirNuevaEntidad);
            escribeCabEntidades(dirNuevaEntidad);
        } else {
            long dir_ant = -1;
            Entidad Entant;
            while (cabecera != -1 && strcmp(actual.nombre, nvo.nombre) < 0) {
                dir_ant = cabecera;
                Entant = actual;
                cabecera = actual.sig;
                if (cabecera != -1) {
                    actual = leeEntidad(cabecera);
                }
            }
            nvo.sig = cabecera;
            reescribeEntidad(nvo, dirNuevaEntidad);
            Entant.sig = dirNuevaEntidad;
            reescribeEntidad(Entant, dir_ant);
        }
    }
}

void CDiccionario::reescribeEntidad(Entidad ent, long direccion) {
    fseek(arch, direccion, SEEK_SET);
    fwrite(&ent, sizeof(Entidad), 1, arch);
}

int main() {
    CDiccionario miGestor;
    miGestor.menuPrincipal();
    return 0;
}