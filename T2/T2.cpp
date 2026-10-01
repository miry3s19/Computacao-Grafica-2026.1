#include <GL/glut.h>
#include <GL/glext.h>
#include <stdio.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



// Personagem (esfera)
float personagemPosX = 0.0f;
float personagemPosZ = 0.0f;
float personagemRaio = 0.5f;
float personagemAngulo = 0.0f;
float personagemBaseY = 0.0f;

float deslocamentoPata = 0.0f; 

float velocidadePulo = 0.0f;
float gravidade = -0.015f;
float forcaPulo = 0.22f;
bool coelhoPulando = false;
void iniciaPulo();

float raposaX = 0.0f;
float raposaZ = 0.0f;
float raposaAngulo = 0.0f;
bool  raposaVisivel = false;
float raposaVelocidade = 0.06f;
float raioColisaoRaposa = 0.7f;

// Elementos de bonificação
typedef struct {
    float x;
    float z;
    int tipo;       // 1 = almeirão, 2 = cenoura, 3 = couve
    int bloco;
    bool ativo;
} Vegetal;

#define NUM_VEGETAIS 16
Vegetal vegetais[NUM_VEGETAIS];

float offsetX = 0.0f;

// Sistema de vidas e pontos
int vidas = 5;
int pontos = 0;
bool imune = false;
int tempoImune = 180;

// Câmera
int modoCamera = 0; // 0 = visão geral, 1 = primeira pessoa
float camDistancia = 2.0f;
float camAltura = 1.0f;

// Rotação da Câmera
float cameraRotacaoY = 0.0;
float cameraRaio = 25.0;

// Texturas
GLuint texID[2]; // 0 = chão, 1 = personagem
char* textureFileNames[2] = {
    "texturas/grass.jpg",
    "texturas/marble.jpg"
};

//TEXTOS
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

void desenhaTextos() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 100, 0, 100);   // 0-100 em X e Y, canto = sempre canto

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);

    glColor3f(0.0f, 0.0f, 0.0f);
    textoVidas(2.0f, 95.0f);
    textoPontos(2.0f, 90.0f);
    if (imune) {
        glColor3f(0.0f, 0.5f, 0.0f);
        textoImune(2.0f, 85.0f);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void initGL() {
    glClearColor(0.62f, 0.85f, 0.90f, 1.0f); // background color

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    float luz1Difusa[]  = { 0.3f, 0.4f, 0.5f, 1.0f };
    float luz1Posicao[] = { -8.0f, 6.0f, -5.0f, 1.0f };
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  luz1Difusa);
    glLightfv(GL_LIGHT1, GL_POSITION, luz1Posicao);

    // Material branco para texturas
    float white[4] = { 1, 1, 1, 1 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white);

    // Configuração da luz
    float luzAmbiente[] = {0.3f, 0.3f, 0.3f, 1.0f};
    float luzDifusa[]  = {0.7f, 0.7f, 0.7f, 1.0f};
    float luzPosicao[] = {0.0f, 10.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
    glLightfv(GL_LIGHT0, GL_POSITION, luzPosicao);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1.0, 0.1, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//Texturas

void loadTextures() {
    int width, height, nrChannels;
    unsigned char *data;

    glGenTextures(2, texID);

    for (int i = 0; i < 2; i++) {
        glBindTexture(GL_TEXTURE_2D, texID[i]);

        // set the texture wrapping/filtering options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // load and generate the texture
        data = stbi_load(textureFileNames[i], &width, &height, &nrChannels, 0);

        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
        } else {
            printf("Failed to load texture: %s\n", textureFileNames[i]);
        }
        stbi_image_free(data);
    }
}

//movimentação com teclado

void keyboardChangeCamera(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // ESC
            exit(0);
            break;
        case 'r':
        case 'R':
            cameraRotacaoY += 15.0;
            break;
        case 'c':
        case 'C':
            modoCamera = (modoCamera + 1) % 2;
            break;
        case 32:
            iniciaPulo();
            break;
    }
    glutPostRedisplay();
}

