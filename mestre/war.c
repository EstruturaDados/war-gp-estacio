#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define NUM_TERRITORIOS 5
#define MAX_NOME 30
#define MAX_COR 10

typedef struct
{
    char nome[MAX_NOME];   // Nome do território
    char cor[MAX_COR];     // Cor do exército que controla o território
    int tropas;            // Número de tropas no território
    char missao[MAX_NOME]; // Missão atribuída ao território
} Territorio;

// Função para alocar dinamicamente os territórios
Territorio *alocarTerritorios(int num_territorios)
{
    // Usando calloc para inicializar a memória com zeros
    Territorio *territorios = (Territorio *)calloc(num_territorios, sizeof(Territorio));

    printf("Memoria alocada com sucesso para %d territorios.\n", num_territorios);

    return territorios;
}

// Função para liberar a memória alocada
void liberarTerritorios(Territorio *territorios)
{
    if (territorios != NULL)
    {
        free(territorios);
        printf("Memoria liberada com sucesso.\n");
    }
}

// Limpa o buffer de entrada
void limparBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

// Remove a quebra de linha do final da string (de fgets)
void removerQuebraLinha(char *str)
{
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
    {
        str[len - 1] = '\0';
    }
}

// Atribuir missão aleatória
void atribuirMissao(char *missao)
{
    // Definindo missões possíveis
    // melhorar para usar dados dinâmicos
    char *missoes[] = {
        "Conquistar 3 territorios",
        "Ter 3 territorios com mais de 5",
        "Controlar Asia e America",
        "Conquistar todos os territorios",
        "Eliminar o exercito Vermelho ou Verde"};

    int totalMissoes = sizeof(missoes);
    int indice = rand() % totalMissoes;

    strcpy(missao, missoes[indice]); // atribui missao 
}

// Exibe a missão atribuída
void exibirMissao(char *missao)
{
    printf("\n=== MISSÃO ATRIBUÍDA ===\n");
    printf("Sua missão é: %s\n", missao);
    printf("=========================\n");
    printf("\n");
}

// verifica se missao esta completa
int verificarMissao(const char *missao, const Territorio *territorios)
{
    int territorios_controlados = 0;
    int territorios_com_mais_de_5 = 0;
    int controla_asia = 0;
    int controla_america = 0;
    int eliminou_vermelho = 1;
    int eliminou_verde = 1;

    char jogador_exercito[MAX_COR];
    strcpy(jogador_exercito, territorios[0].cor);

    for (int i = 0; i < NUM_TERRITORIOS; i++)
    {
        // verifica numero de territorios
        if (strcmp(territorios[i].cor, jogador_exercito) == 0)
        {
            territorios_controlados++;
            if (territorios[i].tropas > 5)
            {
                territorios_com_mais_de_5++;
            }
        }

        // verifica se Asia foi conquistada
        if (strcmp(territorios[i].nome, "Asia") == 0 && strcmp(territorios[i].cor, jogador_exercito) == 0)
        {
            controla_asia = 1;
        }

        // verifica se America foi conquistada
        if (strcmp(territorios[i].nome, "America") == 0 && strcmp(territorios[i].cor, jogador_exercito) == 0)
        {
            controla_america = 1;
        }

        // verifica se Vermelho foi eliminado
        if (strcmp(territorios[i].cor, "Vermelho") == 0)
        {
            eliminou_vermelho = 0;
        }

        // verifica se Verde foi eliminado
        if (strcmp(territorios[i].cor, "Verde") == 0)
        {
            eliminou_verde = 0;
        }
    }

    int missao_cumprida = 0;

    if (strcmp(missao, "Conquistar 3 territorios") == 0 && territorios_controlados >= 3)
    {
        missao_cumprida = 1;
    }
    else if (strcmp(missao, "Ter 3 territorios com mais de 5") == 0 && territorios_com_mais_de_5 >= 3)
    {
        missao_cumprida = 1;
    }
    else if (strcmp(missao, "Controlar Asia e America") == 0 && controla_asia && controla_america)
    {
        missao_cumprida = 1;
    }
    else if (strcmp(missao, "Conquistar todos os territorios") == 0 && territorios_controlados == NUM_TERRITORIOS)
    {
        missao_cumprida = 1;
    }
    else if (strcmp(missao, "Eliminar o exercito Vermelho ou Verde") == 0 && (eliminou_vermelho || eliminou_verde))
    {
        missao_cumprida = 1;
    }

    if (missao_cumprida)
    {
        printf("\n*** PARABENS! MISSÃO CUMPRIDA! ***\n");
        return 1;
    }

    return 0;
}

