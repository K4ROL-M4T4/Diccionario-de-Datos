#include "CDiccionario.h"

//Inicializa los valores de una nueva entidad capturada por teclado
Entidad CDiccionario::capturaEntidad(){
    Entidad nueva;
    cout << "Ingrese el nombre de la entidad: ";
    cin >> nueva.nombre;
    nueva.atr = -1;
    nueva.data = -1;
    nueva.sig = -1;
    return nueva;
}

//Coordina el proceso de capturar, verificar duplicados e insertar una entidad
void CDiccionario::altaEntidad() {
    Entidad nueva = capturaEntidad();
    if (buscaEntidad(nueva.nombre) == -1) {
        dir = escribeEntidad(nueva);
        insertaEntidad(nueva, dir);
    } else {
        cout << "Error: La entidad ya existe" << endl;
    }
}

//Lee el primer long del archivo que representa el inicio de la lista de entidades
long CDiccionario::getCabeceraEntidades() {
    fseek(arch, 0, SEEK_SET);
    if (fread(&cab, sizeof(long), 1, arch) != 1) return -1;
    return cab;
}

//Actualiza el puntero inicial del archivo
void CDiccionario::escribeCabEntidades(long cabecera) {
    fseek(arch, 0, SEEK_SET);
    fwrite(&cabecera, sizeof(long), 1, arch);
}

//Escribe una entidad al final del archivo y retorna su posición
long CDiccionario::escribeEntidad(Entidad ent) {
    fseek(arch, 0, SEEK_END);
    long direccion = ftell(arch);
    fwrite(&ent, sizeof(Entidad), 1, arch);
    return direccion;
}

//Sobreescribe una entidad en una posición específica del archivo
void CDiccionario::reescribeEntidad(Entidad ent, long direccion) {
    fseek(arch, direccion, SEEK_SET);
    fwrite(&ent, sizeof(Entidad), 1, arch);
}

//Recorre la lista ligada en el archivo buscando un nombre y retorna su dirección
long CDiccionario::buscaEntidad(char nombre[30]) {
    long cabecera = getCabeceraEntidades();
    while (cabecera != -1) {
        Entidad actual = leeEntidad(cabecera);
        if (strcmp(actual.nombre, nombre) == 0) return cabecera;
        cabecera = actual.sig;
    }
    return -1;
}

//Recupera una estructura Entidad desde una dirección física del archivo
Entidad CDiccionario::leeEntidad(long direccion) {
    Entidad nvo;
    fseek(arch, direccion, SEEK_SET);
    fread(&nvo, sizeof(Entidad), 1, arch);
    return nvo;
}

// Inserta una entidad manteniendo el orden alfabético de la lista ligada
void CDiccionario::insertaEntidad(Entidad nvo, long dirNueva) {
    long cabecera = getCabeceraEntidades();
    if (cabecera == -1) {
        escribeCabEntidades(dirNueva);
    } else {
        Entidad actual = leeEntidad(cabecera);
        if (strcmp(nvo.nombre, actual.nombre) < 0) {
            nvo.sig = cabecera;
            reescribeEntidad(nvo, dirNueva);
            escribeCabEntidades(dirNueva);
        } else {
            long dirant = cabecera;
            long dirActual = actual.sig;
            while (dirActual != -1) {
                actual = leeEntidad(dirActual);
                if (strcmp(nvo.nombre, actual.nombre) < 0) break;
                dirant = dirActual;
                dirActual = actual.sig;
            }
            nvo.sig = dirActual;
            reescribeEntidad(nvo, dirNueva);
            Entidad anterior = leeEntidad(dirant);
            anterior.sig = dirNueva;
            reescribeEntidad(anterior, dirant);
        }
    }
}

//Imprime en pantalla todas las entidades registradas recorriendo la lista
void CDiccionario::consultaEntidades() {
    long aux = getCabeceraEntidades();
    cout << "\n--- LISTA DE ENTIDADES ---" << endl;
    while (aux != -1) {
        Entidad e = leeEntidad(aux);
        cout << "Nombre: " << e.nombre << " | Atr: " << e.atr << " | Data: " << e.data << " | Sig: " << e.sig << endl;
        aux = e.sig;
    }
}

//Solicita el nombre de una entidad para proceder a su desconexión física
void CDiccionario::bajaEntidad() {
    char nom[30];
    cout << "Ingrese el nombre de la entidad a eliminar: ";
    cin >> nom;
    if (buscaEntidad(nom) == -1) {
        cout << "ERROR: La entidad no existe." << endl;
    } else {
        eliminaEntidad(nom);
        cout << "Entidad eliminada." << endl;
    }
}

