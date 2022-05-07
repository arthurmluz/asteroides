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
    return Ponto(x, y);
} 

#endif