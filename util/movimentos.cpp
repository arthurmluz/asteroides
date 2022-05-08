#include "movimentos.h"

void andaFrente(Instancia &jogador){
    jogador.dir.y += 4;
    float alfa = (jogador.rotacao * M_PI)/180.0f;
    printf("angulo>: %f\n", alfa);
    //float xr = cos(alfa) * jogador.dir.x + (-sin(alfa) * jogador.dir.y) ;

    float xr = cos(alfa) * 0 + (-sin(alfa) * jogador.dir.y);
    float yr = sin(alfa) * 0 + cos(alfa) * jogador.dir.y;
//    float xr = jogador.dir.x * cos(alfa) - jogador.dir.y * sin(alfa);
//    float yr = jogador.dir.x * sin(alfa) - jogador.dir.y * cos(alfa);
    printf("(%f, %f)\n", xr, yr);
    jogador.dir = Ponto(xr, yr);
}

