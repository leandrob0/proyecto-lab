#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>
#include <stdbool.h>
#include <ctype.h>
// #include <carga_arbol.h>
/**
    ESTRUCTURAS Y CONSTANTES A UTILIZAR
*/

#define DICCIONARIO "diccionario.bin"
#define ARCHIVOID "ids.bin"
#define CANT_MAX 200000

typedef struct
{
    char palabra[20];
    int idDOC;
    int pos; // incrementa palabra por palabra, y no letra por letra
} termino;

typedef struct nodoT
{
    int idDOC;
    int pos;
    struct nodoT *sig;
} nodoT;

typedef struct nodoA
{
    char palabra[20];
    int frecuencia;     // representa la cantidad de nodos de la lista
    nodoT *ocurrencias; // ordenada por idDOC, luego por pos
    struct nodoA *der;
    struct nodoA *izq;
} nodoA;

/**
#############################################################

    PROTOTIPADO

#############################################################
*/

nodoT *crearNodoOcurrencias(termino palabra);
nodoA *crearNodoPalabras(char *palabra);
termino agregarTermino(termino t);
void cargarDiccionario(termino arr[], int *validos, char* archivoIds);
void ingresarArbolOrdenado(nodoA **arbolDiccionario, char *palabra);
void cargaDeOcurrencias(nodoA **arbolDiccionario, termino t);
void ingresarOcurrencia(nodoT **listaOcurrencias, termino t);
void pasarTerminosArchivo(termino *terminos, int validos, int idDoc);
void cargarMotorDeBusqueda(char *nombreArchivo, nodoA **lista);
int buscarPalabraEnDiccionario(nodoA *arbolDiccionario, char *palabra);
void mostrarArbol(nodoA *arbol);
void verListaVariosID(nodoT *lista, int* idsArchivo, int validos);
void buscarNodoVariosID(nodoA *arbol, char *palabra, int* idArchivo, int validos);
int retornarIdMayor(char* nombreArchivo);
int verSiYaEstaLaId(int* IDs, int validos, int check);
int pedirVariosID(int id, int* archivoElegido);
void verListaUnID(nodoT *lista, int idArchivo);
void buscarNodoUnID(nodoA *arbol, char *palabra, int idArchivo);
int pedirID();
void pedirUnaPalabra(char* palabra);
int pedirVariasPalabras(char palabras[5][20]);

int menu();
void funcionesMenu(termino *arr, int *validos, nodoA **arbol);

/**
#######################################

    MAIN

#######################################
*/

int main()
{
    nodoA *arbol = NULL;
    termino arr[70000];
    int validos = 0;

    funcionesMenu(arr, &validos, &arbol);
    //cargarDiccionario(arr, &validos);
    //cargarMotorDeBusqueda(DICCIONARIO, &arbol);
    //mostrarArbol(arbol);

    return 0;
}

/**
    FUNCIONES CREAR NODOS
*/

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
void cargarDiccionario(termino arr[], int *validos, char* archivoIds)
{
    system("cls");
    char palabra[20];
    char seguir = 's';
    memset(palabra, 0, sizeof(palabra));
    int flag = 0;
    int cantDoc = 0;
    int pos = 0;
    int i = 0; // para cargar la palabra letra por letra
    int j = 0;
    char letra;

    cantDoc = retornarIdMayor(ARCHIVOID);//lleva la cuenta de cuantos archivos hay para asignarle un id al proximo archivo a cargar.

    if(cantDoc == -1)
    {
        cantDoc = 0;
    }
    else
    {
        cantDoc++; //como la funcion retorna el ultimo id que se agrego, sumo 1 para empezar con el siguiente archivo
    }

    FILE* abrir = fopen(archivoIds, "ab");

    while (seguir == 's')
    {
        system("cls");
        char nombreArchivo[50];
        if (cantDoc > 0 && flag == 0)
        {
            printf("Archivos cargados: %d\n\n", cantDoc);
        }
        if (flag == 1)
        {
            flag = 0;
        }

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
            printf("El archivo no existe. Por favor, vuelva a intentarlo.");
        }
        /// RESETEA LA POSICION AL CAMBIAR DE DOCUMENTO
        if (flag == 0)
        {
            pos = 0;
            pasarTerminosArchivo(arr, *validos, cantDoc);
            fwrite(&cantDoc,sizeof(int),1,abrir);
            cantDoc++;
        }
        printf("cant doc: %d",cantDoc);
        printf("\nCargar otro archivo? S/N: ");
        fflush(stdin);
        scanf("%c", &seguir);

        memset(nombreArchivo, 0, sizeof(nombreArchivo));
    }

    fclose(abrir);
}

///FUNCIONES DE INSERTAR EN LA LISTA|ARBOL
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

///PASA LA LISTA DE TERMINOS A EL ARCHIVO DICCIONARIO
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

///GENERA EL MOTOR DE BUSQUEDA.
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

