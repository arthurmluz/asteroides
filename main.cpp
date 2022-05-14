// **********************************************************************
// PUCRS/Escola Polit�cnica
// COMPUTA��O GR�FICA
//
// Programa basico para criar aplicacoes 2D em OpenGL
//
// Marcio Sarroglia Pinho
// pinho@pucrs.br
// 
//  Trabalho 2 - Asteroides
//   Arthur Musskopf da Luz & Manoella Jarces de Azevedo
// **********************************************************************


#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>


using namespace std;

#ifdef WIN32
#include <windows.h>
#include <glut.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

#include "app/Ponto.h"
#include "app/Instancia.h"

#include "app/Temporizador.h"
#include "app/ListaDeCoresRGB.h"

#include "util/bezierFunc.h"
#include "util/desenhar.h"
#include "util/util.h"
#include "util/movimentos.h"

#define NMONSTROS 4
#define NTIROS 20
#define TAM_MAPA 1000 // MIN(TAM_MAPA, TAM_MAPA)  MAX(TAM_MAPA, TAM_MAPA)
#define TAM_JANELA 1000

Temporizador T;
double AccumDeltaT=0;

Instancia Universo[NMONSTROS];
Instancia jogador, teste;


// Limites l�gicos da �rea de desenho
Ponto Min, Max;

bool pause = false;

Poligono monstro[4], disparador, tiro;
int nInstancias=0, atirados = 0;

float angulo=0.0;
constexpr float escala = 2 * TAM_MAPA/10.0;

void CriaInstancias();

Ponto curvas[NMONSTROS][3];

bool animando = false;

Ponto inicio = Ponto(0,0), fim = Ponto(0,0);


// **************************************************************
//
// **************************************************************
void CarregaModelos()
{
    monstro[0].LeObjeto("txts/monstro1.txt");
    monstro[1].LeObjeto("txts/monstro2.txt");
    monstro[2].LeObjeto("txts/monstro3.txt");
    monstro[3].LeObjeto("txts/monstro4.txt");
    disparador.LeObjeto("txts/nave.txt");
    tiro.LePoligono("txts/tiro.txt");
}
// **************************************************************
//
// **************************************************************
void CriaCurvas()
{
    for(int i = 0; i < NMONSTROS; i++){
        curvas[i][0] = pontoAleatorio(Min, Max);
        curvas[i][1] = Ponto(0, 0);
        curvas[i][2] = Ponto(0, 0);
    }
}
// **************************************************************
//
// **************************************************************
void init()
{
    srand(time(NULL));
    // Define a cor do fundo da tela (CINZA)
  //  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    float d = TAM_MAPA;

    Min = Ponto(-d,-d);
    Max = Ponto(d,d);
    printf("Tamanho Mapa: %d.\n", TAM_MAPA);

    CarregaModelos();
    CriaCurvas();
    CriaInstancias();
    
}

double nFrames=0;
double TempoTotal=0;

