#Projeto de Luiz Henrique Souza Da Silva do Curso de BSI-UFRN 2026.2
#Matricula:20260065013
#Link do video:
https://photos.app.goo.gl/KKQ6MUnfGW66ZYRT8
# FIVE OKARINA OF TIME

Jogo de tabuleiro RPG em modo texto, desenvolvido em linguagem C99.

## História

O mundo está ameaçado por criaturas antigas. O jogador escolhe um herói e percorre três regiões: o Vale das Criaturas, o Reino Mitológico e a Fortaleza Sombria. Em cada região, precisa derrotar três monstros para liberar a próxima fase. Na última fase, o desafio final é o Gigante Feiticeiro.

## Personagens

| Personagem | Classe | Característica | Arma | Armadura |
|---|---|---|---|---|
| Jason | Guerreiro | Força e resistência | Espada ancestral | Armadura de placas |
| Lucia | Mago | Inteligência e magia | Cajado elemental | Manto arcano |
| Carl | Arqueiro | Agilidade e precisão | Arco longo | Couro reforçado |
| Michael | Guardião | Vida e defesa elevadas | Martelo de guerra | Armadura real |
| Franklin | Assassino | Agilidade e ataques rápidos | Adagas duplas | Manto das sombras |

Cada classe possui uma habilidade especial diferente, que pode ser usada uma vez por batalha.

## Fases e monstros

### Fase 1 — Vale das Criaturas
Missão: derrotar os três monstros e libertar o Vale.
- Golem Gigante
- Cobra de Fogo
- Homem-Cavalo

### Fase 2 — Reino Mitológico
Missão: derrotar os guardiões e conquistar o Reino.
- Medusa
- Harpia Gigante
- Ciclope

### Fase 3 — Fortaleza Sombria
Missão: derrotar os protetores e vencer o chefe final.
- Troll das Sombras
- Elfo Corrompido
- Gigante Feiticeiro (chefe final)

A dificuldade aumenta a cada fase por meio dos atributos dos monstros.

## Como jogar

1. Escolha um personagem no menu inicial.
2. Pressione ENTER para lançar o dado de seis lados.
3. O personagem avança pelo tabuleiro e executa o evento da casa.
4. Derrote os monstros para concluir a missão da fase.
5. Na terceira fase, o chefe só pode ser enfrentado depois dos outros dois monstros.

### Casas do tabuleiro

- Início: ponto de partida.
- Vazia: nada acontece.
- Combate: inicia uma batalha.
- Tesouro: recupera vida.
- Cura: recupera vida.
- Armadilha: reduz a vida.
- Missão: apresenta o objetivo da fase.
- Chefe: local do confronto final.

### Combate

No combate, escolha:
1. Atacar: ataque normal com dado de 20 lados.
2. Defender: reduz pela metade o próximo dano recebido.
3. Habilidade especial: efeito próprio da classe, uma vez por batalha.
4. Fugir: tenta escapar usando o resultado do dado e a agilidade.

Se o jogador vencer, recebe XP e um bônus aleatório de atributo. Ao acumular XP suficiente, sobe de nível, melhora seus atributos e recupera a vida.

O jogo termina em derrota quando a vida chega a zero. A vitória acontece quando as três fases são concluídas.

## Compilação e execução

É necessário ter o compilador GCC instalado.

### Linux
```bash
gcc -std=c99 -Wall -Wextra -pedantic main.c -o jogo
./jogo
```

### Windows (MinGW)
```bash
gcc -std=c99 -Wall -Wextra -pedantic main.c -o jogo.exe
jogo.exe
```

## Estruturas e organização

- `Personagem`: guarda classe, atributos, vida, XP, nível e equipamentos.
- `Monstro`: guarda descrição, vida, ataque, defesa, nível e XP de recompensa.
- `Casa`: representa o tipo de evento de uma casa.
- `Fase`: guarda nome, missão e tabuleiro.
- `enum`: representa classes, tipos de casa e ações de combate.
- Ponteiros: funções como `receberDano`, `curar`, `ganharXP`, `aplicarBonusAleatorio` e `batalha` atualizam diretamente os dados do personagem ou monstro.

## Observações para a entrega

Preencha seus dados de identificação conforme solicitado pelo professor. Adicione o link do repositório e do vídeo de demonstração quando estiverem disponíveis. Revise e teste o jogo no computador usado para a gravação.
