#include "gotoxy.h"
#include "Usuarios.h"
#include "interfaz.h"

int menu();
void funcionesMenu(termino *arr, int *validos, nodoA **arbol);

///###################################################
///###############    EXTRA    #######################
///###################################################

void animation();
void cargandoError();
void marcoConsola();
void logo();
void copy();


int main()
{
    system("color 3F ");
    nodoA *arbol = NULL;
    termino arr[70000];
    int validos = 0;

    funcionesMenu(arr, &validos, &arbol);

    return 0;
}


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

///
/// FUNCIONES PUNTOS
void puntoCero(termino *arr, int *validos, nodoA **arbol)
{
    marcoConsola();
    cargarDiccionario(arr, validos, ARCHIVOID); // ojo con cargar muchas veces el archivo, cargarlo 1 vez y comentar esta linea
    cargarMotorDeBusqueda(DICCIONARIO, arbol);
}

void puntoUno(nodoA *arbol, char *palabra)
{

    // COMO ESTA CONSIGNA PIDE SOLO BUSCAR EN UN ARCHIVO LE PIDE AL USUARIO EN CUAL QUIERE BUSCAR
    int id = pedirID();

    if (id == -1)
    {
        marcoConsola();
        copy();
        gotoxy(40, 13);
        printf("No hay archivos agregados.\n");
        gotoxy(35, 15);
        system("pause");
        id = 9;
    }
    else
    {
        pedirUnaPalabra(palabra);

        buscarNodoUnID(arbol, palabra, id);
        memset(palabra, 0, sizeof(palabra));

        printf("\n\n\n");
        getch();
    }
}

void puntoDos(nodoA *arbol, char *palabra)
{
    marcoConsola();
    gotoxy(58, 28);
    printf(COPYRIGHT);
    int id = retornarIdMayor(ARCHIVOID);
    int archivoElegido[id + 1];
    int i = pedirVariosID(id, archivoElegido);

    pedirUnaPalabra(palabra);
    buscarNodoVariosID(arbol, palabra, archivoElegido, i);
    memset(palabra, 0, sizeof(palabra));
    printf("\n\n\n\n");
    system("pause");
}

void puntoTres(nodoA *arbol, char *palabra, char palabras[][20])
{
    marcoConsola();
    gotoxy(58, 28);
    printf(COPYRIGHT);
    int validos = pedirVariasPalabras(arbol,palabras);
    int id = pedirID();

    if (id == -1)
    {
        gotoxy(40, 10);
        printf("No hay archivos cargados.");
        system("pause");
    }
    else
    {
        for (int i = 0; i < validos; i++)
        {
            strcpy(palabra, palabras[i]);

            buscarNodoUnID(arbol, palabra, id);
            printf("\n");
            system("\tpause");

            memset(palabra, 0, sizeof(palabra));
        }
    }
}

void puntoCuatro(nodoA *arbol)
{
    marcoConsola();
    gotoxy(58, 28);
    printf(COPYRIGHT);
    buscarUnaFrase(arbol);
}

void puntoCinco(nodoA *arbol)
{
    marcoConsola();
    copy();
    pyf palabrasFrecuencias[3000];
    int validosFrecuencias = 0;
    int id = retornarIdMayor(ARCHIVOID) + 1;
    int idDOC = 0;

    dibujarCuadro(3, 2, 20, 4); // el marco de ingresar ID
    gotoxy(4, 3);
    printf("Ingrese ID: ");
    scanf("%d", &idDOC);

    if (idDOC < id)
    {
        palabrasYFrecuencias(arbol, idDOC, palabrasFrecuencias, &validosFrecuencias);
        pyf max = palabrasFrecuencias[1];

        for (int i = 2; i <= validosFrecuencias; i++)
        {
            if (palabrasFrecuencias[i].frecuencia > max.frecuencia)
            {
                max = palabrasFrecuencias[i];
            }
        }
        dibujarCuadro(23, 11, 90, 13); // el marco de el id y palabra y frecuencia
        gotoxy(24, 12);
        printf("ID DOC:%i" ,idDOC);
        gotoxy(38, 12);
        printf("Palabra:%s",max.palabra);
        gotoxy(65, 12);
        printf("FRECUENCIA:%i",  max.frecuencia);
        memset(palabrasFrecuencias, 0, sizeof(palabrasFrecuencias));
        validosFrecuencias = 0;
    }
    else
    {
        system("cls");
            marcoConsola();
    copy();
        gotoxy(38,10);
        printf("El ID no existe!.\n");
    }
    gotoxy(3, 27);
    system("pause");
}

void puntoSeis(nodoA *arbol, char *palabra)
{
    marcoConsola();
    copy();

    dibujarCuadro(3, 3, 32, 5);
    gotoxy(4, 4);
    printf("Palabra: ");
    fflush(stdin);
    gets(palabra);
    printf("\n");

    buscarPalabrasSimilares(arbol, palabra);

    system("pause");
}

/// FIN FUNCIONES POR PUNTOS

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

