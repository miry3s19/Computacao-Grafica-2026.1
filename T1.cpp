#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstdio>

const double PI = 3.14159;
const double aspectRatio = 960.0 / 540.0; //Mantém a proporção dos desenhos para 16:9. Multiplicar tudo que depende de x por aspectRatio.

void init(void);
void display(void);
void update (int);

//SIDE SCROLLING
double offsetX = 55.0; 
double velocidade = 0.15;
bool jogoRodando = false;

double offsetNuvens = 40.0;
double velocidadeNuvens = 0.03;

double offsetMontanhas = 40.0;
double velocidadeMontanhas = 0.07;

// PRIMITIVAS GEOMÉTRICAS
void desenhaRetangulo(double, double, float, float, float);
void desenhaTriangulo(double, double, double, double, double, double, float, float, float);
void desenhaCirculo(double, float, float, float);
void desenhaTrapezio(double, double, double, float, float, float);
void desenhaLinhaCentro(float, float, float, float, float, float, float, float, float);

//ELEMENTOS DO CENARIO: CÉU
void desenhaSol();
void desenhaNuvem(double, double, double);
void desenhaMontanha(double, double, double, double, float, float, float);

//ELEMENTOS DO CENARIO: CHÃO
void desenhaGrama();
void desenhaCerca();
void desenhaHorta();

//ELEMENTOS DE BONIFICAÇÃO
void desenhaAlmeirao();
void desenhaCenoura();
void desenhaCouve();
void desenhaLinhasCouve(float, float, float); 

//APARECIMENTO E DESAPARECIMENTO DOS ELEMENTOS DE BONIFICAÇÃO
typedef struct vegetal {
    double x;
    double y;
    int tipo;
    int bloco;
    bool ativo;
} vegetal;

vegetal vegetais[16];
void inicializaVegetais();
void atualizaVegetais();
void colisaoVegetal();

//SISTEMA DE VIDAS E PONTOS
int vidas = 5;
int pontos = 0;

//TEXTOS
void textoGameOver(float, float);
void textoVidas(float, float);
void textoPausa(float, float);
void textoPontos(float, float);
void textoImune(float, float);


//ELEMENTOS DE PERSONAGENS
void desenhaCorpoCoelho();
void desenhaCorpoRaposa();
bool imune = false;
int tempoImune = 180;

//ANIMAÇÃO DO COELHO
float alturaPerna = 0.0;
float velocidadePerna = 3.0;
void animaPerna();
void animaOrelha();

//PULO DO COELHO
float altura = 0.0;
float velocidadePulo = 3.0;
bool coelhoPulando = false;
float gravidade = -0.15;
float forcaPulo = 2.25;
void iniciaPulo();
void atualizaPulo();

//APARECIMENTO DA RAPOSA
float raposaX= 30;
float raposaY = -2;
bool raposaVisivel = false;
void atualizaRaposa();

void init(void)
{
  // define a cor de background da janela
  glClearColor(0.521, 0.776, 0.941, 1.0);

  // define o sistema de visualização - tipo de projeção
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  
  //glOrtho (-80 * aspectRatio, 100 * aspectRatio, -50, 50, -1, 1); //Para ver o mundo completo
  glOrtho (-10 * aspectRatio, 10 * aspectRatio, -10, 10, -1, 1); //Para ver o viewport
}



