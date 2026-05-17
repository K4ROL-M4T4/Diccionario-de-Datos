#include "CDiccionario.h"

// Descripción: Constructor de la clase. Inicializa los punteros y variables, y lanza el menú principal.
CDiccionario::CDiccionario(){
    archivo = NULL;
    nAtributos = 0;
    tambloque = 0;
    menuPrincipal();
}

// Descripción: Muestra las opciones principales para crear, abrir o salir del sistema de diccionarios.
void CDiccionario::menuPrincipal(){
    int op;
    do{
        printf("\n========================================\n");
        printf("           MENU PRINCIPAL\n");
        printf("========================================\n");
        printf("1. Nuevo diccionario\n");
        printf("2. Abrir diccionario\n");
        printf("3. Salir\n");
        printf("Elija una opcion: ");
        scanf("%d", &op);
        switch(op){
            case 1: nuevoDiccionario(); break;
            case 2: abrirDiccionario(); break;
            case 3: printf("\nCerrando sistema...\n"); break;
            default: printf("\nError: Opcion no valida.\n");
        }
    }while(op != 3);
}

// Descripción: Pide el nombre de un archivo nuevo. Si no existe, lo crea y le inserta la cabecera nula (-1).
void CDiccionario::nuevoDiccionario(){
    char nombre[50];
    printf("\nNombre del nuevo diccionario: ");
    scanf("%s", nombre);
    archivo = fopen(nombre, "rb");
    if(archivo){
        printf("\nError: El archivo ya existe. Intente abrirlo desde el menu.\n");
        fclose(archivo);
    }else{
        printf("\nCreando archivo...");
        archivo = fopen(nombre, "wb+");
        if(!archivo){
            printf("\nError al intentar crear el archivo.\n");
            return;
        }
        printf("\nArchivo '%s' creado correctamente.\n", nombre);
        escribeCabEntidades(-1);
        MenuEntidades();
        fclose(archivo);
    }
}

// Descripción: Abre un archivo binario existente en modo lectura/escritura y despliega el menú de entidades.
void CDiccionario::abrirDiccionario(){
    char nombre[50];
    printf("\nNombre del archivo a abrir: ");
    scanf("%s", nombre);
    archivo = fopen(nombre, "rb+");
    if(archivo != NULL){
        printf("\nArchivo abierto correctamente.\n");
        MenuEntidades();
        fclose(archivo);
    }else{
        printf("\nError: No existe un archivo con ese nombre.\n");
    }
}

// Descripción: Menú secundario para la gestión integral de entidades (tablas).
void CDiccionario::MenuEntidades(){
    int op;
    do{
        printf("\n========================================\n");
        printf("           MENU DE ENTIDADES\n");
        printf("========================================\n");
        printf("1. Nueva Entidad\n");
        printf("2. Consultar Entidades\n");
        printf("3. Eliminar Entidad\n");
        printf("4. Modificar Entidad\n");
        printf("5. Gestionar Atributos (Columnas)\n");
        printf("6. Gestionar Bloques (Datos)\n");
        printf("7. Regresar al Menu Principal\n");
        printf("Elija una opcion: ");
        scanf("%d", &op);
        switch(op){
            case 1: altaEntidad(); break;
            case 2: consultarEntidades(); break;
            case 3: bajaEntidad(); break;
            case 4: modificaEntidad(); break;
            case 5: menuAtributos(); break;
            case 6: menuBloques(); break;
            case 7: printf("\nRegresando...\n"); break;
            default: printf("\nError: Opcion no valida.\n");
        }
    }while(op != 7);
}

// ---------------- ATRIBUTOS ----------------

// Descripción: Menú para administrar los atributos de la entidad seleccionada.
void CDiccionario::menuAtributos(){
    if(!pideEntidad()) return;

    int op;
    do{
        printf("\n========================================\n");
        printf("     ATRIBUTOS DE LA ENTIDAD: %s\n", activa.nombre);
        printf("========================================\n");
        printf("1. Nuevo Atributo\n");
        printf("2. Consultar Atributos\n");
        printf("3. Eliminar Atributo\n");
        printf("4. Modificar Atributo\n");
        printf("5. Regresar\n");
        printf("Elija una opcion: ");
        scanf("%d", &op);
        switch(op){
            case 1: nuevoAtributo(); break;
            case 2: consultarAtributo(); break;
            case 3: bajaAtributo(); break;
            case 4: modificaAtributo(); break;
            case 5: printf("\nRegresando...\n"); break;
            default: printf("\nError: Opcion no valida.\n");
        }
    }while(op != 5);
}

