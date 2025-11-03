// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
// Nível Novato
// ============================================================================

#include <stdio.h>
#include <string.h>

#define NUM_TERRITORIOS 5

typedef struct
{
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

void limpar()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main()
{
    Territorio territorios[NUM_TERRITORIOS];

    // Inserindo territórios
    for (int i = 0; i < NUM_TERRITORIOS; i++)
    {
        printf("--- Insira os detalhes do territorio %d ---\n", i + 1);

        printf("Nome do territorio: ");
        fgets(territorios[i].nome, sizeof(territorios[i].nome), stdin);

        printf("Cor do exercito (ex: Azul, Verde): ");
        fgets(territorios[i].cor, sizeof(territorios[i].cor), stdin);

        printf("Numero de tropas: ");
        scanf("%d", &territorios[i].tropas);

        limpar(); // limpa buffer

        printf("\n");
    }

    // Exibindo territórios
    for (int i = 0; i < NUM_TERRITORIOS; i++)
    {
        printf("--- Territorio %d ---\n", i + 1);
        printf("Nome: %s", territorios[i].nome);
        printf("Cor: %s", territorios[i].cor);
        printf("Tropas: %d\n\n", territorios[i].tropas);
    }

    return 0;
}