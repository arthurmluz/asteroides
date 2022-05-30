#include "movimentos.h"

void andaFrente(Instancia &jogador, int tamMapa){
//    printf("y = %f\n", jogador.dir.y);
    jogador.dir.y += 4 * tamMapa/100.0;
    float alfa = (jogador.rotacao * M_PI)/180.0f;
    //printf("angulo>: %f\n", alfa);
    float xr = (-sin(alfa) * jogador.dir.y);
    float yr = cos(alfa) * jogador.dir.y;
//    float xr = jogador.dir.x * cos(alfa) - jogador.dir.y * sin(alfa);
//    float yr = jogador.dir.x * sin(alfa) - jogador.dir.y * cos(alfa);
    if(abs(xr) > tamMapa){
        xr = tamMapa * (xr/abs(xr));
    }
    if(abs(yr) > tamMapa){
        yr = tamMapa * (yr/abs(yr));
    }
    jogador.dir = Ponto(xr, yr);
//    printf("(%f, %f)\n", jogador.dir.x, jogador.dir.y);
}