// ---------------- BLOQUES ----------------

// Descripción: Menú para la administración e ingreso de registros (bloques) en una entidad.
void CDiccionario::menuBloques(){
    if(!pideEntidad()){
        return;
    }
    if(!cargaAtributos()){
        return;
    }

    int op;
    do{
        printf("\n========================================\n");
        printf("      BLOQUES DE LA ENTIDAD: %s\n", activa.nombre);
        printf("========================================\n");
        printf("1. Nuevo Bloque (Registro)\n");
        printf("2. Consultar Todos los Bloques\n");
        printf("3. Eliminar Bloque \n");//En proceso
        printf("4. Modificar Bloque \n");//En proceso
        printf("5. Regresar\n");
        printf("Elija una opcion: ");
        scanf("%d", &op);
        switch(op){
            case 1: altaBloque(); break;
            case 2: ConsultaBloque(); break;
            case 3: printf("\nEliminar bloque \n"); break;//en proceso
            case 4: printf("\nModificar bloque \n"); break;//en proceso
            case 5: printf("\nRegresando...\n"); break;
            default: printf("\nError: Opcion no valida.\n");
        }
    }while(op != 5);
}

// Descripción: Escribe la cabecera principal de las entidades al inicio del archivo (byte 0).
void CDiccionario::escribeCabEntidades(long cab){
    fseek(archivo, 0, SEEK_SET);
    fwrite(&cab, sizeof(long), 1, archivo);
}

// Descripción: Orquesta la captura, escritura en archivo e inserción en la lista ligada de una nueva entidad.
void CDiccionario::altaEntidad(){
    long dir;
    Entidad nueva = capturaEntidad();

    if(buscaEntidad(nueva) == -1){
        dir = escribeEntidad(nueva);
        insertaEntidad(nueva, dir);
        printf("\nEntidad '%s' registrada correctamente.\n", nueva.nombre);
    }else{
        printf("\nError: La entidad '%s' ya existe.\n", nueva.nombre);
    }
}

// Descripción: Solicita el nombre de una nueva entidad y formatea sus apuntadores por defecto en -1.
Entidad CDiccionario::capturaEntidad(){
    Entidad ent;
    printf("\nNombre de la nueva entidad: ");
    scanf(" %[^\n]", ent.nombre);
    ent.atr = -1;
    ent.sig = -1;
    ent.data = -1;
    return ent;
}

// Descripción: Recorre la lista ligada de entidades buscando coincidencias por nombre. Regresa su dirección.
long CDiccionario::buscaEntidad(Entidad ent){
    long cab = getCabEntidades();
    Entidad actual;

    while(cab != -1){
        actual = leeEntidad(cab);
        if(strcmp(actual.nombre, ent.nombre) == 0)
            return cab;
        cab = actual.sig;
    }
    return -1;
}

// Descripción: Añade la estructura de una entidad al final del archivo binario y retorna el offset donde cayó.
long CDiccionario::escribeEntidad(Entidad ent){
    long dir;
    fseek(archivo, 0, SEEK_END);
    dir = ftell(archivo);
    fwrite(&ent, sizeof(Entidad), 1, archivo);
    return dir;
}

// Descripción: Lee una estructura Entidad desde una posición física específica del archivo.
Entidad CDiccionario::leeEntidad(long dir){
    Entidad nvo;
    fseek(archivo, dir, SEEK_SET);
    fread(&nvo, sizeof(Entidad), 1, archivo);
    return nvo;
}

// Descripción: Conecta la nueva entidad en la lista ligada en disco, manteniéndola en orden alfabético.
void CDiccionario::insertaEntidad(Entidad nvo, long dir){
    Entidad act, ant;
    long cab = getCabEntidades(), dirant;

    if(cab == -1){
        cab = dir;
        escribeCabEntidades(cab);
    }else{
        act = leeEntidad(cab);
        if(strcmp(act.nombre, nvo.nombre) > 0){
            nvo.sig = cab;
            reescribeEntidad(dir, nvo);
            escribeCabEntidades(dir);
        }else{
            while(cab != -1 && strcmp(nvo.nombre, act.nombre) > 0){
                dirant = cab;
                ant = act;
                cab = act.sig;
                if(cab != -1) act = leeEntidad(cab);
            }
            nvo.sig = cab;
            reescribeEntidad(dir, nvo);
            ant.sig = dir;
            reescribeEntidad(dirant, ant);
        }
    }
}

