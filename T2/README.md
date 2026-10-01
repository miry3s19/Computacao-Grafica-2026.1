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

## Anotações úteis

Use para inserir um bloco de código:
```cpp

```

Use ctrl + shift + v para visualizar o resultado do markdown no vs code

## Tarefas faltantes (excluir depois!)

- Adicionar descrição das tarefas que já fiz

- Editar personagens e fazer bonificações
- Adicionar display  em visão geral quando ele for finalizado

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

#### display

```cpp

```

## Questões Propostas

### Questão 01 - Modelagem tridimensional

#### Instruções

Utilize os sólidos disponibilizados pela GLUT para implementar, em três dimensões, os obstáculos e os
personagens secundários presentes no primeiro trabalho.
Assim como na primeira etapa, deverão ser implementados o aparecimento aleatório dos obstáculos e dos
objetos de bonificação, respeitando a dinâmica do jogo original.

#### Implementação

##### Formas base

```cpp

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
``

##### Raposa

```cpp

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
``

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

Eu copiei algumas coisas direto no código da professora, eu acho que tem iluminação já.

## Bônus
