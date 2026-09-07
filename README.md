# Computacao-Grafica-2026.1
Trabalhos de Computação Gráfica, disciplina da graduação em Ciência da Computação da UERJ


## Questões propostas

### 1. O plano de fundo deve ter as seguintes características:
-[X] a. A tela de fundo consiste em ~~uma horta fixa~~, com ~~canteiros~~, ~~cercas de madeira~~ e o ~~céu ao fundo~~, ~~visualizada parcialmente a cada instante do jogo, conforme a movimentação do coelho~~;

-[X] b. O cenário deve ser construído a partir de ~~primitivas geométricas: quadriláteros (cercas e canteiros), triângulos (folhagens) e círculos (vegetais ao fundo, sol)~~;

-[X] c. ~~A mesma tela deve ser visualizada em loop~~: quando o coelho chegar ao final da horta, o cenário deve reiniciar de forma imperceptível ao jogador.

### 2. Implemente o personagem principal (o coelho), que deve se movimentar de forma articulada:
-[X] a. O coelho deve correr da esquerda para a direita, movimentando as patas dianteiras e traseiras e as orelhas de forma alternada, simulando o andar característico do animal;

-[X] b. O personagem deve ser construído a partir de primitivas (quadriláteros, círculos e triângulos), modeladas inicialmente em seus próprios sistemas de coordenadas,
centrados na origem, e organizadas de forma hierárquica (corpo como nó pai das patas, orelhas e cauda);

-[X] c. Ao acionar uma tecla do teclado, o coelho deve saltar, sendo capaz de transpor cercas e buracos presentes no caminho.

### 3. Implemente o aparecimento de elementos de bonificação (vegetais, como cenouras, alfaces e rabanetes) que devem ser capturados pelo coelho:
-[X] a. A cada vez que a tela reinicia o loop, ~~os vegetais devem ser adicionados aleatoriamente à paisagem~~, ao alcance do coelho, ~~aparecendo à medida que o personagem avança~~;

-[X] b. Quando o coelho colide com um vegetal, este deve desaparecer da tela; (Corrigir colisão antecipada/incorreta)

-[ ] c. Crie um enredo que dê utilidade aos vegetais capturados: eles podem conceder um impulso extra de velocidade (para distanciar-se da raposa), um salto mais alto, ou vidas extras. Use a sua criatividade!

### 4. Implemente uma raposa que persegue o coelho pelo chão:
-[ ] a. A raposa deve entrar na tela periodicamente, correndo no chão em direção ao coelho a uma velocidade próxima à do jogador, obrigando-o a colher vegetais que concedam impulso ou a saltar obstáculos para escapar;

-[ ] b. Caso a raposa alcance o coelho (colisão), o jogador perde uma vida, um poder adquirido ou o jogo é encerrado; crie um enredo que justifique essa perda.

## Questões bônus
### 1. Implemente elementos para tornar a tela de fundo um pouco mais dinâmica. Exemplos:
-[ ] a. Nuvens se movimentando;

-[ ] b. Borboletas ou abelhas sobrevoando os canteiros;

-[ ] c. Cor do céu se modificando em função do tempo de jogo (amanhecer/entardecer).

### 2. Implemente a capacidade do coelho de se abaixar e entrar em uma toca (buraco no cenário), ficando temporariamente escondido e a salvo da raposa;

### 3. Implemente o surgimento (também aleatório) de uma ave de rapina que ataca a partir do ar, mergulhando em direção ao coelho por meio de uma trajetória parabólica, adicionando uma segunda ameaça simultânea à perseguição da raposa;
### 4. Implemente a capacidade de o coelho correr (aumentando temporariamente sua velocidade) ao pressionar uma tecla específica, com um limite de fôlego que se esgota e precisa ser recuperado.
