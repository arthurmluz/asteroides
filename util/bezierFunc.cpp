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

// *****************************************************************
void andarNaBezier(Instancia &andador, Ponto curva[]){
    if(andador.tAtual > 1.0 || andador.tAtual < 0.0){
         andador.tAtual = 0;
        //andador.deltaT *= -1;
        curva[0] = curva[2];
        for(int i = 1; i < 3; i++){
            float x = rand() % 15;
            float y = rand() % 15;
            if( x > 7 ){
                x = (x - 7) * -1;
            }
            if( y > 7 ){
               y = (y - 7) * -1;
            }
            curva[i] = Ponto(x, y);
        }
    }
    //andador.rotacao += andador.deltaT * 50;
    Ponto P = CalculaBezier3(curva, andador.tAtual);
    andador.posicao = P;
    andador.tAtual += andador.deltaT;
}

