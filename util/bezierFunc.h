#ifndef bezierFunc_hpp
#define bezierFunc_hpp

#ifdef WIN32
#include <windows.h>
#include <glut.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

#include "../app/Ponto.h"
#include "../app/Instancia.h"

#include <math.h>

void TracaPontosDeControle(Ponto PC[]);
Ponto CalculaBezier3(Ponto PC[], double t);
void TracaBezier3Pontos(Ponto curva[]);
void andarNaBezier(Instancia &andador, Ponto pontosUteis[], Ponto curva[], double tempoTotal);


#endif
