#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>
#include <stdbool.h>
#include <ctype.h>
#include <alg.h>
#include "gotoxy.h"
/**
    ESTRUCTURAS Y CONSTANTES A UTILIZAR
*/
#define COPYRIGHT "| Copyright 2021  Leandro Bovino - Jonathan Alvarez -UTNMDP |"
#define DICCIONARIO "diccionario.bin"
#define ARCHIVOID "ids.bin"
/**
    FIN ESTRUCTURAS Y CONSTANTES A UTILIZAR
*/

typedef struct
{
    char palabra[20];
    int frecuencia;
} pyf; // palabra y frecuencia

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
void cargarDiccionario(termino arr[], int *validos, char *archivoIds);
void ingresarArbolOrdenado(nodoA **arbolDiccionario, char *palabra);
void cargaDeOcurrencias(nodoA **arbolDiccionario, termino t);
void ingresarOcurrencia(nodoT **listaOcurrencias, termino t);
void pasarTerminosArchivo(termino *terminos, int validos, int idDoc);
void cargarMotorDeBusqueda(char *nombreArchivo, nodoA **lista);
int buscarPalabraEnDiccionario(nodoA *arbolDiccionario, char *palabra);
void mostrarArbol(nodoA *arbol);
void verListaVariosID(nodoT *lista, int *idsArchivo, int validos);
void buscarNodoVariosID(nodoA *arbol, char *palabra, int *idArchivo, int validos);
int retornarIdMayor(char *nombreArchivo);
int verSiYaEstaLaId(int *IDs, int validos, int check);
int pedirVariosID(int id, int *archivoElegido);
void verListaUnID(nodoT *lista, int idArchivo);
void buscarNodoUnID(nodoA *arbol, char *palabra, int idArchivo);
int pedirID();
int pedirVariasPalabras(char palabras[5][20]);
void pedirUnaPalabra(char *palabra);
int buscarNodoUnIDYRetornar(nodoA *arbol, char *palabra, int idArchivo, int *posiciones);
int buscarPosicionesFrase(nodoT *lista, int idArchivo, int *posiciones);
int verSiSeEncontroLaFrase(int validos, int posicionesFrase[][200], int *validosPosiciones);
void buscarUnaFrase(nodoA *arbol);

int sumaIdDoc(nodoT *lista, int id);                                                            // punto 5
void palabrasYFrecuencias(nodoA *arbol, int idArchivo, pyf *palabrasFrecuencias, int *validos); // punto 5

int Levenshtein(char *s1, char *s2);                       // punto 6
void buscarPalabrasSimilares(nodoA *arbol, char *palabra); // punto 6

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
///##########################       PUNTO 1    ####################################
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
///################################################################################
///################################################################################
///##########################   FIN PUNTO 1    ####################################
///################################################################################
///################################################################################

///################################################################################
///################################################################################
///##########################    PUNTO 2       ####################################
///################################################################################
///################################################################################
/// FUNCION AUXILIAR
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

/// FUNCIONES MOSTRAR Y BUSCAR.
void mostrarArbol(nodoA *arbol)
{
    if (arbol != NULL)
    {
        mostrarArbol(arbol->izq);
        printf("%s ", arbol->palabra);
        mostrarArbol(arbol->der);
    }
}

