#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include<unistd.h>

#define ALTO 10
#define ANCHO 10

typedef struct _snake
{                   
    int x;          //                              1
    int y;          //                              
    int direccion;  //---------------->     2       p       0                    
    int status;     //                              
                    //                              3
}Tsnake;

typedef struct _fruta
{
    int x;
    int y;
    int status;
}Tfruta;

typedef Tsnake Tpieza;

int main()
{   
    int finb=0;
    int i,j,k;
    
    Tsnake snake;
    snake.direccion=-1;
    snake.status=1;
    snake.x=5;
    snake.y=5;

    Tfruta fruta;
    fruta.status=1;
    fruta.x=2;
    fruta.y=2;

    int casillas=ANCHO*ALTO;
    Tpieza cuerpo[casillas];
    
    char tecla = ' ';

    while(!finb)
    {
        //dibujado

        
        system("cls");
        k=0;
        for(i=-1;i<=ANCHO;i++)
        {
            for(j=-1;j<=ALTO;j++)
            {
                if(i==-1)
                {
                    printf("-");
                    k++;
                    continue;
                }
                if(j==-1)
                {
                    printf("-");
                    k++;
                    continue;
                }
                if(i==ANCHO)
                {
                    printf("-");
                    k++;
                    continue;
                }
                if(j==ALTO)
                {
                    printf("-");
                    k++;
                    continue;
                }

                //cabeza
                if(i==snake.x)
                {
                    if(j==snake.y)
                    {
                        printf("*");
                        k++;
                        continue;
                    }
                }

                //fruta
                if(i==fruta.x)
                {
                    if(j==fruta.y)
                    {
                        printf("+");
                        k++;
                        continue;
                    }
                }

                //cuerpo
                if(cuerpo[k].status)
                {
                    if(i==cuerpo[k].x)
                    {
                        if(j==cuerpo[k].y)
                        {
                            printf("*");
                            k++;
                            continue;
                        }
                    }
                }
                //tablero
                printf(" ");
                k++;
            }
            printf("\n");
        }

        

        //input
        if(kbhit())
        {
            fflush(stdin);
            tecla=tolower(getch());
            switch (tecla)
            {
                case 'x':
                    snake.status=0;
                    finb=1;
                    break;

                case 'd':
                    snake.direccion=0;
                    break;

                case 'w':
                    snake.direccion=1;
                    break;

                case 'a':
                    snake.direccion=2;
                    break;

                case 's':
                    snake.direccion=3;
                    break;
                default:
                    sleep(1);
                    break;
            }
            sleep(1);
        }
        else
        {
            sleep(1);
        }

        //movimiento

        //cuerpo
        for(i=0;i<casillas-1;i++)
        {
            if(cuerpo[i].status)
            {
                if(i==0)
                {
                    cuerpo[i].x=snake.x;
                    cuerpo[i].y=snake.y;
                }
                else
                {
                    cuerpo[i].x=cuerpo[i-1].x;
                    cuerpo[i].y=cuerpo[i-1].y;
                }
            }
        }

        //cabeza
        if(snake.direccion != -1)
        {
            switch (snake.direccion)
            {
                case 0:
                    if(snake.y<ALTO)
                    {
                        snake.y++;
                    }
                    break;
                case 1:
                    if(snake.x>0)
                    {
                        snake.x--;
                    }
                    break;
                case 2:
                    if(snake.y>0)
                    {
                        snake.y--;
                    }
                    break;
                case 3:
                    if(snake.x<ANCHO)
                    {
                        snake.x++;
                    }
                    break;
            }
        }

        
    }
    
    return 0;
}