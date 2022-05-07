#ifndef desenhar_hpp
#define desenhar_hpp

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
#include "../app/ListaDeCoresRGB.h"


void DesenhaSeta(Poligono MeiaSeta);
void DesenhaApontador(Poligono MeiaSeta);
void DesenhaHelice(Poligono MeiaSeta);
void DesenhaHelicesGirando(Poligono MeiaSeta, float angulo);
void DesenhaMastro(Poligono mastro);

#endif
