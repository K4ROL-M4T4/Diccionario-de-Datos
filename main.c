#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
//Karol Tovar Mata
//Marian

#define T_CHAR 1
#define T_INT 2
#define T_FLOAT 3
#define T_DOUBLE 4
#define T_LONG 5

void capturar(FILE *f);
void consultar(FILE *f);
void gestionarArchivo(FILE *f);
void limpiarBuffer();

int main() {
    int opc;
    char nombreArchivo[50];
    FILE *f;

    do{
        printf("\nMenu Principal:");
        printf("1: Nuevo\n");
        printf("2: Abrir\n");
        printf("3: Salir\n");
        printf("Elige una opcion: ");
        scanf("%d", &opc);
        limpiarBuffer();

        switch(opc){
            case 1:
                printf("Nombre del archivo a crear: ");
                scanf("%s", nombreArchivo);
                f = fopen(nombreArchivo, "rb");
                if(f != NULL){
                    printf("El archivo ya existe.\n");
                    fclose(f);
                }else{
                    f=fopen(nombreArchivo, "wb+");
                    if(f==NULL){
                        printf("Error al crear el archivo.\n");
                    }else{
                        printf("Archivo creado.\n");
                        gestionarArchivo(f);
                    }
                }
                break;
            case 2:
                printf("Nombre del archivo a abrir: ");
                scanf("%s", nombreArchivo);
                f=fopen(nombreArchivo, "rb+");
                if(f==NULL){
                    printf("El archivo no existe.\n");
                }else{
                    gestionarArchivo(f);
                }
                break;
            case 3:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opcion no valida.\n");
        }
    }while(opc!=3);
    return 0;
}

void gestionarArchivo(FILE *f){
    int subOpc;
    do{
        printf("\nSubmenu\n");
        printf("1: Capturar\n");
        printf("2: Consultar\n");
        printf("3: Regresar al menu anterior\n");
        printf("Opcion: ");
        scanf("%d", &subOpc);
        switch (subOpc){
            case 1:
                capturar(f);
                break;
            case 2:
                consultar(f);
                break;
            case 3:
                fclose(f);
                printf("Archivo cerrado.\n");
                break;
            default:
                printf("Opcion no valida.\n");
        }
    } while(subOpc!=3);
}

void capturar(FILE *f){
    long direccion;
    int tipo;
    printf("\nCapturar datos\n");
    printf("Ingresa la direccion: ");
    scanf("%ld", &direccion);
    if(direccion<0){
        printf("Direccion invalida.\n");
        return;
    }
    fseek(f, direccion, SEEK_SET);
    printf("Tipos: 1-char, 2-int, 3-float, 4-double, 5-long\n");
    printf("Elige el tipo: ");
    scanf("%d", &tipo);
    fwrite(&tipo, sizeof(int), 1, f);
    int i_val;
    float f_val;
    double d_val;
    long l_val;
    char s_val[50];
    int tam_cadena;
    switch (tipo){
        case T_CHAR:
            printf("Tamano de palabra: ");
            scanf("%d", &tam_cadena);
            fwrite(&tam_cadena, sizeof(int), 1, f);
            printf("Ingresa la palabra: ");
            scanf("%s", s_val); 
            fwrite(s_val, sizeof(char), tam_cadena, f);
            break;
        case T_INT:
            printf("Ingresa el entero: ");
            scanf("%d", &i_val);
            fwrite(&i_val, sizeof(int), 1, f);
            break;
        case T_FLOAT:
            printf("Ingresa el float: ");
            scanf("%f", &f_val);
            fwrite(&f_val, sizeof(float), 1, f);
            break;
        case T_DOUBLE:
            printf("Ingresa el double: ");
            scanf("%lf", &d_val);
            fwrite(&d_val, sizeof(double), 1, f);
            break;
        case T_LONG:
            printf("Ingresa el long: ");
            scanf("%ld", &l_val);
            fwrite(&l_val, sizeof(long), 1, f);
            break;
        default:
            printf("Tipo no reconocido.\n");
            break;
    }
    printf("Dato guardado.\n");
}

void consultar(FILE *f){
    long direccion;
    int tipo;
    printf("\nConsultar:\n");
    printf("Ingresa la direccion a leer: ");
    scanf("%ld", &direccion);
    if(direccion<0){
        printf("Direccion invalida.\n");
        return;
    }
    if(fseek(f, direccion, SEEK_SET)!=0){
        printf("Error al buscar la direccion.\n");
        return;
    }
    if (fread(&tipo, sizeof(int),1,f)==0){
        printf("No hay datos en esa direccion o la direccion esta vacia.\n");
        return;
    }
    printf("Tipo detectado: %d. Valor: ", tipo);
    int i_val;
    float f_val;
    double d_val;
    long l_val;
    int tam_cadena;
    char *buffer;
    switch (tipo){
        case T_CHAR:
            fread(&tam_cadena,sizeof(int), 1, f);
            buffer=(char*)malloc(tam_cadena+1);
            fread(buffer,sizeof(char),tam_cadena,f);
            buffer[tam_cadena]='\0';
            printf("%s\n", buffer);
            free(buffer);
            break;
        case T_INT:
            fread(&i_val,sizeof(int),1,f);
            printf("%d\n", i_val);
            break;
        case T_FLOAT:
            fread(&f_val,sizeof(float),1,f);
            printf("%f\n", f_val);
            break;
        case T_DOUBLE:
            fread(&d_val,sizeof(double),1,f);
            printf("%lf\n",d_val);
            break;
        case T_LONG:
            fread(&l_val,sizeof(long),1,f);
            printf("%ld\n", l_val);
            break;
        default:
            printf("Datos corruptos o tipo desconocido.\n");
    }
}

void limpiarBuffer(){
    int c;
    while((c=getchar())!='\n'&&c!=EOF);
}