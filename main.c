#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



typedef enum {
    CLASSE_GUERREIRO,
    CLASSE_MAGO,
    CLASSE_ARQUEIRO,
    CLASSE_GUARDIAO,
    CLASSE_ASSASSINO
} ClassePersonagem;

typedef enum {
    CASA_INICIO,
    CASA_VAZIA,
    CASA_COMBATE,
    CASA_TESOURO,
    CASA_CURA,
    CASA_ARMADILHA,
    CASA_MISSAO,
    CASA_CHEFE
} TipoCasa;

typedef enum {
    ACAO_ATACAR = 1,
    ACAO_DEFENDER,
    ACAO_ESPECIAL,
    ACAO_FUGIR
} AcaoCombate;

typedef struct {
    char nome[30];
    ClassePersonagem classe;
    int vida;
    int vidaMax;
    int forca;
    int defesa;
    int agilidade;
    int inteligencia;
    int xp;
    int nivel;
    int especialUsado;
    char arma[40];
    char armadura[40];
} Personagem;

typedef struct {
    char nome[40];
    char descricao[120];
    int vida;
    int vidaMax;
    int ataque;
    int defesa;
    int nivel;
    int xpRecompensa;
} Monstro;

typedef struct {
    TipoCasa tipo;
} Casa;

typedef struct {
    char nome[70];
    char missao[150];
    Casa tabuleiro[12];
    int tamanho;
} Fase;


int lerInteiro(const char *mensagem, int minimo, int maximo) {
    char linha[100];
    char *fim;
    long valor;

    while (1) {
        printf("%s", mensagem);

        if (fgets(linha, sizeof(linha), stdin) == NULL) {
            printf("\nEntrada encerrada. Encerrando o jogo.\n");
            exit(0);
        }

        valor = strtol(linha, &fim, 10);

        while (*fim == ' ' || *fim == '\t' || *fim == '\n' || *fim == '\r') {
            fim++;
        }

        if (fim == linha || *fim != '\0' || valor < minimo || valor > maximo) {
            printf("Entrada invalida. Digite um numero entre %d e %d.\n",
                   minimo, maximo);
        } else {
            return (int)valor;
        }
    }
}

int rolarDado(int lados) {
    return rand() % lados + 1;
}

int maior(int a, int b) {
    return (a > b) ? a : b;
}

void pausar(void) {
    char linha[8];
    printf("\nPressione ENTER para continuar...");
    fgets(linha, sizeof(linha), stdin);
}

const char *nomeClasse(ClassePersonagem classe) {
    switch (classe) {
        case CLASSE_GUERREIRO: return "Guerreiro";
        case CLASSE_MAGO: return "Mago";
        case CLASSE_ARQUEIRO: return "Arqueiro";
        case CLASSE_GUARDIAO: return "Guardiao";
        case CLASSE_ASSASSINO: return "Assassino";
        default: return "Desconhecida";
    }
}


void inicializarPersonagens(Personagem personagens[]) {
    Personagem modelos[5] = {
        {"Jason", CLASSE_GUERREIRO, 130, 130, 18, 13, 8, 7, 0, 1, 0,
         "Espada ancestral", "Armadura de placas"},
        {"Lucia", CLASSE_MAGO, 95, 95, 8, 7, 12, 22, 0, 1, 0,
         "Cajado elemental", "Manto arcano"},
        {"Carl", CLASSE_ARQUEIRO, 105, 105, 14, 9, 21, 10, 0, 1, 0,
         "Arco longo", "Couro reforcado"},
        {"Michael", CLASSE_GUARDIAO, 145, 145, 12, 20, 6, 12, 0, 1, 0,
         "Martelo de guerra", "Armadura real"},
        {"Franklin", CLASSE_ASSASSINO, 100, 100, 17, 8, 22, 9, 0, 1, 0,
         "Adagas duplas", "Manto das sombras"}
    };

    int i;
    for (i = 0; i < 5; i++) {
        personagens[i] = modelos[i];
    }
}

void mostrarPersonagem(const Personagem *p) {
    printf("\n====================================\n");
    printf("%s - %s | Nivel %d\n", p->nome, nomeClasse(p->classe), p->nivel);
    printf("Vida: %d/%d | XP: %d\n", p->vida, p->vidaMax, p->xp);
    printf("Forca: %d | Defesa: %d | Agilidade: %d | Inteligencia: %d\n",
           p->forca, p->defesa, p->agilidade, p->inteligencia);
    printf("Arma: %s\nArmadura: %s\n", p->arma, p->armadura);
    printf("====================================\n");
}