void verListaVariosID(nodoT *lista, int *idsArchivo, int validos)
{

    while (lista != NULL)
    {
        // loopea el arreglo de los ids y ve si alguna es igual a la actual de la lista.
        for (int i = 0; i < validos; i++)
        {
            if (idsArchivo[i] == lista->idDOC)
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
    int i = 1;
    dibujarCuadro(1,3,50, 5);
    gotoxy(32, 4);printf("Documento ID:%d\n", lista->idDOC);
    gotoxy(2, 6);printf("posicion: ");
    while (lista != NULL)
    {
        if (idArchivo == lista->idDOC)
        {
            printf("%d |", lista->pos);
        }
        if(i == 15){
            printf("\n");
            printf("\t    ");
            i = 0;
        }


        lista = lista->sig;
        i++;
    }


}

// recibe un arreglo de int en caso de que quiera buscar en varios archivos (varias IDS) y sus validos
void buscarNodoVariosID(nodoA *arbol, char *palabra, int *idArchivo, int validos)
{
    if (arbol)
    {
        if (strcmpi(arbol->palabra, palabra) == 0)
        {
            printf("PALABRA: %s", palabra);


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
            gotoxy(2, 4);printf("PALABRA: %s", palabra);

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

/// FUNCIONES PEDIR IDS Y PALABRAS
int pedirVariosID(int id, int *archivoElegido)
{
    int i = 0;
    char seguir = 's';

    do
    {
        if (i == id + 1)
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

            // checkea que no se pase dos veces la misma ID (bastante inutil ya que va a ser lo mismo pero bueno para hacerlo mas estricto)
            int check = verSiYaEstaLaId(archivoElegido, i + 1, aux);
            if (check == 1)
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
        } while (flag == 1 || archivoElegido[i] > id || archivoElegido < 0);

        i++;
        system("cls");
        printf("\nDesea elegir otro archivo?(S/N): ");
        fflush(stdin);
        scanf("%c", &seguir);
        system("cls");
    } while (tolower(seguir) == 's');

    return i;
}

int pedirID()
{
    int id = retornarIdMayor(ARCHIVOID);
    int aux = -1;

    if (id == -1)
    {
        return aux;
    }

    do
    {
        gotoxy(35, 13);
        printf("Ingrese el ID del archivo en el que desee buscar: ");
        scanf("%i", &aux);
    } while (aux > id || aux < 0);

    return aux;
}

void pedirUnaPalabra(char *palabra)
{
    do
    {
        system("cls");
        marcoConsola();
        copy();
        gotoxy(5, 2);
        printf("Ingrese palabra que desea buscar: ");
        fflush(stdin);
        gets(palabra);
    } while (strcmpi(palabra, "") == 0);
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

            memset(palabra, 0, sizeof(palabra));

            printf("Ingrese la palabra que desea buscar: ");
            fflush(stdin);
            gets(palabra);

            strcpy(palabras[validos], palabra);

        } while (strcmpi(palabra, "") == 0);

        validos++;
        if (validos == 5)
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

    } while (tolower(seguir) == 's');

    return validos;
}

/// FUNCIONES AUXILIARES
int retornarIdMayor(char *nombreArchivo)
{
    FILE *fp = fopen(nombreArchivo, "rb");
    int id = 0;
    int aux;

    if (fp != NULL)
    {
        while (fread(&aux, sizeof(int), 1, fp) > 0)
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

int verSiYaEstaLaId(int *IDs, int validos, int check)
{
    for (int i = 0; i < validos - 1; i++)
    {
        if (IDs[i] == check)
        {
            return 1;
        }
    }

    return 0;
}

/**#################################################################################################
    PUNTO 4
    PUNTO 4
    PUNTO 4
#################################################################################################*/

int buscarNodoUnIDYRetornar(nodoA *arbol, char *palabra, int idArchivo, int *posiciones)
{
    int numero;

    if (arbol)
    {
        if (strcmpi(arbol->palabra, palabra) == 0)
        {
            numero = buscarPosicionesFrase(arbol->ocurrencias, idArchivo, posiciones);
        }
        else
        {
            if (strcmpi(arbol->palabra, palabra) > 0)
            {
                return buscarNodoUnIDYRetornar(arbol->izq, palabra, idArchivo, posiciones);
            }
            else
            {
                return buscarNodoUnIDYRetornar(arbol->der, palabra, idArchivo, posiciones);
            }
        }
    }
    else
    {
        numero = 0;
    }

    return numero;
}

// llena el array con las posiciones de la palabra de la frase que pasemos por parametro para despues buscar.
int buscarPosicionesFrase(nodoT *lista, int idArchivo, int *posiciones)
{
    int validos = 0;

    while (lista != NULL)
    {
        if (idArchivo == lista->idDOC)
        {
            posiciones[validos] = lista->pos;
            validos++;
        }
        lista = lista->sig;
    }

    return validos;
}

// FRASE      =     PALABRA[i]    |   PALABRA[i+1]    ...

// POSICIONES       2400 *               600
//                  1000                 2110
//                  451                  2401 *
//
//  * es lo que va a buscar esta funcion, loopeando por cada palabra en el array 2d y buscando si la posicion es 1 mas que la palabra anterior

int verSiSeEncontroLaFrase(int validos, int posicionesFrase[][200], int *validosPosiciones)
{
    int found;

    for (int row = 0; row < validos - 1; row++) // primera capa para una palabra
    {
        found = 0;
        for (int col = 0; (col < validosPosiciones[row]) && found == 0; col++) // segunda capa para la columna de posiciones
        {
            for (int colResto = 0; (colResto < validosPosiciones[row + 1]) && found == 0; colResto++) // tercera capa para las columnas de la siguiente palabra
            {
                if (posicionesFrase[row][col] + 1 == posicionesFrase[row + 1][colResto])
                {
                    found = 1;
                }
            }
        }
        if (found == 0) // si despues de haber escaneado toda la columna de la palabra y de su palabra siguiente, no encontro una posicion que le siga a la primera palabra, sale y retorna 0 (no encontrada);
        {
            break;
        }
    }

    return found;
}

void buscarUnaFrase(nodoA *arbol)
{
    char frase[100];
    char palabras[10][50];
    int j = 0;
    int ctrl = 0;

    printf("Ingrese la frase que desea buscar: ");
    fflush(stdin);
    fgets(frase, sizeof(frase), stdin);

    /// separa la frase en palabras.
    for (int i = 0; i <= strlen(frase); i++)
    {
        // si encuentra un espacio o un caracter null le pone el caracter null a la palabra y busca otra.
        if ((frase[i] == ' ' && frase[i - 1] != ' ') || (frase[i] == '\0' && frase[i - 1] != ' '))
        {
            palabras[ctrl][j] = '\0';
            ctrl++; // siguiente palabra
            j = 0;  // inicia el index en 0 para la siguiente palabra
        }
        else if (frase[i] != ' ' && frase[i] != '\0')
        {
            palabras[ctrl][j] = frase[i];
            j++;
        }
    }

    palabras[ctrl - 1][strlen(palabras[ctrl - 1]) - 1] = '\0'; // al parecer al final de la separacion inserta una nueva linea, asi que la saco. (estuve 10 anios haciendo esto)

    int posiciones[200];
    int posicionesFrase[ctrl][200];
    int validosPosiciones[ctrl];
    int validos = 0;
    int check = 0;
    int found = 0;
    int ids = retornarIdMayor(ARCHIVOID) + 1;

    /// BUSCA EN CADA UNO DE LOS ARCHIVOS SI ESTA LA FRASE.
    for (int i = 0; i < ids; i++)
    {
        memset(posicionesFrase, 0, sizeof(posicionesFrase));
        memset(validosPosiciones, 0, sizeof(validosPosiciones));

        for (int z = 0; z < ctrl; z++)
        {
            /// HAY QUE VER PORQUE LA ULTIMA PALABRA SIEMPRE DA 0 HAY COMO UN SALTO DE LINEA RARO
            validos = buscarNodoUnIDYRetornar(arbol, palabras[z], i, posiciones); // retorna la cantidad de posiciones de la palabra

            if (validos != 0)
            { // significa que encontro almenos una ocurrencia de la palabra en el documento y llena un array de posiciones

                for (int x = 0; x < validos; x++) // llenamos el arreglo 2d con las posiciones de la palabra
                {
                    posicionesFrase[z][x] = posiciones[x];
                }

                validosPosiciones[z] = validos;
                memset(posiciones, 0, sizeof(posiciones));
            }
        }
        check = verSiSeEncontroLaFrase(ctrl, posicionesFrase, validosPosiciones); // busca la frase con los arrays cargados antes

        if (check == 1)
        {
            found = 1;
            printf("La frase fue encontrada en el documento con ID: %i\n", i);
            system("pause");
        }
    }

    if (found == 0)
    {
        printf("La frase no fue encontrada en ningun documento.\n");
        system("pause");
    }
}

/**
#################################################################################################
    FIN PUNTO 4
    FIN PUNTO 4
    FIN PUNTO 4
#################################################################################################
*/

/*#################################################################################################
    PUNTO 5
    PUNTO 5
    PUNTO 5
#################################################################################################*/

int sumaIdDoc(nodoT *lista, int id)
{
    int i = 0;
    while (lista != NULL)
    {
        if (lista->idDOC == id)
        {
            i++;
        }
        lista = lista->sig;
    }

    return i;
}

/// 0 QUEDA VACIO
void palabrasYFrecuencias(nodoA *arbol, int idArchivo, pyf *palabrasFrecuencias, int *validos)
{
    if (arbol != NULL)
    {
        int freq = sumaIdDoc(arbol->ocurrencias, idArchivo);
        palabrasYFrecuencias(arbol->izq, idArchivo, palabrasFrecuencias, validos);
        palabrasYFrecuencias(arbol->der, idArchivo, palabrasFrecuencias, validos);
        (*validos)++;

        strcpy(palabrasFrecuencias[*validos].palabra, arbol->palabra);
        palabrasFrecuencias[*validos].frecuencia = freq;
    }
}

/*#################################################################################################
    FIN PUINTO 5
    FIN PUINTO 5
    FIN PUINTO 5
#################################################################################################*/
/*#################################################################################################
    PUINTO 6
    PUINTO 6
    PUINTO 6
#################################################################################################*/

int Levenshtein(char *s1, char *s2)
{
    int t1, t2, i, j, *m, costo, res, ancho;

    // Calcula tamanios strings
    t1 = strlen(s1);
    t2 = strlen(s2);

    // Verifica que exista algo que comparar
    if (t1 == 0)
        return (t2);
    if (t2 == 0)
        return (t1);
    ancho = t1 + 1;

    // Reserva matriz con malloc                     m[i,j] = m[j*ancho+i] !!
    m = (int *)malloc(sizeof(int) * (t1 + 1) * (t2 + 1));
    if (m == NULL)
        return (-1); // ERROR!!

    // Rellena primera fila y primera columna
    for (i = 0; i <= t1; i++)
        m[i] = i;
    for (j = 0; j <= t2; j++)
        m[j * ancho] = j;

    // Recorremos resto de la matriz llenando pesos
    for (i = 1; i <= t1; i++)
        for (j = 1; j <= t2; j++)
        {
            if (s1[i - 1] == s2[j - 1])
                costo = 0;
            else
                costo = 1;
            m[j * ancho + i] = min(min(m[j * ancho + i - 1] + 1, m[(j - 1) * ancho + i] + 1), m[(j - 1) * ancho + i - 1] + costo);
        } // Sustitucion

    // Devolvemos esquina final de la matriz
    res = m[t2 * ancho + t1];
    free(m);
    return (res);
}
void buscarPalabrasSimilares(nodoA *arbol, char *palabra)
{

    int distancia = 0;
    if (arbol)
    {
        distancia = Levenshtein(palabra, arbol->palabra);

        if (distancia <= 3)
        {
            printf("%s\n\n", arbol->palabra);
        }
        else
        {
            buscarPalabrasSimilares(arbol->izq, palabra);
            buscarPalabrasSimilares(arbol->der, palabra);
        }
    }
}

/*#################################################################################################
   FIN PUINTO 6
   FIN PUINTO 6
   FIN PUINTO 6
#################################################################################################*/

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
///FUNCIONES PUNTOS
void puntoCero(termino *arr, int *validos, nodoA **arbol)
{
    marcoConsola();
    cargarDiccionario(arr, validos, ARCHIVOID); // ojo con cargar muchas veces el archivo, cargarlo 1 vez y comentar esta linea
    cargarMotorDeBusqueda(DICCIONARIO, arbol);
}

void puntoUno(nodoA *arbol, char *palabra)
{
    marcoConsola();
    copy();
    // COMO ESTA CONSIGNA PIDE SOLO BUSCAR EN UN ARCHIVO LE PIDE AL USUARIO EN CUAL QUIERE BUSCAR
    int id = pedirID();

    if (id == -1)
    {
        gotoxy(40, 13);
        printf("No hay archivos agregados.\n");
        gotoxy(35, 15);
        system("pause");

    } else {
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
    system("pause");
}

void puntoTres(nodoA *arbol, char *palabra,char palabras[][20])
{
    marcoConsola();
    gotoxy(58, 28);
    printf(COPYRIGHT);
    int validos = pedirVariasPalabras(palabras);
    int id = pedirID();

    if (id == -1)
    {
        printf("No hay archivos cargados.\n");
        system("pause");
    } else {
        for (int i = 0; i < validos; i++)
        {
            strcpy(palabra, palabras[i]);

            buscarNodoUnID(arbol, palabra, id);
            system("pause");

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
    gotoxy(58, 28);
    printf(COPYRIGHT);
    pyf palabrasFrecuencias[3000];
    int validosFrecuencias = 0;
    int id = retornarIdMayor(ARCHIVOID) + 1;
    int idDOC = 0;

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
        printf("ID DOC %i\nPalabra: %s\nFRECUENCIA: %i\n\n", idDOC, max.palabra, max.frecuencia);
        memset(palabrasFrecuencias, 0, sizeof(palabrasFrecuencias));
        validosFrecuencias = 0;
    }
    else
    {
        printf("El ID no existe!.\n");
    }

    system("pause");
}

void puntoSeis(nodoA *arbol, char *palabra)
{
    marcoConsola();
    gotoxy(58, 28);
    printf(COPYRIGHT);

    printf("Palabra: ");
    fflush(stdin);
    gets(palabra);
    printf("\n");

    buscarPalabrasSimilares(arbol, palabra);

    system("pause");
}

///FIN FUNCIONES POR PUNTOS

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

        int archivoElegido[retornarIdMayor(ARCHIVOID) + 1]; // genera un array con el tamanio justo para almanezar todas la ids si el usuario selecciona todos los archivos

        switch (opcion)
        {
        case 1:
            system("cls");

            puntoCero(arr, validos, arbol);

            break;

        case 2:
            system("cls");

            puntoUno(*arbol, palabra);

            break;

        case 3:
            system("cls");

            puntoDos(*arbol, palabra);

            break;
        case 4: // buscar varias palabras en UN archivo
            system("cls");

            puntoTres(*arbol, palabra,palabras);

            break;

        case 5:
            system("cls");

            puntoCuatro(*arbol);

            break;
        case 6:
            system("cls");

            puntoCinco(*arbol);

            break;
        case 7:
            system("cls");

            puntoSeis(&arbol, palabra);

            break;
        case 8:
            repite = false;
            break;
        default:
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
