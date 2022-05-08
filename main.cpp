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

#define NMONSTROS 3
#define TAM_MAPA 100
Temporizador T;
double AccumDeltaT=0;

Instancia Universo[NMONSTROS];
Instancia jogador;

// Limites l�gicos da �rea de desenho
Ponto Min, Max;

bool desenha = false, pause = false;

Poligono monstro, disparador;
int nInstancias=0;

float angulo=0.0;

void CriaInstancias();

Ponto curvas[NMONSTROS][3];

bool animando = false;

Ponto inicio = Ponto(0,0), fim = Ponto(0,0);


// **************************************************************
//
// **************************************************************
void CarregaModelos()
{
    monstro.LePoligono("txts/monstro1.txt");
    disparador.LePoligono("txts/disparador.txt");
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
    // Define a cor do fundo da tela (AZUL)
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

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
        glTranslatef(-0.25, 0, 0);
        disparador.desenhaPoligono();
    glPopMatrix();
}

void desenhaMonstro(){
    glPushMatrix();
        glLineWidth(2);
        monstro.desenhaPoligono();
    glPopMatrix();
}


// ****************************************************************
// Esta fun��o deve instanciar todos os personagens do cen�rio
// ****************************************************************

void CriaInstancias()
{
    float escala = 2 * Max.x/20.0;
    jogador.posicao = Ponto(0,0) ;
    jogador.rotacao = 0;
    jogador.modelo = desenhaDisparador;
    jogador.escala = Ponto(escala, escala, escala);

    Universo[0].posicao = Ponto (5,5);
    Universo[0].rotacao = 0;
    Universo[0].modelo = desenhaMonstro;
    Universo[0].escala = Ponto (2,2,2);
    Universo[0].escala = Ponto( escala/2, escala/2, escala/2);


    Universo[1].posicao = Ponto (3,0);
    Universo[1].rotacao = 0;
    Universo[1].modelo = desenhaMonstro;
    Universo[1].escala = Ponto( escala/2, escala/2, escala/2);
    
    Universo[2].posicao = Ponto (0,-5);
    Universo[2].rotacao = 0;
    Universo[2].modelo = desenhaMonstro;
    Universo[2].escala = Ponto( escala/2, escala/2, escala/2);

    Universo[3].posicao = Ponto (-5,-5);
    Universo[3].rotacao = 0;
    Universo[3].modelo = desenhaMonstro;
    Universo[3].escala = Ponto( escala/2, escala/2, escala/2);

}


// ****************************************************************
void desenhaMonstros(){

    Ponto pontosUteis[2]{};
    pontosUteis[0] = jogador.posicao;

    for(int i=0; i<NMONSTROS;i++)
    {
        pontosUteis[1] = pontoAleatorio(Min, Max);

        andarNaBezier(Universo[i], pontosUteis, curvas[i]);
        Universo[i].desenha();

            defineCor(NavyBlue);
            TracaBezier3Pontos(curvas[i]);
            defineCor(Pink);
            TracaPontosDeControle(curvas[i]);

    }
}
void DesenhaLinha(Ponto P1, Ponto P2){
    glBegin(GL_LINES);
        glVertex3f(P1.x,P1.y,P1.z);
        glVertex3f(P2.x,P2.y,P2.z);
    glEnd();
}
void desenhaJogador(){
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

	glLineWidth(1);
	glColor3f(1,1,1); // R, G, B  [0..1]
    DesenhaEixos();
    
    glLineWidth(1);

    defineCor(VioletRed);
    desenhaJogador();

//    defineCor(MandarinOrange);
//    desenhaMonstros();

    glLineWidth(2);
    defineCor(Red);
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
    
    if (animando)
    {
        //AvancaPersonagens(dt);
    }
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
            desenha = !desenha;
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
    glutInitWindowSize  ( 500, 500);

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
