#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>
#include <stdbool.h>
#include <ctype.h>
#include <alg.h>
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
    PROTOTIPADO
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
void verLista(nodoT *lista, int *idsArchivo, int validos);
void buscarNodo(nodoA *arbol, char *palabra, int *idArchivo, int validos);
int retornarIdMayor(char *nombreArchivo);
int verSiYaEstaLaId(int *IDs, int validos, int check);
void BuscarPalabrasSimilares(nodoA *arbol, char *palabra);

int menu();
void funcionesMenu(termino *arr, int *validos, nodoA **arbol);
/**
#######################################3
    MAIN
#######################################
*/

int main()
{
    nodoA *arbol = NULL;
    termino arr[70000];
    int validos = 0;

    funcionesMenu(arr, &validos, &arbol);
    // cargarDiccionario(arr, &validos);
    // cargarMotorDeBusqueda(DICCIONARIO, &arbol);
    // mostrarArbol(arbol);

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
/// carga un termino
termino agregarTermino(termino t)
{
    termino term;

    term = t;

    return term;
}

/// Cargar el diccionario con los datos de los archivos
void cargarDiccionario(termino arr[], int *validos, char *archivoIds)
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

    cantDoc = retornarIdMayor(ARCHIVOID);

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
            fwrite(&cantDoc, sizeof(int), 1, abrir);
            cantDoc++;
        }
        printf("cant doc: %d", cantDoc);
        printf("\nCargar otro archivo? S/N: ");
        fflush(stdin);
        scanf("%c", &seguir);

        memset(nombreArchivo, 0, sizeof(nombreArchivo));
    }

    fclose(abrir);
}

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

/// CARGAR EN LA LISTA DE OCURRENCIAS DE EL NODO SELECCIONADO
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

/// CARGA EL MOTOR DE BUSQUEDA.
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

/// FUNCION QUE SE FIJA SI EL TERMINO YA ESTA AGREGADO EN EL ARBOL. retorna 1 si lo encontro y 0 si no
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

void mostrarArbol(nodoA *arbol)
{
    if (arbol != NULL)
    {
        mostrarArbol(arbol->izq);
        printf("%s ", arbol->palabra);
        mostrarArbol(arbol->der);
    }
}

void verLista(nodoT *lista, int *idsArchivo, int validos)
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

// recibe un arreglo de int en caso de que quiera buscar en varios archivos (varias IDS) y sus validos
void buscarNodo(nodoA *arbol, char *palabra, int *idArchivo, int validos)
{

    if (arbol)
    {

        if (strcmpi(arbol->palabra, palabra) == 0)
        {

            verLista(arbol->ocurrencias, idArchivo, validos);
        }
        else
        {
            if (strcmpi(arbol->palabra, palabra) > 0)
            {

                buscarNodo(arbol->izq, palabra, idArchivo, validos);
            }
            else
            {

                buscarNodo(arbol->der, palabra, idArchivo, validos);
            }
        }
    }
}

///######################## punto 6 #################################################
///######################## punto 6 #################################################
///######################## punto 6 #################################################

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
void BuscarPalabrasSimilares(nodoA *arbol, char *palabra)
{
    int distancia = 0;

    if (arbol)
    {
        distancia = Levenshtein(palabra, arbol->palabra);

        if (distancia <= 3)
        {
            printf("%s\n\n", arbol->palabra);
        }
        else{

            BuscarPalabrasSimilares(arbol->izq, palabra);
            BuscarPalabrasSimilares(arbol->der, palabra);
        }
        

    }

}

///######################## FIN punto 6 #################################################
///######################## FIN punto 6 #################################################
///######################## FIN punto 6 #################################################

/// RETORNA LA ULTIMA ID AGREGADA.
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
            cargarDiccionario(arr, validos, ARCHIVOID); // ojo con cargar muchas veces el archivo, cargarlo 1 vez y comentar esta linea
            cargarMotorDeBusqueda(DICCIONARIO, arbol);
            break;

        case 2:
            system("cls");

            /// COMO ESTA CONSIGNA PIDE SOLO BUSCAR EN UN ARCHIVO LE PIDE AL USUARIO EN CUAL QUIERE BUSCAR
            id = retornarIdMayor(ARCHIVOID);
            // si no hay archivos agregados al diccionario todavia sale.

            if (id == -1)
            {
                printf("No hay archivos agregados.\n");
                system("pause");
                break;
            }

            do
            {
                printf("Ingrese el ID del archivo en el que desee buscar: ");
                scanf("%i", &archivoElegido[0]);
            } while (archivoElegido[0] > id && archivoElegido[0] < 0);

            printf("\n\nIngrese palabra que desea buscar: ");
            fflush(stdin);
            gets(palabra);

            buscarNodo(*arbol, palabra, archivoElegido, 1);
            memset(palabra, 0, sizeof(palabra));

            system("pause");

            break;

        case 3:
            system("cls");

            id = retornarIdMayor(ARCHIVOID);
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
                    }
                    else
                    {
                        archivoElegido[i] = aux;
                        flag = 0;
                    }
                } while (flag == 1 || archivoElegido[i] > id || archivoElegido < 0);

                i++;
                printf("\nDesea elegir otro archivo?(S/N): ");
                fflush(stdin);
                scanf("%c", &seguir);
                system("cls");
            } while (tolower(seguir) == 's');
            if (i > 1)
            {
                printf("\nIngrese palabra que desea buscar: ");
                fflush(stdin);
                gets(palabra);
                buscarNodo(*arbol, palabra, archivoElegido, i);
                memset(palabra, 0, sizeof(palabra));
            }
            else
            {
                printf("\nDebe ingresar minimo 2 ID!\n");
            }

            system("pause");

            break;
        case 4:
            system("cls");

            system("pause");
            break;

        case 5:
            system("cls");
            system("pause");

            break;
        case 6:
            system("cls");
            system("pause");

            break;
        case 7:
            system("cls");
            char palabra[20];

            int distancia = 0;
            printf("Palabra: ");
            fflush(stdin);
            gets(palabra);

            BuscarPalabrasSimilares(*arbol, palabra);

            system("pause");

            break;
        case 8:
            repite = false;
            break;
        default:
            break;
        }
    } while (repite);
}
