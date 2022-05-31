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
#include "app/Modelo.h"

#include "app/Temporizador.h"
#include "app/ListaDeCoresRGB.h"

#include "util/bezierFunc.h"
#include "util/desenhar.h"
#include "util/util.h"
#include "util/movimentos.h"

#define NMONSTROS 1 
#define NVIDAS 3 
#define MODELOS_MONSTROS 4
#define NTIROS 10
#define TAM_MAPA 100 // MIN(TAM_MAPA, TAM_MAPA)  MAX(TAM_MAPA, TAM_MAPA)
#define TAM_JANELA 800

Temporizador T;
double AccumDeltaT=0;

//Instancias
Instancia Universo[NMONSTROS];
Instancia jogador, teste;

// Curvas
Ponto curvas[NMONSTROS][3];

// Modelo dos personagens
Poligono tiro, derrota, vitoria;
Modelo monstro[MODELOS_MONSTROS], disparador;

// Limites l�gicos da �rea de desenho
Ponto Min, Max;

// pausar (P) e debug (D)
bool pause = false, debug = false, imune = false;

// tiros do jogador
int atirados = 0;
// escala do jogador e outras coisas
constexpr float escala = 2 * TAM_MAPA/10.0;

int monstrosVivos = NMONSTROS;

bool animando = false;

