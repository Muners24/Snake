

#include "C:\raylib\raylib\src\raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

// resolucion
#define RANCHO 1080
#define RALTO 720
// divisiones
#define CUADRICULA 40
#define FILAS RALTO / CUADRICULA
#define COLUMNAS RANCHO / CUADRICULA
#define CASILLAS FILAS *COLUMNAS

#define CONTORNO 4
#define AJUSTEFRUTA CONTORNO / 2 + 1
#define TAMFRUTA CUADRICULA - CONTORNO * 2
//
#define VELOCIDADSNK 2

// dierecciones
#define DERECHA 0
#define ARRIBA 1
#define IZQUIERDA 2
#define ABAJO 3

#define INDETERMINADO -1

typedef Rectangle Trec;

typedef struct _snake
{                  //                              1
    Trec pos;      //
    int direccion; //---------------->     2       p       0
                   //
                   //                              3
    int anteriorDir;
} Tsnake;

typedef struct _nodo
{
    Tsnake snake;
    struct _nodo *siguiente;
} Tnodo;

typedef Tnodo *Tlista;

typedef struct _fruta
{
    Trec pos;
    bool status;
} Tfruta;

char input(char tecla);
void draw(Tlista lista, Tfruta fruta);
void movimientoSnake(Tlista &lista, char tecla);
char colisiones(Tlista &lista, Tfruta &fruta, char tecla);
bool colisionSnake(Tsnake snake, Trec hitbox);
bool colisionPared(Tsnake snake);
Vector2 cabezaSnake(Tsnake snake);

int main(void)
{
    InitWindow(RANCHO, RALTO, "window");
    SetTargetFPS(60);
    srand(time(NULL));

    int i;

    char tecla = '0';

    Tnodo *cabeza = (Tnodo *)malloc(sizeof(Tnodo));
    Tlista lista = cabeza;
    // inicializa nodo 1
    cabeza->snake.pos.x = CUADRICULA * 10;
    cabeza->snake.pos.y = CUADRICULA * 10;
    cabeza->snake.pos.height = CUADRICULA;
    cabeza->snake.pos.width = CUADRICULA;
    cabeza->snake.direccion = INDETERMINADO;
    cabeza->snake.anteriorDir = INDETERMINADO;
    cabeza->siguiente = NULL;

    Tfruta fruta;
    fruta.status = true;
    fruta.pos.x = CUADRICULA * (rand() % COLUMNAS) + 1 + AJUSTEFRUTA;
    fruta.pos.y = CUADRICULA * (rand() % FILAS) + 1 + AJUSTEFRUTA;
    fruta.pos.height = TAMFRUTA;
    fruta.pos.width = TAMFRUTA;

    while (tecla != 'x')
    {

        draw(lista, fruta);

        tecla = input(tecla);

        movimientoSnake(lista, tecla);

        tecla = colisiones(lista, fruta, tecla);
    }

    CloseWindow();

    return 0;
}

char input(char tecla)
{
    if (IsKeyPressed(KEY_W))
    {
        return 'w';
    }

    if (IsKeyPressed(KEY_A))
    {
        return 'a';
    }

    if (IsKeyPressed(KEY_S))
    {
        return 's';
    }

    if (IsKeyPressed(KEY_D))
    {
        return 'd';
    }

    if (IsKeyPressed(KEY_X))
    {
        return 'x';
    }

    if (IsKeyPressed(KEY_P))
    {
        return 'p';
    }

    return tecla;
}

void draw(Tlista lista, Tfruta fruta)
{
    int i;
    Tlista indice = lista;

    BeginDrawing();

    for (i = 0; i < RANCHO; i++)
    {
        if (i % CUADRICULA == 0)
        {
            DrawLine(i, 0, i, RALTO, WHITE);
        }
    }

    for (i = 0; i < RALTO; i++)
    {
        if (i % CUADRICULA == 0)
        {
            DrawLine(0, i, RANCHO, i, WHITE);
        }
    }

    ClearBackground(BLACK);

    // snake
    DrawRectangleRec(indice->snake.pos, GREEN);
    while (indice->siguiente != NULL)
    {
        indice = indice->siguiente;
        DrawRectangleRec(indice->snake.pos, GREEN);
    }

    //fruta
    DrawRectangleRec(fruta.pos, RED);

    EndDrawing();
}