// Descripción: Sobrescribe la estructura de una entidad ya existente en una posición específica del disco.
void CDiccionario::reescribeEntidad(long dir, Entidad ent){
    fseek(archivo, dir, SEEK_SET);
    fwrite(&ent, sizeof(Entidad), 1, archivo);
}

// Descripción: Interfaz para pedir el nombre de la entidad a eliminar e invocar el proceso lógico.
void CDiccionario::bajaEntidad(){
    long dir;
    char nom[30];

    printf("\nNombre de la entidad a eliminar: ");
    scanf("%s", nom);

    Entidad aux;
    strcpy(aux.nombre, nom);
    dir = buscaEntidad(aux);

    if(dir == -1) {
        printf("\nError: La entidad '%s' no existe.\n", nom);
    } else {
        eliminaEntidad(nom);
        printf("\nEntidad '%s' eliminada del sistema.\n", nom);
    }
}

// Descripción: Desenlaza una entidad de la lista ligada haciendo que el nodo anterior apunte al siguiente.
long CDiccionario::eliminaEntidad(cadena nom){
    Entidad ant, le;
    long dirant, cab;

    cab = getCabEntidades();
    le = leeEntidad(cab);

    if(strcmp(nom, le.nombre) == 0){
        escribeCabEntidades(le.sig);
        return cab;
    }else{
        while(cab != -1 && strcmp(nom, le.nombre) != 0){
            dirant = cab;
            ant = le;
            cab = le.sig;
            if(cab != -1) le = leeEntidad(cab);
        }
        if(strcmp(le.nombre, nom) == 0){
            ant.sig = le.sig;
            reescribeEntidad(dirant, ant);
            return cab;
        }else return -1;
    }
}

// Descripción: Elimina logicamente la entidad anterior y reinserta una nueva versión con los datos actualizados.
void CDiccionario::modificaEntidad(){
    Entidad nueva, aux;
    long dir;

    printf("\nNombre de la entidad que desea modificar: ");
    scanf(" %[^\n]", aux.nombre);

    if(buscaEntidad(aux) != -1){
        printf("\n--- Captura de Nuevos Datos ---");
        nueva = capturaEntidad();
        if(buscaEntidad(nueva) == -1){
            dir = eliminaEntidad(aux.nombre);
            reescribeEntidad(dir, nueva);
            insertaEntidad(nueva, dir);
            printf("\nOperacion exitosa: Entidad modificada correctamente.\n");
        }else {
            printf("\nError: No se puede modificar porque el nuevo nombre ya esta ocupado.\n");
        }
    }else {
        printf("\nError: La entidad a modificar no existe.\n");
    }
}

// Descripción: Lee y retorna la posición del archivo apuntada en el bloque 0 (cabecera global de entidades).
long CDiccionario::getCabEntidades(){
    long dir;
    fseek(archivo, 0, SEEK_SET);
    fread(&dir, sizeof(long), 1, archivo);
    return dir;
}

// Descripción: Extrae y lista las entidades.
void CDiccionario::consultarEntidades(){
    Entidad actual;
    long cab = getCabEntidades();
    int contador = 1;

    printf("\n\t=== LISTA DE ENTIDADES ===\n");
    if(cab == -1){
        printf("\nNo hay entidades registradas en el sistema.\n");
        return;
    }

    while(cab != -1){
        actual = leeEntidad(cab);
        printf("\n----------------------------------------\n");
        printf(" ENTIDAD #%d: %s\n", contador, actual.nombre);
        printf("----------------------------------------\n");
        printf(" Dir. Atributos: %ld | Dir. Datos: %ld | Dir. Siguiente: %ld\n", actual.atr, actual.data, actual.sig);

        cab = actual.sig;
        contador++;
    }
  }


// ====================== FUNCIONES NUEVAS DE ATRIBUTOS ====================== //

