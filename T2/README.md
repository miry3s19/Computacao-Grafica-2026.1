# Relatório Trabalho 02

## Índice

- [Contextualização](#contextualização-do-trabalho)
- [Visão Geral](#visão-geral)
- [Questão 01](#questão-01---modelagem-tridimensional)
- [Questão 02](#questão-02---texturas)
- [Questão 03](#questão-03---movimentação-do-personagem)
- [Questão 04](#questão-04---alternância-da-câmera)
- [Questão 05](#questão-05---rotação-da-câmera)
- [Questão 06](#questão-06---iluminação)

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

##### Personagens

Adicionar aqui o código e descrição dos personagens. A princípio os personagens são esferas, a professora falou que poderiamos primeiro fazer simples para focar em outras coisas, já ganharemos ponto, mas é bom garantir botando bichos bonitinhos no final.

A bola está com textura, então talvez seja preciso alterar isso pra cor lisa.

##### Cenário

Tentativa de um cenário infinito, com um chão que aparece a medida que o personagem se move.

###### Variáveis de controle

```cpp
float chaoDeslocX = 0.0f;
float chaoDeslocZ = 0.0f;
float chaoCelula = 2.0f;
int chaoNumCelulas = 20;
```

- chaoDeslocX: desloca a grade (grid) no eixo x.
- chaoDeslocZ: desloca a grade no eixo z.
- chaoCelula: tamanho de cada celula (tile) do chão, nesse caso, cada celula tem um tamanho 2x2.
- chaoNumCelulas: quantidade de celulas em cada direção, nesse caso, forma um quadrado com 20x20 celulas.

###### função principal

```cpp
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
```

*O que o código faz?*

- Empilha a matriz de tranformação atual, desconsiderando qualquer transformação anterior.

```cpp
glPushMatrix();
```

- Calcula o deslocamento da grade baseado na posição atual do personagem:

```cpp
chaoDeslocX = floor(personagemPosX / chaoCelula) * chaoCelula;
chaoDeslocZ = floor(personagemPosZ / chaoCelula) * chaoCelula;
```

A posição do personagem é dividida pelo tamanho da célula, obtém o inteiro mais próximo utilizando a função de arredondamento floor() e multiplica novamente pelo tamanho da célula. Esse processo é feito para os eixos X e Y e permite que a grade sempre fique alinhada com a posição atual do personagem.

- Translada a grade para a posição calculada

```cpp
glTranslatef(chaoDeslocX, 0.0f, chaoDeslocZ);
```
Move a grade para que ela esteja centrada na posição atual do personagem.

- Habilita e aplica a textura do chão

- Desenha a grade de quadrados

``` cpp
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
```
São desenhadas 20x20 células, onde cada uma é um quadrado no plano Y=0. Para aplicar a textura, utilizam-se as coordenadas (0,0) e (1,1) para que exibam a textura completa. A normal (0,1,0) indica que a superfície aponta pra cima para aplicação da iluminação.

- Desabilita a textura e restaura a matriz de transformação

``` cpp
glDisable(GL_TEXTURE_2D);
glPopMatrix();
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

-Componentes:

sin(rad) fornece a componente horizontal (eixo X) da direção.
cos(rad) fornece a componente vertical (eixo Z) da direção.

-Seta pra cima:

```cpp
case GLUT_KEY_UP:
        personagemPosX += velocidade * sin(rad);
        personagemPosZ -= velocidade * cos(rad);
        break;
```

Move o personagem para frente na direção que ele está olhando. Para isso, a função soma a posição do personagem em X com velocidade * sin(rad) e  subtrai a posição do personagem em Z com velocidade * cos(rad).

-Seta pra baixo:

```cpp
case GLUT_KEY_DOWN:
        personagemPosX -= velocidade * sin(rad);
        personagemPosZ += velocidade * cos(rad);
        break;
```

Move o personagem para trás na direção que ele está olhando. Para isso, a função subtrai a posição do personagem em X e soma a posição do personagem em Z com velocidade * cos(rad).

-Seta pra esquerda:

```cpp
case GLUT_KEY_LEFT:
        personagemAngulo -= velocidadeRotacao;
        break;
```
Rotaciona o personagem no sentido anti-horário diminuindo o ângulo atual do personagem.

-Seta pra direita:

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

 Para alterar a câmera, foi utilzada uma função "keyboardChangeCamera" que é passada como parâmetro de "glutKeyboardFunc".
 Dentro da função foram adicionados dois casos:

 - Um para parar o jogo. (ainda precisa ser implementado como pausa, não parar o jogo).
 - Quando a pessoa pressiona a tecla 'c' para alternar o modo da câmera.

```cpp
void keyboardChangeCamera(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // ESC
            exit(0);
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
    gluLookAt(
        0.0, 15.0, 20.0, // posição camera
        0.0, 0.0, 0.0,   // mira
        0.0, 1.0, 0.0    // pra cima
    );
}
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

```cpp
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

```

### Questão 06 - Iluminação

#### Instruções

Utilize pelo menos duas fontes de iluminação na cena.
As fontes deverão contribuir para a iluminação dos diferentes elementos tridimensionais presentes no jogo.

#### Implementação

Eu copiei algumas coisas direto no código da professora, eu acho que tem iluminação já.

## Bônus