//desconexión de punteros para eliminar una entidad de la lista
long CDiccionario::eliminaEntidad(char nombre[30]) {
    long actual = getCabeceraEntidades();
    long anterior = -1;
    while (actual != -1) {
        Entidad ent = leeEntidad(actual);
        if (strcmp(ent.nombre, nombre) == 0) {
            if (anterior == -1) { // Es la cabeza
                escribeCabEntidades(ent.sig);
            } else { // Está en medio o al final
                Entidad entAnt = leeEntidad(anterior);
                entAnt.sig = ent.sig;
                reescribeEntidad(entAnt, anterior);
            }
            return actual;
        }
        anterior = actual;
        actual = ent.sig;
    }
    return -1;
}

//Permite renombrar o actualizar una entidad eliminando la anterior e insertando la nueva
void CDiccionario::modificaEntidad() {
    char nom[30];
    cout << "Ingrese el nombre de la entidad a modificar: ";
    cin >> nom;
    long posicion = buscaEntidad(nom);
    
    if (posicion != -1) {
        Entidad nueva = capturaEntidad();
        if (buscaEntidad(nueva.nombre) == -1 || strcmp(nom, nueva.nombre) == 0) {
            eliminaEntidad(nom);
            long nuevaDir = escribeEntidad(nueva);
            insertaEntidad(nueva, nuevaDir);
            cout << "Entidad modificada con exito." << endl;
        } else {
            cout << "Error: El nuevo nombre ya existe." << endl;
        }
    } else {
        cout << "Error: La entidad no existe." << endl;
    }
}

//funciones en clase 
void CDiccionario::modificaAtributo() {
    char nombre[30];
    Atributo nvo;
    printf("Atributo a modificar: ");
    scanf("%s", nombre);
    
    if (buscaAtributo(nombre) != -1) {
        printf("Ingresa la nueva informacion: ");
        nvo = capturaAtributo();
        long dir = buscaAtributo(nvo.nombre); // Nota: Agregué 'long' asumiendo que no estaba declarada globalmente
        
        if (strcmp(nvo.nombre, nombre) == 0 || dir == -1) {
            long dir3 = eliminaAtributo(nombre); // Asumo que dir3 es long
            insertaAtributo(nvo, dir3);
        }
        else
            printf("No se puede modificar porque ya existe uno con ese nombre");
    }
    else
        printf("No existe el atributo");
}

void CDiccionario::bajaAtributo() {
    char nombre[50];
    printf("Ingrese el nombre del atributo a eliminar: ");
    scanf("%s", nombre);
    long dir = buscaAtributo(nombre);
    
    if(dir == -1)
        printf("Error: el atributo no existe");
    else
        eliminaAtributo(nombre);
}

void CDiccionario::EliminaAtributo(char cad[]) {
    long cab = activa.atr;
    long dirant = -1;
    Atributo ant, actual;
    
    actual = leeAtributo(cab);
    if(strcmp(actual.nombre, cad) == 0)
        reescribeEntidad(activa, directiva); // En tus apuntes dice "directiva", probablemente sea "dirActiva"
    else
        while (cab != -1 && strcmp(cad, actual.nombre) < 0) {
            dirant = cab;
            ant = actual;
            cab = actual.sig;
            if (cab != -1)
                //...

void CDiccionario::insertaAtributo(Atributo nvo, long dir) {
    if(activa.atr == -1) {
        activa.atr = dir;
        reescribeEntidad(activa, directiva); 
    }
    
    Atributo actual = leeAtributo(activa.atr);
    if(strcmp(actual.nombre, nvo.nombre) > 0) {
        nvo.sig = activa.atr;
        reescribeAtributo(nvo, dir);
        activa.atr = dir;
        reescribeEntidad(activa, directiva);
    } else {
        long cab = activa.atr;
        while(cab != -1 && strcmp(nvo.nombre, actual.nombre) > 0) {
            long dirAnt = cab;
            Atributo atrAnt = actual;
            cab = actual.sig;
            if(cab != -1)
                actual = leeAtributo(cab);
        } 
        
        if (cab != -1) {
            nvo.sig = cab;
            reescribeAtributo(nvo, dir);
        }
        atrAnt.sig = dir;
        reescribeAtributo(atrAnt, dirAnt);
    }
}

void CDiccionario::modificaEntidad() {
    Entidad nueva;
    char nombre[50]; 
    
    cout << "¿Que entidad desea modificar? ";
    cin >> nombre;

    if (buscaEntidad(nombre) != -1) {
        cout << "Ingrese la nueva info: ";
        nueva = capturaEntidad();
        
        if (buscaEntidad(nueva.nombre) == -1) {
            long arr = eliminaEntidad(nombre); 
            reescribeEntidad(nueva, arr);    
            insertaEntidad(nueva, arr);        
        } else {
            cout << "No se puede";
        }
    } else {
        cout << "No existe la entidad";
    }
}