/*
 * Sistema de optimizacion de produccion - Fabrica de componentes electronicos
 * Programacion 1 (ISWZ1102)
 *
 * Restricciones del ejercicio:
 *  - No se usan estructuras (struct) ni archivos.
 *  - Los datos se manejan con arreglos paralelos y punteros.
 *  - La busqueda de productos se programa con logica propia (busqueda lineal).
 *  - No se usan funciones predefinidas de ordenamiento ni de busqueda.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PRODUCTOS 5 // la fabrica maneja cinco productos distintos
#define LONG_NOMBRE 30  // 29 caracteres + '\0'
#define LONG_LINEA 100  // tamano del buffer de lectura

// prototipos de funciones: lectura y validacion
void leerLinea(char *linea, int tam);
void leerTexto(const char *mensaje, char *destino, int tam);
int leerEntero(const char *mensaje, int minimo, int maximo);
float leerReal(const char *mensaje, float minimo, float maximo);
int leerSiNo(const char *mensaje);

// prototipos de funciones: busqueda propia
int sonIguales(const char *a, const char *b);
int buscarProducto(char (*nombres)[LONG_NOMBRE], int total, const char *buscado);

// prototipos de funciones: gestion de productos
void mostrarMenu(void);
void ingresarProducto(char (*nombres)[LONG_NOMBRE], float *tiempos, float *recursos, int *demandas, int *total);
void mostrarProductos(char (*nombres)[LONG_NOMBRE], float *tiempos, float *recursos, int *demandas, int total);
void editarProducto(char (*nombres)[LONG_NOMBRE], float *tiempos, float *recursos, int *demandas, int total);
void eliminarProducto(char (*nombres)[LONG_NOMBRE], float *tiempos, float *recursos, int *demandas, int *total);

// prototipos de funciones: calculos
void ingresarDisponibilidad(float *tiempoDisp, float *recursoDisp);
float calcularTiempoTotal(float *tiempos, int *demandas, int total);
float calcularRecursosTotales(float *recursos, int *demandas, int total);
void verificarProduccion(char (*nombres)[LONG_NOMBRE], float *tiempos, float *recursos, int *demandas, int total, float tiempoDisp, float recursoDisp);

int main()
{
    // arreglos paralelos: la posicion i de cada arreglo describe al mismo producto
    char nombres[MAX_PRODUCTOS][LONG_NOMBRE];
    float tiempos[MAX_PRODUCTOS];  // minutos necesarios para fabricar 1 unidad
    float recursos[MAX_PRODUCTOS]; // unidades de recurso necesarias para 1 unidad
    int demandas[MAX_PRODUCTOS];   // unidades que se deben producir
    int total = 0;                 // cantidad de productos registrados
    float tiempoDisp = 0;          // minutos disponibles en la linea de produccion
    float recursoDisp = 0;         // unidades de recurso disponibles
    int opcion;

    printf("===== Sistema de optimizacion de produccion =====\n");
    do
    {
        mostrarMenu();
        opcion = leerEntero("Seleccione una opcion: ", 0, 6);
        switch (opcion)
        {
        case 1:
            ingresarProducto(nombres, tiempos, recursos, demandas, &total);
            break;
        case 2:
            mostrarProductos(nombres, tiempos, recursos, demandas, total);
            break;
        case 3:
            editarProducto(nombres, tiempos, recursos, demandas, total);
            break;
        case 4:
            eliminarProducto(nombres, tiempos, recursos, demandas, &total);
            break;
        case 5:
            ingresarDisponibilidad(&tiempoDisp, &recursoDisp);
            break;
        case 6:
            verificarProduccion(nombres, tiempos, recursos, demandas, total, tiempoDisp, recursoDisp);
            break;
        case 0:
            printf("\nPrograma finalizado.\n");
            break;
        }
    } while (opcion != 0);

    return 0;
}

/* ------------------------- lectura y validacion ------------------------- */

void leerLinea(char *linea, int tam)
{
    int i = 0;
    int c;

    if (fgets(linea, tam, stdin) == NULL)
    {
        printf("\nFin de la entrada. Programa terminado.\n");
        exit(0);
    }
    while (*(linea + i) != '\0' && *(linea + i) != '\n')
    {
        i++;
    }
    if (*(linea + i) == '\n')
    {
        *(linea + i) = '\0'; // quitamos el salto de linea
    }
    else
    {
        // la linea era mas larga que el buffer: descartamos el resto
        while ((c = getchar()) != '\n' && c != EOF)
        {
        }
    }
}

