//Vega Torres Marian
//Tovar Mata Karol

#include "CDiccionario.h"
int main()
{
    CDiccionario *DB;
    DB = new CDiccionario();
    
    DB->menuPrincipal();
    
    delete DB;
    return 0;
}