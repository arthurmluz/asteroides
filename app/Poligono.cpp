//
//  Poligono.cpp
//  OpenGLTest
//
//  Created by Márcio Sarroglia Pinho on 18/08/20.
//  Copyright © 2020 Márcio Sarroglia Pinho. All rights reserved.
//
#include <iostream>
#include <fstream>
using namespace std;

#include "Poligono.h"
#include "Cor.h"

Poligono::Poligono()
{
    
}

void Poligono::insereVertice(Ponto p)
{
    Vertices.push_back(p);
}

void Poligono::insereVertice(Ponto p, int pos)
{
    if ((pos < 0) || (pos>Vertices.size()))
    {
        cout << "Metodo " << __FUNCTION__ << ". Posicao Invalida. Vertice nao inserido." << endl;
        return;
    }
    Vertices.insert(Vertices.begin()+pos, p);
}

Ponto Poligono::getVertice(int i)
{
    return Vertices[i];
}

void Poligono::pintaPoligono()
{
    glBegin(GL_POLYGON);
    for (int i=0; i<Vertices.size(); i++)
        glVertex3f(Vertices[i].x,Vertices[i].y,Vertices[i].z);
    glEnd();
}

void Poligono::desenhaPoligono()
{
    glBegin(GL_LINE_LOOP);
    for (int i=0; i<Vertices.size(); i++)
        glVertex3f(Vertices[i].x,Vertices[i].y,Vertices[i].z);
    glEnd();
}
void Poligono::desenhaVertices()
{
    glBegin(GL_POINTS);
    for (int i=0; i<Vertices.size(); i++)
        glVertex3f(Vertices[i].x,Vertices[i].y,Vertices[i].z);
    glEnd();
}

void Poligono::desenhaVerticesColoridas()
{
    glBegin(GL_POINTS);
    for (int i=0; i<Vertices.size(); i++){
        Cor a = Vertices[i].cor;
        glColor3f(a.r, a.g, a.b);
        glVertex3f(Vertices[i].x,Vertices[i].y,Vertices[i].z);
    }
    glEnd();
}

void Poligono::imprime()
{
    for (int i=0; i<Vertices.size(); i++)
        Vertices[i].imprime();
}
unsigned long Poligono::getNVertices()
{
    return Vertices.size();
}

void Poligono::obtemLimites(Ponto &Min, Ponto &Max)
{
    Max = Min = Vertices[0];
    
    for (int i=0; i<Vertices.size(); i++)
    {
        Min = ObtemMinimo (Vertices[i], Min);
        Max = ObtemMaximo (Vertices[i], Max);
    }
}
// **********************************************************************
//
// **********************************************************************
void Poligono::LePoligono(const char *nome)
{
    ifstream input;            // ofstream arq;
    input.open(nome, ios::in); //arq.open(nome, ios::out);
    if (!input)
    {
        cout << "Erro ao abrir " << nome << ". " << endl;
        exit(0);
    }
    cout << "Lendo arquivo " << nome << "...";
    string S;
    //int nLinha = 0;
    unsigned int qtdVertices;
    
    input >> qtdVertices;  // arq << qtdVertices

    for (int i=0; i< qtdVertices; i++)
    {
        double x,y;
        // Le cada elemento da linha
        input >> x >> y; // arq << x  << " " << y << endl
        if(!input)
            break;
        //nLinha++;
        insereVertice(Ponto(x,y));
    }
    cout << "Poligono lido com sucesso!" << endl;

}

// **********************************************************************
//
// **********************************************************************
void Poligono::LeObjeto(const char *nome){
    ifstream input;            // ofstream arq;
    input.open(nome, ios::in); //arq.open(nome, ios::out);
    if (!input)
    {
        cout << "Erro ao abrir " << nome << ". " << endl;
        exit(0);
    }
    cout << "Lendo arquivo " << nome << "...";
    string S;
    //int nLinha = 0;

    std::string a;
    getline(input, a); // #CORES

    unsigned int qtdCores;
    input >> qtdCores;

    Cor cores[qtdCores];
    for(int i = 0; i < qtdCores; i++){
        int tmp, r, g, b;

        // le as cores
        input >> tmp >> cores[i].r >> cores[i].g >> cores[i].b >> a;
       // cout << "R: " << cores[i].r << " G: " << cores[i].g << " B: " << cores[i].b << endl;
    }

    // linha vazia
    input >> a;

    int lin, col;
    input >> lin >> col;
//    cout << "lin: " << lin << " col: " << col << endl;

    int tmp;
    for(int y = lin-1; y >= 0; y--){
        for(int x = 0; x < col; x++){
            input >> tmp;
            Ponto pnt = Ponto(x,y);
            pnt.cor = cores[tmp-1];
            insereVertice(pnt);
        }
    }

    cout << "Poligono lido com sucesso!" << endl;

}

void Poligono::getAresta(int n, Ponto &P1, Ponto &P2)
{
    P1 = Vertices[n];
    int n1 = (n+1) % Vertices.size();
    P2 = Vertices[n1];
}

void Poligono::desenhaAresta(int n)
{
    glBegin(GL_LINES);
        glVertex3f(Vertices[n].x,Vertices[n].y,Vertices[n].z);
        int n1 = (n+1) % Vertices.size();
        glVertex3f(Vertices[n1].x,Vertices[n1].y,Vertices[n1].z);
    glEnd();
}

void Poligono::ObtemVerticesLimite(int &Esq, int &Dir, int &Inf, int &Sup)
{

    Esq = Dir = Inf = Sup = 0;
    
    for (int i=1; i< Vertices.size(); i++)
    {
        if (Vertices[i].x < Vertices[Esq].x)
            Esq = i;
        if (Vertices[i].x > Vertices[Dir].x)
            Dir = i;
        if (Vertices[i].y < Vertices[Inf].y)
            Inf = i;
        if (Vertices[i].y > Vertices[Sup].y)
            Sup = i;
    }
    cout << "Esq: " << Esq << "\n";
    cout << "Dir: " << Dir << "\n";
    cout << "Inf: " << Inf << "\n";
    cout << "Sup: " << Sup << endl;
    
}

