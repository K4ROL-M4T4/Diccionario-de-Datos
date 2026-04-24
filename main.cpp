//Vega Torres Marian
//Tovar Mata Karol
#include "CDiccionario.h"
void menuEntidades(CDiccionario &dic) {
    int op;
    do {
        cout << "\nMENU DE ENTIDADES" << endl;
        cout << "1. Nueva Entidad\n2. Consultar Entidades\n3. Eliminar Entidad\n4. Modificar Entidad\n5. Menu Atributos\n6. Regresar" << endl;
        cout << "Seleccione una opcion: ";
        cin >> op;

        switch(op) {
            case 1: dic.altaEntidad(); break;
            case 2: dic.consultaEntidades(); break;
            case 3: dic.bajaEntidad(); break;
            case 4: dic.modificaEntidad(); break;
            case 5: menuAtributos(); break;
        }
    } while (op != 6);
}

int main() {
    CDiccionario gestor;
    menuEntidades(gestor);
    return 0;
}