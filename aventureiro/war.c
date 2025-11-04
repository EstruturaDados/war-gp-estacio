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
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

// Função para alocar dinamicamente os territórios
Territorio* alocarTerritorios(int num_territorios)
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

// Valida se o território tem tropas suficientes para atacar
int podeAtacar(const Territorio *territorio)
{
    return territorio->tropas > 0; // Precisa de pelo menos 1 tropa para atacar
}

// Verifica se todos os territórios pertencem ao mesmo jogador
int verificarVitoria(const Territorio *territorios)
{
    for (int i = 1; i < NUM_TERRITORIOS; i++)
    {
        if (strcmp(territorios[i].cor, territorios[0].cor) != 0)
        {
            return 0; // Ainda há territórios de cores diferentes
        }
    }
    return 1; // Todos os territórios são da mesma cor
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

// Função para simular um ataque entre dois territórios
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
            
            // Garante que pelo menos 1 tropa seja transferida
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

        if (atacante->tropas <= 0)
        {
            printf("\n*** ATAQUE FALHOU COMPLETAMENTE! ***\n");
            printf("Atacante %s perdeu todas as tropas!\n", atacante->nome);
            atacante->tropas = 1; // Mantém pelo menos 1 tropa para não deixar vazio
            strcpy(atacante->cor, defensor->cor); // defensor invade
        }
    }
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

void novaPartida(Territorio *territorios)
{
    int atacante_idx, defensor_idx;
    int rodada = 1;

    printf("\n=== INICIANDO PARTIDA ===\n");
    exibirTerritorios(territorios);

    while (1)
    {
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

        // Verifica se alguém venceu
        if (verificarVitoria(territorios))
        {
            printf("\n*** VITORIA! ***\n");
            printf("O exercito %s conquistou todos os territorios!\n", territorios[0].cor);
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

    // insere 5 territórios pré-definidos para testes
    // for (int i = 0; i < NUM_TERRITORIOS; i++)
    // {
    //     snprintf(territorios[i].nome, sizeof(territorios[i].nome), "Territorio %d", i + 1);
    //     snprintf(territorios[i].cor, sizeof(territorios[i].cor), "Cor%d", (i % 2) + 1);
    //     territorios[i].tropas = 3 + (rand() % 5); // Entre 3 e 7 tropas
    // }

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

        limparBuffer();
        printf("\n");
    }

    novaPartida(territorios);

    liberarTerritorios(territorios);
    
    printf("\nObrigado por jogar!\n");
    return 0;
}
