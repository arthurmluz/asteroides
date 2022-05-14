#include "bezierFunc.h"

// **************************************************************
void TracaPontosDeControle(Ponto PC[])
{
    glPointSize(10);
    glBegin(GL_POINTS);
        glVertex3f(PC[0].x, PC[0].y, PC[0].z);
        glVertex3f(PC[1].x, PC[1].y, PC[1].z);
        glVertex3f(PC[2].x, PC[2].y, PC[2].z);
    glEnd();
}

// **************************************************************
Ponto CalculaBezier3(Ponto PC[], double t)
{
    Ponto P;
    double UmMenosT = 1-t;
    
    P =  PC[0] * UmMenosT * UmMenosT + PC[1] * 2 * UmMenosT * t + PC[2] * t*t;
    return P;
}

// *****************************************************************
void TracaBezier3Pontos(Ponto curva[])
{
    double t=0.0;
    double DeltaT = 1.0/50;
    Ponto P;
    //cout << "DeltaT: " << DeltaT << endl;
    glBegin(GL_LINE_STRIP);
    
    while(t<1.0)
    {
        P = CalculaBezier3(curva, t);
        glVertex2f(P.x, P.y);
        t += DeltaT;
    }
    P = CalculaBezier3(curva, 1.0); // faz o acabamento da curva
    glVertex2f(P.x, P.y);
    
    glEnd();
}

constexpr float ang = 180/M_PI;

// calcula o angulo entre 2 vértices (pontos)
double anguloVetores(Ponto monstro, Ponto jogador){
    double delta_x, delta_y;
    delta_x = monstro.x - jogador.x;
    delta_y = jogador.y - monstro.y;

    return atan2(delta_x, delta_y) * 180 / 3.14159265; //descomentar isso me dá a resposta em angulos (debugar)
}
// *****************************************************************
void andarNaBezier(Instancia &andador, Ponto pontosUteis[], Ponto curva[]){
    if(andador.tAtual > 1.0 || andador.tAtual < 0.0){
         andador.tAtual = 0;
        //andador.deltaT *= -1;
        curva[0] = curva[2];
        curva[1] = pontosUteis[0]; // 0 = disparador.posicao
        curva[2] = pontosUteis[1]; // 1 = ponto aleatorio
    }

    andador.rotacao = anguloVetores(andador.posicao, pontosUteis[0]);
    //andador.rotacao += andador.deltaT * 50;
    Ponto P = CalculaBezier3(curva, andador.tAtual);
    andador.posicao = P;
    andador.tAtual += andador.deltaT;
}