// Descripción: Interfaz requerida para elegir sobre qué entidad se trabajarán los atributos y bloques.
bool CDiccionario::pideEntidad(){
    if(getCabEntidades() == -1){
        printf("\nError: No hay entidades registradas en el sistema.\n");
        return false;
    }
    Entidad temp;
    printf("\n¿En cual entidad desea trabajar?: ");
    scanf(" %[^\n]", temp.nombre);

    long d = buscaEntidad(temp);
    if(d != -1){
        diractiva = d;
        activa = leeEntidad(d);
        printf("\nEntidad '%s' activada.\n", activa.nombre);
        return true;
    }
    printf("\nError: La entidad ingresada no existe.\n");
    return false;
}

// Descripción: Descarga a memoria RAM la estructura Atributo desde la ubicación física dada.
Atributo CDiccionario::leeAtributo(long dir){
    Atributo nvo;
    fseek(archivo, dir, SEEK_SET);
    fread(&nvo, sizeof(Atributo), 1, archivo);
    return nvo;
}

// Descripción: Plasma los bytes de un atributo nuevo al final del documento binario.
long CDiccionario::escribeAtributo(Atributo atr){
    long dir;
    fseek(archivo, 0, SEEK_END);
    dir = ftell(archivo);
    fwrite(&atr, sizeof(Atributo), 1, archivo);
    return dir;
}

// Descripción: Sustituye en disco un atributo existente por una versión actualizada en el mismo byte.
void CDiccionario::reescribeAtributo(long dir, Atributo atr){
    fseek(archivo, dir, SEEK_SET);
    fwrite(&atr, sizeof(Atributo), 1, archivo);
}

// Descripción: Pide y procesa interactivamente los tipos de datos, tamaños y restricciones para crear una columna.
Atributo CDiccionario::capturaAtributo(){
    Atributo nuevaA;
    printf("\nNombre del atributo: ");
    scanf("%s", nuevaA.nombre);

    printf("Tipo de dato:\n  1)char\n  2)int\n  3)float\n  4)double\n  5)long\nSeleccion: ");
    scanf("%d", &nuevaA.tipo);

    if(nuevaA.tipo == 1){
        printf("¿Cuantas letras (tamano)?: ");
        scanf("%d", &nuevaA.tamano);
    }else{
        switch(nuevaA.tipo){
            case 2: nuevaA.tamano = sizeof(int); break;
            case 3: nuevaA.tamano = sizeof(float); break;
            case 4: nuevaA.tamano = sizeof(double); break;
            case 5: nuevaA.tamano = sizeof(long); break;
        }
    }

    // Validación de nulidad dependiente de si es Clave Primaria o no
    printf("¿Este atributo es Clave Primaria? (S/N): ");
    scanf(" %c", &nuevaA.lskp);

    if(nuevaA.lskp == 'S' || nuevaA.lskp == 's'){
        nuevaA.nulo = 'N';
        printf("Nota: Clave Primaria, NO permite Nulos.\n");
    } else {
        printf("¿Permite Nulos? (S/N): ");
        scanf(" %c", &nuevaA.nulo);
    }

    printf("Descripcion corta: ");
    scanf(" %[^\n]", nuevaA.descripcion);

    nuevaA.sig = -1;
    return nuevaA;
}

// Descripción: Gestiona el alta de un atributo validando primero que no esté duplicado en la entidad activa.
void CDiccionario::nuevoAtributo(){
    Atributo nuevoA = capturaAtributo();
    if(buscaAtributo(nuevoA.nombre) == -1){
        long dir = escribeAtributo(nuevoA);
        insertaAtributo(nuevoA, dir);
        printf("\nAtributo '%s' guardado correctamente.\n", nuevoA.nombre);
    }else{
        printf("\nError: Ese atributo ya existe en esta entidad.\n");
    }
}

// Descripción: Acomoda y enlaza un atributo a la sublista de la entidad activa ordenando alfabéticamente.
void CDiccionario::insertaAtributo(Atributo nvo, long dir){
    if(activa.atr == -1){
        activa.atr = dir;
        reescribeEntidad(diractiva, activa);
    }else{
        Atributo actual = leeAtributo(activa.atr);
        if(strcmp(actual.nombre, nvo.nombre) > 0){
            nvo.sig = activa.atr;
            reescribeAtributo(dir, nvo);
            activa.atr = dir;
            reescribeEntidad(diractiva, activa);
        }else{
            long cab = activa.atr;
            long dirAnt;
            Atributo atrAnt;
            while(cab != -1 && strcmp(nvo.nombre, actual.nombre) > 0){
                dirAnt = cab;
                atrAnt = actual;
                cab = actual.sig;
                if(cab != -1) actual = leeAtributo(cab);
            }
            if(cab != -1){
                nvo.sig = cab;
                reescribeAtributo(dir, nvo);
            }
            atrAnt.sig = dir;
            reescribeAtributo(dirAnt, atrAnt);
        }
    }
}

