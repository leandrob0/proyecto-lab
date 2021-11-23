#include "interfaz.h"



void animation()
{

    gotoxy(29, 10);
    printf("******************************************************************\n");
    gotoxy(29, 11);
    printf("****************************CARGANDO******************************\n");
    gotoxy(29, 12);
    printf("******************************************************************\n");

    for (int i = 0; i < 62; i++)
    {
        int cont = i * 1;

        Sleep(010);
        gotoxy(29 + i + 1, 14);
        printf("%c", 219);
        gotoxy(60, 15);
        printf("%%%d", cont + 39);
    }
}
void marcoConsola()
{ // para no andar modificando individualmente en cada menu
    dibujarCuadro(0, 0, 119, 28);
}
void cargandoError()
{ // ANIMATIONS
    system("cls");
    gotoxy(30, 10);
    printf("**************************************************************\n");
    gotoxy(30, 11);
    printf("************************CARGA FALLIDA*************************\n");
    gotoxy(30, 12);
    printf("**************************************************************\n");

    printf("\n\n");
}
void logo()
{

    gotoxy(27, 6);
    printf(" dP88b8 88   88 88     88   88     dP88b8 88   88 88     88   88 ");
    gotoxy(27, 7);
    printf("dP      88   88 88     88   88    dP      88   88 88     88   88 ");
    gotoxy(27, 8);
    printf("Yb  888 Y8   8P 88  oo Y8   8P    Yb  888 Y8   8P 88  oo Y8   8P ");
    gotoxy(27, 9);
    printf(" YboodP  YbodP  88ood8  YbodP      YboodP  YbodP  88ood8  YbodP  ");
}

void copy()
{

    gotoxy(58, 28);
    printf(COPYRIGHT);
}