// Valida se o território tem tropas suficientes para atacar
int podeAtacar(const Territorio *territorio)
{
    return territorio->tropas > 0; // Precisa de pelo menos 1 tropa para atacar
}

// Funcao de atacar
void atacar(Territorio *atacante, Territorio *defensor)
{
    int dado_atacante = rand() % 6 + 1;
    int dado_defensor = rand() % 6 + 1;

    printf("\n>>> %s (Exercito %s) ataca %s (Exercito %s) <<<\n",
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    printf("Dado do atacante: %d\n", dado_atacante);
    printf("Dado do defensor: %d\n", dado_defensor);

    if (dado_atacante > dado_defensor)
    {
        defensor->tropas--;
        printf(">>> VITORIA DO ATACANTE! Defensor perde 1 tropa.\n");

        // Verifica conquista do território
        if (defensor->tropas <= 0)
        {
            printf("\n*** TERRITORIO CONQUISTADO! ***\n");
            printf("%s foi conquistado pelo exercito %s!\n",
                   defensor->nome, atacante->cor);

            // Transfere cor e metade das tropas
            strcpy(defensor->cor, atacante->cor);
            int tropas_transferidas = atacante->tropas / 2;

            // Garante que pelo menos 1 tropa fique em cada território
            if (tropas_transferidas < 1)
                tropas_transferidas = 1;

            defensor->tropas = tropas_transferidas;
            atacante->tropas -= tropas_transferidas;
        }
    }
    else
    {
        atacante->tropas--;
        printf(">>> VITORIA DO DEFENSOR! Atacante perde 1 tropa.\n");

        // Se o atacante ficar sem tropas (situação rara mas possível)
        if (atacante->tropas <= 0)
        {
            printf("\n*** ATAQUE FALHOU COMPLETAMENTE! ***\n");
            printf("Atacante %s perdeu todas as tropas!\n", atacante->nome);
            atacante->tropas = 1;                 // Mantém pelo menos 1 tropa para não deixar vazio
            strcpy(atacante->cor, defensor->cor); // defensor invade
        }
    }
}

// Exibe o estado atual de todos os territórios
void exibirTerritorios(const Territorio *territorios)
{
    printf("\n=== MAPA DOS TERRITORIOS ===\n");
    for (int i = 0; i < NUM_TERRITORIOS; i++)
    {
        printf("%d. %s | Exercito: %s | Tropas: %d\n",
               i + 1, territorios[i].nome, territorios[i].cor, territorios[i].tropas);
    }
    printf("============================\n\n");
}

// Valida a escolha de territórios para ataque
int validarEscolha(int atacante_idx, int defensor_idx, const Territorio *territorios)
{
    // Verifica se os índices estão no intervalo válido
    if (atacante_idx < 1 || atacante_idx > NUM_TERRITORIOS ||
        defensor_idx < 1 || defensor_idx > NUM_TERRITORIOS)
    {
        printf("Erro: Escolha invalida. Use numeros entre 1 e %d.\n", NUM_TERRITORIOS);
        return 0;
    }

    // Verifica se não está atacando a si mesmo
    if (atacante_idx == defensor_idx)
    {
        printf("Erro: Um territorio nao pode atacar a si mesmo!\n");
        return 0;
    }

    Territorio *atacante = (Territorio *)&territorios[atacante_idx - 1];
    Territorio *defensor = (Territorio *)&territorios[defensor_idx - 1];

    // Verifica se o atacante tem tropas suficientes
    if (!podeAtacar(atacante))
    {
        printf("Erro: %s precisa de pelo menos 1 tropas para atacar!\n", atacante->nome);
        return 0;
    }

    // Verifica se não está atacando território da mesma cor
    if (strcmp(atacante->cor, defensor->cor) == 0)
    {
        printf("Erro: Nao pode atacar territorio do mesmo exercito!\n");
        return 0;
    }

    return 1;
}

// Exibe menu de opções
void exibirMenu()
{
    printf("=== JOGO DE TERRITORIOS ===\n");
    printf("1. Iniciar nova partida\n");
    printf("2. Verificar Missao\n");
    printf("0. Sair\n");
    printf("==========================\n");
}

// Inicia uma nova partida
void novaPartida(Territorio *territorios)
{
    int atacante_idx, defensor_idx;
    int opcao; // Declaração da variável opcao
    int rodada = 1;

    printf("\n=== INICIANDO PARTIDA ===\n");

    exibirTerritorios(territorios);
    exibirMissao(territorios[0].missao);

    while (1)
    {
        exibirMenu();
        printf("\n");

        if (scanf("%d", &opcao) != 1)
        {
            printf("Erro: Entrada invalida!\n");
            limparBuffer();
            continue;
        }
        if (opcao == 2)
        {
            exibirMissao(territorios[0].missao);
            continue;
        }
        else if (opcao == 0)
        {
            printf("\nPartida encerrada pelo jogador.\n");
            break;
        }

        printf("\n--- RODADA %d ---\n", rodada);
        printf("Escolha o territorio atacante (1-%d) ou 0 para sair: ", NUM_TERRITORIOS);

        if (scanf("%d", &atacante_idx) != 1)
        {
            printf("Erro: Entrada invalida!\n");
            limparBuffer();
            continue;
        }

        if (atacante_idx == 0)
        {
            printf("\nPartida encerrada pelo jogador.\n");
            break;
        }

        printf("Escolha o territorio defensor (1-%d): ", NUM_TERRITORIOS);
        if (scanf("%d", &defensor_idx) != 1)
        {
            printf("Erro: Entrada invalida!\n");
            limparBuffer();
            continue;
        }

        // Valida a escolha
        if (!validarEscolha(atacante_idx, defensor_idx, territorios))
        {
            continue;
        }

        // Realiza o ataque
        atacar(&territorios[atacante_idx - 1], &territorios[defensor_idx - 1]);

        // Exibe o estado dos territórios
        exibirTerritorios(territorios);

        // verifica se missao foi cumprida
        if (verificarMissao(territorios[0].missao, territorios))
        {
            break;
        }

        rodada++;
    }

    limparBuffer();
}

int main()
{
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    Territorio *territorios = NULL;

    // Aloca memória dinamicamente para os territórios
    territorios = alocarTerritorios(NUM_TERRITORIOS);

    printf("=== CONFIGURACAO DO JOGO ===\n\n");

    // Inserindo territórios
    for (int i = 0; i < NUM_TERRITORIOS; i++)
    {
        printf("--- Territorio %d ---\n", i + 1);

        printf("Nome do territorio: ");
        fgets(territorios[i].nome, sizeof(territorios[i].nome), stdin);
        removerQuebraLinha(territorios[i].nome);

        printf("Cor do exercito (ex: Azul, Verde): ");
        fgets(territorios[i].cor, sizeof(territorios[i].cor), stdin);
        removerQuebraLinha(territorios[i].cor);

        printf("Numero de tropas: ");
        while (scanf("%d", &territorios[i].tropas) != 1 || territorios[i].tropas < 1)
        {
            printf("Erro: Digite um numero valido (minimo 1): ");
            limparBuffer();
        }

        // Atribuindo uma missão aleatória ao território
        atribuirMissao(territorios[i].missao);

        limparBuffer();
        printf("\n");
    }

    novaPartida(territorios);

    liberarTerritorios(territorios);

    printf("\nObrigado por jogar!\n");

    return 0;
}