int escolherPersonagem(Personagem personagens[]) {
    int i;

    printf("\n========== ESCOLHA SEU HEROI ==========\n");
    for (i = 0; i < 5; i++) {
        printf("%d - %s (%s) | Vida: %d | Forca: %d | Defesa: %d\n",
               i + 1, personagens[i].nome, nomeClasse(personagens[i].classe),
               personagens[i].vida, personagens[i].forca, personagens[i].defesa);
    }

    return lerInteiro("Opcao: ", 1, 5) - 1;
}

void receberDano(Personagem *p, int dano) {
    p->vida -= dano;
    if (p->vida < 0) p->vida = 0;
}

void curar(Personagem *p, int quantidade) {
    p->vida += quantidade;
    if (p->vida > p->vidaMax) p->vida = p->vidaMax;
}

void ganharXP(Personagem *p, int quantidade) {
    p->xp += quantidade;
    printf("\nVoce recebeu %d pontos de experiencia!\n", quantidade);

    while (p->xp >= p->nivel * 100) {
        p->xp -= p->nivel * 100;
        p->nivel++;
        p->vidaMax += 12;
        p->vida = p->vidaMax;
        p->forca += 2;
        p->defesa += 1;
        p->agilidade += 1;
        p->inteligencia += 1;

        printf("\n*** SUBIU DE NIVEL! Agora voce esta no nivel %d! ***\n", p->nivel);
        printf("Seus atributos melhoraram e sua vida foi restaurada.\n");
    }
}

void aplicarBonusAleatorio(Personagem *p) {
    int bonus = rolarDado(5);

    printf("\nBonus de atributo recebido: ");
    switch (bonus) {
        case 1:
            p->forca += 2;
            printf("+2 de forca!\n");
            break;
        case 2:
            p->defesa += 2;
            printf("+2 de defesa!\n");
            break;
        case 3:
            p->agilidade += 2;
            printf("+2 de agilidade!\n");
            break;
        case 4:
            p->inteligencia += 2;
            printf("+2 de inteligencia!\n");
            break;
        default:
            p->vidaMax += 10;
            curar(p, 10);
            printf("+10 de vida maxima!\n");
            break;
    }
}



void inicializarMonstrosFase1(Monstro m[]) {
    Monstro dados[3] = {
        {"Golem Gigante", "Uma criatura de pedra muito resistente.", 115, 115, 17, 8, 2, 65},
        {"Cobra de Fogo", "Uma serpente veloz que domina as chamas.", 90, 90, 19, 5, 2, 60},
        {"Homem-Cavalo", "Um guerreiro mitologico de grande forca.", 125, 125, 18, 9, 3, 80}
    };
    memcpy(m, dados, sizeof(dados));
}

void inicializarMonstrosFase2(Monstro m[]) {
    Monstro dados[3] = {
        {"Medusa", "Seu olhar e perigoso e sua magia e poderosa.", 145, 145, 22, 10, 4, 90},
        {"Harpia Gigante", "Uma criatura alada que ataca rapidamente.", 120, 120, 24, 7, 4, 85},
        {"Ciclope", "Um gigante de um olho e forca assustadora.", 175, 175, 25, 12, 5, 110}
    };
    memcpy(m, dados, sizeof(dados));
}

void inicializarMonstrosFase3(Monstro m[]) {
    Monstro dados[3] = {
        {"Troll das Sombras", "Um troll resistente que protege a fortaleza.", 185, 185, 27, 12, 6, 110},
        {"Elfo Corrompido", "Um antigo guardiao dominado pela escuridao.", 170, 170, 29, 10, 6, 115},
        {"Gigante Feiticeiro", "O chefe final que controla a Fortaleza Sombria.", 140, 260, 32, 15, 8, 180}
    };
    memcpy(m, dados, sizeof(dados));
}

