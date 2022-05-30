#ifndef util_hpp
#define util_hpp

#include "../app/Ponto.h"

float RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

Ponto pontoAleatorio(Ponto min, Ponto max){
    float x = RandomFloat(min.x, max.x);
    float y = RandomFloat(min.y, max.y);
    if( x == 0 || y == 0) return pontoAleatorio(min, max);
    return Ponto(x, y);
} 

Ponto pontoAleatorioMonstro(Ponto min, Ponto max, int n, int NMONSTRO){
    float x, y;

    switch( n % NMONSTRO ){
        case 0:
           x = RandomFloat(min.x, max.x);
           y = RandomFloat(max.y, max.y+max.y);
           break;
        case 1:
           x = RandomFloat(min.x, min.x-max.x);
           y = RandomFloat(min.y, max.y);
           break; 
        case 2:
           x = RandomFloat(min.x, max.x);
           y = RandomFloat(min.y, min.y-max.y);
           break;
        default:
           x = RandomFloat(max.x, max.x+max.x);
           y = RandomFloat(min.y, max.y);
    }
    
    if( x == 0 || y == 0) return pontoAleatorioMonstro(min, max, n, NMONSTRO);
    return Ponto(x, y);
} 


#endif