void leerTexto(const char *mensaje, char *destino, int tam)
{
    char linea[LONG_LINEA];
    int valido = 0;
    int i;
    int hayLetra;

    while (!valido)
    {
        printf("%s", mensaje);
        leerLinea(linea, LONG_LINEA);

        // comprobamos que exista al menos un caracter que no sea espacio
        hayLetra = 0;
        for (i = 0; *(linea + i) != '\0'; i++)
        {
            if (*(linea + i) != ' ')
            {
                hayLetra = 1;
            }
        }

        if (!hayLetra)
        {
            printf("  Error: el texto no puede estar vacio.\n");
        }
        else if ((int)strlen(linea) >= tam)
        {
            printf("  Error: maximo %d caracteres.\n", tam - 1);
        }
        else
        {
            strcpy(destino, linea);
            valido = 1;
        }
    }
}

int leerEntero(const char *mensaje, int minimo, int maximo)
{
    char linea[LONG_LINEA];
    char sobrante;
    int numero;

    while (1)
    {
        printf("%s", mensaje);
        leerLinea(linea, LONG_LINEA);
        // %c detecta caracteres extra despues del numero (ej. "5 abc")
        if (sscanf(linea, "%d %c", &numero, &sobrante) == 1 && numero >= minimo && numero <= maximo)
        {
            return numero;
        }
        printf("  Error: ingrese un numero entero entre %d y %d.\n", minimo, maximo);
    }
}

float leerReal(const char *mensaje, float minimo, float maximo)
{
    char linea[LONG_LINEA];
    char sobrante;
    float numero;

    while (1)
    {
        printf("%s", mensaje);
        leerLinea(linea, LONG_LINEA);
        if (sscanf(linea, "%f %c", &numero, &sobrante) == 1 && numero >= minimo && numero <= maximo)
        {
            return numero;
        }
        printf("  Error: ingrese un numero entre %.2f y %.2f.\n", minimo, maximo);
    }
}

int leerSiNo(const char *mensaje)
{
    char linea[LONG_LINEA];

    while (1)
    {
        printf("%s", mensaje);
        leerLinea(linea, LONG_LINEA);
        if (sonIguales(linea, "S") || sonIguales(linea, "SI"))
        {
            return 1;
        }
        if (sonIguales(linea, "N") || sonIguales(linea, "NO"))
        {
            return 0;
        }
        printf("  Error: responda SI o NO.\n");
    }
}

/* ----------------------------- busqueda propia ----------------------------- */

// compara dos cadenas sin distinguir mayusculas de minusculas
int sonIguales(const char *a, const char *b)
{
    while (*a != '\0' && *b != '\0')
    {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
        {
            return 0;
        }
        a++;
        b++;
    }
    return (*a == '\0' && *b == '\0');
}

// busqueda lineal: devuelve la posicion del producto o -1 si no existe
int buscarProducto(char (*nombres)[LONG_NOMBRE], int total, const char *buscado)
{
    for (int i = 0; i < total; i++)
    {
        if (sonIguales(*(nombres + i), buscado))
        {
            return i;
        }
    }
    return -1;
}

/* --------------------------- gestion de productos --------------------------- */

void mostrarMenu(void)
{
    printf("\n------------------ MENU ------------------\n");
    printf("1. Ingresar producto\n");
    printf("2. Mostrar productos\n");
    printf("3. Editar producto\n");
    printf("4. Eliminar producto\n");
    printf("5. Ingresar disponibilidad (tiempo y recursos)\n");
    printf("6. Calcular y verificar la produccion\n");
    printf("0. Salir\n");
    printf("------------------------------------------\n");
}