void specialKeyFunction(int key, int x, int y) {
    float velocidade = 0.2;
    float velocidadeRotacao = 5.0;
    float rad = personagemAngulo * M_PI / 180.0;

    switch (key) {
        case GLUT_KEY_UP:
            personagemPosX += velocidade * sin(rad);
            personagemPosZ -= velocidade * cos(rad);
            break;
        case GLUT_KEY_DOWN:
            personagemPosX -= velocidade * sin(rad);
            personagemPosZ += velocidade * cos(rad);
            break;
        case GLUT_KEY_LEFT:
              personagemAngulo -= velocidadeRotacao;
            break;
        case GLUT_KEY_RIGHT:
              personagemAngulo += velocidadeRotacao; 
            break;
    }
    glutPostRedisplay();
}

//Câmera

void cameraVisaoGeral() {

    float rad = cameraRotacaoY * M_PI / 180;

    float camX = cameraRaio * sin(rad);
    float camZ = cameraRaio * cos(rad);
    float camY = 10.0;

    gluLookAt(
        camX, camY, camZ, // posição camera
        0.0, 0.0, 0.0,   // mira
        0.0, 1.0, 0.0    // pra cima
    );
}

void cameraPrimeiraPessoa() {
    float rad = personagemAngulo * M_PI / 180.0;

    float cameraX = personagemPosX - camDistancia * sin(rad);
    float cameraZ = personagemPosZ + camDistancia * cos(rad);
    float cameraY = camAltura;

    float alvoX = personagemPosX + 5.0 * sin(rad);
    float alvoZ = personagemPosZ - 5.0 * cos(rad);
    float alvoY = camAltura;

    gluLookAt(
        cameraX, cameraY, cameraZ,
        alvoX, alvoY, alvoZ,
        0.0, 1.0, 0.0
    );
}

//Cenário

void desenhaChao() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[0]);

    float tamanho = 200.0f; // 200x200 cobre -100 ate +100
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); 
        glVertex3f(-tamanho, 0.0f, -tamanho);
        glTexCoord2f(50.0f, 0.0f); 
        glVertex3f( tamanho, 0.0f, -tamanho);
        glTexCoord2f(50.0f, 50.0f); 
        glVertex3f( tamanho, 0.0f,  tamanho);
        glTexCoord2f(0.0f, 50.0f); 
        glVertex3f(-tamanho, 0.0f,  tamanho);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

//Personagens