///FUNCION AUXILIAR
int buscarPalabraEnDiccionario(nodoA *arbolDiccionario, char *palabra)
{
    if (arbolDiccionario != NULL)
    {
        if (strcmpi(arbolDiccionario->palabra, palabra) == 0)
        {
            return 1; // encontrado
        }
        else
        {
            if (strcmpi(arbolDiccionario->palabra, palabra) > 0)
            {
                return buscarPalabraEnDiccionario(arbolDiccionario->izq, palabra);
            }
            else
            {
                return buscarPalabraEnDiccionario(arbolDiccionario->der, palabra);
            }
        }
    }
    else
    {
        return 0;
    }
}

/**
#################################################################################################
    FUNCIONES AUXILIARES MENU
    FUNCIONES AUXILIARES MENU
    FUNCIONES AUXILIARES MENU
#################################################################################################
*/

///FUNCIONES MOSTRAR Y BUSCAR.
void mostrarArbol(nodoA *arbol)
{
    if (arbol != NULL)
    {
        mostrarArbol(arbol->izq);
        printf("%s ", arbol->palabra);
        mostrarArbol(arbol->der);
    }
}

void verListaVariosID(nodoT *lista, int* idsArchivo, int validos)
{

    while (lista != NULL)
    {
        //loopea el arreglo de los ids y ve si alguna es igual a la actual de la lista.
        for(int i = 0; i < validos; i++)
        {
            if(idsArchivo[i] == lista->idDOC)
            {
                printf("Documento ID:%d\n", lista->idDOC);
                printf("posicion:%d\n", lista->pos);
                printf("\n");
            }
        }
        lista = lista->sig;
    }
}

void verListaUnID(nodoT *lista, int idArchivo)
{
    while (lista != NULL)
    {
        if(idArchivo == lista->idDOC)
        {
            printf("Documento ID:%d\n", lista->idDOC);
            printf("posicion:%d\n", lista->pos);
            printf("\n");
        }
        lista = lista->sig;
    }
}

//recibe un arreglo de int en caso de que quiera buscar en varios archivos (varias IDS) y sus validos
void buscarNodoVariosID(nodoA *arbol, char *palabra, int* idArchivo, int validos)
{
    if (arbol)
    {
        if (strcmpi(arbol->palabra, palabra) == 0)
        {
            printf("PALABRA: %s", palabra);
            printf("\n");
            verListaVariosID(arbol->ocurrencias, idArchivo, validos);
        }
        else
        {
            if (strcmpi(arbol->palabra, palabra) > 0)
            {
                buscarNodoVariosID(arbol->izq, palabra, idArchivo, validos);
            }
            else
            {
                buscarNodoVariosID(arbol->der, palabra, idArchivo, validos);
            }
        }
    }
}

void buscarNodoUnID(nodoA *arbol, char *palabra, int idArchivo)
{

    if (arbol)
    {
        if (strcmpi(arbol->palabra, palabra) == 0)
        {
            printf("\nPALABRA: %s", palabra);
            printf("\n");
            verListaUnID(arbol->ocurrencias, idArchivo);
        }
        else
        {
            if (strcmpi(arbol->palabra, palabra) > 0)
            {
                buscarNodoUnID(arbol->izq, palabra, idArchivo);
            }
            else
            {
                buscarNodoUnID(arbol->der, palabra, idArchivo);
            }
        }
    }
}

///FUNCIONES PEDIR IDS Y PALABRAS
int pedirVariosID(int id, int* archivoElegido)
{
    int i = 0;
    char seguir = 's';

    do
    {
        if(i == id + 1)
        {
            printf("Todos los archivos seleccionados.\n");

            system("pause");
            system("cls");

            break;
        }

        int flag = 0;

        do
        {
            int aux = 0;
            printf("Ingrese el ID del archivo en el que desee buscar: ");
            scanf("%i", &aux);

            //checkea que no se pase dos veces la misma ID (bastante inutil ya que va a ser lo mismo pero bueno para hacerlo mas estricto)
            int check = verSiYaEstaLaId(archivoElegido, i + 1, aux);
            if(check == 1)
            {
                flag = 1;
                printf("ID ya ingresada.\n");
                system("pause");
                system("cls");
            }
            else
            {
                archivoElegido[i] = aux;
                flag = 0;
            }
        }
        while(flag == 1 || archivoElegido[i] > id || archivoElegido < 0);

        i++;
        printf("\nDesea elegir otro archivo?(S/N): ");
        fflush(stdin);
        scanf("%c", &seguir);
        system("cls");
    }
    while(tolower(seguir) == 's');

    return i;
}

int pedirID()
{
    int id = retornarIdMayor(ARCHIVOID);
    int aux = -1;

    if(id == -1)
    {
        return aux;
    }

    do
    {
        printf("Ingrese el ID del archivo en el que desee buscar: ");
        scanf("%i",&aux);
    }
    while(aux > id || aux < 0);

    return aux;
}

