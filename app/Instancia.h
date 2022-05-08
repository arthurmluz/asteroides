//
//  Instancia.hpp
//  OpenGLTest
//
//  Created by Márcio Sarroglia Pinho on 22/09/20.
//  Copyright © 2020 Márcio Sarroglia Pinho. All rights reserved.
//


#ifndef Instancia_hpp
#define Instancia_hpp
#include <iostream>
using namespace std;

#include "Poligono.h"
typedef void TipoFuncao();

class Instancia{
public:
    //Poligono *modelo;
    int nroCurva; // nao consigo deletar sem bugar
    float tAtual;
    float deltaT = 1.0/50;
    Instancia();
    TipoFuncao *modelo;
    Ponto posicao, escala, dir;
    float rotacao;
    void desenha();
    void setModelo(Poligono *P);
};


#endif /* Instancia_hpp */
