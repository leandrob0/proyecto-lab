#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>
#include <ctype.h>
// #include <carga_arbol.h>
/**
    ESTRUCTURAS Y CONSTANTES A UTILIZAR
*/

#define BABEL "la_biblioteca_de_babel.txt"
#define FUNES "funes_el_memorioso.txt"
#define CANT_MAX 200000
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
termino agregarTermino(termino t);
void cargarDiccionario(termino arr[], int* validos);
void ingresarArbolOrdenado(nodoA** arbolDiccionario, char* palabra);
void cargaDeOcurrencias(nodoA** arbolDiccionario, termino t);
void ingresarOcurrencia(nodoT** listaOcurrencias, termino t);
void pasarTerminosArchivo(termino* terminos, int validos, int idDoc);
void cargarMotorDeBusqueda(char* nombreArchivo, nodoA** lista);
int buscarPalabraEnDiccionario(nodoA* arbolDiccionario, char* palabra);
void mostrarArbol(nodoA* arbol);
void verLista(nodoT* lista );
void buscarNodo(nodoA* arbol, char* palabra);
/**
#######################################
    MAIN
#######################################
*/

int main()
{
    char palabra[20];
    nodoA* arbol = NULL;
    termino arr[70000];
    termino t;
    int validos = 0;
    cargarDiccionario(arr, &validos); //ojo con cargar muchas veces el archivo, cargarlo 1 vez y comentar esta linea
    cargarMotorDeBusqueda("diccionario.bin", &arbol);
    mostrarArbol(arbol); //al mostrar al algunas pocas letras sueltas (ver eso despues) creo que son los textos


    printf("\n\nIngrese palabra que desea buscar: ");
    fflush(stdin);
    gets(palabra);

    tolower(palabra);
    buscarNodo(arbol, palabra);
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
///carga un termino
termino agregarTermino(termino t)
{
    termino term;

    term = t;

    return term;
}


///Cargar el diccionario con los datos de los archivos
void cargarDiccionario(termino arr[], int* validos)
{
    system("cls");
    char palabra[20];
    char seguir = 's';
    memset(palabra, 0, sizeof(palabra));
    int flag = 0;
    int cantDoc = 0;
    int pos = 0;
    int i = 0; //para cargar la palabra letra por letra
    int j = 0;
    char letra;

    while(seguir == 's')
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



        FILE* fp = fopen(nombreArchivo, "rb");

        if(fp != NULL)
        {
            while(fread(&letra,sizeof(char),1,fp) > 0 && i < 20)
            {
                termino t;

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
                        t.idDOC = cantDoc;

                        ///aca cargaria el array de terminos
                        arr[j] = agregarTermino(t);
                        j++;
                        (*validos)++;
                        ///-

                        memset(palabra, 0, sizeof(palabra)); //resetea el array a 0
                        memset(t.palabra, 0, sizeof(t.palabra));

                        pos++;
                        // printf("posicion: %d", pos);
                    }
                    i = 0;
                }
            }

            fclose(fp);

        }else{
            flag = 1;
            system("cls");
            printf("El archivo no existe. Por favor, vuelva a intentarlo.");
        }
        ///RESETEA LA POSICION AL CAMBIAR DE DOCUMENTO
        if (flag == 0)
        {
            pos = 0;
            pasarTerminosArchivo(arr,*validos, cantDoc);
        }
        


        printf("\nCargar otro archivo? S/N: ");
        fflush(stdin);
        scanf("%c", &seguir);
        if (flag == 0)
        {
            cantDoc++;
            
        }

        memset(nombreArchivo,0,sizeof(nombreArchivo));

    }
}

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
                cargaDeOcurrencias(&((*arbolDiccionario)->izq), t);
            }
            else
            {
                cargaDeOcurrencias(&((*arbolDiccionario)->der), t);
            }
        }
    }
}