// Descripción: Recorre y evalúa la lista ligada de atributos hasta encontrar uno que coincida por nombre.
long CDiccionario::buscaAtributo(char *atr){
    long cab = activa.atr;
    Atributo actual;
    while(cab != -1){
        actual = leeAtributo(cab);
        if(strcmp(actual.nombre, atr) == 0){
            return cab;
        }
        cab = actual.sig;
    }
    return -1;
}

// Descripción: Imprime el esquema de columnas de la entidad activa.
void CDiccionario::consultarAtributo(){
    Atributo nvo;
    long cab = activa.atr;
    int contador = 1;

    printf("\n\t=== ATRIBUTOS DE: %s ===\n", activa.nombre);
    if(cab == -1){
        printf("\nNo hay atributos registrados en esta entidad.\n");
        return;
    }

    while(cab != -1){
        nvo = leeAtributo(cab);
        printf("\n----------------------------------------\n");
        printf(" ATRIBUTO #%d: %s\n", contador, nvo.nombre);
        printf("----------------------------------------\n");
        printf(" Tipo de dato: %d | Tamano: %d bytes\n", nvo.tipo, nvo.tamano);
        printf(" Clave Primaria (PK): %c | Permite Nulos: %c\n", nvo.lskp, nvo.nulo);
        printf(" Descripcion: %s\n", nvo.descripcion);

        cab = nvo.sig;
        contador++;
    }
}

// Descripción: Controla la búsqueda y ejecución de la baja de un atributo por nombre.
void CDiccionario::bajaAtributo(){
    long dir;
    char nombre[30];
    printf("\nNombre del atributo a eliminar: ");
    scanf("%s", nombre);

    dir = buscaAtributo(nombre);
    if(dir == -1){
        printf("\nError: El atributo no existe en la entidad activa.\n");
    } else {
        eliminaAtributo(nombre);
        printf("\nAtributo eliminado correctamente.\n");
    }
}

// Descripción: Salta el nodo especificado en la lista de atributos para desvincularlo sin corromper el archivo.
long CDiccionario::eliminaAtributo(char cad[30]){
    long cab = activa.atr;
    long dirant = -1;
    Atributo ant, actual;

    actual = leeAtributo(cab);
    if(strcmp(actual.nombre, cad) == 0){
        activa.atr = actual.sig;
        reescribeEntidad(diractiva, activa);
    }
    else {
        while(cab != -1 && strcmp(actual.nombre, cad) < 0){
            dirant = cab;
            ant = actual;
            cab = actual.sig;
            if(cab != -1){
                actual = leeAtributo(cab);
            }
        }
        if(strcmp(cad, actual.nombre) == 0){
            ant.sig = actual.sig;
            reescribeAtributo(dirant, ant);
        }
    }
    return cab;
}

// Descripción: Renueva los parámetros de configuración de un atributo reemplazándolo con una captura nueva.
void CDiccionario::modificaAtributo(){
   Atributo nvo, aux;
   long dir, dir2;

   printf("\nNombre del atributo a modificar: ");
   scanf("%s", aux.nombre);

   if(buscaAtributo(aux.nombre) != -1) {
       printf("\n--- Captura de Nueva Informacion ---\n");
       nvo = capturaAtributo();
       dir2 = buscaAtributo(nvo.nombre);

       if(strcmp(nvo.nombre, aux.nombre) == 0 || dir2 == -1) {
           dir = eliminaAtributo(aux.nombre);
           reescribeAtributo(dir, nvo);
           insertaAtributo(nvo, dir);
           printf("\nAtributo modificado correctamente.\n");
       } else {
           printf("\nError: No se pudo modificar porque ya existe un atributo con ese nuevo nombre.\n");
       }
   } else {
       printf("\nError: No existe el atributo a modificar.\n");
   }
}