void inicializarFase(Fase *fase, int numero) {
    TipoCasa mapa[12] = {
        CASA_INICIO, CASA_VAZIA, CASA_COMBATE, CASA_TESOURO,
        CASA_COMBATE, CASA_CURA, CASA_ARMADILHA, CASA_COMBATE,
        CASA_VAZIA, CASA_TESOURO, CASA_MISSAO, CASA_CHEFE
    };

    int i;
    fase->tamanho = 12;

    if (numero == 1) {
        strcpy(fase->nome, "FASE 1 - VALE DAS CRIATURAS");
        strcpy(fase->missao, "Derrote os tres monstros e liberte o Vale das Criaturas.");
    } else if (numero == 2) {
        strcpy(fase->nome, "FASE 2 - REINO MITOLOGICO");
        strcpy(fase->missao, "Derrote os tres guardioes e conquiste o Reino Mitologico.");
    } else {
        strcpy(fase->nome, "FASE 3 - FORTALEZA SOMBRIA");
        strcpy(fase->missao, "Derrote os protetores e vença o Gigante Feiticeiro.");
    }

    for (i = 0; i < fase->tamanho; i++) {
        fase->tabuleiro[i].tipo = mapa[i];
    }
}

void mostrarTabuleiro(const Fase *fase, int posicao) {
    int i;
    printf("\n============== TABULEIRO ==============\n");
    for (i = 0; i < fase->tamanho; i++) {
        if (i == posicao) printf("[ P ] ");
        else if (fase->tabuleiro[i].tipo == CASA_COMBATE) printf("[ L ] ");
        else if (fase->tabuleiro[i].tipo == CASA_TESOURO) printf("[ $ ] ");
        else if (fase->tabuleiro[i].tipo == CASA_CURA) printf("[ + ] ");
        else if (fase->tabuleiro[i].tipo == CASA_ARMADILHA) printf("[ ! ] ");
        else if (fase->tabuleiro[i].tipo == CASA_MISSAO) printf("[ M ] ");
        else if (fase->tabuleiro[i].tipo == CASA_CHEFE) printf("[ C ] ");
        else if (fase->tabuleiro[i].tipo == CASA_INICIO) printf("[ I ] ");
        else printf("[ . ] ");
    }
    printf("\nP=Jogador L=Luta $=Tesouro +=Cura !=Armadilha M=Missao C=Chefe\n");
    printf("=======================================\n");
}



int habilidadeEspecial(Personagem *p, Monstro *m, int dado) {
    int dano = 0;

    switch (p->classe) {
        case CLASSE_GUERREIRO:
            dano = dado + p->forca * 2 + 15 - m->defesa;
            printf("%s usa Golpe Devastador!\n", p->nome);
            break;
        case CLASSE_MAGO:
            dano = dado + p->inteligencia * 2 + 12 - m->defesa;
            printf("%s conjura Explosao Arcana!\n", p->nome);
            break;
        case CLASSE_ARQUEIRO:
            dano = dado + p->agilidade + p->forca + 18 - m->defesa;
            printf("%s usa Flecha Precisa!\n", p->nome);
            break;
        case CLASSE_GUARDIAO:
            dano = dado + p->forca + 8 - m->defesa;
            curar(p, 18);
            printf("%s usa Escudo Sagrado e recupera 18 de vida!\n", p->nome);
            break;
        case CLASSE_ASSASSINO:
            dano = dado + p->agilidade * 2 + 10 - m->defesa;
            printf("%s usa Ataque das Sombras!\n", p->nome);
            break;
        default:
            dano = dado + p->forca - m->defesa;
            break;
    }

    return maior(1, dano);
}

