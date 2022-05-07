#include "desenhar.h"

// **********************************************************************
void DesenhaSeta(Poligono MeiaSeta)
{
    glPushMatrix();
        MeiaSeta.desenhaPoligono();
        glScaled(1,-1, 1);
        MeiaSeta.desenhaPoligono();
    glPopMatrix();
}

// **********************************************************************
void DesenhaApontador(Poligono MeiaSeta)
{
    glPushMatrix();
        glTranslated(-4, 0, 0);
        DesenhaSeta(MeiaSeta);
    glPopMatrix();
}

// ****************************************************************
void DesenhaHelice(Poligono MeiaSeta)
{
    glPushMatrix();
    for(int i=0;i < 4; i++)
    {
        glRotatef(90, 0, 0, 1);
        DesenhaApontador(MeiaSeta);
    }
    glPopMatrix();
}

// ****************************************************************
void DesenhaHelicesGirando(Poligono MeiaSeta, float angulo)
{
    glPushMatrix();
        glRotatef(angulo, 0, 0, 1);
        DesenhaHelice(MeiaSeta);
   glPopMatrix();
}

// ****************************************************************
void DesenhaMastro(Poligono Mastro)
{
    Mastro.desenhaPoligono();
}

// ****************************************************************
