#include "interfaz.h"


int menu()
{
    int opcion = 0;
    marcoConsola();
    gotoxy(58, 28);
    printf(COPYRIGHT);
    logo();
    dibujarCuadro(32, 16, 88, 26);
    gotoxy(35, 17);
    printf("[1]Escanear archivos\n");
    gotoxy(35, 18);
    printf("[2]Ver termino en un solo archivo\n");
    gotoxy(35, 19);
    printf("[3]Ver termino en varios archivos\n");
    gotoxy(35, 20);
    printf("[4]Buscar mas de un termino en un documento\n");
    gotoxy(35, 21);
    printf("[5]Buscar una frase completa\n");
    gotoxy(35, 22);
    printf("[6]Buscar palabra que mas se repite en un documento\n");
    gotoxy(35, 23);
    printf("[7]buscar palabras similares\n");
    gotoxy(35, 24);
    printf("[8]Salir\n");
    gotoxy(53, 25);
    printf("OPCION: ");
    gotoxy(60, 25);
    scanf("%d", &opcion);
    return opcion;
}
void funcionesMenu(termino *arr, int *validos, nodoA **arbol)
{

    bool repite = true;
    int opcion = 0;
    char palabra[20];
    char palabras[5][20];

    FILE *check = fopen(DICCIONARIO, "rb");

    // COMPRUEBA QUE EL ARCHIVO "DICCIONARIO" HAYA SIDO CREADO ANTES DE CARGARLO EN EL MOTOR DE BUSQUEDA.
    if (check != NULL)
    {
        cargarMotorDeBusqueda(DICCIONARIO, arbol);
    }

    fclose(check);

    do
    {
        system("cls");
        opcion = menu();

        switch (opcion)
        {
        case 1:
            system("cls");

            puntoCero(arr, validos, arbol); // check

            break;

        case 2:
            system("cls");

            puntoUno(*arbol, palabra); // check

            break;

        case 3:
            system("cls");

            puntoDos(*arbol, palabra); // check

            break;
        case 4: // buscar varias palabras en UN archivo
            system("cls");

            puntoTres(*arbol, palabra, palabras); // check

            break;

        case 5:
            system("cls");

            puntoCuatro(*arbol);// check

            break;
        case 6:
            system("cls");

            puntoCinco(*arbol); // check

            break;
        case 7:
            system("cls");

            puntoSeis(*arbol,palabra);

            break;
        case 8:
            repite = false;
            break;
        default:
            repite = false;
            break;
        }
    } while (repite);
}



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