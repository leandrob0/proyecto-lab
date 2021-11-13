#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <windows.h>
#include <ctype.h>
// #include <carga_arbol.h>
/**
    ESTRUCTURAS Y CONSTANTES A UTILIZAR
*/

#define BABEL "la_biblioteca_de_babel.txt"
#define FUNES "funes_el_memorioso.txt"

typedef struct
{
    char palabra[20];
    int idDOC;
    int pos; //incrementa palabra por palabra, y no letra por letra
} termino;

typedef struct nodoT
{
    int idDOC;
    int pos;
    struct nodoT* sig;
} nodoT;

typedef struct nodoA
{
    char palabra[20];
    int frecuencia; //representa la cantidad de nodos de la lista
    nodoT* ocurrencias; //ordenada por idDOC, luego por pos
    struct nodoA* der;
    struct nodoA* izq;
} nodoA;

/**
    PROTOTIPADO
*/

nodoT* crearNodoOcurrencias(termino palabra);
nodoA* crearNodoPalabras(char* palabra);
void cargarDiccionario(nodoA** arbolDiccionario);
int buscarPalabraEnDiccionario(nodoA* arbolDiccionario, char* palabra);
void ingresarArbolOrdenado(nodoA** arbolDiccionario, char* palabra);
void cargaDeOcurrencias(nodoA** arbolDiccionario, termino t);
void ingresarOcurrencia(nodoT** listaOcurrencias, termino t);

/**
#######################################
    MAIN
#######################################
*/

int main()
{
    nodoA* arbol;
    cargarDiccionario(&arbol);

    return 0;
}

/**
    FUNCIONES CREAR NODOS
*/

nodoT* crearNodoOcurrencias(termino palabra)
{
    nodoT* nuevo = (nodoT*)malloc(sizeof(nodoT));
    nuevo->idDOC = palabra.idDOC;
    nuevo->pos = palabra.pos;
    nuevo->sig = NULL;

    return nuevo;
}

nodoA* crearNodoPalabras(char* palabra)
{
    nodoA* nuevo = (nodoA*)malloc(sizeof(nodoA));
    nuevo->izq = NULL;
    nuevo->der = NULL;
    nuevo->ocurrencias = NULL;
    nuevo->frecuencia = 0;
    strcpy(nuevo->palabra, palabra);

    return nuevo;
}

///Cargar el diccionario con los datos de los archivos
void cargarDiccionario(nodoA** arbolDiccionario)
{
    char palabra[20];
    memset(palabra, 0, sizeof(palabra));

    int cantDoc = 0;
    int pos = 0;
    int i = 0; //para cargar la palabra letra por letra
    char letra;

    while(cantDoc < 2)
    {
        char nombreArchivo[50];

        if(cantDoc == 0)
        {
            strcpy(nombreArchivo, BABEL);
        } else
        {
            memset(nombreArchivo,0,sizeof(nombreArchivo));
            strcpy(nombreArchivo, FUNES);
        }

        FILE* fp = fopen(nombreArchivo, "rb");

        if(fp != NULL)
        {
            while(fread(&letra,sizeof(char),1,fp) > 0 && i < 20)
            {
                termino t;
                // int esLetra = verificarLetra(letra);

                if(isalpha(letra))
                {
                    palabra[i] = letra;
                    i++;
                }
                else
                {
                    //si la palabra esta vacia, no hace nada
                    if(strcmpi(palabra, "") != 0)
                    {
                        ///copia la palabra en la estructura
                        strcpy(t.palabra, palabra);
                        t.pos = pos;
                        t.idDOC = 0;
                        printf("%s ", t.palabra);

                        ///aca cargaria el array de terminos
                        ///-

                        memset(palabra, 0, sizeof(palabra)); //resetea el array a 0
                        memset(t.palabra, 0, sizeof(t.palabra));

                        pos++;
                    }
                    i = 0;
                }
            }

            fclose(fp);
        }
        cantDoc++;
    }
}

///FUNCION QUE SE FIJA SI EL TERMINO YA ESTA AGREGADO EN EL ARBOL. retorna 1 si lo encontro y 0 si no
int buscarPalabraEnDiccionario(nodoA* arbolDiccionario, char* palabra)
{
    int encontrado = 0;

    if(arbolDiccionario != NULL)
    {
        if(strcmpi(arbolDiccionario->palabra, palabra) == 0)
        {
            encontrado = 1; // encontrado
        }
        else
        {
            if(strcmpi(arbolDiccionario->palabra, palabra) > 0)
            {
                encontrado = buscarPalabraEnDiccionario(arbolDiccionario->izq, palabra);
            }
            else
            {
                encontrado = buscarPalabraEnDiccionario(arbolDiccionario->der, palabra);
            }
        }
    }

    return encontrado;
}

///INGRESA EL NODO EN EL ARBOL ORDENADO ALFABETICAMENTE
void ingresarArbolOrdenado(nodoA** arbolDiccionario, char* palabra)
{
    if(*arbolDiccionario == NULL)
    {
        *arbolDiccionario = crearNodoPalabras(palabra);
    }
    else
    {
        if(strcmpi((*arbolDiccionario)->palabra, palabra) > 0)
        {
            ingresarArbolOrdenado(&(*arbolDiccionario)->izq, palabra);
        }
        else
        {
            ingresarArbolOrdenado(&(*arbolDiccionario)->der, palabra);
        }
    }
}

///CARGAR EN LA LISTA DE OCURRENCIAS DE EL NODO SELECCIONADO
void cargaDeOcurrencias(nodoA** arbolDiccionario, termino t)
{
    if(*arbolDiccionario != NULL)
    {
        if(strcmpi((*arbolDiccionario)->palabra, t.palabra) == 0)
        {
            (*arbolDiccionario)->frecuencia++;
            ingresarOcurrencia(&(*arbolDiccionario)->ocurrencias, t);
        }
        else
        {
            if(strcmpi((*arbolDiccionario)->palabra, t.palabra) > 0)
            {
                cargaDeOcurrencias(&(*arbolDiccionario)->izq, t);
            }
            else
            {
                cargaDeOcurrencias(&(*arbolDiccionario)->der, t);
            }
        }
    }
}

void ingresarOcurrencia(nodoT** listaOcurrencias, termino t)
{
    if(*listaOcurrencias == NULL)
    {
        *listaOcurrencias = crearNodoOcurrencias(t);
    }
    else
    {
        nodoT* aux = *listaOcurrencias;

        while(aux->sig != NULL)
        {
            aux = aux->sig;
        }

        aux->sig = crearNodoOcurrencias(t);
    }
}

void pasarTerminosArchivo(termino* terminos, int validos, int idDoc)
{
    char nombreArchivo[20];
    char num = (idDoc == 1) ? '1' : '2';
    memset(nombreArchivo,0,sizeof(nombreArchivo));
    strcpy(nombreArchivo,"diccionario");
    strcat(nombreArchivo,num);
    strcat(nombreArchivo,".bin");

    FILE* fp = fopen(nombreArchivo, "ab");

    if(fp)
    {
        for(int i = 0; i < validos; i++)
        {
            fwrite(&terminos[i],sizeof(termino),1,fp);
        }

        fclose(fp);
    }
}
