#include "C:\raylib\raylib\src\raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

// resolucion DEBE SER MULTIPLO DE CUADRICULA
#define RANCHO 1080
#define RALTO 720

// divisiones
#define CUADRICULA 40
#define FILAS RALTO / CUADRICULA - BORDE_Y
#define COLUMNAS RANCHO / CUADRICULA - BORDE_X
#define CASILLAS FILAS *COLUMNAS
// bordes
// 1 cada lado
#define BORDE_X 2
// 2 arriba 1 abajo
#define BORDE_Y 3
#define BORDE_UP CUADRICULA * 2
#define BORDE_RIGHT RANCHO - CUADRICULA
#define BORDE_LEFT CUADRICULA
#define BORDE_DOWN RALTO - CUADRICULA

// fruta
#define AJUSTEFRUTA 4
#define TAMFRUTA CUADRICULA - AJUSTEFRUTA * 2

// velocidad DEBE SER MULTIPLO DE CUADRICULA
#define VELOCIDADSNK 4

// direcciones
#define DERECHA 0
#define ARRIBA 1
#define IZQUIERDA 2
#define ABAJO 3

#define INDETERMINADO -1

// puntuacion
#define SCORE 20

typedef Rectangle rec_t;

typedef struct
{                  //                              1
    rec_t pos;     //
    int direccion; //---------------->     2       p       0
                   //
                   //                              3
    int anteriorDir;
} snake_t;

typedef struct _nodo
{
    snake_t snake;
    struct _nodo *siguiente;
} nodo_t;

typedef nodo_t *lista_t;

typedef struct
{
    rec_t pos;
    bool b_status;
} fruta_t;

char input(char tecla);
void draw(lista_t lista, fruta_t fruta, bool b_inicio,bool b_ganar, int score);
void movimiento_snake(lista_t &lista, char tecla);
char colisiones(lista_t &lista, fruta_t &fruta,char tecla, int &pieza_c);
bool colision_snake(snake_t snake, rec_t hitbox);
bool colision_pared(snake_t snake);
Vector2 cabeza_snake(snake_t snake);