void display() {

  // Limpa a janela, colocando na tela a cor definida pela função glClearColor
  glClear(GL_COLOR_BUFFER_BIT);
 
  
  // Sol
    glPushMatrix();
    glTranslated(-8.5 * aspectRatio,8,0);
    //glRotated(-(float)frameNumber/4,0,0,1);
    desenhaSol();
    glPopMatrix();
    
  // Montanhas
    glPushMatrix();
    glTranslated(offsetMontanhas, -2, 0);
    
    for(int bloco = -2; bloco <= 2; bloco++) {
        double baseX = bloco * 40.0;
        
        desenhaMontanha(baseX - 15 * aspectRatio, 0, 12 * aspectRatio, 5.5, 0.60, 0.24, 0.43);
        desenhaMontanha(baseX - 5 * aspectRatio, 0, 10 * aspectRatio, 4.0, 0.58, 0.47, 0.71);
        desenhaMontanha(baseX + 5 * aspectRatio, 0, 14 * aspectRatio, 6.0, 0.55, 0.57, 0.82);
        desenhaMontanha(baseX + 15 * aspectRatio, 0, 11 * aspectRatio, 4.5, 0.69, 0.47, 0.61);
        desenhaMontanha(baseX + 22 * aspectRatio, 0, 8 * aspectRatio, 3.0, 0.78, 0.41, 0.65);
    }
    
    glPopMatrix();
    
  // Nuvens
    glPushMatrix();
    glTranslated(offsetNuvens, 0, 0);
    
    for(int bloco = -2; bloco < 6; bloco++) {
        double baseX = bloco * 40.0;
        
        desenhaNuvem(baseX - 18 * aspectRatio, 7 + 0.5 * sin(bloco), 1.0);
        desenhaNuvem(baseX - 10 * aspectRatio, 8.5 + 0.3 * cos(bloco * 0.7), 0.8);
        desenhaNuvem(baseX - 2 * aspectRatio, 6.5 + 0.4 * sin(bloco * 1.2), 1.2);
        desenhaNuvem(baseX + 6 * aspectRatio, 7.8 + 0.3 * sin(bloco * 0.5), 0.9);
        desenhaNuvem(baseX + 14 * aspectRatio, 6.2 + 0.4 * cos(bloco * 0.8), 0.7);
        desenhaNuvem(baseX + 22 * aspectRatio, 8.0 + 0.2 * sin(bloco * 1.5), 0.5);
    }

    glPopMatrix();
    
    
    for(int i = -2; i < 6; i++) {
        glPushMatrix();
        glTranslated(offsetX + (i * 40), 0, 0);  // i=0: posição atual, i=1: +40 adiante
        
       // Grama
        glPushMatrix();
        glTranslated(0, -6, 0);
        desenhaGrama();
        glPopMatrix();
        
       // Cerca
        glPushMatrix();
        glTranslated(0, -1.8, 0);
        desenhaCerca();
        glPopMatrix();
        
        if(i >= 0 && i <= 4) {
       // Horta
        glPushMatrix();
        glTranslated(0, -4, 0);
        desenhaHorta();
        glPopMatrix();
        }
                        
        glPopMatrix();
    }
    
    for(int j = 0; j < 16; j++) {
        if(vegetais[j].ativo) { 
            glPushMatrix();
            glTranslated(offsetX + (vegetais[j].bloco * 40) + vegetais[j].x, vegetais[j].y, 0);
            
            switch(vegetais[j].tipo) {
                case 1:
                    desenhaAlmeirao();
                    break;
                case 2:
                    desenhaCenoura();
                    break;
                case 3:
                    desenhaCouve();
                    break;
                default:
                    break;
            }
            glPopMatrix();
        }
    }   
   
    glPushMatrix();
    glTranslated(-12,-2,0);
    desenhaCorpoCoelho();
    glPopMatrix();

    glPushMatrix();
    glTranslated(raposaX,raposaY,0);
    desenhaCorpoRaposa();
    glPopMatrix();

    glColor3f(0,0,0);
    textoVidas(6.0 * aspectRatio, 9.0);
    glColor3f(0,0,0);
    textoPontos(6.0 * aspectRatio, 8.0);
    glColor3f(0,0,0);
    if(vidas <= 0) {
        textoGameOver(-4.0 * aspectRatio, 2.0);
    }
    if(!jogoRodando && vidas > 0){
        textoPausa(-3.0 * aspectRatio, 2.0);
    }
    if(imune){
        textoImune(-3.0 * aspectRatio, 5.0);
    }
 // Libera o buffer de comando de desenho para fazer o desenho acontecer o mais rápido possível.
  glFlush();
  glutSwapBuffers(); // Evita efeito de "flicker" (piscada) na tela
}


//INTERAÇÕES

void keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
        case 13 :
            jogoRodando = !jogoRodando;
        break;
        case 32: //pra adicionar o pulo
            iniciaPulo();
        break;
        case 'r':
        case 'R':
            if(vidas == 0) {
                vidas = 5;
                pontos = 0;
                jogoRodando = true;
                offsetX = 0.0;
                offsetNuvens = 0.0;
                offsetMontanhas = 0.0;
                raposaVisivel = false;
                raposaX = 30.0;
                altura = 0.0;
                coelhoPulando = false;
                inicializaVegetais();
            }
            break;
    }
