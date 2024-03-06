

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

#define NADA -1

typedef Rectangle Trec;

typedef struct _snake
{                  //                              1
    Trec pos;      //
    int direccion; //---------------->     2       p       0
                   //
    bool status;   //                              3
    int anteriorDir;
} Tsnake;

typedef struct _nodo
{
    Tsnake cuerpo;
    struct _nodo *siguiente;
}Tnodo;



typedef struct _cuerpo
{                  //                              1
    Trec pos;      //
    int direccion; //---------------->     2       p       0
    //
    bool status; //                              3
    int anteriorDir;
    bool enlace;
} Tcuerpo;

typedef struct _fruta
{
    Trec pos;
    bool status;
} Tfruta;

char input(char tecla);
void draw(Tsnake snake, Tcuerpo pieza[], Tfruta fruta);
void movimientoSnake(Tsnake &snake, Tcuerpo pieza[], char tecla);
char colisiones(Tsnake &snake, Tcuerpo pieza[], Tfruta &fruta,char tecla);
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

    Tsnake snake;
    snake.pos.x = CUADRICULA * 10;
    snake.pos.y = CUADRICULA * 10;
    snake.pos.height = CUADRICULA;
    snake.pos.width = CUADRICULA;
    snake.status = true;
    snake.direccion = -1;
    snake.anteriorDir = -1;

    Tfruta fruta;
    fruta.status = true;
    fruta.pos.x = CUADRICULA * (rand() % COLUMNAS) + 1 + AJUSTEFRUTA;
    fruta.pos.y = CUADRICULA * (rand() % FILAS) + 1 + AJUSTEFRUTA;
    fruta.pos.height = TAMFRUTA;
    fruta.pos.width = TAMFRUTA;

    Tcuerpo pieza[CASILLAS];
    for (i = 0; i < CASILLAS; i++)
    {
        pieza[i].pos.height = CUADRICULA;
        pieza[i].pos.width = CUADRICULA;
        pieza[i].pos.x = -CUADRICULA * 4;
        pieza[i].pos.y = -CUADRICULA * 4;
        pieza[i].status = false;
        pieza[i].enlace = false;
    }

    while (tecla != 'x')
    {

        draw(snake, pieza, fruta);

        tecla = input(tecla);

        movimientoSnake(snake, pieza, tecla);

        tecla = colisiones(snake, pieza, fruta,tecla);

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

void draw(Tsnake snake, Tcuerpo pieza[], Tfruta fruta)
{
    int i;
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

    DrawRectangleRec(snake.pos, GREEN);
    DrawRectangleRec(fruta.pos, RED);

    for (i = 0; i < CASILLAS; i++)
    {
        if (pieza[i].status)
        {
            DrawRectangleRec(pieza[i].pos, GREEN);
        }
    }
    EndDrawing();
}

void movimientoSnake(Tsnake &snake, Tcuerpo pieza[], char tecla)
{
    int i;

    static bool cruzarX = false;
    static bool cruzarY = false;

    int(snake.pos.y) % CUADRICULA == 0 ? cruzarY = true : cruzarY = false;
    int(snake.pos.x) % CUADRICULA == 0 ? cruzarX = true : cruzarX = false;

    // direccion de cabeza
    switch (tecla)
    {
    case 'd':
        if (snake.direccion != IZQUIERDA)
        {
            if (cruzarY)
            {
                if (snake.direccion != -1)
                {
                    snake.anteriorDir = snake.direccion;
                }
                snake.direccion = DERECHA;
            }
        }
        break;
    case 'w':
        if (snake.direccion != ABAJO)
        {
            if (cruzarX)
            {
                if (snake.direccion != -1)
                {
                    snake.anteriorDir = snake.direccion;
                }
                snake.direccion = ARRIBA;
            }
        }
        break;
    case 'a':
        if (snake.direccion != DERECHA)
        {
            if (cruzarY)
            {
                if (snake.direccion != -1)
                {
                    snake.anteriorDir = snake.direccion;
                }
                snake.direccion = IZQUIERDA;
            }
        }
        break;
    case 's':
        if (snake.direccion != ARRIBA)
        {
            if (cruzarX)
            {
                if (snake.direccion != -1)
                {
                    snake.anteriorDir = snake.direccion;
                }
                snake.direccion = ABAJO;
            }
        }
        break;
    case 'x':
        snake.direccion = -1;
        break;
    }

    // direccion cuerpo
    for (i = 0; i < CASILLAS; i++)
    {
        if (pieza[i].status)
        {
            if (pieza[i].enlace)
            {
                if (cruzarX)
                {
                    if (cruzarY)
                    {
                        pieza[i].anteriorDir = pieza[i].direccion;
                        pieza[i].direccion = snake.anteriorDir;
                    }
                }
            }
            else
            {
                if (cruzarX)
                {
                    if (cruzarY)
                    {
                        pieza[i].anteriorDir = pieza[i].direccion;
                        pieza[i].direccion = pieza[i - 1].anteriorDir;
                    }
                }
            }
        }
    }

    // movimiento snake
    if (snake.direccion != -1)
    {
        switch (snake.direccion)
        {
        case DERECHA:
            snake.pos.x += VELOCIDADSNK;
            break;
        case ARRIBA:
            snake.pos.y -= VELOCIDADSNK;
            break;
        case IZQUIERDA:
            snake.pos.x -= VELOCIDADSNK;
            break;
        case ABAJO:
            snake.pos.y += VELOCIDADSNK;
            break;
        }
    }

    // movimiento  cuerpo
    for (i = 0; i < CASILLAS; i++)
    {
        if (pieza[i].status)
        {
            switch (pieza[i].direccion)
            {
            case 0:
                pieza[i].pos.x += VELOCIDADSNK;
                break;
            case 1:
                pieza[i].pos.y -= VELOCIDADSNK;
                break;
            case 2:
                pieza[i].pos.x -= VELOCIDADSNK;
                break;

            case 3:
                pieza[i].pos.y += VELOCIDADSNK;
                break;
            }
        }
    }
}

char colisiones(Tsnake &snake, Tcuerpo pieza[], Tfruta &fruta,char tecla)
{
    static int piezaC = 0;
    int i;

    // colision con fruta
    if (colisionSnake(snake, fruta.pos))
    {
        //posicion fruta
        fruta.pos.x = CUADRICULA * (rand() % COLUMNAS) + 1 + AJUSTEFRUTA;
        fruta.pos.y = CUADRICULA * (rand() % FILAS) + 1 + AJUSTEFRUTA;

        //generar pieza de cuerpo
        for (i = 0; i < CASILLAS; i++)
        {
            if (!pieza[i].status)
            {
                pieza[i].status = true;
                if (piezaC == 0)
                {
                    pieza[i].enlace = true;
                    pieza[i].direccion = snake.direccion;
                    switch (snake.direccion)
                    {
                    case DERECHA:
                        pieza[i].pos.x = snake.pos.x - CUADRICULA;
                        pieza[i].pos.y = snake.pos.y;
                        break;
                    case ARRIBA:
                        pieza[i].pos.y = snake.pos.y + CUADRICULA;
                        pieza[i].pos.x = snake.pos.x;
                        break;
                    case IZQUIERDA:
                        pieza[i].pos.x = snake.pos.x + CUADRICULA;
                        pieza[i].pos.y = snake.pos.y;
                        break;
                    case ABAJO:
                        pieza[i].pos.y = snake.pos.y - CUADRICULA;
                        pieza[i].pos.x = snake.pos.x;
                        break;
                    }
                }
                else
                {
                    pieza[i].enlace = false;
                    pieza[i].direccion = pieza[i - 1].direccion;
                    switch (pieza[i - 1].direccion)
                    {
                    case DERECHA:
                        pieza[i].pos.x = pieza[i - 1].pos.x - CUADRICULA;
                        pieza[i].pos.y = pieza[i - 1].pos.y;
                        break;
                    case ARRIBA:
                        pieza[i].pos.y = pieza[i - 1].pos.y + CUADRICULA;
                        pieza[i].pos.x = pieza[i - 1].pos.x;
                        break;
                    case IZQUIERDA:
                        pieza[i].pos.x = pieza[i - 1].pos.x + CUADRICULA;
                        pieza[i].pos.y = pieza[i - 1].pos.y;
                        break;
                    case ABAJO:
                        pieza[i].pos.y = pieza[i - 1].pos.y - CUADRICULA;
                        pieza[i].pos.x = pieza[i - 1].pos.x;
                        break;
                    }
                }
                piezaC++;
                i = CASILLAS;
            }
        }
    }

    //colision con cuerpo
    for(i=0;i<CASILLAS;i++)
    {
        if(pieza[i].status)
        {
            if(colisionSnake(snake,pieza[i].pos))
            {
                return 'x';
            }
        }
    }

    //colision con pared
    if(colisionPared(snake))
    {
        return 'x';
    }

    return tecla;
}

bool colisionSnake(Tsnake snake, Trec hitbox)
{
    Vector2 cabeza=cabezaSnake(snake);
    return (CheckCollisionPointRec(cabeza, hitbox));
}

Vector2 cabezaSnake(Tsnake snake)
{
    Vector2 cabeza;
    switch (snake.direccion)
    {
    case DERECHA:
        cabeza.x=(snake.pos.x + snake.pos.width);
        cabeza.y=(snake.pos.y + snake.pos.height / 2);
        break;
    case ARRIBA:
        cabeza.x=(snake.pos.x + snake.pos.width / 2);
        cabeza.y=(snake.pos.y);
        break;
    case IZQUIERDA:
        cabeza.x=(snake.pos.x);
        cabeza.y=(snake.pos.y + snake.pos.height / 2);
        break;
    case ABAJO:
        cabeza.x=(snake.pos.x + snake.pos.width / 2);
        cabeza.y=(snake.pos.y + snake.pos.height);
        break;
    }
    return cabeza;
}

bool colisionPared(Tsnake snake)
{
    Vector2 cabeza=cabezaSnake(snake);
    switch (snake.direccion)
    {
    case DERECHA:
        if(cabeza.x > RANCHO)
        {
            return true;
        }
        break;
    case ARRIBA:
        if(cabeza.y < 0)
        {
            return true;
        }
        break;
    case IZQUIERDA:
        if(cabeza.x < 0)
        {
            return true;
        }
        break;
    case ABAJO:
        if(cabeza.y > RALTO)
        {
            return true;
        }
        break;
    }
    return false;
}