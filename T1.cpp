/*

cores: https://mycolor.space/?hex=%2385C6F0&sub=1 (Pode mudar a paleta à vontade, só usei como base)


*/




#include <GL/glut.h>
#include <cmath>

const double PI = 3.14159;
const double aspectRatio = 960.0 / 540.0; //Mantém a proporção dos desenhos para 16:9. Multiplicar tudo que depende de x por aspectRatio.

void init(void);
void display(void);
void update (int);

// PRIMITIVAS GEOMÉTRICAS
void desenhaRetangulo(double, double, float, float, float);
void desenhaTriangulo(double, double, double, double, double, double, float, float, float);
void desenhaCirculo(double, float, float, float);
void desenhaTrapezio(double, double, double, float, float, float);

//ELEMENTOS DO CENARIO: CÉU
void desenhaSol();

//ELEMENTOS DO CENARIO: CHÃO
void desenhaGrama();
void desenhaCerca();
void desenhaHorta();

//ELEMENTOS DE PERSONAGENS
void desenhaCorpoCoelho();


int polygon = 4;
//SIDE SCROLLING
double offsetX = 0.0; 
double velocidade = 0.1;
bool jogoRodando = false;

//ANIMAÇÃO DO COELHO
float anguloPerna = 0.0;
float velocidadePerna = 3.0;
void animaPerna();

//PULO DO COELHO
float altura = 0.0;
float velocidadePulo = 3.0;
bool coelhoPulando = false;
float gravidade = -0.4;
float forcaPulo = 3;
void iniciaPulo();
void atualizaPulo();

void init(void)
{
  // define a cor de background da janela
  glClearColor(0.521, 0.776, 0.941, 1.0);

  // define o sistema de visualização - tipo de projeção
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  
  //glOrtho (-100 * aspectRatio, 100 * aspectRatio, -100, 100, -1, 1); //Para ver o mundo completo
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
    
    for(int i = 0; i < 4; i++) {
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
        
       // Horta
        glPushMatrix();
        glTranslated(0, -4, 0);
        desenhaHorta();
        glPopMatrix();
        
        glPopMatrix();
    }
   
    glPushMatrix();
    glTranslated(-12,-2,0);
    desenhaCorpoCoelho();
    glPopMatrix();

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
    }
display();
}

void mouse(int button, int action, int x, int y)//apagar depois??? vamos usar mouse???
{
  switch(button)
  {
    case GLUT_LEFT_BUTTON:
    {
    	polygon = 3;
      break;
    }
    case GLUT_MIDDLE_BUTTON:
    {

      break;
    }
    case GLUT_RIGHT_BUTTON:
    {
    	polygon = 4;
      break;
    }
    default: break;
  }

  display();
}

void update(int valor) {
    if(jogoRodando){
        offsetX -= velocidade;
        if (offsetX < -40) offsetX += 40;

        animaPerna();
        atualizaPulo();
    }
    glutTimerFunc(16, update, 0);
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB); //GLUT_DOUBLE evita efeito de "flicker" (piscada) na tela

  glutInitWindowSize (960, 540);
  glutInitWindowPosition (200, 50);

  glutCreateWindow ("Corre, coelho, corre!  -- Tecle enter para iniciar ou pausar o jogo --");

  init();

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


//ELEMENTOS DO CENÁRIO: CÉU

void desenhaSol(){
    // Círculo (sol)
    desenhaCirculo(1.7,0.976, 0.973, 0.443);

    // Raios solares (linhas) [talvez eu implemente depois]
    /*glPushMatrix();
    glScaled(2,2,1);
    glBegin(GL_LINES);
    glColor3f(1,1,0.85);
      for(int i=0; i<12; i++){
        float angle = (2*M_PI/12)*i;
        glVertex3f(0,0,0);
        glVertex3f(cos(angle), sin(angle),0);
      }*/
    //glEnd();
    //glPopMatrix();

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
        glBegin(GL_LINES);
        glEnd();
        glPopMatrix();
    }
}

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
    glTranslated(-0.8, 2.4, 0);
    glScaled(1, 3, 1);
    glRotatef(25.0, 0, 0, 1); 
    desenhaTriangulo(-0.5, -0.5, 0.5, -0.5, 0.0, 0.5, 1, 0.92, 0.8);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.2, 2.4, 0);
    glScaled(1, 3, 1);
    glRotatef(25.0, 0, 0, 1); 
    desenhaTriangulo(-0.5, -0.5, 0.5, -0.5, 0.0, 0.5, 1, 0.92, 0.8);
    glPopMatrix();

    //pernas
    glPushMatrix();
    glTranslated(-2, -3.2, 0);
    glTranslated(0, -0.5, 0);     
    glRotatef(-anguloPerna, 0, 0, 1);
    glTranslated(0, 0.5, 0);
    desenhaRetangulo(0.7, 1, 1, 0.92, 0.8);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-3.5, -3.2, 0);
    glTranslated(0, -0.5, 0);
    glRotatef(anguloPerna, 0, 0, 1);
    glTranslated(0, 0.5, 0);
    desenhaRetangulo(0.7, 1, 1, 0.92, 0.8);
    glPopMatrix();

    glPopMatrix();
}

void animaPerna(){
    if(jogoRodando){
        anguloPerna += velocidadePerna;
        if(anguloPerna > 25) {
            anguloPerna = -25;
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