void ingresarProducto(char (*nombres)[LONG_NOMBRE], float *tiempos, float *recursos, int *demandas, int *total)
{
    char nombre[LONG_NOMBRE];

    if (*total >= MAX_PRODUCTOS)
    {
        printf("\nSe alcanzo el maximo de %d productos. Elimine uno para agregar otro.\n", MAX_PRODUCTOS);
        return;
    }

    printf("\n--- Ingreso del producto %d de %d ---\n", *total + 1, MAX_PRODUCTOS);
    leerTexto("Nombre del producto: ", nombre, LONG_NOMBRE);
    while (buscarProducto(nombres, *total, nombre) != -1)
    {
        printf("  Error: ya existe un producto con ese nombre.\n");
        leerTexto("Nombre del producto: ", nombre, LONG_NOMBRE);
    }

    strcpy(*(nombres + *total), nombre);
    *(demandas + *total) = leerEntero("Cantidad demandada (0 a 100000): ", 0, 100000);
    *(tiempos + *total) = leerReal("Tiempo de fabricacion por unidad en minutos (0.01 a 10000): ", 0.01f, 10000.0f);
    *(recursos + *total) = leerReal("Recursos por unidad (0.01 a 100000): ", 0.01f, 100000.0f);

    (*total)++;
    printf("\nProducto registrado correctamente.\n");
}

void mostrarProductos(char (*nombres)[LONG_NOMBRE], float *tiempos, float *recursos, int *demandas, int total)
{
    if (total == 0)
    {
        printf("\nNo hay productos registrados.\n");
        return;
    }

    printf("\n%-3s %-30s %10s %12s %12s\n", "N", "Producto", "Demanda", "Min/unidad", "Rec/unidad");
    printf("---------------------------------------------------------------------\n");
    for (int i = 0; i < total; i++)
    {
        printf("%-3d %-30s %10d %12.2f %12.2f\n", i + 1, *(nombres + i), *(demandas + i), *(tiempos + i), *(recursos + i));
    }
    printf("---------------------------------------------------------------------\n");
}

void editarProducto(char (*nombres)[LONG_NOMBRE], float *tiempos, float *recursos, int *demandas, int total)
{
    char buscado[LONG_NOMBRE];
    char nuevo[LONG_NOMBRE];
    int posicion;
    int otro;
    int opcion;

    if (total == 0)
    {
        printf("\nNo hay productos registrados para editar.\n");
        return;
    }

    mostrarProductos(nombres, tiempos, recursos, demandas, total);
    leerTexto("Nombre del producto a editar: ", buscado, LONG_NOMBRE);
    posicion = buscarProducto(nombres, total, buscado);
    if (posicion == -1)
    {
        printf("\nProducto no encontrado.\n");
        return;
    }

    do
    {
        printf("\nEditando: %s\n", *(nombres + posicion));
        printf("1. Nombre\n2. Cantidad demandada\n3. Tiempo por unidad\n4. Recursos por unidad\n0. Terminar edicion\n");
        opcion = leerEntero("Dato a modificar: ", 0, 4);

        switch (opcion)
        {
        case 1:
            leerTexto("Nuevo nombre: ", nuevo, LONG_NOMBRE);
            otro = buscarProducto(nombres, total, nuevo);
            if (otro != -1 && otro != posicion)
            {
                printf("  Error: ya existe un producto con ese nombre.\n");
            }
            else
            {
                strcpy(*(nombres + posicion), nuevo);
                printf("  Nombre actualizado.\n");
            }
            break;
        case 2:
            *(demandas + posicion) = leerEntero("Nueva cantidad demandada (0 a 100000): ", 0, 100000);
            printf("  Cantidad actualizada.\n");
            break;
        case 3:
            *(tiempos + posicion) = leerReal("Nuevo tiempo por unidad en minutos (0.01 a 10000): ", 0.01f, 10000.0f);
            printf("  Tiempo actualizado.\n");
            break;
        case 4:
            *(recursos + posicion) = leerReal("Nuevos recursos por unidad (0.01 a 100000): ", 0.01f, 100000.0f);
            printf("  Recursos actualizados.\n");
            break;
        }
    } while (opcion != 0);
}

void eliminarProducto(char (*nombres)[LONG_NOMBRE], float *tiempos, float *recursos, int *demandas, int *total)
{
    char buscado[LONG_NOMBRE];
    int posicion;

    if (*total == 0)
    {
        printf("\nNo hay productos registrados para eliminar.\n");
        return;
    }

    mostrarProductos(nombres, tiempos, recursos, demandas, *total);
    leerTexto("Nombre del producto a eliminar: ", buscado, LONG_NOMBRE);
    posicion = buscarProducto(nombres, *total, buscado);
    if (posicion == -1)
    {
        printf("\nProducto no encontrado.\n");
        return;
    }

    if (!leerSiNo("Esta seguro de eliminarlo? (SI/NO): "))
    {
        printf("\nEliminacion cancelada.\n");
        return;
    }

    // desplazamos a la izquierda los elementos que estan despues de la posicion
    for (int i = posicion; i < *total - 1; i++)
    {
        strcpy(*(nombres + i), *(nombres + i + 1));
        *(tiempos + i) = *(tiempos + i + 1);
        *(recursos + i) = *(recursos + i + 1);
        *(demandas + i) = *(demandas + i + 1);
    }
    (*total)--;
    printf("\nProducto eliminado correctamente.\n");
}

