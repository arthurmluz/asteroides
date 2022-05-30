#ifndef Modelo_hpp
#define Modelo_hpp

#include <iostream>
using namespace std;


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

#include "Ponto.h"

class Modelo
{
    // lin, col, cores
    int matriz[200][200][1];
    int lin, col, cores;
    Cor ListaCores[20];
public:
    Modelo();
    int getColunas();
    int getLinhas();
    int getCores();
    void desenhaVerticesColoridas();
    void imprime();
    void atualizaLimites();
    void obtemLimites(int &linhas, int &colunas);
    void LeObjeto(const char *nome);


};

#endif 
