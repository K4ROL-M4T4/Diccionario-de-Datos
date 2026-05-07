#include "CDiccionario.h"
//Marian, revisa esto o me mato
//Por esta que ya no voy a faltar
CDiccionario::CDiccionario(){
	archivo = NULL;
	nAtributos = 0;
	tambloque = 0;
    menuPrincipal();
}

void CDiccionario::menuPrincipal(){
    int op;
    do{
        printf("\n¿Que desea hacer?\n1.Nuevo diccionario\n2.Abrir diccionario\n3.Salir\n");
        scanf("%d", &op);
        switch(op){
            case 1:
                nuevoDiccionario();
                break;
            case 2:
                abrirDiccionario();
                break;
            case 3:
                printf("\nSaliendo");
                break;
            default:
                printf("\nNo es una opcion, brother");
        }
    }while(op != 3);
}

void CDiccionario::nuevoDiccionario(){
    char nombre[50];
    printf("\nNombre del nuevo diccionario: ");
    scanf("%s", nombre);
    archivo = fopen(nombre, "rb");
    if(archivo)
    {
        printf("\nEl archivo ya existe\n");
        fclose(archivo);
    }else{
        printf("\nEl archivo no existe, crearemos uno.");
        archivo = fopen(nombre, "wb+");
        if(!archivo)
        {
            printf("\nError al crear el archivo.");
            return;
        }
        printf("\nArchivo %s creado correctamente.\n", nombre);
        escribeCabEntidades(-1);
        MenuEntidades();
        fclose(archivo);
    }
}

void CDiccionario::abrirDiccionario(){
    char nombre[50];
    printf("\nNombre del archivo: ");
    scanf("%s", nombre);
    archivo=fopen(nombre, "rb+");
    if(archivo != NULL) {
        printf("\nArchivo abierto correctamente");
        MenuEntidades();
        fclose(archivo);
    }
    else{
        printf("\nNo existe el archivo");
    }
}



void CDiccionario::MenuEntidades(){
    int op;
    do{
        printf("\n1.Nueva---2.Consultar---3.Eliminar---4.Modificar---5.Atributos---6.Datos---7.Regresar\n");
        scanf("%d", &op);
        switch(op){
            case 1: altaEntidad(); break;
            case 2: consultarEntidades(); break;
            case 3: bajaEntidad(); break;
            case 4: modificaEntidad(); break;
            case 5: menuAtributos(); break;
            case 6: menuDatos(); break;
            case 7: printf("\nRegresando..."); break;
            default: printf("\nOpcion no valida");
        }

    } while(op != 7);
}

//ATRIBUTOS
void CDiccionario::menuAtributos(){
    int op;
    do{
        printf("\n1.Nuevo 2.Consultar 3.Eliminar 4.Modificar 5.Regresar\n");
        scanf("%d", &op);
        switch(op)
        {
            case 1: nuevoAtributo(); break;
            case 2: consultarAtributo(); break;
            case 3: eliminaAtributo(); break;
            case 4: modificaAtributo(); break;
            case 5: printf("\nRegresando..."); break;
            default: printf("\nOpcion no valida");
        }
    } while(op != 5);
}

//DATOS
void CDiccionario::menuDatos()
{
    int op;
    do{
        printf("\n1.Nuevo 2.Consultar 3.Eliminar 4.Modificar 5.Regresar\n");
        scanf("%d", &op);
        switch(op)
        {
            case 1: nuevoRegistro(); break;
            case 2: consultarRegistro(); break;
            case 3: eliminaRegistro(); break;
            case 4: modificaRegistro(); break;
            case 5: printf("\nRegresando..."); break;
            default: printf("\nOpcion no valida");
        }

    } while(op != 5);
}

void CDiccionario::escribeCabEntidades(long cab){
	fseek(archivo, 0, SEEK_SET);
	fwrite(&cab, sizeof(long), 1, archivo);
}

void CDiccionario::altaEntidad(){
	long dir;
	Entidad nueva = capturaEntidad();
	if(buscaEntidad(nueva) == -1){
		dir = escribeEntidad(nueva);
		insertaEntidad(nueva, dir);
	} else
		printf("Error: La entidad ya existe");
}

Entidad CDiccionario::capturaEntidad()
{
    Entidad ent;
    printf("\nNombre de la entidad: ");
    scanf(" %[^\n]", ent.nombre);
    ent.atr = -1;
    ent.sig = -1;
    ent.data = -1;
    return ent;
}

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

long CDiccionario::escribeEntidad(Entidad ent){
	long dir;

	fseek(archivo, 0, SEEK_END);
	dir = ftell(archivo);
	fwrite(&ent, sizeof(Entidad), 1, archivo);

	return dir;
}

Entidad CDiccionario::leeEntidad(long dir)
{
	Entidad nvo;
	fseek(archivo, dir, SEEK_SET);
	fread(&nvo, sizeof(Entidad), 1, archivo);
	return nvo;
}

void CDiccionario::insertaEntidad(Entidad nvo, long dir)
{
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
		}else
		{
			while(cab != -1 && strcmp(nvo.nombre, act.nombre) > 0){
				dirant = cab;
				ant = act;
				cab = act.sig;
				if(cab != -1)
					act = leeEntidad(cab);
			}
			nvo.sig = cab;
			reescribeEntidad(dir, nvo);
			ant.sig = dir;
			reescribeEntidad(dirant, ant);
		}
	}
}