void ingresarOcurrencia(nodoT** listaOcurrencias, termino t)
{
    nodoT* nuevo = crearNodoOcurrencias(t);

    if(*listaOcurrencias == NULL)
    {
        *listaOcurrencias = nuevo;
    }
    else
    {
        nodoT* ant = *listaOcurrencias;
        nodoT* act = (*listaOcurrencias)->sig;

        if((*listaOcurrencias)->idDOC > t.idDOC || ((*listaOcurrencias)->pos > t.pos && (*listaOcurrencias)->idDOC  == t.idDOC))
        {
            nuevo->sig = *listaOcurrencias;
            *listaOcurrencias = nuevo;
        }

        while(act != NULL && (act->idDOC < t.idDOC && (act->pos < t.pos && act->idDOC == t.idDOC)))
        {
            ant = act;
            act = act->sig;
        }

        ant->sig = nuevo;
        nuevo->sig = act;
    }
}

void pasarTerminosArchivo(termino* terminos, int validos, int idDoc)
{
    char nombreArchivo[20];
    memset(nombreArchivo,0,sizeof(nombreArchivo));
    strcpy(nombreArchivo, "diccionario");
    strcat(nombreArchivo,".bin");

    FILE* fp = fopen(nombreArchivo, "ab");

    if(fp)
    {
        for(int i = 0; i < validos; i++)
        {
            if(terminos[i].idDOC == idDoc)
            {
                fwrite(&terminos[i],sizeof(termino),1,fp);
            }
        }

        fclose(fp);
    }
}

///CARGA EL MOTOR DE BUSQUEDA.
void cargarMotorDeBusqueda(char* nombreArchivo, nodoA** lista)
{
    FILE* fp = fopen(nombreArchivo, "rb");
    int found = 0;
    termino t;

    if(fp != NULL)
    {
        while(fread(&t,sizeof(termino),1,fp) > 0)
        {
            found = buscarPalabraEnDiccionario(*lista, t.palabra);

            if(found == 0) //si la palabra no esta cargada todavia, la inserta en el arbol
            {
                ingresarArbolOrdenado(lista, t.palabra);
            }
            cargaDeOcurrencias(lista, t); //carga la ocurrencia
        }

        fclose(fp);
    }
    else
    {
        printf("Error al abrir el archivo.\n");
    }
}

///FUNCION QUE SE FIJA SI EL TERMINO YA ESTA AGREGADO EN EL ARBOL. retorna 1 si lo encontro y 0 si no
int buscarPalabraEnDiccionario(nodoA* arbolDiccionario, char* palabra)
{
    if(arbolDiccionario != NULL)
    {
        if(strcmpi(arbolDiccionario->palabra, palabra) == 0)
        {
            return 1; // encontrado
        }
        else
        {
            if(strcmpi(arbolDiccionario->palabra, palabra) > 0)
            {
                return buscarPalabraEnDiccionario(arbolDiccionario->izq, palabra);
            }
            else
            {
                return buscarPalabraEnDiccionario(arbolDiccionario->der, palabra);
            }
        }
    } else
    {
        return 0;
    }
}

void mostrarArbol(nodoA* arbol)
{
    if(arbol != NULL)
    {
        mostrarArbol(arbol->izq);
        printf("%s ", arbol->palabra);
        mostrarArbol(arbol->der);
    }
}

void verLista(nodoT* lista ){

    while (lista != NULL) 
    {
        printf("Documento ID:%d\n", lista->idDOC);
        printf("posicion:%d\n", lista->pos);


        lista = lista->sig;
    }
    
}
void buscarNodo(nodoA* arbol, char* palabra){

    char ver;

    if (arbol)
    {
        
        if (strcmpi(arbol->palabra,palabra) == 0)
        {
        
            printf("El nodo se ah encontrado con exito!\n");
            printf("palabra: %s\n", arbol->palabra);
            printf("Frecuencia de la palabra: %d\n\n", arbol->frecuencia);
            printf("Desea ver su lista? S/N");
            scanf("%c", &ver);

            if (ver == 's')
            {
                verLista(arbol->ocurrencias);
            }
        
        }else{
            if(strcmpi(arbol->palabra, palabra) > 0){


            buscarNodo(arbol->izq, palabra);   
            }
            else
            {

            buscarNodo(arbol->der, palabra);    
            }
        }
            
    }
}