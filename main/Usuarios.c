#include "Usuarios.h"

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

int verSiYaEstaLaId(int *IDs, int validos, int check) /// verifica si la id ya existe
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

/// FUNCIONES PEDIR IDS Y PALABRAS
int pedirVariosID(int id, int *archivoElegido)
{
    int i = 0;

    char seguir = 's';

    do
    {
        if (i == id + 1)
        {
            marcoConsola();
            copy();
            gotoxy(38, 10);
            printf("Todos los archivos seleccionados.\n");

            gotoxy(1, 27);
            system("pause");
            system("cls");

            break;
        }

        int flag = 0;

        do
        {
            marcoConsola();
            copy();
            int aux = 0;
            gotoxy(35, 10);
            printf("Ingrese el ID del archivo en el que desee buscar: ");
            scanf("%i", &aux);

            // checkea que no se pase dos veces la misma ID (bastante inutil ya que va a ser lo mismo pero bueno para hacerlo mas estricto)
            int check = verSiYaEstaLaId(archivoElegido, i + 1, aux);
            if (check == 1)
            {
                flag = 1;
                system("cls");
                marcoConsola();
                copy();
                gotoxy(49, 10);
                printf("ID ya ingresada.\n");
                gotoxy(1, 27);
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
        gotoxy(35, 12);
        printf("Desea elegir otro archivo?(S/N): ");
        fflush(stdin);
        scanf("%c", &seguir);
        system("cls");
    } while (tolower(seguir) == 's');

    return i;
}

/**
#################################################################################################
    FUNCIONES AUXILIARES MENU
    FUNCIONES AUXILIARES MENU
    FUNCIONES AUXILIARES MENU
#################################################################################################
*/

void verListaVariosID(nodoT *lista, int *idsArchivo, int validos, char *palabra)
{
    system("cls");
    int j = 1;
    dibujarCuadro(0, 0, 119, 500); // dibuja el nuevo marco de la consola mas grande para que no se buguee

    gotoxy(28, 0);
    printf(COPYRIGHT);
    dibujarCuadro(27, 2, 100, 4); // el marco de el id y palabra
    gotoxy(28, 3);
    printf("PALABRA: %s", palabra);
    gotoxy(56, 3);
    printf("DOCUMENTOS ID: ");
    for (int i = 0; i < validos; i++)
    {
        printf(" %d | ", idsArchivo[i]);
    }
    printf("\n\n");
    while (lista != NULL)
    {
        // loopea el arreglo de los ids y ve si alguna es igual a la actual de la lista.
        for (int i = 0; i < validos; i++)
        {
            if (lista->idDOC == idsArchivo[i])
            {
                printf("   ID:%d Pos: %d |", lista->idDOC, lista->pos);
            }
        }
        j++;
        if (j == 6)
        {
            printf("\n");
            j = 0;
        }
        lista = lista->sig;
    }
}

// recibe un arreglo de int en caso de que quiera buscar en varios archivos (varias IDS) y sus validos
void buscarNodoVariosID(nodoA *arbol, char *palabra, int *idArchivo, int validos)
{
    if (arbol)
    {
        if (strcmpi(arbol->palabra, palabra) == 0)
        {

            verListaVariosID(arbol->ocurrencias, idArchivo, validos, palabra);
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

//************************************************************************************************
//************************************************************************************************
//******************************    PUNTO 1    ***************************************************
//************************************************************************************************
void verListaUnIdMuchasPalabras(nodoT *lista, int idArchivo, char *palabra)
{
    system("cls");
    int i = 1;
    int flag = 0;
    dibujarCuadro(0, 0, 119, 500); // dibuja el nuevo marco de la consola mas grande para que no se buguee

    gotoxy(28, 0);
    printf(COPYRIGHT);
    dibujarCuadro(27, 2, 90, 4); // el marco de el id y palabra
    gotoxy(28, 3);
    printf("PALABRAS: %s", palabra);
    gotoxy(68, 3);
    printf("DOCUMENTO ID:%d\n", lista->idDOC);
    gotoxy(2, 6);
    printf("posicion: ");

    while (lista != NULL)
    {
        if (idArchivo == lista->idDOC)
        {
            printf("%d |", lista->pos);
            flag = 1;
        }
        if (i == 15)
        {
            printf("\n");
            printf("\t    ");
            i = 0;
        }

        lista = lista->sig;
        if (flag == 1)
        {

            i++;
        }
        flag = 0;
    }
}
void verListaUnID(nodoT *lista, int idArchivo, char *palabra)
{
    system("cls");
    int i = 1;

    dibujarCuadro(0, 0, 119, 500); // dibuja el nuevo marco de la consola mas grande para que no se buguee

    gotoxy(28, 0);
    printf(COPYRIGHT);
    dibujarCuadro(27, 2, 90, 4); // el marco de el id y palabra
    gotoxy(28, 3);
    printf("PALABRA: %s", palabra);
    gotoxy(68, 3);
    printf("DOCUMENTO ID:%d\n", lista->idDOC);
    gotoxy(2, 6);
    printf("posicion: ");

    while (lista != NULL)
    {
        if (idArchivo == lista->idDOC)
        {
            printf("%d |", lista->pos);
        }
        if (i == 15)
        {
            printf("\n");
            printf("\t    ");
            i = 0;
        }

        lista = lista->sig;
        i++;
    }
}

void buscarNodoUnID(nodoA *arbol, char *palabra, int idArchivo)
{

    if (arbol)
    {

        if (strcmpi(arbol->palabra, palabra) == 0)
        {

            verListaUnID(arbol->ocurrencias, idArchivo, palabra);
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
        system("cls");
        marcoConsola();
        copy();
        gotoxy(35, 13);
        printf("Ingrese el ID del archivo en el que desee buscar: ");
        scanf("%i", &aux);
    } while (isdigit(aux));

    return aux;
}

// corresponde a PuntoUno
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
//************************************************************************************************
//******************************     FIN       ***************************************************
//******************************    PUNTO 1    ***************************************************
//************************************************************************************************

int pedirVariasPalabras(nodoA *arbol, char palabras[5][20])
{
    int validos = 0;
    char seguir = 's';
    char palabra[20];
    int check = 0;
    do
    {
        do
        {
            marcoConsola();
            gotoxy(58, 28);
            printf(COPYRIGHT);
            memset(palabra, 0, sizeof(palabra));

            gotoxy(38, 10);
            printf("Ingrese la palabra que desea buscar: ");
            fflush(stdin);
            gets(palabra);
            check = buscarPalabraEnDiccionario(arbol, palabra);
            if (check == 1)
            {
                /* code */
                strcpy(palabras[validos], palabra);
            }
            else
            {
                system("cls");
                marcoConsola();
                gotoxy(58, 28);
                printf(COPYRIGHT);
                gotoxy(38, 10);
                printf("La palabra no existe en el arbol.");
            }

        } while (strcmpi(palabra, "") == 0);

        if (check == 1)
        {
            validos++;
        }

        if (validos == 5)
        {
            system("cls");
            marcoConsola();
            gotoxy(58, 28);
            printf(COPYRIGHT);
            gotoxy(38, 10);
            printf("Limite de palabras alcanzado.\n");
            system("pause");
            system("cls");
            break;
        }

        gotoxy(38, 12);
        printf("Desea cargar otra palabra?(S/N): ");
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

    gotoxy(3, 4);
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
            system("cls");
            marcoConsola();
                copy();
            gotoxy(38, 10);
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
    int i = 1;
    int distancia = 0;
    if (arbol)
    {
        distancia = Levenshtein(palabra, arbol->palabra);

        if (distancia <= 3)
        {
        
            printf("\t%s\n", arbol->palabra);

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
