#include "motor.h"


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