display();
}

void update(int valor) {
    if(jogoRodando){
        atualizaVegetais();
        
        offsetX -= velocidade;
        printf("%f\n", offsetX);
        if (offsetX < -175) offsetX += 245;
        
        offsetNuvens -= velocidadeNuvens;
        if (offsetNuvens < -40) offsetNuvens += 80;
        
        offsetMontanhas -= velocidadeMontanhas;
        if (offsetMontanhas < -40) offsetMontanhas += 80;

        animaPerna();
        animaOrelha();
        atualizaPulo();
        atualizaRaposa();
        
        if(imune) {
            tempoImune--;
            if(tempoImune <= 0) imune = false;
        }
        
        colisaoVegetal();
    }
    glutTimerFunc(16, update, 0);
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
  srand(time(NULL));
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB); //GLUT_DOUBLE evita efeito de "flicker" (piscada) na tela

  glutInitWindowSize (960, 540);
  glutInitWindowPosition (200, 50);

  glutCreateWindow ("Corre, coelho, corre!  -- Tecle enter para iniciar ou pausar o jogo --");

  init();

  inicializaVegetais();
  
  glutDisplayFunc(display);
  glutTimerFunc(0, update, 0);
  
  // Indica que sempre que uma tecla for pressionada no teclado, GLUT deverá chama a função keyboard() para tratar eventos de teclado (keyboard callback).
  // A função de teclado deve possuir o seguinte protótipo:
  glutKeyboardFunc(keyboard);


  glutMouseFunc(mouse);

  glutMainLoop();

  return 0;

}


// PRIMITIVAS GEOMÉTRICAS

void desenhaRetangulo(double width, double height, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
        glVertex2d(-width / 2, -height / 2); // Canto inferior esquerdo
        glVertex2d( width / 2, -height / 2); // Canto inferior direito
        glVertex2d( width / 2,  height / 2); // Canto superior direito
        glVertex2d(-width / 2,  height / 2); // Canto superior esquerdo
    glEnd();
}

void desenhaTriangulo(double v1x, double v1y, double v2x,double v2y, double v3x, double v3y, float r, float g, float b){
	glColor3f(r, g, b);
	glBegin(GL_POLYGON);
	glVertex2d(v1x,v1y);
	glVertex2d(v2x,v2y);
	glVertex2d(v3x,v3y);
	glEnd();
}

void desenhaCirculo(double radius, float r, float g, float b) {
    int d;
    glColor3f(r,g,b);
    glBegin(GL_POLYGON);
    for (d = 0; d < 32; d++) {
        double angle = (2*PI/32) * d;
        glVertex2d(radius*cos(angle), radius*sin(angle));    
    }
    glEnd();
}

void desenhaTrapezio(double baseMaior, double baseMenor, double height, float r, float g, float b) {
    desenhaRetangulo(baseMenor, height, r, g, b);

    double sobra = (baseMaior - baseMenor) / 2.0;
    double yMin = -height / 2.0; // Borda inferior
    double yMax =  height / 2.0; // Borda superior

    desenhaTriangulo(
        -baseMenor / 2.0, yMax,          
        -baseMenor / 2.0, yMin,          
        -(baseMenor / 2.0 + sobra), yMin,
        r, g, b
    );

    desenhaTriangulo(
        baseMenor / 2.0, yMax,           
        baseMenor / 2.0, yMin,           
        baseMenor / 2.0 + sobra, yMin,   
        r, g, b
    );
}

void desenhaLinhaCentro(float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b) {
    float mx = (x2 + x3) / 2.0f;
    float my = (y2 + y3) / 2.0f;

    glColor3f(r, g, b);
    glLineWidth(1.4f);
    glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(mx, my);
    glEnd();
}


//ELEMENTOS DO CENÁRIO: CÉU
void desenhaSol(){
    // Círculo (sol)
    desenhaCirculo(1.7,0.976, 0.973, 0.443);
}

