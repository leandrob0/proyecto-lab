#ifndef MOTOR_H_INCLUDED
#define MOTOR_H_INCLUDED

#include "Usuarios.h"



nodoT *crearNodoOcurrencias(termino palabra);
nodoA *crearNodoPalabras(char *palabra);
termino agregarTermino(termino t);
void cargarDiccionario(termino arr[], int *validos, char *archivoIds);
void ingresarArbolOrdenado(nodoA **arbolDiccionario, char *palabra);
void cargaDeOcurrencias(nodoA **arbolDiccionario, termino t);
void ingresarOcurrencia(nodoT **listaOcurrencias, termino t);
void pasarTerminosArchivo(termino *terminos, int validos, int idDoc);
void cargarMotorDeBusqueda(char *nombreArchivo, nodoA **lista);

#endif