void CriaInstancias();
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
    derrota.LeObjeto("txts/derrota.txt");
    vitoria.LeObjeto("txts/vitoria.txt");
}
// **************************************************************
//
// **************************************************************
void CriaCurvas()
{
    for(int i = 0; i < NMONSTROS; i++){
        curvas[i][0] = pontoAleatorioMonstro(Min, Max, i, MODELOS_MONSTROS);
        curvas[i][1] = pontoAleatorio(Min, Max);
        curvas[i][2] = pontoAleatorio(Min, Max);
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
        glutPostRedisplay();
    }
    if (TempoTotal > 5.0)
    {
 //       cout << "Tempo Acumulado: "  << TempoTotal << " segundos. " ;
//        cout << "Nros de Frames sem desenho: " << nFrames << endl;
//        cout << "FPS(sem desenho): " << nFrames/TempoTotal << endl;
 //       TempoTotal = 0;
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

void desenhaDisparador(int num){
    int lin, col;
    disparador.obtemLimites(lin, col);
    glPushMatrix();
        glTranslatef(-(col/2), -(lin/2), 0);
        disparador.desenhaVerticesColoridas();
    glPopMatrix();
}

void desenhaTiro(int num){
    glPushMatrix();
        glLineWidth(1);
        if(num == 0)
            defineCor(Yellow);
        else
            defineCor(Red);
        glTranslatef(-0.05, 0, 0);
        tiro.pintaPoligono();
    glPopMatrix();
}

void desenhaMonstro(int num){
    int lin, col;
    monstro[num%MODELOS_MONSTROS].obtemLimites(lin, col);
    glPushMatrix();
        glTranslatef(-(col/2), -(lin/2), 0);
        monstro[num%MODELOS_MONSTROS].desenhaVerticesColoridas();
    glPopMatrix();
}

void desenhaVidas(){
    float xTopo = Min.x, yTopo = Max.y;
    //constexpr float deslocX = (3* TAM_MAPA/100), deslocY = (12.2 * TAM_MAPA/100);
    float deslocX = (3* jogador.escala.x), deslocY = (35 * jogador.escala.y);

    for(int i = 0; i < jogador.vidas; i++){
        glPushMatrix();
            glPointSize(3);
            glTranslatef(xTopo+deslocX+(2*i*jogador.raio), yTopo-deslocY, 0);
            glScalef(jogador.escala.x, jogador.escala.y, jogador.escala.z);
            disparador.desenhaVerticesColoridas();
            glPointSize(1);
        glPopMatrix();
    }

}

void desenhaDerrota(){
    glPushMatrix();
        glPointSize(8);
        glScalef(2, 2, 2);
        glTranslatef(-18, 0, 0);
        derrota.desenhaVerticesColoridas();
        glPointSize(1);
    glPopMatrix();
}

void desenhaVitoria(){
    glPushMatrix();
        glPointSize(8);
        glScalef(2, 2, 2);
        glTranslatef(-18, 0, 0);
        vitoria.desenhaVerticesColoridas();
        glPointSize(1);
    glPopMatrix();
}

// ****************************************************************
// Esta fun��o deve instanciar todos os personagens do cen�rio
// ****************************************************************

void CriaInstancias()
{

    int lin, col;
    disparador.obtemLimites(lin, col);
    jogador.posicao = Ponto(0,0) ;
    jogador.rotacao = 0;
    jogador.modelo = desenhaDisparador;

    //jogador.escala = Ponto(escala/20, escala/20, escala/20);
    jogador.escala = Ponto(TAM_MAPA/(100.0 * (lin/10)), TAM_MAPA/(100.0 * (lin/10)), TAM_MAPA/100.0);

    if( lin > col ) 
        jogador.raio = lin/2 * jogador.escala.x;
        //jogador.raio = (max.x/5) * jogador.escala.x;//TAM_MAPA/100; 
    else
        jogador.raio = col/2 * jogador.escala.x;
        //jogador.raio = (max.y/5) * jogador.escala.x;//TAM_MAPA/100;  

    jogador.vidas = NVIDAS;

    for(int i = 0; i < NMONSTROS; i++ ){
        Universo[i].rotacao = 0;
        Universo[i].posicao = curvas[i][0];
//        Universo[i].escala = Ponto( escala/10, escala/10, escala/10);
        Universo[i].escala = Ponto(TAM_MAPA/100.0, TAM_MAPA/100.0, TAM_MAPA/100.0);

        Universo[i].modelo = desenhaMonstro;

        int min, max;
        monstro[i%MODELOS_MONSTROS].obtemLimites(lin, col);
        if( lin > col ) 
            Universo[i].raio = (1.5+lin/2) * Universo[i].escala.x ;//TAM_MAPA/100; 
        else
            Universo[i].raio = (1.5+col/2) * Universo[i].escala.y ;//TAM_MAPA/100; 

    }

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
    novoTiro.modelo = desenhaTiro;
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
        if( Universo[i].vidas <= 0 ) continue;
        pontosUteis[1] = pontoAleatorio(Min, Max);

        defineCor(MandarinOrange);
        andarNaBezier(Universo[i], pontosUteis, curvas[i], TempoTotal);
        Universo[i].desenha(i);

        if(debug){
            defineCor(NavyBlue);
            TracaBezier3Pontos(curvas[i]);
            defineCor(Pink);
            TracaPontosDeControle(curvas[i]);
        }

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
    if( !(verificaParedes.x <= Min.x+(TAM_MAPA/10) || verificaParedes.x >= Max.x-(TAM_MAPA/10)) ){
        if( !(verificaParedes.y <= Min.y+ (TAM_MAPA/10)|| verificaParedes.y >= Max.y-(TAM_MAPA/10)) ){
            jogador.posicao = verificaParedes;
        }
    }
    jogador.dir = jogador.dir * 0.95;
//    if(abs(jogador.dir.x) * 10 < 1) jogador.dir.x = 0;
//    if(abs(jogador.dir.y) * 10 < 1) jogador.dir.y = 0;
    jogador.desenha(0);
}

void animaTiros(){
    int idx = 0;
    for(Instancia &it: jogador.tiros){
        it.posicao = it.posicao + it.dir;

        if(it.vidas > 0)
            it.desenha(0);
        if( it.posicao.x > Max.x || it.posicao.x < Min.x || it.posicao.y > Max.x || it.posicao.y < Min.y ){
            atirados--;
            jogador.tiros.erase(jogador.tiros.begin()+idx);
            idx--;
        }
        idx++;
    }
    for(int i = 0; i < NMONSTROS; i++){
        idx = 0;
        for(Instancia &it: Universo[i].tiros){
            it.posicao = it.posicao + it.dir;
            if(it.vidas > 0)
                it.desenha(1);
            if( it.posicao.x > Max.x || it.posicao.x < Min.x || it.posicao.y > Max.x || it.posicao.y < Min.y ){
                Universo[i].tiros.erase(Universo[i].tiros.begin()+idx);
                idx--;
            }
        }
        idx++;
    }
}

// funcao copiada da internet (usada apenas para debugar visualizando o raio)
void DrawCircle(Ponto C, float r, int num_segments) {
    glBegin(GL_LINE_LOOP);
    for (int ii = 0; ii < num_segments; ii++)   {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle 
        float x = r * cosf(theta);//calculate the x component 
        float y = r * sinf(theta);//calculate the y component 
        glVertex2f(x + C.x, y + C.y);//output vertex 
    }
    glEnd();
}

// ****************************************************************

void testaColisao(){
    for(int i = 0; i < NMONSTROS; i++){
        // colisão tiros do monstro contra usuário
        for(Instancia &tiro: Universo[i].tiros){
            if(tiro.vidas <= 0) continue;
            float dist = sqrt(pow(tiro.posicao.x - jogador.posicao.x, 2) + pow(tiro.posicao.y - jogador.posicao.y, 2)); 
            if(dist < jogador.raio){
                if(!imune)
                    jogador.vidas--;
                tiro.vidas = 0;
            }
        }

        if( Universo[i].vidas <= 0 ) continue;

        if(debug){
            glLineWidth(1);
            glColor3f(1,1,1); // R, G, B  [0..1]
            DrawCircle(Universo[i].posicao, Universo[i].raio, 30);
            DrawCircle(jogador.posicao, jogador.raio, 30);
        }


        float dist = sqrt(pow(jogador.posicao.x - Universo[i].posicao.x, 2) + pow(jogador.posicao.y - Universo[i].posicao.y, 2)); 
        if( dist < jogador.raio + Universo[i].raio ){

            if(!imune){
                jogador.vidas--;
                Universo[i].vidas--;
                monstrosVivos--;
            }
        }

        // colisão tiros do jogador contra monstros
        for(Instancia &tiro: jogador.tiros){
            if(tiro.vidas <= 0) continue;
            float dist = sqrt(pow(tiro.posicao.x - Universo[i].posicao.x, 2) + pow(tiro.posicao.y - Universo[i].posicao.y, 2)); 
            if(dist < Universo[i].raio){
                Universo[i].vidas--;
                tiro.vidas = 0;
                monstrosVivos--;
                tiro.posicao.x = TAM_MAPA+TAM_MAPA;
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

    if(debug){
        glLineWidth(1);
        glColor3f(1,1,1); // R, G, B  [0..1]
        DesenhaEixos();
    }
    
    //glPointSize(TAM_MAPA*10/100.0);


    glLineWidth(1);
    glPointSize(1);
    animaJogador();

    animaTiros();

    animaMonstros();

    testaColisao();
    if(jogador.vidas <= 0){
        pause = true;
        desenhaDerrota();
    }

    if(monstrosVivos <= 0 && NMONSTROS != 0 ){
        monstrosVivos = 0;
        for(int i = 0; i < NMONSTROS; i++){
            if(Universo[i].vidas > 0) monstrosVivos++;
        }
        if(monstrosVivos == 0){
            pause = true;
            desenhaVitoria();
        }
    }

    desenhaVidas();

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
            jogador.vidas = NVIDAS;
            pause = false;
            break;
        case 'm':
            monstrosVivos = NMONSTROS;
            for(int i = 0; i < NMONSTROS; i++){
                Universo[i].vidas = 1;
            }
            break;
        case 'd':
            debug = !debug;
            break;
        case 'b':
            imune = !imune;
            break;
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