void pedirUnaPalabra(char* palabra)
{
    do
    {
        printf("\n\nIngrese palabra que desea buscar: ");
        fflush(stdin);
        gets(palabra);
    }
    while(strcmpi(palabra,"") == 0);
}

int pedirVariasPalabras(char palabras[5][20])
{
    int validos = 0;
    char seguir = 's';
    char palabra[20];

    do
    {
        do
        {

            memset(palabra,0,sizeof(palabra));

            printf("Ingrese la palabra que desea buscar: ");
            fflush(stdin);
            gets(palabra);

            strcpy(palabras[validos], palabra);

        }while(strcmpi(palabra,"") == 0);

        validos++;
        if(validos == 5)
        {
            printf("Limite de palabras alcanzado.\n");
            system("pause");
            system("cls");
            break;
        }

        printf("\nDesea cargar otra palabra?(S/N): ");
        fflush(stdin);
        scanf("%c", &seguir);
        system("cls");

    }while(tolower(seguir) == 's');

    return validos;
}

///FUNCIONES AUXILIARES
int retornarIdMayor(char* nombreArchivo)
{
    FILE* fp = fopen(nombreArchivo, "rb");
    int id = 0;
    int aux;

    if(fp != NULL)
    {
        while(fread(&aux,sizeof(int),1,fp) > 0)
        {
            id = aux;
        }
    }
    else
    {
        return -1;
    }

    return id;
}

int verSiYaEstaLaId(int* IDs, int validos, int check)
{
    for(int i = 0; i < validos - 1; i++)
    {
        if(IDs[i] == check)
        {
            return 1;
        }
    }

    return 0;
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

    printf("[1]Escanear archivos\n");
    printf("[2]Ver termino en un solo archivo\n");
    printf("[3]Ver termino en varios archivos\n");
    printf("[4]Buscar mas de un termino en un documento\n");
    printf("[5]Buscar una frase completa\n");
    printf("[6]Buscar palabra que mas se repite en un documento\n");
    printf("[7]buscar palabras similares\n");
    printf("[8]Salir\n");
    printf("Opcion: ");
    scanf("%d", &opcion);
    return opcion;
}

void funcionesMenu(termino *arr, int *validos, nodoA **arbol)
{

    bool repite = true;
    int opcion = 0;
    int id = 0;
    char palabra[20];
    char palabras[5][20];

    FILE* check = fopen(DICCIONARIO, "rb");

    //COMPRUEBA QUE EL ARCHIVO "DICCIONARIO" HAYA SIDO CREADO ANTES DE CARGARLO EN EL MOTOR DE BUSQUEDA.
    if(check != NULL)
    {
        cargarMotorDeBusqueda(DICCIONARIO, arbol);
    }

    fclose(check);

    do
    {
        system("cls");
        opcion = menu();

        int archivoElegido[retornarIdMayor(ARCHIVOID) + 1];// genera un array con el tamanio justo para almanezar todas la ids si el usuario selecciona todos los archivos

        switch (opcion)
        {
        case 1:
            cargarDiccionario(arr, validos, ARCHIVOID); // ojo con cargar muchas veces el archivo, cargarlo 1 vez y comentar esta linea
            cargarMotorDeBusqueda(DICCIONARIO, arbol);
            break;

        case 2:
            system("cls");

            ///COMO ESTA CONSIGNA PIDE SOLO BUSCAR EN UN ARCHIVO LE PIDE AL USUARIO EN CUAL QUIERE BUSCAR
            int id = pedirID();

            if(id == -1)
            {
                printf("No hay archivos agregados.\n");
                system("pause");
                break;
            }

            pedirUnaPalabra(palabra);

            buscarNodoUnID(*arbol, palabra, id);
            memset(palabra, 0, sizeof(palabra));

            system("pause");

            break;

        case 3:
            system("cls");

            id = retornarIdMayor(ARCHIVOID);
            int i = pedirVariosID(id, archivoElegido);

            pedirUnaPalabra(palabra);
            buscarNodoVariosID(*arbol, palabra, archivoElegido, i);

            memset(palabra, 0, sizeof(palabra));
            system("pause");

            break;
        case 4: //buscar varias palabras en UN archivo
            system("cls");

            int validos = pedirVariasPalabras(palabras);
            id = pedirID();

            if(id == -1)
            {
                printf("No hay archivos cargados.\n");
                system("pause");
                break;
            }

            for(int i = 0; i < validos; i++)
            {
                strcpy(palabra,palabras[i]);

                buscarNodoUnID(*arbol,palabra, id);
                system("pause");

                memset(palabra, 0, sizeof(palabra));
            }

            break;

        case 5:
            system("cls");

            break;
        case 6:
            system("cls");

            break;
        case 7:
            system("cls");

            break;
        case 8:
            repite = false;
            break;
        default:
            break;
        }
    }
    while (repite);
}
