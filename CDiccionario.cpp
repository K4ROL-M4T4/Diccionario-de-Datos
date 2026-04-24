#include "CDiccionario.h"

/* Inicializa los valores de una nueva entidad capturada por teclado */
Entidad CDiccionario::capturaEntidad() {
    Entidad nueva;
    cout << "Ingrese el nombre de la entidad: ";
    cin >> nueva.nombre;
    nueva.atr = -1;
    nueva.data = -1;
    nueva.sig = -1;
    return nueva;
}

/* Coordina el proceso de capturar, verificar duplicados e insertar una entidad */
void CDiccionario::altaEntidad() {
    Entidad nueva = capturaEntidad();
    if (buscaEntidad(nueva.nombre) == -1) {
        dir = escribeEntidad(nueva);
        insertaEntidad(nueva, dir);
    } else {
        cout << "Error: La entidad ya existe" << endl;
    }
}

/* Lee el primer long del archivo que representa el inicio de la lista de entidades */
long CDiccionario::getCabeceraEntidades() {
    fseek(arch, 0, SEEK_SET);
    if (fread(&cab, sizeof(long), 1, arch) != 1) return -1;
    return cab;
}

/* Actualiza el puntero inicial del archivo */
void CDiccionario::escribeCabEntidades(long cabecera) {
    fseek(arch, 0, SEEK_SET);
    fwrite(&cabecera, sizeof(long), 1, arch);
}

/* Escribe una entidad al final del archivo y retorna su posición */
long CDiccionario::escribeEntidad(Entidad ent) {
    fseek(arch, 0, SEEK_END);
    long direccion = ftell(arch);
    fwrite(&ent, sizeof(Entidad), 1, arch);
    return direccion;
}

/* Sobreescribe una entidad en una posición específica del archivo */
void CDiccionario::reescribeEntidad(Entidad ent, long direccion) {
    fseek(arch, direccion, SEEK_SET);
    fwrite(&ent, sizeof(Entidad), 1, arch);
}

/* Recorre la lista ligada en el archivo buscando un nombre y retorna su dirección */
long CDiccionario::buscaEntidad(char nombre[30]) {
    long cabecera = getCabeceraEntidades();
    while (cabecera != -1) {
        Entidad actual = leeEntidad(cabecera);
        if (strcmp(actual.nombre, nombre) == 0) return cabecera;
        cabecera = actual.sig;
    }
    return -1;
}

/* Recupera una estructura Entidad desde una dirección física del archivo */
Entidad CDiccionario::leeEntidad(long direccion) {
    Entidad nvo;
    fseek(arch, direccion, SEEK_SET);
    fread(&nvo, sizeof(Entidad), 1, arch);
    return nvo;
}

/* Inserta una entidad manteniendo el orden alfabético de la lista ligada */
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

/* Imprime en pantalla todas las entidades registradas recorriendo la lista */
void CDiccionario::consultaEntidades() {
    long aux = getCabeceraEntidades();
    cout << "\n--- LISTA DE ENTIDADES ---" << endl;
    while (aux != -1) {
        Entidad e = leeEntidad(aux);
        cout << "Nombre: " << e.nombre << " | Atr: " << e.atr << " | Data: " << e.data << " | Sig: " << e.sig << endl;
        aux = e.sig;
    }
}

/* Solicita el nombre de una entidad para proceder a su desconexión física */
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

/* Lógica de desconexión de punteros para "eliminar" una entidad de la lista ligada */
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

/* Permite renombrar o actualizar una entidad eliminando la anterior e insertando la nueva */
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