void movimientoSnake(Tlista &lista, char tecla)
{
    bool mov = true;
    Tlista indice = lista;
    static bool cruzarX = false;
    static bool cruzarY = false;

    int(lista->snake.pos.y) % CUADRICULA == 0 ? cruzarY = true : cruzarY = false;
    int(lista->snake.pos.x) % CUADRICULA == 0 ? cruzarX = true : cruzarX = false;

    // direccion de cabeza
    switch (tecla)
    {
    case 'd':
        if (lista->snake.direccion != IZQUIERDA)
        {
            if (cruzarY)
            {
                if (lista->snake.direccion != INDETERMINADO)
                {
                    lista->snake.anteriorDir = lista->snake.direccion;
                }
                lista->snake.direccion = DERECHA;
            }
        }
        break;
    case 'w':
        if (lista->snake.direccion != ABAJO)
        {
            if (cruzarX)
            {
                if (lista->snake.direccion != INDETERMINADO)
                {
                    lista->snake.anteriorDir = lista->snake.direccion;
                }
                lista->snake.direccion = ARRIBA;
            }
        }
        break;
    case 'a':
        if (lista->snake.direccion != DERECHA)
        {
            if (cruzarY)
            {
                if (lista->snake.direccion != INDETERMINADO)
                {
                    lista->snake.anteriorDir = lista->snake.direccion;
                }
                lista->snake.direccion = IZQUIERDA;
            }
        }
        break;
    case 's':
        if (lista->snake.direccion != ARRIBA)
        {
            if (cruzarX)
            {
                if (lista->snake.direccion != INDETERMINADO)
                {
                    lista->snake.anteriorDir = lista->snake.direccion;
                }
                lista->snake.direccion = ABAJO;
            }
        }
        break;
    case 'x':
        lista->snake.direccion = INDETERMINADO;
        break;
    }

    Tlista anterior = indice;
    // direccion cuerpo
    while (indice->siguiente != NULL)
    {
        indice = indice->siguiente;
        if (cruzarX)
        {
            if (cruzarY)
            {
                indice->snake.anteriorDir = indice->snake.direccion;
                indice->snake.direccion = anterior->snake.anteriorDir;
            }
        }
        anterior = anterior->siguiente;
    }

    indice = lista;
    // movimiento snake

    while (mov)
    {

        if (indice->snake.direccion != INDETERMINADO)
        {
            switch (indice->snake.direccion)
            {
            case DERECHA:
                indice->snake.pos.x += VELOCIDADSNK;
                break;
            case ARRIBA:
                indice->snake.pos.y -= VELOCIDADSNK;
                break;
            case IZQUIERDA:
                indice->snake.pos.x -= VELOCIDADSNK;
                break;
            case ABAJO:
                indice->snake.pos.y += VELOCIDADSNK;
                break;
            }
        }
        if (indice->siguiente != NULL)
        {
            indice = indice->siguiente;
        }
        else
        {
            mov = false;
        }
    }
}

char colisiones(Tlista &lista, Tfruta &fruta, char tecla)
{
    Tlista indice = lista;
    // colision con fruta
    if (colisionSnake(indice->snake, fruta.pos))
    {
        // posicion fruta
        fruta.pos.x = CUADRICULA * (rand() % COLUMNAS) + 1 + AJUSTEFRUTA;
        fruta.pos.y = CUADRICULA * (rand() % FILAS) + 1 + AJUSTEFRUTA;

        // generar e inicializar pieza de cuerpo
        Tnodo *nuevo = (Tnodo *)malloc(sizeof(Tnodo));
        if (nuevo)
        {
            nuevo->snake.pos.height = CUADRICULA;
            nuevo->snake.pos.width = CUADRICULA;
            nuevo->siguiente = NULL;

            // posicion e ingreso a la lista
            while (indice->siguiente != NULL)
            {
                indice = indice->siguiente;
            }

            nuevo->snake.direccion = indice->snake.direccion;
            switch (indice->snake.direccion)
            {
            case DERECHA:
                nuevo->snake.pos.x = indice->snake.pos.x - CUADRICULA;
                nuevo->snake.pos.y = indice->snake.pos.y;
                break;
            case ARRIBA:
                nuevo->snake.pos.y = indice->snake.pos.y + CUADRICULA;
                nuevo->snake.pos.x = indice->snake.pos.x;
                break;
            case IZQUIERDA:
                nuevo->snake.pos.x = indice->snake.pos.x + CUADRICULA;
                nuevo->snake.pos.y = indice->snake.pos.y;
                break;
            case ABAJO:
                nuevo->snake.pos.y = indice->snake.pos.y - CUADRICULA;
                nuevo->snake.pos.x = indice->snake.pos.x;
                break;
            }

            indice->siguiente = nuevo;
        }
    }

    indice = lista;
    // colision con cuerpo

    while (indice->siguiente != NULL)
    {
        indice = indice->siguiente;

        if (colisionSnake(lista->snake, indice->snake.pos))
        {
            return 'x';
        }
    }

    // colision con pared
    if (colisionPared(lista->snake))
    {
        return 'x';
    }

    return tecla;
}

bool colisionSnake(Tsnake snake, Trec hitbox)
{
    Vector2 cabeza = cabezaSnake(snake);
    return (CheckCollisionPointRec(cabeza, hitbox));
}

Vector2 cabezaSnake(Tsnake snake)
{
    Vector2 cabeza;
    switch (snake.direccion)
    {
    case DERECHA:
        cabeza.x = (snake.pos.x + snake.pos.width);
        cabeza.y = (snake.pos.y + snake.pos.height / 2);
        break;
    case ARRIBA:
        cabeza.x = (snake.pos.x + snake.pos.width / 2);
        cabeza.y = (snake.pos.y);
        break;
    case IZQUIERDA:
        cabeza.x = (snake.pos.x);
        cabeza.y = (snake.pos.y + snake.pos.height / 2);
        break;
    case ABAJO:
        cabeza.x = (snake.pos.x + snake.pos.width / 2);
        cabeza.y = (snake.pos.y + snake.pos.height);
        break;
    }
    return cabeza;
}

bool colisionPared(Tsnake snake)
{
    Vector2 cabeza = cabezaSnake(snake);
    switch (snake.direccion)
    {
    case DERECHA:
        if (cabeza.x > RANCHO)
        {
            return true;
        }
        break;
    case ARRIBA:
        if (cabeza.y < 0)
        {
            return true;
        }
        break;
    case IZQUIERDA:
        if (cabeza.x < 0)
        {
            return true;
        }
        break;
    case ABAJO:
        if (cabeza.y > RALTO)
        {
            return true;
        }
        break;
    }
    return false;
}