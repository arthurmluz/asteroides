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
    glPointSize(1);
}

// **************************************************************
Ponto CalculaBezier3(Ponto PC[], double t)
{
    Ponto P;
    double UmMenosT = 1-t;
    
    P =  PC[0] * UmMenosT * UmMenosT + PC[1] * 2 * UmMenosT * t + PC[2] * t*t;
    return P;
}

Ponto Calcula(double t, Ponto curva[])
{
    Ponto P;
    double UmMenosT = 1-t;
    
    P =  curva[0] * UmMenosT * UmMenosT + curva[1] * 2 * UmMenosT * t + curva[2] * t*t;
    return P;
}

// *****************************************************************
float calculaDistancia(Ponto p1, Ponto p2){
    float dist = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2)); 
    return dist;
}

// *****************************************************************
double calculaComprimentoDaCurva(Ponto curva[])
{
    double DeltaT = 1.0/50;
    double t=DeltaT;
    Ponto P1, P2;
    
    double ComprimentoTotalDaCurva = 0;
    
    P1 = Calcula(0.0, curva);
    while(t<1.0)
    {
        P2 = Calcula(t, curva);
        ComprimentoTotalDaCurva += calculaDistancia(P1,P2);
        P1 = P2;
        t += DeltaT;
    }
    P2 = Calcula(1.0, curva); // faz o fechamento da curva
    ComprimentoTotalDaCurva += calculaDistancia(P1,P2);
    return ComprimentoTotalDaCurva;
} 

// *****************************************************************
double calculaT(double distanciaPercorrida, double ComprimentoTotalDaCurva)
{
    double a = (distanciaPercorrida/ComprimentoTotalDaCurva);
    //printf(" a= %f\n", a);
    return a;
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

    return atan2(delta_x, delta_y) * ang; //descomentar isso me dá a resposta em angulos (debugar)
}
// *****************************************************************
void andarNaBezier(Instancia &andador, Ponto pontosUteis[], Ponto curva[], double tempoTotal){
    if(andador.tAtual == 0){
        andador.tAtual = 0.001;
        andador.comprimentoCurva = calculaComprimentoDaCurva(pontosUteis);
        andador.tempoInicial = tempoTotal;
        andador.velocidade = andador.comprimentoCurva/1000;

    }

    if(andador.tAtual > 1.0 ){
        andador.tAtual = 0.001;
        andador.velocidade = 0.3;
        curva[0] = curva[2];
        curva[1] = pontosUteis[0]; // 0 = disparador.posicao
        curva[2] = pontosUteis[1]; // 1 = ponto aleatorio
        andador.comprimentoCurva = calculaComprimentoDaCurva(pontosUteis);
        andador.tempoInicial = tempoTotal;
    }

    double deslocamento = andador.velocidade*(tempoTotal-andador.tempoInicial);
    //printf("%f\n", andador.velocidade);
    andador.deltaT = calculaT(deslocamento, andador.comprimentoCurva);
    //printf("dT: %f, desloc = %f, tempo = %f, inicial = %f\n", andador.deltaT, deslocamento,tempoTotal, andador.tempoInicial);
    //andador.tempoInicial = tempoTotal;
    if((andador.deltaT) == 0){
        andador.deltaT = 0.002;
    }
    andador.rotacao = anguloVetores(andador.posicao, pontosUteis[0]);
    Ponto P = CalculaBezier3(curva, andador.tAtual);
    andador.posicao = P;
    andador.tAtual += andador.deltaT;
}