void desenhaNuvem(double x, double y, double escala) {
    glPushMatrix();
    glTranslated(x, y, 0);
    glScaled(escala, escala, 1);
    glColor3f(1.0, 1.0, 1.0);
    
    desenhaCirculo(1.5, 1.0, 1.0, 1.0);
    
    glPushMatrix();
    glTranslated(-1.8, 0.5, 0);
    desenhaCirculo(1.2, 1.0, 1.0, 1.0);
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(1.8, 0.3, 0);
    desenhaCirculo(1.3, 1.0, 1.0, 1.0);
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(-1.0, 1.2, 0);
    desenhaCirculo(1.0, 1.0, 1.0, 1.0);
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(1.0, 1.0, 0);
    desenhaCirculo(1.1, 1.0, 1.0, 1.0);
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(0.0, 1.5, 0);
    desenhaCirculo(0.9, 1.0, 1.0, 1.0);
    glPopMatrix();
    
    glPopMatrix();
}

void desenhaMontanha(double x, double y, double largura, double altura, float r, float g, float b) {
    glPushMatrix();
    glTranslated(x, y, 0);
    glColor3f(r, g, b);
    
    desenhaTriangulo(
        -largura/2, 0,    
        largura/2, 0,     
        0, altura,        
        r, g, b
    );
    
    desenhaTriangulo(
        -largura/6, altura * 0.7,
        largura/6, altura * 0.7,
        0, altura,
        1.0, 1.0, 1.0  // Branco
    );
    
    glPopMatrix();
    
}

//ELEMENTOS DO CENÁRIO: CHÃO
void desenhaGrama() {
   //Base
    desenhaRetangulo(40 * aspectRatio, 8, 0.427, 0.592, 0.314);
    double inicio = (-20 * aspectRatio) - 0.5;
    double fim = (20 * aspectRatio)+ 0.5;
    double passo = 1;
    
   //Serrilhado do topo
    for(int x = inicio; x <= fim; x++) {
        desenhaTriangulo(x, 4.5, x + 0.5, 4, x - 0.5, 4, 0.427, 0.592, 0.314);
    }
}

void desenhaCerca() {
   //Tábuas verticais
    double inicio = -19.75 * aspectRatio;
    double fim = 19.75 * aspectRatio;
    double passo = 2;
    for(double x = inicio; x <= fim; x+=passo) {
        glPushMatrix();
        glTranslated(x,0,0);
        desenhaRetangulo(0.225 * aspectRatio, 3, 0.804, 0.502, 0.075); 
        glPopMatrix();
    }
    
   //Tábuas horizontais
    inicio = 0.75;
    fim = -0.75;
    passo = -1.5;
    for(double y = inicio; y >= fim; y+=passo) {
        glPushMatrix();
        glTranslated(0,y,0);
        desenhaRetangulo(40 * aspectRatio, 0.4, 0.922, 0.604, 0.157);
        glPopMatrix();
    }    
}

void desenhaHorta() {
    double posicoes[4] = {-18 * aspectRatio, -6 * aspectRatio, 6 * aspectRatio, 18 * aspectRatio};
    
    for(int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslated(posicoes[i],-1.5,0);
        desenhaTrapezio(7.5 * aspectRatio, 4.5 * aspectRatio, 2, 0.561, 0.306, 0.0);
        glPopMatrix();
    }
}

//ELEMENTOS DE BONIFICAÇÃO
void desenhaAlmeirao() {
    // Cor das folhas
    float r = 0.698f;
    float g = 0.980f;
    float b = 0.576f;
    
    // Cor das linhas
    float lr = 0.55f;
    float lg = 0.0f;
    float lb = 0.55f;
    
    // Caule
    glColor3f(r * 0.8, g * 0.8, b * 0.8);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
        glVertex2f(0.0f, 0.25f);
        glVertex2f(0.0f, -0.7f);
    glEnd();
    glLineWidth(1.0f);
    
    // Folhas
    desenhaTriangulo(0.0f, 2.0f, 0.35f, 0.25f, -0.35f, 0.25f, r, g, b);
    desenhaLinhaCentro(0.0f, 2.0f, 0.35f, 0.25f, -0.35f, 0.25f, lr, lg, lb);
    
    desenhaTriangulo(-1.5f, 1.5f, -0.45f, 0.25f, 0.15f, 0.25f, r * 0.8, g * 0.8, b * 0.8);
    desenhaLinhaCentro(-1.5f, 1.5f, -0.45f, 0.25f, 0.15f, 0.25f, lr, lg, lb);
    
    glPushMatrix();
    glScaled(-1, 1, 1);
    desenhaTriangulo(-1.5f, 1.5f, -0.45f, 0.25f, 0.15f, 0.25f, r * 0.8, g * 0.8, b * 0.8);
    desenhaLinhaCentro(-1.5f, 1.5f, -0.45f, 0.25f, 0.15f, 0.25f, lr, lg, lb);
    glPopMatrix();  
    
    desenhaTriangulo(-1.7f, 0.75f, 0.45f, 0.25f, -0.05f, -0.1f, r * 0.9, g * 0.9, b * 0.9);
    desenhaLinhaCentro(-1.7f, 0.75f, 0.45f, 0.25f, -0.05f, -0.1f, lr, lg, lb);
    
    glPushMatrix();
    glScaled(-1, 1, 1);
    desenhaTriangulo(-1.7f, 0.75f, 0.45f, 0.25f, -0.05f, -0.1f, r * 0.95, g * 0.95, b * 0.95);
    desenhaLinhaCentro(-1.7f, 0.75f, 0.45f, 0.25f, -0.05f, -0.1f, lr, lg, lb);
    glPopMatrix();    
}

