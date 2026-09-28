#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



// Personagem (esfera)
float personagemPosX = 0.0f;
float personagemPosZ = 0.0f;
float personagemRaio = 0.5f;
float personagemAngulo = 0.0f;

// Câmera
int modoCamera = 0; // 0 = visão geral, 1 = primeira pessoa
float camDistancia = 10.0f;
float camAltura = 2.0f;

// Rotação da Câmera
float cameraRoacaoY = 0.0;
float cameraRaio = 25.0;

// Chão
float chaoDeslocX = 0.0f;
float chaoDeslocZ = 0.0f;
float chaoCelula = 2.0f;
int chaoNumCelulas = 20;

// Texturas
GLuint texID[2]; // 0 = chão, 1 = personagem
char* textureFileNames[2] = {
    "texturas/grass.jpg",
    "texturas/marble.jpg"
};


void initGL() {
    glClearColor(0.62f, 0.85f, 0.90f, 1.0f); // background color

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

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
            cameraRoacaoY += 15.0;
            break;
        case 'c':
            modoCamera = (modoCamera + 1) % 2;
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
            personagemAngulo += velocidadeRotacao;
            break;
        case GLUT_KEY_RIGHT:
            personagemAngulo -= velocidadeRotacao;
            break;
    }
    glutPostRedisplay();
}

//Câmera

void cameraVisaoGeral() {

    float rad = cameraRoacaoY * M_PI / 180;

    float camX = cameraRaio * sin(rad);
    float camZ = cameraRaio * cos(rad);
    float camY = 15.0;

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
    float alvoY = personagemRaio;

    gluLookAt(
        cameraX, cameraY, cameraZ,
        alvoX, alvoY, alvoZ,
        0.0, 1.0, 0.0
    );
}

//Cenário

void desenhaChao() {
    glPushMatrix();

    chaoDeslocX = floor(personagemPosX / chaoCelula) * chaoCelula;
    chaoDeslocZ = floor(personagemPosZ / chaoCelula) * chaoCelula;
    glTranslatef(chaoDeslocX, 0.0f, chaoDeslocZ);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[0]);

    for (int i = -chaoNumCelulas; i < chaoNumCelulas; i++) {
        for (int j = -chaoNumCelulas; j < chaoNumCelulas; j++) {
            float x1 = i * chaoCelula;
            float z1 = j * chaoCelula;
            float x2 = (i + 1) * chaoCelula;
            float z2 = (j + 1) * chaoCelula;

            glBegin(GL_QUADS);
                glNormal3f(0.0, 1.0, 0.0);
                glTexCoord2f(0.0, 0.0);
                glVertex3f(x1, 0.0f, z1);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(x2, 0.0f, z1);
                glTexCoord2f(1.0, 1.0);
                glVertex3f(x2, 0.0f, z2);
                glTexCoord2f(0.0, 1.0);
                glVertex3f(x1, 0.0f, z2);
            glEnd();
        }
    }

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

//Personagens

void desenhaPersonagem() {
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
}

void display() {
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
    desenhaPersonagem();

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

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboardChangeCamera);
    glutSpecialFunc(specialKeyFunction);
    glutIdleFunc(display);

    glutMainLoop();
    return 0;
}