void desenhaCoelho() {
    float corPelo[] = { 0.93f, 0.87f, 0.75f, 1.0f };  // bege claro
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, corPelo);
    
    glPushMatrix();                                        
    glTranslatef(personagemPosX, personagemBaseY, personagemPosZ);  
    glRotatef(-personagemAngulo, 0.0, 1.0, 0.0);
    
   //Corpo
    glPushMatrix();
    glTranslatef(0.0f, 0.55f, 0.0f);          
    glScalef(1.0f, 0.85f, 1.3f);             
    glutSolidSphere(0.4, 20, 20);             
    glPopMatrix();

    //Cabeça 
    glPushMatrix();
    glTranslatef(0.0f, 0.92f, -0.42f);
    glScalef(1.0f, 0.95f, 1.15f);           
    glutSolidSphere(0.26, 20, 20);
    glPopMatrix();

    //Focinho 
    glPushMatrix();
    glTranslatef(0.0f, 0.86f, -0.66f);
    glScalef(1.1f, 0.85f, 1.0f);
    glutSolidSphere(0.075, 12, 12);
    glPopMatrix();

    //Nariz 
    glPushMatrix();
    glTranslatef(0.0f, 0.90f, -0.72f);
    glutSolidSphere(0.022, 8, 8);
    glPopMatrix();

    //Dentes 
    glPushMatrix();
    glTranslatef(-0.028f, 0.79f, -0.70f);
    glScalef(0.045f, 0.06f, 0.03f);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.028f, 0.79f, -0.70f);
    glScalef(0.045f, 0.06f, 0.03f);
    glutSolidCube(1.0);
    glPopMatrix();
    
    //Orelhas
    glPushMatrix();
    glTranslatef(-0.12f, 1.28f, -0.42f);
    glRotatef(-12.0f, 0.0f, 0.0f, 1.0f);     
    glRotatef(-10.0f, 1.0f, 0.0f, 0.0f);     
    glScalef(0.08f, 0.42f, 0.08f);            
    glutSolidSphere(1.0, 12, 12);
    glPopMatrix();

    
    glPushMatrix();
    glTranslatef(0.12f, 1.28f, -0.42f);
    glRotatef(12.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(-10.0f, 1.0f, 0.0f, 0.0f);
    glScalef(0.08f, 0.42f, 0.08f);
    glutSolidSphere(1.0, 12, 12);
    glPopMatrix();
    
   

    //Olhos
    float corOlho[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, corOlho);

    glPushMatrix();
    glTranslatef(-0.10f, 0.98f, -0.70f);
    glScalef(1.0f, 1.1f, 0.5f);
    glutSolidSphere(0.028, 12, 12);    // <-- era 0.05
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.10f, 0.98f, -0.70f);
    glScalef(1.0f, 1.1f, 0.5f);
    glutSolidSphere(0.028, 12, 12);
    glPopMatrix();

    //Patas
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, corPelo);
    
   //Pata dianteira esquerda
    glPushMatrix();
    glTranslatef(-0.18f, 0.18f + deslocamentoPata, -0.22f);
    glScalef(0.10f, 0.36f, 0.10f);
    glutSolidCube(1.0);
    glPopMatrix();

   //Pata dianteira direita
    glPushMatrix();
    glTranslatef(0.18f, 0.18f - deslocamentoPata, -0.22f);
    glScalef(0.10f, 0.36f, 0.10f);
    glutSolidCube(1.0);
    glPopMatrix();

   //Pata traseira esquerda
    glPushMatrix();
    glTranslatef(-0.18f, 0.18f - deslocamentoPata, 0.25f);
    glScalef(0.12f, 0.36f, 0.12f);
    glutSolidCube(1.0);
    glPopMatrix();

   //Pata traseira direita
    glPushMatrix();
    glTranslatef(0.18f, 0.18f + deslocamentoPata, 0.25f);
    glScalef(0.12f, 0.36f, 0.12f);
    glutSolidCube(1.0);
    glPopMatrix();

   //Rabo
    glPushMatrix();
    glTranslatef(0.0f, 0.60f, 0.55f);
    glutSolidSphere(0.10, 10, 10);
    glPopMatrix();
    
    glPopMatrix();
}

void iniciaPulo() {
    if (!coelhoPulando) {
        coelhoPulando = true;
        velocidadePulo = forcaPulo;
    }
}

void atualizaPulo() {
    if (coelhoPulando) {
        velocidadePulo += gravidade;       
        personagemBaseY += velocidadePulo; 

        if (personagemBaseY <= 0.0f) {
            personagemBaseY = 0.0f;
            velocidadePulo = 0.0f;
            coelhoPulando = false;
        }
    }
}