void desenhaCenoura() {
    //Cor da raiz
    float r = 0.9686f;
    float g = 0.6039f;
    float b = 0.0941f;
    
    //Cor das folhas
    float fr = 0.0f;
    float fg = 0.2784f;
    float fb = 0.2314f;
    
    //Raiz
    desenhaTriangulo(0, 0, 0.5, 2, -0.5, 2, r, g, b);
    
    //Folhas
    glPushMatrix();
    glTranslated(0, 2, 0);
    desenhaTriangulo(0, 1.2, 0.25, 0, -0.25, 0, fr, fg, fb);
    glPopMatrix();    
    
    glPushMatrix();
    glTranslated(-0.25, 2, 0);
    desenhaTriangulo(-0.25, 0, -0.25, 0.8, 0, 0, fr, fg, fb);
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(0.25, 2, 0);
    glScaled(-1,1,0);
    desenhaTriangulo(-0.25, 0, -0.25, 0.8, 0, 0, fr, fg, fb);
    glPopMatrix();     
    
}

void desenhaLinhasCouve(float r, float g, float b) {
    glColor3f(r, g, b);
    glLineWidth(2.0f);

    glBegin(GL_LINES);
        //Linha central
        glVertex2f(0.0f, -0.1f); 
        glVertex2f(0.0f, 2.8f); 

        // Linhas laterais
        glVertex2f(0.0f, 0.6f);
        glVertex2f(0.6f, 1.1f);

        glVertex2f(0.0f, 0.6f);
        glVertex2f(-0.6f, 1.1f);
        
        glVertex2f(0.0f, 1.2f);
        glVertex2f(0.7f, 1.8f); 

        glVertex2f(0.0f, 1.2f);
        glVertex2f(-0.7f, 1.8f);

        glVertex2f(0.0f, 1.9f);
        glVertex2f(0.6f, 2.5f);

        glVertex2f(0.0f, 1.9f);
        glVertex2f(-0.6f, 2.5f);
    glEnd();

    glLineWidth(1.0f); 
}

void desenhaCouve() {
    
    glPushMatrix();
    glTranslated(0, 1.7, 0);
    glScaled(1.1,1.5,1);
    desenhaCirculo(1, 0.0f, 0.2784f, 0.2314f);
    glPopMatrix();  
    
    
    desenhaLinhasCouve(0.698f, 0.980f, 0.576f);
}

//APARECIMENTO E DESAPARECIMENTO DOS ELEMENTOS DE BONIFICAÇÃO


void inicializaVegetais() {
    double hortasIndex[4] = {-18 * aspectRatio, -6 * aspectRatio, 6 * aspectRatio, 18 * aspectRatio};
    
    for(int bloco = 0; bloco < 4; bloco++) {
        for(int horta = 0; horta < 4; horta++) {
            int i = bloco * 4 + horta;
            vegetais[i].x = hortasIndex[horta];  // Posição relativa ao bloco
            vegetais[i].y = -5;
            vegetais[i].tipo = (rand() % 3) + 1;
            vegetais[i].bloco = bloco;
            vegetais[i].ativo = true;
        }
    }
}