int batalha(Personagem *jogador, Monstro *inimigo) {
    int opcao, dado, dano, defesaAtiva, ataque;

    jogador->especialUsado = 0;
    defesaAtiva = 0;

    printf("\n====================================\n");
    printf("             BATALHA!\n");
    printf("====================================\n");
    printf("%s - Nivel %d\n%s\n", inimigo->nome, inimigo->nivel, inimigo->descricao);

    while (jogador->vida > 0 && inimigo->vida > 0) {
        printf("\n%s: %d/%d de vida\n", jogador->nome, jogador->vida, jogador->vidaMax);
        printf("%s: %d/%d de vida\n", inimigo->nome, inimigo->vida, inimigo->vidaMax);
        printf("\n1 - Atacar\n2 - Defender\n3 - Habilidade especial\n4 - Fugir\n");

        opcao = lerInteiro("Escolha sua acao: ", 1, 4);

        if (opcao == ACAO_FUGIR) {
            dado = rolarDado(20);
            printf("\nTeste de fuga: dado %d + agilidade %d.\n", dado, jogador->agilidade);
            if (dado + jogador->agilidade >= 22) {
                printf("Voce conseguiu fugir! Nao recebera recompensa.\n");
                return 2;
            }
            printf("A fuga falhou! O monstro aproveita para atacar.\n");
        } else if (opcao == ACAO_DEFENDER) {
            defesaAtiva = 1;
            printf("\nVoce se preparou para reduzir o proximo dano.\n");
        } else if (opcao == ACAO_ATACAR) {
            dado = rolarDado(20);
            ataque = dado + jogador->forca + jogador->agilidade / 4;
            dano = maior(1, ataque - inimigo->defesa);
            inimigo->vida -= dano;
            if (inimigo->vida < 0) inimigo->vida = 0;
            printf("\nDado: %d | Ataque total: %d | Dano causado: %d\n", dado, ataque, dano);
        } else if (opcao == ACAO_ESPECIAL) {
            if (jogador->especialUsado) {
                printf("\nVoce ja utilizou sua habilidade especial nesta batalha.\n");
                continue;
            }
            dado = rolarDado(20);
            dano = habilidadeEspecial(jogador, inimigo, dado);
            inimigo->vida -= dano;
            if (inimigo->vida < 0) inimigo->vida = 0;
            jogador->especialUsado = 1;
            printf("Dado: %d | Dano especial: %d\n", dado, dano);
        }

        if (inimigo->vida <= 0) break;

        dado = rolarDado(20);
        dano = maior(1, dado + inimigo->ataque - jogador->defesa);

        if (defesaAtiva) {
            dano = (dano + 1) / 2;
            defesaAtiva = 0;
        }

        receberDano(jogador, dano);
        printf("\n%s atacou! Dano recebido: %d\n", inimigo->nome, dano);
    }

    if (jogador->vida <= 0) {
        printf("\nVoce foi derrotado!\n");
        return 0;
    }

    printf("\nParabens! Voce derrotou %s!\n", inimigo->nome);
    ganharXP(jogador, inimigo->xpRecompensa);
    aplicarBonusAleatorio(jogador);
    return 1;
}



int executarEvento(TipoCasa tipo, Personagem *jogador, Monstro monstros[],
                   int quantidadeMonstros, int *derrotados, int fase) {
    int quantidade;
    int indice;

    switch (tipo) {
        case CASA_VAZIA:
            printf("\nCasa vazia. O caminho esta tranquilo.\n");
            break;

        case CASA_TESOURO:
            quantidade = 15 + rolarDado(26);
            curar(jogador, quantidade);
            printf("\nVoce encontrou um tesouro com uma pocao! Recuperou ate %d de vida.\n",
                   quantidade);
            break;

        case CASA_CURA:
            quantidade = 20 + rolarDado(21);
            curar(jogador, quantidade);
            printf("\nUma fonte magica restaurou sua vida em ate %d pontos.\n", quantidade);
            break;

        case CASA_ARMADILHA:
            quantidade = 8 + rolarDado(18);
            receberDano(jogador, quantidade);
            printf("\nArmadilha! Voce perdeu %d pontos de vida.\n", quantidade);
            break;

        case CASA_MISSAO:
            printf("\nMISSAO: %s\n", fase == 1 ?
                   "Encontre forcas para libertar o Vale." :
                   fase == 2 ? "Conquiste o Reino das Criaturas Mitologicas." :
                   "Chegue ao chefe e salve o mundo.");
            break;

        case CASA_COMBATE:
        case CASA_CHEFE:
            if (*derrotados >= quantidadeMonstros) {
                printf("\nNao ha mais monstros nesta fase.\n");
                break;
            }

            indice = *derrotados;
            if (tipo == CASA_CHEFE && indice < quantidadeMonstros - 1) {
                printf("\nO chefe esta protegido! Derrote os outros inimigos primeiro.\n");
                break;
            }

            if (tipo == CASA_COMBATE && fase == 3 && indice == quantidadeMonstros - 1) {
                printf("\nO Gigante Feiticeiro so pode ser enfrentado na casa do chefe!\n");
                break;
            }

            printf("\nUm inimigo apareceu!\n");
            {
                int resultado = batalha(jogador, &monstros[indice]);
                if (resultado == 0) return -1;
                if (resultado == 2) return 0;
            }

            (*derrotados)++;
            return 1;

        case CASA_INICIO:
        default:
            printf("\nVoce esta no inicio da jornada.\n");
            break;
    }

    printf("\nVida atual: %d/%d\n", jogador->vida, jogador->vidaMax);
    return 0;
}