void desenhaRaposa() {
    float corPelo[]     = { 0.95f, 0.51f, 0.49f, 1.0f };
    float corBarriga[]  = { 1.00f, 1.00f, 1.00f, 1.0f };
    float corOlho[]     = { 0.05f, 0.05f, 0.05f, 1.0f };

    // Corpo
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, corPelo);
    glPushMatrix();
    glTranslatef(0.0f, 0.55f, 0.0f);
    glScalef(1.0f, 0.85f, 1.5f);   
    glutSolidSphere(0.4, 16, 16);
    glPopMatrix();

    // Cabeça
    glPushMatrix();
    glTranslatef(0.0f, 0.95f, -0.5f);
    glutSolidSphere(0.24, 16, 16);
    glPopMatrix();

    // Focinho
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, corBarriga);
    glPushMatrix();
    glTranslatef(0.0f, 0.92f, -0.55f);      
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);    
    glTranslatef(0.0f, 0.10f, 0.0f);        
    glutSolidCone(0.10, 0.20, 10, 10);
    glPopMatrix();

    // Nariz
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, corOlho);
    glPushMatrix();
    glTranslatef(0.0f, 0.92f, -0.86f);
    glutSolidSphere(0.03, 8, 8);
    glPopMatrix();

    // Olhos
    glPushMatrix();
    glTranslatef(-0.10f, 1.00f, -0.68f);
    glutSolidSphere(0.04, 8, 8);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.10f, 1.00f, -0.68f);
    glutSolidSphere(0.04, 8, 8);
    glPopMatrix();

    // Orelhas
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, corPelo);
    for (int lado = -1; lado <= 1; lado += 2) {
        glPushMatrix();
        glTranslatef(lado * 0.12f, 1.20f, -0.48f);
        glRotatef(lado * -15.0f, 0.0f, 0.0f, 1.0f);   // inclina para os lados
        glRotatef(-10.0f, 1.0f, 0.0f, 0.0f);          // inclina levemente para trás
        glutSolidCone(0.06, 0.20, 8, 8);
        glPopMatrix();
    }

    // Patas
    for (int lado = -1; lado <= 1; lado += 2) {
        // Dianteiras
        glPushMatrix();
        glTranslatef(lado * 0.18f, 0.18f, -0.30f);
        glScalef(0.10f, 0.36f, 0.10f);
        glutSolidCube(1.0);
        glPopMatrix();

        // Traseiras
        glPushMatrix();
        glTranslatef(lado * 0.18f, 0.18f, 0.35f);
        glScalef(0.11f, 0.36f, 0.11f);
        glutSolidCube(1.0);
        glPopMatrix();
    }

    // Rabo
    glPushMatrix();
    glTranslatef(0.0f, 0.65f, 0.60f);
    glRotatef(-100.0f, 1.0f, 0.0f, 0.0f);  
    glScalef(1.0f, 1.8f, 1.0f);
    glutSolidCone(0.08, 0.30, 8, 8);
    glPopMatrix();
}

void atualizaRaposa() {
    float coelhoX = personagemPosX;
    float coelhoZ = personagemPosZ;

    if (!raposaVisivel) {
        int numeroAleatorio = rand() % 100;

        if (numeroAleatorio < 1) {
            float distancia = 15.0f;
            raposaX = coelhoX - distancia;   
            raposaZ = coelhoZ + (rand() % 10 - 5);  
            raposaVisivel = true;
        }
    } else {
        float dx = coelhoX - raposaX;
        float dz = coelhoZ - raposaZ;
        float dist = sqrtf(dx*dx + dz*dz);

        if (dist > 0.01f) {
            float nx = dx / dist;
            float nz = dz / dist;
            raposaX += nx * raposaVelocidade;
            raposaZ += nz * raposaVelocidade;

            raposaAngulo = atan2f(-nz, nx) * 180.0f / M_PI;
        }

        float dxCol = coelhoX - raposaX;
        float dzCol = coelhoZ - raposaZ;
        float distCol = sqrtf(dxCol*dxCol + dzCol*dzCol);

        if (distCol < raioColisaoRaposa && personagemBaseY < 0.1f && !imune) {
            vidas--;
            if (vidas <= 0) {
                // Colocar o "game over" 
                // jogoRodando = false;
            }
            raposaVisivel = false;
        }

        if (dist > 25.0f) {
            raposaVisivel = false;
        }
    }
}

void desenhaPersonagem() {
    glPushMatrix();
    glTranslatef(personagemPosX, personagemBaseY, personagemPosZ);
    glRotatef(personagemAngulo, 0.0, 1.0, 0.0);
    glRotatef(180.0, 0.0, 1.0, 0.0);
    desenhaCoelho();
    glPopMatrix();
}