void atualizaVegetais() {
    for(int bloco = 0; bloco < 4; bloco++) {
        double posBloco = offsetX + (bloco * 40);
        
        if(posBloco < -80.0 || posBloco > 160.0) {
            // Recicla APENAS este bloco
            for(int horta = 0; horta < 4; horta++) {
                int i = bloco * 4 + horta;
                vegetais[i].tipo = (rand() % 3) + 1;
                vegetais[i].ativo = true;
            }
        }
    }
}

void colisaoVegetal() {
    double coelhoX = -12.0;
    double coelhoY = -2.0 + altura;
    double raio = 3.7;  
    
    for(int i = 0; i < 16; i++) {
        if(!vegetais[i].ativo) continue;
        
        double vegetalX = offsetX + (vegetais[i].bloco * 40) + vegetais[i].x;
        double vegetalY = vegetais[i].y;  // -5.0
        
        double dx = coelhoX - vegetalX;
        double dy = coelhoY - vegetalY;
        double distancia = sqrt(dx*dx + dy*dy);
        
        if(distancia < raio) {
            vegetais[i].ativo = false;  
            switch(vegetais[i].tipo) {
                case 1: // Almeirao
                    pontos++;
                    vidas++;
                    break;
                case 2: // Cenoura
                    pontos = pontos + 10;
                    break;
                case 3: // Couve
                    pontos++;
                    imune = true;
                    tempoImune = 180;
                    break;
                default:
                    break;
            }
        }
    }
}

//ELEMENTOS DE PERSONAGENS

void desenhaCorpoCoelho() {

    glPushMatrix();
    glTranslated(0, altura, 0);

    //cabeça
    desenhaCirculo(1.7, 1, 0.92, 0.8);

    //corpo
    glPushMatrix();
    glTranslated(-2.5,-1.5,0);
    desenhaCirculo(1.9, 1, 0.92, 0.8);
    glPopMatrix();

    //focinho
    glPushMatrix();
    glScaled(0.3,0.3,1);
    glTranslated(5,-0.7,0);
    desenhaCirculo(1.9, 1, 1, 1);
    glPopMatrix();

    glPushMatrix();
    glScaled(0.3,0.3,1);
    glTranslated(6,0.3,0);
    desenhaCirculo(0.5, 0.90, 0.67, 0.81);
    glPopMatrix();

    //pompom
    glPushMatrix();
    glScaled(0.3,0.3,1);
    glTranslated(-15,-7,0);
    desenhaCirculo(1.9, 1, 0.92, 0.8);
    glPopMatrix();

    //olho
    glPushMatrix();
    glScaled(0.3,0.3,1);
    glTranslated(1,0.7,0);
    desenhaCirculo(0.5, 0, 0, 0);
    glPopMatrix();

    //orelhas
    glPushMatrix();
    glTranslated(-0.8, 2.4 + alturaPerna, 0);
    glScaled(1, 3, 1);
    glRotatef(25.0, 0, 0, 1); 
    desenhaTriangulo(-0.5, -0.5, 0.5, -0.5, 0.0, 0.5, 1, 0.92, 0.8);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.2, 2.4 - alturaPerna, 0);
    glScaled(1, 3, 1);
    glRotatef(25.0, 0, 0, 1); 
    desenhaTriangulo(-0.5, -0.5, 0.5, -0.5, 0.0, 0.5, 1, 0.92, 0.8);
    glPopMatrix();

    //pernas
    glPushMatrix();
    glTranslated(-2, -3.2 - alturaPerna, 0);
    desenhaRetangulo(0.7, 1, 1, 0.92, 0.8);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-3.5, -3.2 + alturaPerna, 0);
    desenhaRetangulo(0.7, 1, 1, 0.92, 0.8);
    glPopMatrix();

    glPopMatrix();
}

