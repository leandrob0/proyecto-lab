#include "gotoxy.h"
#include "Usuarios.h"

/**
#############################################################

    PROTOTIPADO

#############################################################
*/

nodoT *crearNodoOcurrencias(termino palabra);
nodoA *crearNodoPalabras(char *palabra);
termino agregarTermino(termino t);
void cargarDiccionario(termino arr[], int *validos, char *archivoIds);
void ingresarArbolOrdenado(nodoA **arbolDiccionario, char *palabra);
void cargaDeOcurrencias(nodoA **arbolDiccionario, termino t);
void ingresarOcurrencia(nodoT **listaOcurrencias, termino t);
void pasarTerminosArchivo(termino *terminos, int validos, int idDoc);
void cargarMotorDeBusqueda(char *nombreArchivo, nodoA **lista);

int menu();
void funcionesMenu(termino *arr, int *validos, nodoA **arbol);

///###################################################
///###############    EXTRA    #######################
///###################################################
///###################################################

void animation();
void cargandoError();
void marcoConsola();
void logo();
void copy();
/**
#######################################

    MAIN

#######################################
*/

int main()
{
    system("color 3F ");
    nodoA *arbol = NULL;
    termino arr[70000];
    int validos = 0;

    funcionesMenu(arr, &validos, &arbol);

    return 0;
}

/**
    FUNCIONES CREAR NODOS
*/
///################################################################################
///################################################################################
///##########################       PUNTO 0    ####################################
///################################################################################
///################################################################################

nodoT *crearNodoOcurrencias(termino palabra)
{
    nodoT *nuevo = (nodoT *)malloc(sizeof(nodoT));
    nuevo->idDOC = palabra.idDOC;
    nuevo->pos = palabra.pos;
    nuevo->sig = NULL;

    return nuevo;
}

nodoA *crearNodoPalabras(char *palabra)
{
    nodoA *nuevo = (nodoA *)malloc(sizeof(nodoA));
    nuevo->izq = NULL;
    nuevo->der = NULL;
    nuevo->ocurrencias = NULL;
    nuevo->frecuencia = 0;
    strcpy(nuevo->palabra, palabra);

    return nuevo;
}

termino agregarTermino(termino t)
{
    termino term;
    term = t;

    return term;
}

/// FUNCION PRINCIPAL PARA CARGAR EL DICCIONARIO
void cargarDiccionario(termino arr[], int *validos, char *archivoIds)
{
    char palabra[20];
    char seguir = 's';
    memset(palabra, 0, sizeof(palabra));
    int flag = 0;
    int cantDoc = 0;
    int pos = 0;
    int i = 0; // para cargar la palabra letra por letra
    int j = 0;
    char letra;

    cantDoc = retornarIdMayor(ARCHIVOID); // lleva la cuenta de cuantos archivos hay para asignarle un id al proximo archivo a cargar.

    if (cantDoc == -1)
    {
        cantDoc = 0;
    }
    else
    {
        cantDoc++; // como la funcion retorna el ultimo id que se agrego, sumo 1 para empezar con el siguiente archivo
    }

    FILE *abrir = fopen(archivoIds, "ab");

    while (seguir == 's')
    {
        system("cls");
        marcoConsola();
        copy();
        char nombreArchivo[50];
        if (cantDoc > 0 && flag == 0)
        {
            gotoxy(4, 3);
            printf("Archivos cargados: %d\n\n", cantDoc);
        }
        if (flag == 1)
        {
            flag = 0;
        }
        //
        gotoxy(35, 13);
        printf("nombre del archivo con su extencion: ");
        fflush(stdin);
        gets(nombreArchivo);

        FILE *fp = fopen(nombreArchivo, "rb");

        if (fp != NULL)
        {
            while (fread(&letra, sizeof(char), 1, fp) > 0 && i < 20)
            {
                termino t;

                if (isalpha(letra))
                {
                    palabra[i] = letra;
                    i++;
                }
                else
                {
                    // si la palabra esta vacia, no hace nada
                    if (strcmpi(palabra, "") != 0)
                    {
                        /// copia la palabra en la estructura
                        strcpy(t.palabra, palabra);
                        t.pos = pos;
                        t.idDOC = cantDoc;

                        /// aca cargaria el array de terminos
                        arr[j] = agregarTermino(t);
                        j++;
                        (*validos)++;
                        ///-

                        memset(palabra, 0, sizeof(palabra)); // resetea el array a 0
                        memset(t.palabra, 0, sizeof(t.palabra));

                        pos++;
                        // printf("posicion: %d", pos);
                    }
                    i = 0;
                }
            }

            fclose(fp);
        }
        else
        {
            flag = 1;
            system("cls");
            marcoConsola();
            copy();
            cargandoError();
            gotoxy(35, 13);
            printf("El archivo no existe. Por favor, vuelva a intentarlo.\n\n");
            system("pause");
        }
        /// RESETEA LA POSICION AL CAMBIAR DE DOCUMENTO
        if (flag == 0)
        {
            system("cls");
            marcoConsola();
            copy();
            printf("\n");
            animation();
            printf("\n");
            pos = 0;
            pasarTerminosArchivo(arr, *validos, cantDoc);
            fwrite(&cantDoc, sizeof(int), 1, abrir);
            cantDoc++;
        }
        system("cls");
        marcoConsola();
        copy();
        gotoxy(40, 13);
        printf("%cDesea cargar otro archivo? S/N: ", 168);
        fflush(stdin);
        gotoxy(73, 13);
        scanf("%c", &seguir);

        memset(nombreArchivo, 0, sizeof(nombreArchivo));
    }

    fclose(abrir);
}

