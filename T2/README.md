# Relatório Trabalho 02

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

```


#### display

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

### Questão 02 - Texturas (feito para o cenário)

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

Texturas:

- uma textura de grama para o chão

### Questão 03 - Movimentação do personagem (feito)

Permita que o personagem principal seja controlado por meio das setas do teclado, de acordo com as seguintes
regras:
- Seta para cima: movimenta o personagem para frente;
- Seta para baixo: movimenta o personagem para trás;
- Seta para esquerda: rotaciona o personagem em torno do seu próprio eixo;
- Seta para direita: rotaciona o personagem em torno do seu próprio eixo.
O deslocamento para frente e para trás deve respeitar a direção para a qual o personagem está orientado.

### Questão 04 - Alternância da câmera (feito)

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


### Questão 05 - Rotação da câmera (feito)

#### Instruções

Quando a visão geral estiver acionada, permita que, ao pressionar a tecla “r”, a posição da câmera seja
rotacionada em relação ao eixo Y, possibilitando observar a cena a partir de diferentes ângulos.

#### Implementação

```cpp



```

### Questão 06 - Iluminação

#### Instruções

Utilize pelo menos duas fontes de iluminação na cena.
As fontes deverão contribuir para a iluminação dos diferentes elementos tridimensionais presentes no jogo.

#### Implementação

Eu copiei algumas coisas direto no código da professora, eu acho que tem iluminação já.

## Bônus