int jogarFase(Personagem *jogador, int numero, Monstro monstros[], int quantidadeMonstros) {
    Fase fase;
    int posicao = 0;
    int derrotados = 0;
    int resultado;
    int dado;

    inicializarFase(&fase, numero);

    printf("\n\n========================================\n");
    printf("%s\n", fase.nome);
    printf("Missao: %s\n", fase.missao);
    printf("========================================\n");

    if (numero > 1) {
        curar(jogador, 25);
        printf("Ao iniciar a nova fase, voce recuperou ate 25 pontos de vida.\n");
    }

    while (derrotados < quantidadeMonstros) {
        if (jogador->vida <= 0) return 0;

        mostrarTabuleiro(&fase, posicao);
        mostrarPersonagem(jogador);
        printf("\nMonstros derrotados: %d/%d\n", derrotados, quantidadeMonstros);
        pausar();

        dado = rolarDado(6);
        printf("\nVoce tirou %d no dado de seis lados!\n", dado);
        posicao += dado;
        if (posicao >= fase.tamanho) posicao = fase.tamanho - 1;

        printf("Voce avancou para a casa %d.\n", posicao + 1);
        resultado = executarEvento(fase.tabuleiro[posicao].tipo, jogador,
                                   monstros, quantidadeMonstros, &derrotados, numero);

        if (resultado == -1 || jogador->vida <= 0) return 0;

        if (posicao == fase.tamanho - 1 && derrotados < quantidadeMonstros) {
            printf("\nVoce chegou ao fim do caminho, mas ainda ha inimigos.\n");
            printf("Retornando ao inicio para continuar a busca.\n");
            posicao = 0;
        }
    }

    printf("\n========================================\n");
    printf("FASE %d CONCLUIDA!\n", numero);
    printf("Missao cumprida: %s\n", fase.missao);
    printf("========================================\n");
    return 1;
}


int main(void) {
    Personagem personagens[5];
    Personagem jogador;
    Monstro monstrosFase1[3];
    Monstro monstrosFase2[3];
    Monstro monstrosFase3[3];
    int escolha;

    srand((unsigned int)time(NULL));

    inicializarPersonagens(personagens);
    inicializarMonstrosFase1(monstrosFase1);
    inicializarMonstrosFase2(monstrosFase2);
    inicializarMonstrosFase3(monstrosFase3);

    printf("========================================\n");
    printf("          FIVE OKARINA OF TIME\n");
    printf("       Uma aventura de RPG em C\n");
    printf("========================================\n");
    printf("\nO mundo esta ameacado por criaturas antigas.\n");
    printf("Escolha um heroi, atravesse tres regioes e salve o mundo!\n");

    escolha = escolherPersonagem(personagens);
    jogador = personagens[escolha];

    printf("\nVoce escolheu %s, o(a) %s!\n", jogador.nome, nomeClasse(jogador.classe));
    mostrarPersonagem(&jogador);
    pausar();

    if (!jogarFase(&jogador, 1, monstrosFase1, 3)) {
        printf("\nGAME OVER! Sua jornada terminou na Fase 1.\n");
        return 0;
    }

    if (!jogarFase(&jogador, 2, monstrosFase2, 3)) {
        printf("\nGAME OVER! Sua jornada terminou na Fase 2.\n");
        return 0;
    }

    if (!jogarFase(&jogador, 3, monstrosFase3, 3)) {
        printf("\nGAME OVER! Sua jornada terminou na Fase 3.\n");
        return 0;
    }

    printf("\n========================================\n");
    printf("               VITORIA!\n");
    printf("        VOCE SALVOU O MUNDO!\n");
    printf("========================================\n");
    mostrarPersonagem(&jogador);
    printf("\nObrigado por jogar FIVE OKARINA OF TIME!\n");

    return 0;
}