/// FUNCIONES DE INSERTAR EN LA LISTA|ARBOL
void ingresarArbolOrdenado(nodoA **arbolDiccionario, char *palabra)
{

    if (*arbolDiccionario == NULL)
    {
        *arbolDiccionario = crearNodoPalabras(palabra);
    }
    else
    {
        if (strcmpi((*arbolDiccionario)->palabra, palabra) > 0)
        {
            ingresarArbolOrdenado(&(*arbolDiccionario)->izq, palabra);
        }
        else
        {
            ingresarArbolOrdenado(&(*arbolDiccionario)->der, palabra);
        }
    }
}

void cargaDeOcurrencias(nodoA **arbolDiccionario, termino t)
{
    if (*arbolDiccionario != NULL)
    {
        if (strcmpi((*arbolDiccionario)->palabra, t.palabra) == 0)
        {
            (*arbolDiccionario)->frecuencia++;
            ingresarOcurrencia(&(*arbolDiccionario)->ocurrencias, t);
        }
        else
        {
            if (strcmpi((*arbolDiccionario)->palabra, t.palabra) > 0)
            {
                cargaDeOcurrencias(&((*arbolDiccionario)->izq), t);
            }
            else
            {
                cargaDeOcurrencias(&((*arbolDiccionario)->der), t);
            }
        }
    }
}

void ingresarOcurrencia(nodoT **listaOcurrencias, termino t)
{
    nodoT *nuevo = crearNodoOcurrencias(t);

    if (*listaOcurrencias == NULL)
    {
        *listaOcurrencias = nuevo;
    }
    else
    {
        nodoT *ant = *listaOcurrencias;
        nodoT *act = (*listaOcurrencias)->sig;

        if ((*listaOcurrencias)->idDOC > t.idDOC || ((*listaOcurrencias)->pos > t.pos && (*listaOcurrencias)->idDOC == t.idDOC))
        {
            nuevo->sig = *listaOcurrencias;
            *listaOcurrencias = nuevo;
        }

        while (act != NULL && (act->idDOC < t.idDOC && (act->pos < t.pos && act->idDOC == t.idDOC)))
        {
            ant = act;
            act = act->sig;
        }

        ant->sig = nuevo;
        nuevo->sig = act;
    }
}

/// PASA LA LISTA DE TERMINOS A EL ARCHIVO DICCIONARIO
void pasarTerminosArchivo(termino *terminos, int validos, int idDoc)
{
    char nombreArchivo[20];
    memset(nombreArchivo, 0, sizeof(nombreArchivo));
    strcpy(nombreArchivo, "diccionario");
    strcat(nombreArchivo, ".bin");

    FILE *fp = fopen(nombreArchivo, "ab");

    if (fp)
    {
        for (int i = 0; i < validos; i++)
        {
            if (terminos[i].idDOC == idDoc)
            {
                fwrite(&terminos[i], sizeof(termino), 1, fp);
            }
        }

        fclose(fp);
    }
}

/// GENERA EL MOTOR DE BUSQUEDA.
void cargarMotorDeBusqueda(char *nombreArchivo, nodoA **lista)
{
    FILE *fp = fopen(nombreArchivo, "rb");
    int found = 0;
    termino t;

    if (fp != NULL)
    {
        while (fread(&t, sizeof(termino), 1, fp) > 0)
        {
            found = buscarPalabraEnDiccionario(*lista, t.palabra);

            if (found == 0) // si la palabra no esta cargada todavia, la inserta en el arbol
            {
                ingresarArbolOrdenado(lista, t.palabra);
            }
            cargaDeOcurrencias(lista, t); // carga la ocurrencia
        }

        fclose(fp);
    }
    else
    {
        printf("Error al abrir el archivo.\n");
    }
}

/**
#################################################################################################
    MENU
    MENU
    MENU
#################################################################################################
*/

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
    int validos = pedirVariasPalabras(palabras);
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