// Descripción: Precarga el esquema, valida PK única, hace offset de puntero y suma dinámicamente los tamaños.
bool CDiccionario::cargaAtributos() {
    if(activa.atr == -1){
        printf("\nError: La entidad '%s' no tiene atributos. Debe crear columnas primero.\n", activa.nombre);
        return false;
    }

    long cab = activa.atr;
    Atributo actual;
    int contPK = 0;
    int cont = 0;

    tambloque = sizeof(long);

    while(cab != -1){
        actual = leeAtributo(cab);
        arrAtributo[cont] = actual;
        tambloque += actual.tamano;

        if(actual.lskp == 's' || actual.lskp == 'S'){
            contPK++;
        }

        cont++;
        cab = actual.sig;
    }

    nAtributos = cont;

    // Validación estricta: No se admiten tablas sin PK o con multiples PK.
    if(contPK != 1){
        printf("\nError: Carga cancelada. Se detectaron %d Claves Primarias y debe haber exactamente 1.\n", contPK);
        return false;
    }

    // Acomodo forzado de la PK hacia el indice 0 del arreglo para el comparador.
    for(int i = 0; i < nAtributos; i++){
        if(arrAtributo[i].lskp == 's' || arrAtributo[i].lskp == 'S'){
            if(i != 0){
                Atributo temp = arrAtributo[0];
                arrAtributo[0] = arrAtributo[i];
                arrAtributo[i] = temp;
            }
            break;
        }
    }

    return true;
}

//---------------------- Funciones de Bloques ----------------------

// Descripción: Analiza e infiere de qué tipo es la Clave Primaria para resolver si un bloque es mayor o menor a otro.
float CDiccionario::comparaBloques(void *b1, void *b2){
    long desp = sizeof(long);
    switch (arrAtributo[0].tipo){
        case 1: // char
            return strcmp((char*)b1 + desp, (char*)b2 + desp);
        case 2: // int
            return *(int*)((char*)b1 + desp) - *(int*)((char*)b2 + desp);
        case 3: // float
            return *(float*)((char*)b1 + desp) - *(float*)((char*)b2 + desp);
        case 4: // double
            return *(double*)((char*)b1 + desp) - *(double*)((char*)b2 + desp);
        case 5: // long
            return *(long*)((char*)b1 + desp) - *(long*)((char*)b2 + desp);
        default:
            printf("\nError: Tipo de dato PK desconocido.\n");
            return 0;
    }
}

// Descripción: Navega en el sistema de disco contrastando la PK buscada contra los registros guardados.
long CDiccionario::buscaBloque(void *buscado) {
    long cab = activa.data;
    void *actual = NULL;

    while (cab != -1) {
        actual = LeeBloque(cab);
        if (comparaBloques(actual, buscado) == 0) {
            free(actual);
            return cab;
        }
        long sig = *(long*)actual;
        free(actual);
        cab = sig;
    }
    return -1;
}

// Descripción: Crea el malloc e itera sobre los atributos pidiendo al usuario el valor exacto para su tipo.
void* CDiccionario::CapturaBloque() {
    void *bloque = malloc(tambloque);
    long desp = sizeof(long);
    *(long*)bloque = -1;

    printf("\n--- Captura de Datos ---\n");
    for(int i = 0; i < nAtributos; i++) {
        printf("Ingrese %s (%s): ", arrAtributo[i].nombre, arrAtributo[i].descripcion);

        switch(arrAtributo[i].tipo) {
            case 1: // char
                scanf(" %[^\n]", (char*)((char*)bloque + desp));
                break;
            case 2: // int
                scanf("%d", (int*)((char*)bloque + desp));
                break;
            case 3: // float
                scanf("%f", (float*)((char*)bloque + desp));
                break;
            case 4: // double
                scanf("%lf", (double*)((char*)bloque + desp));
                break;
            case 5: // long
                scanf("%ld", (long*)((char*)bloque + desp));
                break;
        }
        desp += arrAtributo[i].tamano;
    }
    return bloque;
}

// Descripción: Orquesta la recolección, revisión de unicidad, inserción y notificaciones de alta de datos.
void CDiccionario::altaBloque() {
    if(!cargaAtributos()) return;

    void *nuevo = CapturaBloque();

    if (buscaBloque(nuevo) == -1) {
        long dir = EscribeBloque(nuevo);
        InsertaBloque(nuevo, dir);
        printf("\nRegistro guardado correctamente.\n");
    } else {
        printf("\nError: Ya existe un registro con esa misma Clave Primaria.\n");
        free(nuevo);
    }
}