/*void desenhaPersonagem() {
    glPushMatrix();

    glTranslatef(personagemPosX, personagemRaio, personagemPosZ);
    glRotatef(personagemAngulo, 0.0, 1.0, 0.0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[1]);

    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluQuadricNormals(quad, GLU_SMOOTH);
    gluSphere(quad, personagemRaio, 32, 32);
    gluDeleteQuadric(quad);

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}*/


    //Elementos de bonificação

    void desenhaCenoura3D() {
        //Raiz
        float corRaiz[] = { 0.97f, 0.60f, 0.09f, 1.0f };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, corRaiz);

        glPushMatrix();
        glTranslatef(0.0f, 0.20f, 0.0f);       
        glRotatef(180.0f, 1.0f, 0.0f, 0.0f);   
        glutSolidCone(0.12, 0.4, 12, 12);
        glPopMatrix();

        //Folhas
        float corFolha[] = { 0.0f, 0.28f, 0.23f, 1.0f };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, corFolha);

        for (int i = 0; i < 3; i++) {
            glPushMatrix();
            glTranslatef(0.0f, 0.40f, 0.0f);
            glRotatef(i * 120.0f, 0.0f, 1.0f, 0.0f);
            glRotatef(-25.0f, 1.0f, 0.0f, 0.0f);
            glTranslatef(0.0f, 0.12f, 0.0f);
            glutSolidCone(0.035, 0.22, 8, 8);
            glPopMatrix();
        }
    }

    void desenhaAlmeirao3D() {
        float corFolha[] = { 0.70f, 0.98f, 0.58f, 1.0f };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, corFolha);

        // Caule 
        glPushMatrix();
        glTranslatef(0.0f, 0.2f, 0.0f);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        GLUquadric* q = gluNewQuadric();
        gluCylinder(q, 0.02, 0.02, 0.4, 8, 1);
        gluDeleteQuadric(q);
        glPopMatrix();

        // Folhas 
        for (int i = 0; i < 3; i++) {
            glPushMatrix();
            glTranslatef(0.0f, 0.4f + i * 0.08f, 0.0f);
            glRotatef(i * 120.0f, 0.0f, 1.0f, 0.0f);
            glTranslatef(0.12f, 0.0f, 0.0f);
            glScalef(1.4f, 0.25f, 0.7f);
            glutSolidSphere(0.12, 10, 10);
            glPopMatrix();
        }
    }

    void desenhaCilindroVertical(float altura, float raioBase, float raioTopo,
                                 float r, float g, float b) {
        float cor[] = { r, g, b, 1.0f };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, cor);
        glPushMatrix();
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        GLUquadric* q = gluNewQuadric();
        gluCylinder(q, raioBase, raioTopo, altura, 10, 1);
        gluDeleteQuadric(q);
        glPopMatrix();
    }

    void desenhaCouve3D() {
        float corVerdeClaro[]  = { 0.70f, 0.98f, 0.58f, 1.0f };
        float corVerdeEscuro[] = { 0.00f, 0.28f, 0.23f, 1.0f };

        //Folha
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, corVerdeEscuro);
        glPushMatrix();
        glTranslatef(0.0f, 0.30f, 0.0f);
        glScalef(0.75f, 1.0f, 0.10f);
        glutSolidSphere(0.28, 16, 16);
        glPopMatrix();

        //Caule + nervuras
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.0f);
        desenhaCilindroVertical(0.55f, 0.035f, 0.008f,
                                corVerdeClaro[0], corVerdeClaro[1], corVerdeClaro[2]);
        glPopMatrix();

        // Linhas laterais
        float comprimento  = 0.12f;
        float raioNervura  = 0.008f;
        float zFrente      = 0.045f;
        float zTras        = -0.045f;

        float angulo = 40.0f;

        float ySaida[3] = { 0.18f, 0.28f, 0.38f };

        for (int lado = -1; lado <= 1; lado += 2) {   // esquerda e direita
            for (int face = 0; face < 2; face++) {    // frente e trás
                float zFace = (face == 0) ? zFrente : zTras;

                for (int i = 0; i < 3; i++) {
                    glPushMatrix();
                    glTranslatef(0.0f, ySaida[i], zFace);

                    glRotatef(lado * angulo, 0.0f, 0.0f, 1.0f);
                    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

                    desenhaCilindroVertical(comprimento, raioNervura, raioNervura,
                                            corVerdeClaro[0], corVerdeClaro[1], corVerdeClaro[2]);
                    glPopMatrix();
                }
            }
        }
    }

    void desenhaVegetal(Vegetal v) {
        glPushMatrix();
        glTranslatef(v.x, 0.0f, v.z);
        switch (v.tipo) {
            case 1: desenhaAlmeirao3D(); break;
            case 2: desenhaCenoura3D();  break;
            case 3: desenhaCouve3D();    break;
        }
        glPopMatrix();
    }

    void desenhaVegetais() {
        for (int i = 0; i < NUM_VEGETAIS; i++) {
            if (vegetais[i].ativo) {
                desenhaVegetal(vegetais[i]);
            }
        }
    }
    
    void inicializaVegetais() {
        for (int i = 0; i < NUM_VEGETAIS; i++) {
            vegetais[i].x = (rand() % 40) - 20.0f;    // -20 a +20
            vegetais[i].z = (rand() % 40) - 20.0f;
            vegetais[i].tipo = (rand() % 3) + 1;
            vegetais[i].bloco = 0;
            vegetais[i].ativo = true;
        }
    }

    void atualizaVegetais() {
        // Recicla vegetais que ficaram para trás do coelho
        for (int i = 0; i < NUM_VEGETAIS; i++) {
            if (vegetais[i].ativo && vegetais[i].x < personagemPosX - 30.0f) {
                vegetais[i].x = personagemPosX + 30.0f + (rand() % 20);
                vegetais[i].z = (rand() % 40) - 20.0f;
                vegetais[i].tipo = (rand() % 3) + 1;
                vegetais[i].ativo = true;
            }
        }
    }

    void colisaoVegetal() {
        float coelhoX = personagemPosX;
        float coelhoZ = personagemPosZ;
        float raioColeta = 0.6f;   

        for (int i = 0; i < NUM_VEGETAIS; i++) {
            if (!vegetais[i].ativo) continue;

            float dx = coelhoX - vegetais[i].x;
            float dz = coelhoZ - vegetais[i].z;
            float distancia = sqrtf(dx*dx + dz*dz);

            if (distancia < raioColeta) {
                vegetais[i].ativo = false;

                switch (vegetais[i].tipo) {
                    case 1: 
                        pontos++;
                        vidas++;
                        break;
                    case 2: 
                        pontos += 10;
                        break;
                    case 3: 
                        pontos++;
                        imune = true;
                        tempoImune = 180;
                        break;
                }
            }
        }
    }

void display() {
    atualizaVegetais();
    colisaoVegetal();
    atualizaRaposa();
    atualizaPulo();
    
    if (imune) {
        tempoImune--;
        if (tempoImune <= 0) imune = false;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    switch (modoCamera) {
        case 0:
            cameraVisaoGeral();
            break;
        case 1:
            cameraPrimeiraPessoa();
            break;
    }
    
    desenhaChao();
    desenhaVegetais();
    desenhaCoelho();
    if (raposaVisivel) {
        glPushMatrix();
        glTranslatef(raposaX, 0.0f, raposaZ);
        glRotatef(raposaAngulo, 0.0f, 1.0f, 0.0f);
        desenhaRaposa();
        glPopMatrix();
    }

    /*desenhaPersonagem();*/

    desenhaTextos();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (double)w / (double)h, 0.1, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

// função principal

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("T2");

    initGL();
    loadTextures();
    inicializaVegetais();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboardChangeCamera);
    glutSpecialFunc(specialKeyFunction);
    glutIdleFunc(display);
    
    

    glutMainLoop();
    return 0;
}