/* -------------------------------- calculos -------------------------------- */

void ingresarDisponibilidad(float *tiempoDisp, float *recursoDisp)
{
    printf("\n--- Disponibilidad de la fabrica ---\n");
    *tiempoDisp = leerReal("Tiempo de produccion disponible en minutos (1 a 1000000): ", 1.0f, 1000000.0f);
    *recursoDisp = leerReal("Recursos disponibles en unidades (1 a 10000000): ", 1.0f, 10000000.0f);
    printf("\nDisponibilidad registrada.\n");
}

float calcularTiempoTotal(float *tiempos, int *demandas, int total)
{
    float suma = 0;
    for (int i = 0; i < total; i++)
    {
        suma += *(demandas + i) * *(tiempos + i); // demanda x tiempo por unidad
    }
    return suma;
}

float calcularRecursosTotales(float *recursos, int *demandas, int total)
{
    float suma = 0;
    for (int i = 0; i < total; i++)
    {
        suma += *(demandas + i) * *(recursos + i); // demanda x recursos por unidad
    }
    return suma;
}

void verificarProduccion(char (*nombres)[LONG_NOMBRE], float *tiempos, float *recursos, int *demandas, int total, float tiempoDisp, float recursoDisp)
{
    float tiempoReq;
    float recursoReq;
    float tiempoProducto;
    float mayorTiempo;
    int posicionMayor = 0;

    if (total == 0)
    {
        printf("\nPrimero ingrese al menos un producto (opcion 1).\n");
        return;
    }
    if (tiempoDisp <= 0 || recursoDisp <= 0)
    {
        printf("\nPrimero ingrese la disponibilidad de tiempo y recursos (opcion 5).\n");
        return;
    }

    tiempoReq = calcularTiempoTotal(tiempos, demandas, total);
    recursoReq = calcularRecursosTotales(recursos, demandas, total);

    // buscamos el producto que mas tiempo consume (posicion del mayor valor)
    mayorTiempo = *(demandas) * *(tiempos);
    for (int i = 1; i < total; i++)
    {
        tiempoProducto = *(demandas + i) * *(tiempos + i);
        if (tiempoProducto > mayorTiempo)
        {
            mayorTiempo = tiempoProducto;
            posicionMayor = i;
        }
    }

    printf("\n========== RESULTADO DE LA PLANIFICACION ==========\n");
    printf("Tiempo total requerido : %10.2f min (%.2f h)\n", tiempoReq, tiempoReq / 60);
    printf("Tiempo disponible      : %10.2f min (%.2f h)\n", tiempoDisp, tiempoDisp / 60);
    printf("Recursos requeridos    : %10.2f unidades\n", recursoReq);
    printf("Recursos disponibles   : %10.2f unidades\n", recursoDisp);
    printf("Producto que mas tiempo consume: %s (%.2f min)\n", *(nombres + posicionMayor), mayorTiempo);
    printf("---------------------------------------------------\n");

    if (tiempoReq <= tiempoDisp && recursoReq <= recursoDisp)
    {
        printf("RESULTADO: LA FABRICA PUEDE CUMPLIR CON LA DEMANDA.\n");
        printf("Tiempo sobrante  : %.2f min\n", tiempoDisp - tiempoReq);
        printf("Recursos sobrantes: %.2f unidades\n", recursoDisp - recursoReq);
    }
    else
    {
        printf("RESULTADO: LA FABRICA NO PUEDE CUMPLIR CON LA DEMANDA.\n");
        if (tiempoReq > tiempoDisp)
        {
            printf("Faltan %.2f min de produccion.\n", tiempoReq - tiempoDisp);
        }
        if (recursoReq > recursoDisp)
        {
            printf("Faltan %.2f unidades de recursos.\n", recursoReq - recursoDisp);
        }
    }
    printf("===================================================\n");
}
