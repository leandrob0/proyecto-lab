#ifndef USUARIOS_H_INCLUDED
#define USUARIOS_H_INCLUDED
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

    PROTOTIPADOS

*/

void verListaVariosID(nodoT *lista, int *idsArchivo, int validos);
void buscarNodoVariosID(nodoA *arbol, char *palabra, int *idArchivo, int validos);
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

int buscarPalabraEnDiccionario(nodoA *arbolDiccionario, char *palabra); //aux
int retornarIdMayor(char *nombreArchivo); //aux
int verSiYaEstaLaId(int *IDs, int validos, int check); //aux
#endif // USUARIOS_H_INCLUDED