void desenhaCorpoRaposa(){

        //cabeça
    desenhaCirculo(1.7, 0.95, 0.51, 0.49);

    //corpo
    glPushMatrix();
    glTranslated(2.5,-1.5,0);
    desenhaCirculo(1.9, 0.95, 0.51, 0.49);
    glPopMatrix();

    //focinho
    glPushMatrix();
    glScaled(1.5,1.5,1.5);
    glTranslated(-1.3,-0.4,0);
    glRotated(90,0,0,1);
    desenhaTriangulo(-0.5, -0.5, 0.5, -0.5, 0.0, 0.5, 1, 1, 1);
    glPopMatrix();

    glPushMatrix();
    glScaled(0.3,0.3,1);
    glTranslated(-8.52,-1.5,0);
    desenhaCirculo(0.5, 0, 0, 0);
    glPopMatrix();

    //rabo
    glPushMatrix();
    glTranslated(5, 1,0);
    glRotated(150,0,0,1);
    glScaled(2,4,1);
    desenhaTriangulo(-0.5, -0.5, 0.5, -0.5, 0.0, 0.5, 0.95, 0.51, 0.49);
    glPopMatrix();

    //olho
    glPushMatrix();
    glScaled(0.3,0.3,1);
    glTranslated(-1,0.7,0);
    desenhaCirculo(0.5, 0, 0, 0);
    glPopMatrix();

    //orelhas
    glPushMatrix();
    glTranslated(0.8, 2.0, 0);
    glScaled(1, 2, 1);
    glRotatef(-25.0, 0, 0, 1); 
    desenhaTriangulo(-0.5, -0.5, 0.5, -0.5, 0.0, 0.5,0.95, 0.51, 0.49);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.2, 2.0, 0);
    glScaled(1, 2, 1);
    glRotatef(-25.0, 0, 0, 1); 
    desenhaTriangulo(-0.5, -0.5, 0.5, -0.5, 0.0, 0.5, 0.95, 0.51, 0.49);
    glPopMatrix();

    //pernas
    glPushMatrix();
    glTranslated(2, -3.2 - alturaPerna, 0);
    desenhaRetangulo(0.7, 1, 0.95, 0.51, 0.49);
    glPopMatrix();

    glPushMatrix();
    glTranslated(3.5, -3.2 + alturaPerna, 0);
    desenhaRetangulo(0.7, 1, 0.95, 0.51, 0.49);
    glPopMatrix();
}

void animaPerna(){
    if(jogoRodando){
        alturaPerna += 0.03;
        if(alturaPerna > 0.3) {
            alturaPerna = -0.3;
        }
    }
}

void animaOrelha(){
    if(jogoRodando){
        alturaPerna += 0.03;
        if(alturaPerna > 0.3) {
            alturaPerna = -0.3;
        }
    }
}

void iniciaPulo(){
    if(!coelhoPulando) {
        coelhoPulando = true;
        velocidadePulo = forcaPulo;
    }
}

void atualizaPulo(){
    if(coelhoPulando){
        velocidadePulo += gravidade;
        altura += velocidadePulo;

        if (altura <= 0){
            altura = 0;
            coelhoPulando = false;
            velocidadePulo = 0;
        }
    }
}

void atualizaRaposa(){
    if(!jogoRodando) return;

    if(!raposaVisivel){
        int numeroAleatorio = rand() % 100;

        if (numeroAleatorio < 1){
            raposaX = 30.0;
            raposaY = -2.0;
            raposaVisivel = true;
        }
    }
    else{
        raposaX -= 0.4;

        double coelhoX = -12.0;
        if(fabs(coelhoX - raposaX) < 2.0 && altura <= 0.1 && !imune){
            vidas--;
            if(vidas == 0){
                jogoRodando = false;
            }
            raposaVisivel = false;
        }

        if(raposaX < -30.0){
            raposaVisivel = false;
        }
    }
}

//TEXTO

void textoGameOver(float x, float y) {
    glRasterPos2f(x, y);
    char texto[] = "GAME OVER! Suas vidas acabaram :( Aperte 'R' para reiniciar.";
    for(int i = 0; texto[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, texto[i]);
    }
}

void textoPausa(float x, float y) {
    glRasterPos2f(x, y);
    char texto[] = "JOGO PAUSADO";
    for(int i = 0; texto[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, texto[i]);
    }
}

void textoVidas(float x, float y) {
    glRasterPos2f(x, y);
    char texto[10];
    sprintf(texto, "Vidas: %d", vidas);
    for(int i = 0; texto[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, texto[i]);
    }
}

void textoPontos(float x, float y) {
    glRasterPos2f(x, y);
    char texto[10];
    sprintf(texto, "Pontos: %d", pontos);
    for(int i = 0; texto[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, texto[i]);
    }
}

void textoImune(float x, float y) {
    glRasterPos2f(x, y);
    char texto[] = "IMUNIDADE ATIVADA!";
    for(int i = 0; texto[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, texto[i]);
    }
}
