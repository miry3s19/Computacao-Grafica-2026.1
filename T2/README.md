# Relatório Trabalho 02

## Índice

- [Contextualização](#contextualização-do-trabalho)
- [Visão Geral](#visão-geral)
- [Questão 01 - Modelagem tridimensional](#questão-01---modelagem-tridimensional)
- [Questão 02 - Texturas](#questão-02---texturas)
- [Questão 03 - Movimentação do personagem](#questão-03---movimentação-do-personagem)
- [Questão 04 - Alternância da Câmera](#questão-04---alternância-da-câmera)
- [Questão 05 - Rotação da Câmera](#questão-05---rotação-da-câmera)
- [Questão 06 - Iluminação](#questão-06---iluminação)



## Contextualização do Trabalho

Na primeira etapa do trabalho, foi requisitada a implementação de um jogo 2D. Nesta segunda etapa, com o
objetivo de fixar os conteúdos referentes à modelagem e à visualização tridimensional, o jogo desenvolvido
deverá ser expandido para uma versão 3D.
Nesta versão, o personagem principal deverá se movimentar sobre um quadrilátero contido no plano XZ,
centrado na origem. Esse quadrilátero representa a base de um cubo que delimita o mundo no qual o jogo se
desenvolve.
As características e a dinâmica básica do jogo devem ser preservadas em relação à primeira etapa, sendo
realizadas as adaptações necessárias para a representação e interação em um ambiente tridimensional.

## Visão Geral

### main
```cpp
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("T2 - Computacao Grafica 3D");

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
```

### funções auxiliares

#### init
```cpp
void init() {
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
```
#### loadTextures

```cpp
GLuint texID[2]; // 0 = chão, 1 = personagem
char* textureFileNames[2] = {
    "texturas/grass.jpg",
    "texturas/marble.jpg"
};

void loadTextures() {
    int width, height, nrChannels;
    unsigned char *data;

    glGenTextures(2, texID);

    for (int i = 0; i < 2; i++) {
        glBindTexture(GL_TEXTURE_2D, texID[i]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
```

- **Geração de IDs**: cria 2 identificadores de textura (`glGenTextures`).
- **Configuração de wrapping/filtragem**: define repetição (`GL_REPEAT`) e filtragem linear (`GL_LINEAR`), garantindo que a textura se repita corretamente no chão.
- **Carregamento com stb_image**: lê o arquivo de imagem e obtém largura, altura e canais.
- **Envio para a GPU**: `glTexImage2D` envia os dados para a OpenGL; `GL_GENERATE_MIPMAP` gera mipmaps automaticamente.
- **Liberação de memória**: `stbi_image_free` libera o buffer da imagem após o envio.

#### Funções de texto (HUD)

```cpp
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
```

- **Posicionamento**: `glRasterPos2f` define a posição do texto no espaço da janela.
- **Renderização caractere a caractere**: `glutBitmapCharacter` desenha cada caractere usando a fonte `GLUT_BITMAP_TIMES_ROMAN_24`.
- **Formatação dinâmica**: `sprintf` formata strings com valores atuais de vidas e pontos.

```cpp
void desenhaTextos() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 100, 0, 100);

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
```

- **Troca para projeção ortográfica**: `gluOrtho2D(0, 100, 0, 100)` cria um sistema de coordenadas 2D fixo, independente da câmera 3D, garantindo que o HUD fique sempre no canto da tela.
- **Desabilitação temporária**: desliga iluminação, textura e teste de profundidade para que o texto seja desenhado por cima da cena sem interferência.
- **Restauração de estados**: reativa `GL_DEPTH_TEST` e `GL_LIGHTING` ao final, e restaura as matrizes com `glPopMatrix`.

#### display

```cpp
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

    desenhaTextos();

    glutSwapBuffers();
}
```

- **Atualizações de estado**: chama as funções que atualizam a posição dos vegetais, verificam colisões, movem a raposa e atualizam a física do pulo.
- **Controle de imunidade**: decrementa o contador de tempo de imunidade a cada frame; quando chega a zero, desativa o efeito.
- **Limpeza do buffer**: limpa os buffers de cor e profundidade.
- **Seleção da câmera**: escolhe entre a visão geral e a primeira pessoa com base na variável `modoCamera`.
- **Desenho da cena**: desenha o chão, os vegetais, o coelho e, se visível, a raposa.
- **Textos**: desenha os textos de interface (vidas, pontos, imunidade).
- **Troca de buffers**: exibe o frame renderizado (`glutSwapBuffers`).

#### reshape

```cpp
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (double)w / (double)h, 0.1, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}
```

- **Ajuste do viewport**: define a área de renderização para toda a janela.
- **Reconfiguração da projeção**: recria a matriz de projeção perspectiva com o novo aspecto (`w/h`), evitando distorção ao redimensionar a janela.
- **Retorno ao modelo**: volta para `GL_MODELVIEW` para que as transformações da cena continuem funcionando corretamente.

## Questões Propostas

### Questão 01 - Modelagem tridimensional


#### Instruções

Utilize os sólidos disponibilizados pela GLUT para implementar, em três dimensões, os obstáculos e os
personagens secundários presentes no primeiro trabalho.
Assim como na primeira etapa, deverão ser implementados o aparecimento aleatório dos obstáculos e dos
objetos de bonificação, respeitando a dinâmica do jogo original.

#### Implementação

### Formas base

Foram utilizados os seguintes sólidos da GLUT para compor os personagens e elementos do jogo:

| Sólido | Utilização |
|--------|-----------|
| `glutSolidSphere` | Corpo, cabeça, focinho, nariz, olhos, orelhas, rabo, folhas arredondadas |
| `glutSolidCube` | Dentes, patas |
| `glutSolidCone` | Orelhas da raposa, rabo da raposa, raiz da cenoura, folhas da cenoura |
| `gluCylinder` | Caule do almeirão, caule e nervuras da couve |

### Raposa

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
        glRotatef(lado * -15.0f, 0.0f, 0.0f, 1.0f);
        glRotatef(-10.0f, 1.0f, 0.0f, 0.0f);
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
```
Foram utilizadas esferas, cones e cubos para compor a raposa, com transformações de escala para deformar os sólidos conforme desejado.

### Verduras

#### Couve

```cpp
void desenhaCouve3D() {
    float corVerdeClaro[]  = { 0.70f, 0.98f, 0.58f, 1.0f };
    float corVerdeEscuro[] = { 0.00f, 0.28f, 0.23f, 1.0f };

    // Folha
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, corVerdeEscuro);
    glPushMatrix();
    glTranslatef(0.0f, 0.30f, 0.0f);
    glScalef(0.75f, 1.0f, 0.10f);
    glutSolidSphere(0.28, 16, 16);
    glPopMatrix();

    // Caule + nervuras
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

    for (int lado = -1; lado <= 1; lado += 2) {
        for (int face = 0; face < 2; face++) {
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
```
Foram utilizadas esferas e cilindros para compor a couve, com transformações de escala para deformar os sólidos conforme desejado. Os cilindros laterais saem do caule em diferentes alturas, tanto na frente quanto atrás, simulando as nervuras da folha.

#### Função auxiliar para cilindros

```cpp
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
```
### Aparecimento aleatório dos obstáculos e bonificações

```cpp
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
```

### Aparecimento aleatório da raposa

```cpp
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
        // ... movimentação e colisão ...
    }
}
```

##### Coelho

```cpp
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
```

##### Raposa

```cpp
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
``

##### Verduras

```cpp
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
```

### Sistema de vidas, pontos e imunidade

```cpp
int vidas = 5;
int pontos = 0;
bool imune = false;
int tempoImune = 180;
```

- **vidas**: começa em 5; decrementada ao colidir com a raposa.
- **pontos**: incrementado ao coletar vegetais.
- **imune**: ativado ao coletar couve; impede dano da raposa.
- **tempoImune**: contador em frames (180 ≈ 3 segundos a 60 FPS).

```cpp
if (imune) {
    tempoImune--;
    if (tempoImune <= 0) imune = false;
}
```

- A cada frame, o contador é decrementado; ao chegar a zero, a imunidade é desativada.

### Colisão com vegetais

```cpp
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
```

- **Detecção**: calcula a distância euclidiana no plano XZ entre o coelho e cada vegetal ativo.
- **Coleta**: se a distância for menor que `raioColeta` (0.6), o vegetal é desativado e seus efeitos são aplicados.
- **Efeitos por tipo**:
  - Tipo 1 (almeirão): +1 ponto e +1 vida.
  - Tipo 2 (cenoura): +10 pontos.
  - Tipo 3 (couve): +1 ponto e ativa imunidade por 180 frames.

##### Cenário

Tentativa de um cenário infinito, com um chão que aparece a medida que o personagem se move.

```cpp

```

###### Variáveis de controle internas

```cpp
 float tamanho = 200.0;
```
- tamanho: determina o tamanho do quadrado, que nesse caso, representa o chão. Um tamanho grande é escolhido para dar a ilusão de um chão infinito.

###### função principal

```cpp
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
```

*O que o código faz?*

- Habilita e aplica a textura.

```cpp
glEnable(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, texID[0]);
```
- Desenha o quadrado do chão e aplica as texturas.

```cpp
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
```

- Desabilita a textura.

``` cpp
glDisable(GL_TEXTURE_2D);
```

### Questão 02 - Texturas

#### Instruções
Insira texturas na cena, buscando proporcionar um maior grau de detalhamento e realismo aos elementos do
jogo.
As texturas devem ser utilizadas, no mínimo, nos seguintes elementos:
- chão da cena;
- personagem principal;
- personagens secundários;
- obstáculos;
- elementos de bonificação.
A escolha das imagens utilizadas como texturas é livre, desde que sejam adequadas à proposta do jogo.

#### Implementação

```cpp
GLuint texID[2]; // 0 = chão, 1 = personagem
char* textureFileNames[2] = {
    "texturas/grass.jpg",
    "texturas/marble.jpg"
};
```

Foi utilizado o código base dado em aula:

```cpp
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
```

Texturas:

- uma textura de grama para o chão

### Questão 03 - Movimentação do personagem

Permita que o personagem principal seja controlado por meio das setas do teclado, de acordo com as seguintes
regras:
- Seta para cima: movimenta o personagem para frente;
- Seta para baixo: movimenta o personagem para trás;
- Seta para esquerda: rotaciona o personagem em torno do seu próprio eixo;
- Seta para direita: rotaciona o personagem em torno do seu próprio eixo.
O deslocamento para frente e para trás deve respeitar a direção para a qual o personagem está orientado.

#### Implementação

##### Variáveis de controle internas

```cpp
float velocidade = 0.2;
float velocidadeRotacao = 5.0;
float rad = personagemAngulo * M_PI / 180.0;
```
- velocidade: determina a velocidade de deslocamento do personagem, nesse caso é 0.2.
- velocidadeRotacao: determina a velocidade de rotação do personagem, nesse caso é 5.0.
- rad: converte o angulo atual do personagem para radianos, porque sin() e cos() exigem valores em radiano.

##### Função principal

```cpp
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
```

*O que o código faz?*

- Componentes:

sin(rad) fornece a componente horizontal (eixo X) da direção.
cos(rad) fornece a componente vertical (eixo Z) da direção.

- Seta pra cima:

```cpp
case GLUT_KEY_UP:
        personagemPosX += velocidade * sin(rad);
        personagemPosZ -= velocidade * cos(rad);
        break;
```

Move o personagem para frente na direção que ele está olhando. Para isso, a função soma a posição do personagem em X com velocidade * sin(rad) e  subtrai a posição do personagem em Z com velocidade * cos(rad).

- Seta pra baixo:

```cpp
case GLUT_KEY_DOWN:
        personagemPosX -= velocidade * sin(rad);
        personagemPosZ += velocidade * cos(rad);
        break;
```

Move o personagem para trás na direção que ele está olhando. Para isso, a função subtrai a posição do personagem em X e soma a posição do personagem em Z com velocidade * cos(rad).

- Seta pra esquerda:

```cpp
case GLUT_KEY_LEFT:
        personagemAngulo -= velocidadeRotacao;
        break;
```
Rotaciona o personagem no sentido anti-horário diminuindo o ângulo atual do personagem.

- Seta pra direita:

```cpp
case GLUT_KEY_LEFT:
        personagemAngulo += velocidadeRotacao;
        break;
```
Rotaciona o personagem no sentido horário aumentando o ângulo atual do personagem.

- Atualiza a tela

```cpp
glutPostRedisplay();
```

Por fim, redesenha a tela com as mudanças realizadas.

### Sistema de pulo

```cpp
float velocidadePulo = 0.0f;
float gravidade = -0.015f;
float forcaPulo = 0.22f;
bool coelhoPulando = false;
```

- **velocidadePulo**: velocidade vertical atual do coelho.
- **gravidade**: aceleração aplicada a cada frame.
- **forcaPulo**: impulso inicial aplicado ao pular.
- **coelhoPulando**: flag que indica se o coelho está no ar.

```cpp
void iniciaPulo() {
    if (!coelhoPulando) {
        coelhoPulando = true;
        velocidadePulo = forcaPulo;
    }
}
```

- **Acionamento**: chamada pela tecla espaço (`case 32` em `keyboardChangeCamera`).
- **Restrição**: só permite pular se o coelho já não estiver pulando (evita pulo duplo).

```cpp
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
```

- **Física**: aplica gravidade à velocidade vertical e atualiza a posição Y (`personagemBaseY`).
- **Aterrissagem**: quando o coelho retorna ao chão (Y ≤ 0), zera a velocidade, reseta a posição e desativa a flag.

### Animação das patas

```cpp
float deslocamentoPata = 0.0f; 
```

```cpp
//Pata dianteira esquerda
glTranslatef(-0.18f, 0.18f + deslocamentoPata, -0.22f);
//Pata dianteira direita
glTranslatef(0.18f, 0.18f - deslocamentoPata, -0.22f);
//Pata traseira esquerda
glTranslatef(-0.18f, 0.18f - deslocamentoPata, 0.25f);
//Pata traseira direita
glTranslatef(0.18f, 0.18f + deslocamentoPata, 0.25f);
```

- **Efeito**: desloca verticalmente as patas em pares opostos (dianteira esquerda sobe enquanto dianteira direita desce), simulando o movimento de caminhada/corrida.
- **Observação**: no estado atual do código, `deslocamentoPata` não é modificado em nenhum ponto — permanece 0, portanto a animação está implementada mas inativa.


### Questão 04 - Alternância da câmera

#### Instruções

Permita que, ao pressionar a tecla “c”, a visualização da cena seja alternada entre:
- uma visão geral da cena, posicionada, por exemplo, no ponto (0, 10, 10); e
- uma visão em primeira pessoa, posicionada de acordo com a orientação e a posição do personagem principal.

#### Implementação

##### Variáveis de controle
```cpp
    int modoCamera = 0;
    float camDistancia = 2.0f;
    float camAltura = 1.0f;
    float cameraRotacaoY = 0.0;
    float cameraRaio = 25.0;
```

- modoCamera: modo atual da câmera, 0 para visão geral, 1 para visão em primeira pessoa
- camDistancia: distancia atrás do coelho em primeira pessoa
- camAltura: altura da câmera vista em primeira pessoa
- cameraRotacaoY: angulo atual da camera em visão geral.
- cameraRaio: raio orbital em visão geral.

#### Funções principais

 Para alterar a câmera, foi utilzada uma função "keyboardChangeCamera" que é passada como parâmetro de "glutKeyboardFunc".
 Dentro da função foram adicionados três casos:

 - Uma para fechar o jogo quando ESC é pressionado.
 - Quando a pessoa pressiona a tecla 'c' para alternar o modo da câmera.
 - Quando a pessoa pressiona a tecla 'r' para alternar o ângulo de visão geral (essa opção será abordada no tópico seguinte).

```cpp
void keyboardChangeCamera(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // ESC
            exit(0);
            break;
        case 'r':
            cameraRotacaoY += 15.0;
            break;
        case 'c':
            modoCamera = (modoCamera + 1) % 2;
            break;
    }
    glutPostRedisplay();
}
```

```cpp
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
```
*O que esse código faz?*

- Componentes:

sin(rad) fornece a componente horizontal (eixo X) da direção.
cos(rad) fornece a componente vertical (eixo Z) da direção.

- rad: converte o angulo atual do personagem para radianos, porque sin() e cos() exigem valores em radiano.

```cpp
 float rad = cameraRotacaoY * M_PI / 180;
```

- a partir do raio da câmera, é calculada a posição dela baseada nos eixos X e Z. Ela tem uma altura fixa.

```cpp
 float camX = cameraRaio * sin(rad);
    float camZ = cameraRaio * cos(rad);
    float camY = 10.0;
```

- a câmera sempre mira na origem.

```cpp
gluLookAt(
        camX, camY, camZ, // posição camera
        0.0, 0.0, 0.0,   // mira
        0.0, 1.0, 0.0    // pra cima
    );
```

```cpp
```

```cpp
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
```

*O que esse código faz?*

- Componentes:

sin(rad) fornece a componente horizontal (eixo X) da direção.
cos(rad) fornece a componente vertical (eixo Z) da direção.

- rad: converte o angulo atual do personagem para radianos, porque sin() e cos() exigem valores em radiano.

```cpp
float rad = personagemAngulo * M_PI / 180.0;
```

- a posição da câmera depende da posição atual do personagem em cada um dos eixos X e Z. Ela é calculada no eixo X a partir da posição do personagem menos a distância no eixo X, no eixo Z, ela é calculada a partir da posição do personagem menos a distância no eixo Z. A altura da câmera é fixa.

```cpp
float cameraX = personagemPosX - camDistancia * sin(rad);
float cameraZ = personagemPosZ + camDistancia * cos(rad);
float cameraY = camAltura;
```

- a mira acompanha sempre a posição atual do personagem nos eixos X e Z e mostra 5.0 a frente do coelho. A altura da mira é fixa.

```cpp
float alvoX = personagemPosX + 5.0 * sin(rad);
float alvoZ = personagemPosZ - 5.0 * cos(rad);
float alvoY = personagemRaio;
```

### Questão 05 - Rotação da câmera

#### Instruções

Quando a visão geral estiver acionada, permita que, ao pressionar a tecla “r”, a posição da câmera seja
rotacionada em relação ao eixo Y, possibilitando observar a cena a partir de diferentes ângulos.

#### Implementação

```cpp
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
```

*O que esse código faz?*

- Quando 'r' é pressionado, o ângulo de rotação em volta do eixo Y é incrementado, alterando o ângulo diretamente em visão geral.

```cpp
case 'r':
            cameraRoacaoY += 15.0;
            break;
```

### Questão 06 - Iluminação

#### Instruções

Utilize pelo menos duas fontes de iluminação na cena.
As fontes deverão contribuir para a iluminação dos diferentes elementos tridimensionais presentes no jogo.

#### Implementação

Foram utilizadas **duas fontes de luz** na cena:

```cpp
glEnable(GL_LIGHT0);
glEnable(GL_LIGHT1);

// Luz 1 - difusa, posicionada
float luz1Difusa[]  = { 0.3f, 0.4f, 0.5f, 1.0f };
float luz1Posicao[] = { -8.0f, 6.0f, -5.0f, 1.0f };
glLightfv(GL_LIGHT1, GL_DIFFUSE,  luz1Difusa);
glLightfv(GL_LIGHT1, GL_POSITION, luz1Posicao);

// Luz 0 - ambiente + difusa, posicionada no topo
float luzAmbiente[] = {0.3f, 0.3f, 0.3f, 1.0f};
float luzDifusa[]  = {0.7f, 0.7f, 0.7f, 1.0f};
float luzPosicao[] = {0.0f, 10.0f, 0.0f, 1.0f};
glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
glLightfv(GL_LIGHT0, GL_POSITION, luzPosicao);
```

### Descrição das luzes

| Luz | Tipo | Posição | Contribuição |
|-----|------|---------|--------------|
| GL_LIGHT0 | Ambiente + Difusa | (0, 10, 0) | Iluminação principal, vinda de cima |
| GL_LIGHT1 | Difusa | (-8, 6, -5) | Luz de preenchimento, vinda de trás/esquerda |

### Material padrão

```cpp
float white[4] = { 1, 1, 1, 1 };
glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white);
```


## Bônus