int main(void)
{
    InitWindow(RANCHO, RALTO, "window");
    SetTargetFPS(60);
    srand(time(NULL));
    int i;
    int pieza_c;
    char tecla;

    bool b_fin;
    bool b_ganar;
    bool b_inicio;

    nodo_t *cabeza;

    lista_t lista;

    fruta_t fruta;
    do
    {
        pieza_c = 0;
        tecla = '0';
        
        b_fin = false;
        b_ganar=false;
        b_inicio = false;
        cabeza = (nodo_t *)malloc(sizeof(nodo_t));

        lista = cabeza;
        // inicializa nodo 1
        cabeza->snake.pos.x = CUADRICULA * 10;
        cabeza->snake.pos.y = CUADRICULA * 10;
        cabeza->snake.pos.height = CUADRICULA;
        cabeza->snake.pos.width = CUADRICULA;
        cabeza->snake.direccion = INDETERMINADO;
        cabeza->snake.anteriorDir = INDETERMINADO;
        cabeza->siguiente = NULL;

        fruta.b_status = true;
        fruta.pos.x = CUADRICULA * (rand() % (COLUMNAS)) + AJUSTEFRUTA + CUADRICULA;
        fruta.pos.y = CUADRICULA * (rand() % (FILAS)) + AJUSTEFRUTA + CUADRICULA * 2;
        fruta.pos.height = TAMFRUTA;
        fruta.pos.width = TAMFRUTA;

        do
        {

            draw(lista, fruta, b_inicio, b_ganar, pieza_c);

            tecla = input(tecla);

            if ('p' == tecla)
            {
                b_inicio = true;
            }

            if (b_inicio)
            {
                if(!b_fin)
                {
                    if(!b_ganar)
                    {
                        movimiento_snake(lista, tecla);
                    }
                    tecla = colisiones(lista, fruta, tecla,pieza_c);
                }
            }

            if('x'==tecla)
            {
                b_fin = true;
            }

            if (5 == pieza_c)
            {
                b_ganar = true;
                tecla= 'x';
                b_fin= false;
            }
            
        }while ('x' != tecla);
    } while (!b_fin);
    
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

void draw(lista_t lista, fruta_t fruta, bool b_inicio,bool b_ganar, int score)
{
    int i;
    lista_t indice = lista;

    BeginDrawing();
    ClearBackground(BLACK);

    // HORIZONTALES
    for (i = BORDE_UP; i <= BORDE_DOWN; i++)
    {
        if (i % CUADRICULA == 0)
        {
            DrawLine(BORDE_LEFT, i, BORDE_RIGHT, i, WHITE);
        }
    }

    // verticales
    for (i = BORDE_LEFT; i <= BORDE_RIGHT; i++)
    {
        if (i % CUADRICULA == 0)
        {
            DrawLine(i, BORDE_UP, i, BORDE_DOWN, WHITE);
        }
    }

    // snake
    rec_t rec;
    rec = indice->snake.pos;
    rec.x += 1;
    rec.y += 1;
    rec.width -= 2;
    rec.height -= 2;
    DrawRectangleRec(indice->snake.pos, GREEN);
    DrawRectangleRec(rec, GREEN);
    while (NULL != indice->siguiente)
    {
        indice = indice->siguiente;
        rec = indice->snake.pos;
        rec.x += 1;
        rec.y += 1;
        rec.width -= 2;
        rec.height -= 2;
        DrawRectangleRec(indice->snake.pos, GREEN);
        DrawRectangleRec(rec, GREEN);
    }

    // fruta
    DrawRectangleRec(fruta.pos, RED);

    //mensajes
    if (!b_inicio)
    {
        rec_t control_inicio;
        control_inicio.width = CUADRICULA * 7;
        control_inicio.height = CUADRICULA * 6;
        control_inicio.x = BORDE_LEFT + CUADRICULA;
        control_inicio.y = BORDE_UP + CUADRICULA;
        DrawRectangleRec(control_inicio, DARKGRAY);
        DrawText("\n\t\t  Controles\n\n\t\t\t\t    w\n\n\t\t\t  a\ts\td", control_inicio.x, control_inicio.y, 30, WHITE);
        DrawText("\n\n\n\n\t\tP para iniciar\n\n\n\t\t X para salir",control_inicio.x-5, control_inicio.y+60, 30, WHITE);
    }
    else
    {
        // puntuacion
        char text[50];
        char txtscore[50];
        itoa(score * SCORE, txtscore, 10);
        strcpy(text, "Score: ");
        strcat(text, txtscore);
        DrawText(text, CUADRICULA, CUADRICULA, 30, WHITE);
    }

    if(b_ganar)
    {
        rec_t control_inicio;
        control_inicio.width = CUADRICULA * 8;
        control_inicio.height = CUADRICULA * 5;
        control_inicio.x = BORDE_LEFT + CUADRICULA;
        control_inicio.y = BORDE_UP + CUADRICULA;
        DrawRectangleRec(control_inicio, DARKGRAY);
        DrawText("\n\t\t\t Felicidades\n\n\t\t\t   Ganaste\n\n\n\t\t ", control_inicio.x, control_inicio.y, 30, WHITE);
        DrawText("\n\n\t   P para repetir\n\n\n\t    X para salir",control_inicio.x-5, control_inicio.y+60, 30, WHITE);
    }
    EndDrawing();
}

void movimiento_snake(lista_t &lista, char tecla)
{
    bool b_mov = true;
    lista_t indice = lista;
    static bool b_cruzarX = false;
    static bool b_cruzarY = false;

    int(lista->snake.pos.y) % CUADRICULA == 0 ? b_cruzarY = true : b_cruzarY = false;
    int(lista->snake.pos.x) % CUADRICULA == 0 ? b_cruzarX = true : b_cruzarX = false;

    // direccion de cabeza
    switch (tecla)
    {
    case 'd':
        if (lista->snake.direccion != IZQUIERDA)
        {
            if (b_cruzarY)
            {
                if (INDETERMINADO != lista->snake.direccion)
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
            if (b_cruzarX)
            {
                if (INDETERMINADO != lista->snake.direccion)
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
            if (b_cruzarY)
            {
                if (INDETERMINADO != lista->snake.direccion)
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
            if (b_cruzarX)
            {
                if (INDETERMINADO != lista->snake.direccion)
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

    lista_t anterior = indice;
    // direccion cuerpo
    while (indice->siguiente != NULL)
    {
        indice = indice->siguiente;
        if (b_cruzarX)
        {
            if (b_cruzarY)
            {
                indice->snake.anteriorDir = indice->snake.direccion;
                indice->snake.direccion = anterior->snake.anteriorDir;
            }
        }
        anterior = anterior->siguiente;
    }

    indice = lista;
    // movimiento snake

    while (b_mov)
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
        if (NULL != indice->siguiente)
        {
            indice = indice->siguiente;
        }
        else
        {
            b_mov = false;
        }
    }
}

char colisiones(lista_t &lista, fruta_t &fruta, char tecla, int &pieza_c)
{
    lista_t indice = lista;
    bool b_recalcular = false;
    // colision con fruta
    if (colision_snake(indice->snake, fruta.pos))
    {
        // posicion fruta
        do
        {
            b_recalcular = false;
            fruta.pos.x = CUADRICULA * (rand() % (COLUMNAS)) + AJUSTEFRUTA + CUADRICULA;
            fruta.pos.y = CUADRICULA * (rand() % (FILAS)) + AJUSTEFRUTA + CUADRICULA * 2;

            while (NULL != indice->siguiente)
            {
                indice = indice->siguiente;

                if (CheckCollisionRecs(fruta.pos, indice->snake.pos))
                {
                    b_recalcular = true;
                    printf("recalculo pos frtura\n\n");
                }
            }
        } while (b_recalcular);

        // generar e inicializar pieza de cuerpo
        nodo_t *nuevo = (nodo_t *)malloc(sizeof(nodo_t));
        if (nuevo)
        {
            nuevo->snake.pos.height = CUADRICULA;
            nuevo->snake.pos.width = CUADRICULA;
            nuevo->siguiente = NULL;
            pieza_c++;
            // posicion e ingreso a la lista
            while (NULL != indice->siguiente)
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
    while (NULL != indice->siguiente)
    {
        indice = indice->siguiente;

        if (colision_snake(lista->snake, indice->snake.pos))
        {
            return 'x';
        }
    }

    // colision con pared
    if (colision_pared(lista->snake))
    {
        return 'x';
    }

    return tecla;
}

bool colision_snake(snake_t snake, rec_t hitbox)
{
    Vector2 cabeza = cabeza_snake(snake);
    return (CheckCollisionPointRec(cabeza, hitbox));
}

Vector2 cabeza_snake(snake_t snake)
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

bool colision_pared(snake_t snake)
{
    Vector2 cabeza = cabeza_snake(snake);
    switch (snake.direccion)
    {
    case DERECHA:
        if (cabeza.x > BORDE_RIGHT)
        {
            return true;
        }
        break;
    case ARRIBA:
        if (cabeza.y < BORDE_UP)
        {
            return true;
        }
        break;
    case IZQUIERDA:
        if (cabeza.x < BORDE_LEFT)
        {
            return true;
        }
        break;
    case ABAJO:
        if (cabeza.y > BORDE_DOWN)
        {
            return true;
        }
        break;
    }
    return false;
}