void CDiccionario::reescribeEntidad(long dir, Entidad ent){
    fseek(archivo, dir, SEEK_SET);
    fwrite(&ent, sizeof(Entidad), 1, archivo);
}

void CDiccionario::bajaEntidad()
{
	long dir;
	char nom[30];
	printf("Ingrese nombre: ");
	scanf("%s", nom);
	Entidad aux;
	strcpy(aux.nombre, nom);
	dir = buscaEntidad(aux);
	if(dir == -1)
		printf("Error");
	else
		eliminaEntidad(nom);
}

long CDiccionario::eliminaEntidad(cadena nom)
{
	Entidad ant, le;
	long dirant, cab;
	cab = getCabEntidades();
	le = leeEntidad(cab);
	if(strcmp(nom, le.nombre) == 0)
	{
		escribeCabEntidades(le.sig);
		return cab;
	}
	else{
		while(cab != -1 && strcmp(nom, le.nombre) != 0){
			dirant = cab;
			ant = le;
			cab = le.sig;
			if(cab != -1)
				le = leeEntidad(cab);
		}
		if(strcmp(le.nombre, nom) == 0)
		{
			ant.sig = le.sig;
			reescribeEntidad(dirant, ant);
			return cab;
		}
		else
			return -1;
	}
}

void CDiccionario::modificaEntidad(){
	Entidad nueva, aux;
	long dir;
	printf("¿Que entidad desea modificar? ");
	scanf(" %[^\n]",aux.nombre);
	if(buscaEntidad(aux) != -1){
		printf("Ingrese la nueva info: ");
		nueva = capturaEntidad();
		if(buscaEntidad(nueva) == -1){
			dir = eliminaEntidad(aux.nombre);
			reescribeEntidad(dir, nueva);
			insertaEntidad(nueva, dir);
		} else
			printf("No se puede actualizar.");
	} else
		printf("No existe la entidad.");
}

//REGISTROS
void CDiccionario::nuevoRegistro()
{
    if(!archivo){
        printf("\nPrimero crea o abre un archivo");
        return;
    }

    int total = (TAM*sizeof(char)) + sizeof(int) + sizeof(float) + sizeof(double) + sizeof(long);
    void *paciente = malloc(total);
    char *ptr = (char*)paciente;
    int cont = 0;
    printf("\nNombre: ");
    scanf(" %[^\n]", ptr + cont);
    cont += TAM;
    printf("Edad: ");
    scanf("%d", (int*)(ptr+cont));
    cont += sizeof(int);
    printf("Peso: ");
    scanf("%f", (float*)(ptr+cont));
    cont += sizeof(float);
    printf("Estatura: ");
    scanf("%lf", (double*)(ptr+cont));
    cont += sizeof(double);
    printf("ID: ");
    scanf("%ld", (long*)(ptr+cont));
    fseek(archivo, 0, SEEK_END);
    fwrite(paciente, total, 1, archivo);
    printf("\nRegistro guardado");
    free(paciente);
}

void CDiccionario::consultarRegistro(){
    if(!archivo)
    {
        printf("\nPrimero abre un archivo");
        return;
    }
    long pos;
    int total = (TAM*sizeof(char)) + sizeof(int) + sizeof(float) + sizeof(double) + sizeof(long);
    void *paciente = malloc(total);
    printf("\nPosicion: ");
    scanf("%ld", &pos);
    fseek(archivo, pos, SEEK_SET);
    if(fread(paciente, total, 1, archivo) != 1)
    {
        printf("\nNo hay datos");
        free(paciente);
        return;
    }
    char *ptr = (char*)paciente;
    int cont = 0;
    printf("\nNombre: %s", ptr);
    cont += TAM;
    printf("\nEdad: %d", *(int*)(ptr+cont));
    cont += sizeof(int);
    printf("\nPeso: %.2f", *(float*)(ptr+cont));
    cont += sizeof(float);
    printf("\nEstatura: %.2lf", *(double*)(ptr+cont));
    cont += sizeof(double);
    printf("\nID: %ld", *(long*)(ptr+cont));
    free(paciente);
}

long CDiccionario::getCabEntidades()
{
	long dir;
	fseek(archivo, 0, SEEK_SET);
	fread(&dir, sizeof(long), 1, archivo);
	return dir;
}

void CDiccionario::consultarEntidades()
{
    Entidad actual;
    long cab = getCabEntidades();
    while(cab != -1)
    {
        actual = leeEntidad(cab);
        printf("\n%s|%ld|%ld|%ld\n", actual.nombre, actual.atr, actual.data, actual.sig);
        cab = actual.sig;
    }
}

void CDiccionario::nuevoAtributo()
{
   printf("\nNuevo atributo...");
}

void CDiccionario::consultarAtributo()
{
  printf("\nConsultar atributo...");
}

void CDiccionario::eliminaAtributo()
{
   printf("\nEliminar atributo...");
}

void CDiccionario::modificaAtributo()
{
   printf("\nModificar atributo...");
}

void CDiccionario::eliminaRegistro()
{
   printf("\nEliminar registro...");
}

void CDiccionario::modificaRegistro()
{
   printf("\nModificar registro...");
}