// Descripción: Transfiere un bloque de datos brutos del disco hacia la memoria principal RAM.
void *CDiccionario :: LeeBloque(long dir) {
    void *bloque = malloc(tambloque);
    fseek(archivo, dir, SEEK_SET);
    fread(bloque, tambloque, 1, archivo);
    return bloque;
}

// Descripción: Coloca la información consolidada en un malloc hacia el final del archivo físico.
long CDiccionario :: EscribeBloque(void *bloque) {
    long dir;
    fseek(archivo, 0, SEEK_END);
    dir = ftell(archivo);
    fwrite(bloque, tambloque, 1, archivo);
    return dir;
}

// Descripción: Impacta un bloque desde RAM directo al espacio físico indicado en disco para modificaciones.
void CDiccionario :: ReescribeBloque(long dir, void *bloque) {
    fseek(archivo, dir, SEEK_SET);
    fwrite(bloque, tambloque, 1, archivo);
}

// Descripción: Conecta la memoria dinámica del bloque nuevo dentro de la cadena lógica usando la PK como medida de ordenación.
void CDiccionario :: InsertaBloque(void *nuevo, long dirnvo) {
    if (activa.data == -1) {
        activa.data = dirnvo;
        reescribeEntidad(diractiva, activa);
    } else {
        void *actual = LeeBloque(activa.data);

        if (comparaBloques(actual, nuevo) > 0) {
            // --- INSERCION AL INICIO ---
            *(long*)nuevo = activa.data;
            ReescribeBloque(dirnvo, nuevo);
            activa.data = dirnvo;
            reescribeEntidad(diractiva, activa);
            free(actual);
        } else {
            long cab = activa.data;
            long dirAnt = -1;
            void *bloqueAnt = NULL;

            while (cab != -1 && comparaBloques(nuevo, actual) > 0) {
                dirAnt = cab;
                if(bloqueAnt != NULL) free(bloqueAnt);
                bloqueAnt = actual;
                cab = *(long*)actual;
                if (cab != -1) {
                    actual = LeeBloque(cab);
                }
            }

            // --- INSERCION EN MEDIO O AL FINAL ---
            if (cab != -1) {
                *(long*)nuevo = cab;
                ReescribeBloque(dirnvo, nuevo);
                free(actual);
            } else {
                *(long*)nuevo = -1;
                ReescribeBloque(dirnvo, nuevo);
            }

            *(long*)bloqueAnt = dirnvo;
            ReescribeBloque(dirAnt, bloqueAnt);
            free(bloqueAnt);
        }
    }
}

// Descripción: Consulta los bloques
void CDiccionario :: ConsultaBloque() {
    printf("\n\t=== REGISTROS DE %s ===\n", activa.nombre);

    void *bloque;
    long cab = activa.data;
    int numeroFila = 1;

    if (cab == -1) {
        printf("\nNo hay registros guardados en esta entidad todavia.\n");
        return;
    }

    while (cab != -1) {
        bloque = LeeBloque(cab);

        // Encabezado de la Ficha
        printf("\n----------------------------------------\n");
        printf(" REGISTRO #%d \n", numeroFila);
        printf(" Dir. Logica: %ld | Siguiente: %ld\n", cab, *(long*)bloque);
        printf("----------------------------------------\n");

        long desp = sizeof(long);
        for (int i = 0; i < nAtributos; i++) {
            printf(" %s: ", arrAtributo[i].nombre);

            switch (arrAtributo[i].tipo) {
                case 1: // char
                    printf("%s\n", (char *)((char*)bloque + desp));
                    break;
                case 2: // int
                    printf("%d\n", *(int *)((char*)bloque + desp));
                    break;
                case 3: // float
                    printf("%.2f\n", *(float *)((char*)bloque + desp));
                    break;
                case 4: // double
                    printf("%.2lf\n", *(double *)((char*)bloque + desp));
                    break;
                case 5: // long
                    printf("%ld\n", *(long *)((char*)bloque + desp));
                    break;
            }
            desp += arrAtributo[i].tamano;
        }

        cab = *(long*)bloque;
        free(bloque);
        numeroFila++;
    }
    printf("\n");
}