// **************************************************************
//
// **************************************************************
void animate()
{
    double dt;
    dt = T.getDeltaT();
    AccumDeltaT += dt;
    TempoTotal += dt;
    nFrames++;

    if (AccumDeltaT > 1.0/30) // fixa a atualiza��o da tela em 30
    {
        AccumDeltaT = 0;
        angulo+=2;
        glutPostRedisplay();
    }
    if (TempoTotal > 5.0)
    {
//        cout << "Tempo Acumulado: "  << TempoTotal << " segundos. " ;
//        cout << "Nros de Frames sem desenho: " << nFrames << endl;
//        cout << "FPS(sem desenho): " << nFrames/TempoTotal << endl;
        TempoTotal = 0;
        nFrames = 0;
    }
}
// **************************************************************
//  void reshape( int w, int h )
//  trata o redimensionamento da janela OpenGL
// **************************************************************
void reshape( int w, int h )
{
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define a area a ser ocupada pela area OpenGL dentro da Janela
    glViewport(0, 0, w, h);
    // Define os limites logicos da area OpenGL dentro da Janela
    glOrtho(Min.x,Max.x, Min.y,Max.y, -10,+10);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
// ****************************************************************
// nao sei pra que serve
// ****************************************************************
//void RotacionaAoRedorDeUmPonto(float alfa, Ponto P)
//{
//    glTranslatef(P.x, P.y, P.z);
//    glRotatef(alfa, 0,0,1);
//    glTranslatef(-P.x, -P.y, -P.z);
//}
// **************************************************************
//
// **************************************************************
void DesenhaEixos()
{
    Ponto Meio;
    Meio.x = (Max.x+Min.x)/2;
    Meio.y = (Max.y+Min.y)/2;
    Meio.z = (Max.z+Min.z)/2;

    glBegin(GL_LINES);
    //  eixo horizontal
        glVertex2f(Min.x,Meio.y);
        glVertex2f(Max.x,Meio.y);
    //  eixo vertical
        glVertex2f(Meio.x,Min.y);
        glVertex2f(Meio.x,Max.y);
    glEnd();
}

// ****************************************************************

void desenhaDisparador(){
    glPushMatrix();
        glLineWidth(2);
        glPointSize(6);
        glTranslatef(-13.5, 0, 0);
        disparador.desenhaVerticesColoridas();
        glPointSize(1);
    glPopMatrix();
}

void desenhaTiro(){
    glPushMatrix();
        glLineWidth(1);
        defineCor(Yellow);
        //glTranslatef(-0.25, 0, 0);
        tiro.pintaPoligono();
    glPopMatrix();
}

void desenhaTiroMonstro(){
    glPushMatrix();
        glLineWidth(1);
        defineCor(Red);
        //glTranslatef(-0.25, 0, 0);
        tiro.pintaPoligono();
    glPopMatrix();
}


void desenhaMonstro(){
    glPushMatrix();
        glPointSize(6);
        glTranslatef(-5, 0,0 );
        monstro[0].desenhaVerticesColoridas();
        glPointSize(1);
    glPopMatrix();
}

void desenhaMonstro1(){
    glPushMatrix();
        glPointSize(6);
        glTranslatef(-3.5, 0,0 );
        monstro[1].desenhaVerticesColoridas();
        glPointSize(1);
    glPopMatrix();
}

void desenhaMonstro2(){
    glPushMatrix();
        glPointSize(6);
        glTranslatef(-3.5, 0,0 );
        monstro[2].desenhaVerticesColoridas();
        glPointSize(1);
    glPopMatrix();
}

void desenhaMonstro3(){
    glPushMatrix();
        glPointSize(6);
        glTranslatef(-3.5, 0,0 );
        monstro[3].desenhaVerticesColoridas();
        glPointSize(1);
    glPopMatrix();
}


// ****************************************************************
// Esta fun��o deve instanciar todos os personagens do cen�rio
// ****************************************************************

void CriaInstancias()
{

    Ponto min, max;
    disparador.obtemLimites(min, max);
    jogador.posicao = Ponto(0,0) ;
    jogador.rotacao = 0;
    jogador.modelo = desenhaDisparador;

    //jogador.escala = Ponto(escala, escala, escala);
    jogador.escala = Ponto(TAM_MAPA/(100.0 * (max.x/10)), TAM_MAPA/(100.0 * (max.y/10)), TAM_MAPA/100.0);
    jogador.vidas = 3;

    for(int i = 0; i < NMONSTROS; i++ ){
        Universo[i].rotacao = 0;
        Universo[i].posicao = pontoAleatorio(Min, Max);
        switch(i % 4){
            case 0: 
                Universo[i].modelo = desenhaMonstro;
                break;
            case 1: 
                Universo[i].modelo = desenhaMonstro1;
                break;
            case 2: 
                Universo[i].modelo = desenhaMonstro2;
                break;
            case 3: 
                Universo[i].modelo = desenhaMonstro3;
                break;

        }
//        Universo[i].escala = Ponto( escala/2, escala/2, escala/2);
        Universo[i].escala = Ponto(TAM_MAPA/100.0, TAM_MAPA/100.0, TAM_MAPA/100.0);
    }

    //teste.posicao = Ponto(0,0);
    //teste.modelo = desenhaTeste;
    //teste.escala = Ponto(TAM_MAPA/100.0, TAM_MAPA/100.0, TAM_MAPA/100.0);
}

// ****************************************************************

void atirar(){
    if(atirados == 10) return;

    jogador.tiros.push_back(Instancia());
    Instancia &novoTiro = jogador.tiros.back();
    novoTiro.modelo = desenhaTiro;
    novoTiro.escala = Ponto(escala/2, escala/2, escala/2);

    novoTiro.rotacao = jogador.rotacao;
    float alfa = (novoTiro.rotacao * M_PI)/180.0f;
    float xr = cos(alfa) * 0 + (-sin(alfa) * 2);
    float yr = sin(alfa) * 0 + cos(alfa) * 2;
    novoTiro.dir = Ponto(xr, yr) * TAM_MAPA/100.0;

    novoTiro.posicao = novoTiro.dir*3 + jogador.posicao; 

    atirados++;
}

void atirarMonstro(Instancia &atirador){
    if(atirador.delay > 0 ) {
        atirador.delay--;
        return;
    }
    atirador.tiros.push_back(Instancia());
    Instancia &novoTiro = atirador.tiros.back();
    novoTiro.modelo = desenhaTiroMonstro;
    novoTiro.escala = Ponto(escala/2, escala/2, escala/2);

    novoTiro.rotacao = atirador.rotacao;

    float alfa = (novoTiro.rotacao * M_PI)/180.0f;
    float xr = cos(alfa) * 0 + (-sin(alfa) * 2);
    float yr = sin(alfa) * 0 + cos(alfa) * 2;
    novoTiro.dir = Ponto(xr, yr) * TAM_MAPA/100.0;

    novoTiro.posicao = novoTiro.dir*3 + atirador.posicao; 
    atirador.delay = 3;
}


// ****************************************************************
void animaMonstros(){

    Ponto pontosUteis[2]{};
    pontosUteis[0] = jogador.posicao;

    for(int i=0; i<NMONSTROS;i++)
    {
        pontosUteis[1] = pontoAleatorio(Min, Max);

        defineCor(MandarinOrange);
        andarNaBezier(Universo[i], pontosUteis, curvas[i]);
        Universo[i].desenha();
           // defineCor(NavyBlue);
           // TracaBezier3Pontos(curvas[i]);
           // defineCor(Pink);
           // TracaPontosDeControle(curvas[i]);

        if( rand() % 20 == 0) atirarMonstro(Universo[i]);

    }
}

void DesenhaLinha(Ponto P1, Ponto P2){
    glBegin(GL_LINES);
        glVertex3f(P1.x,P1.y,P1.z);
        glVertex3f(P2.x,P2.y,P2.z);
    glEnd();
}

void animaJogador(){
    Ponto verificaParedes = jogador.posicao + jogador.dir;
    fim = fim + jogador.dir;
    if( !(verificaParedes.x <= Min.x+(TAM_MAPA/10) || verificaParedes.x >= Max.x-(TAM_MAPA/10)) ){
        if( !(verificaParedes.y <= Min.y+ (TAM_MAPA/10)|| verificaParedes.y >= Max.y-(TAM_MAPA/10)) ){
            jogador.posicao = verificaParedes;
        }
    }
    jogador.dir = jogador.dir * 0.95;
//    if(abs(jogador.dir.x) * 10 < 1) jogador.dir.x = 0;
//    if(abs(jogador.dir.y) * 10 < 1) jogador.dir.y = 0;
    if(abs(jogador.dir.x * TAM_MAPA) > 1 || abs(jogador.dir.y * TAM_MAPA) > 1){
        inicio = jogador.posicao;
        fim = jogador.dir + jogador.posicao;
    }
    else {
        inicio = Ponto(0,0);
        fim = Ponto(0,0);
    }

    jogador.desenha();
}

void animaTiros(){
    for(Instancia &it: jogador.tiros){
        it.posicao = it.posicao + it.dir;
        it.desenha();
        if( it.posicao.x > Max.x || it.posicao.x < Min.x || it.posicao.y > Max.x || it.posicao.y < Min.y ){
            atirados--;
            jogador.tiros.erase(jogador.tiros.begin());
        }
    }
    for(int i = 0; i < NMONSTROS; i++){
        for(Instancia &it: Universo[i].tiros){
            it.posicao = it.posicao + it.dir;
            it.desenha();
            if( it.posicao.x > Max.x || it.posicao.x < Min.x || it.posicao.y > Max.x || it.posicao.y < Min.y ){
                Universo[i].tiros.erase(Universo[i].tiros.begin());
            }
        }
    }
}

// ****************************************************************
//  void display( void )
// ****************************************************************
void display( void )
{

	// Limpa a tela coma cor de fundo
	glClear(GL_COLOR_BUFFER_BIT);

    // Define os limites l�gicos da �rea OpenGL dentro da Janela
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// Coloque aqui as chamadas das rotinas que desenham os objetos
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    if(pause){
        return;
    }

    glPushMatrix();
        glLineWidth(1);
        glColor3f(1,1,1); // R, G, B  [0..1]
        DesenhaEixos();
    glPopMatrix();
    

    glPointSize(TAM_MAPA*10/100.0);
   // teste.desenha();
    //glPointSize(1);

    animaJogador();

    animaTiros();

    animaMonstros();

    glLineWidth(2);
    defineCor(Red);
    // foguinho atrás do personagem
    DesenhaLinha(inicio, fim);

//    andarNaBezier(Universo[3], Curva1);
//    DesenhaUniverso();
//    defineCor(VioletRed);
//    TracaBezier3Pontos(Curva1);
//    defineCor(MandarinOrange);
//    TracaPontosDeControle(Curva1);
    
    double dt;
    dt = T.getDeltaT();
    
    
    // Como instanciar o mapa de forma que ocupe a tela toda?
    //Mapa.desenhaPoligono();
    
	glutSwapBuffers();
}
// ****************************************************************
// ContaTempo(double tempo)
//      conta um certo n�mero de segundos e informa quanto frames
// se passaram neste per�odo.
// ****************************************************************
void ContaTempo(double tempo)
{
    Temporizador T;

    unsigned long cont = 0;
    cout << "Inicio contagem de " << tempo << "segundos ..." << flush;
    while(true)
    {
        tempo -= T.getDeltaT();
        cont++;
        if (tempo <= 0.0)
        {
            cout << "fim! - Passaram-se " << cont << " frames." << endl;
            break;
        }
    }
}
// ****************************************************************
//  void keyboard ( unsigned char key, int x, int y )
// ****************************************************************
void keyboard ( unsigned char key, int x, int y )
{

    switch ( key )
    {
        case 27:        // Termina o programa qdo
            exit ( 0 );   // a tecla ESC for pressionada
            break;
        case 't':
            ContaTempo(3);
            break;
        case ' ':
            atirar();
        break;
        case 'p':
            pause = !pause;
            if(pause)
                printf("PAUSADO\n");
            else printf("DESPAUSADO\n");
            break;
        case 'r':
            jogador.posicao = Ponto(0,0);
            jogador.rotacao = 0;
            jogador.dir = Ponto(0,0,0);
            atirados = 0;
        default:
			break;
	}
}
// ****************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
// ****************************************************************

void arrow_keys ( int a_keys, int x, int y )
{
	switch ( a_keys )
	{
        case GLUT_KEY_LEFT:
            if(pause) return;
            jogador.rotacao += 10;
            if(jogador.rotacao >= 360){
               jogador.rotacao = 0; 
            }

//            printf("rotac %f\n", jogador.rotacao);
            break;

        case GLUT_KEY_RIGHT:
            if(pause) return;
            jogador.rotacao -= 10;
            if(jogador.rotacao <= 0){
               jogador.rotacao = 360; 
            }
//            printf("rotac %f\n", jogador.rotacao);
            break;

		case GLUT_KEY_UP:     
            if(pause) return;
//            inicio = jogador.posicao; esses 2 faz o traçado grande
//            fim = jogador.posicao;
            andaFrente(jogador, TAM_MAPA);
			break;

	    case GLUT_KEY_DOWN:     
			break;
		default:
			break;
	}
}

// ****************************************************************
//  void main ( int argc, char** argv )
// ****************************************************************
int  main ( int argc, char** argv )
{
    cout << "Programa OpenGL" << endl;

    glutInit            ( &argc, argv );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    glutInitWindowPosition (0,0);

    // Define o tamanho inicial da janela grafica do programa
    glutInitWindowSize  ( TAM_JANELA, TAM_JANELA);

    // Cria a janela na tela, definindo o nome da
    // que aparecera na barra de t�tulo da janela.
    glutCreateWindow    ( "Transformacoes Geometricas em OpenGL" );

    // executa algumas inicializa��es
    init ();

    // Define que o tratador de evento para
    // o redesenho da tela. A funcao "display"
    // ser� chamada automaticamente quando
    // for necess�rio redesenhar a janela
    glutDisplayFunc ( display );

    // Define que o tratador de evento para
    // o invalida��o da tela. A funcao "display"
    // ser� chamada automaticamente sempre que a
    // m�quina estiver ociosa (idle)
    glutIdleFunc(animate);

    // Define que o tratador de evento para
    // o redimensionamento da janela. A funcao "reshape"
    // ser� chamada automaticamente quando
    // o usu�rio alterar o tamanho da janela
    glutReshapeFunc ( reshape );

    // Define que o tratador de evento para
    // as teclas. A funcao "keyboard"
    // ser� chamada automaticamente sempre
    // o usu�rio pressionar uma tecla comum
    glutKeyboardFunc ( keyboard );

    // Define que o tratador de evento para
    // as teclas especiais(F1, F2,... ALT-A,
    // ALT-B, Teclas de Seta, ...).
    // A funcao "arrow_keys" ser� chamada
    // automaticamente sempre o usu�rio
    // pressionar uma tecla especial
    glutSpecialFunc ( arrow_keys );

    // inicia o tratamento dos eventos
    glutMainLoop ( );

    return 0;
}
