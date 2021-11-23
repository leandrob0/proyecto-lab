#include "gotoxy.h"
#include "Usuarios.h"
#include "interfaz.h"


int main()
{
    system("cls");
    system("color 3F ");
    nodoA *arbol = NULL;
    termino arr[70000];
    int validos = 0;

    funcionesMenu(arr, &validos, &arbol);

    return 0;
}

