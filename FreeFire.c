#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// --- Constantes e Definições Globais ---
#define MAX_ITENS 10
#define TAM_NOME 50
#define TAM_TIPO 30

// --- Estruturas e Enumerações ---

// Enum CriterioOrdenacao: Define os critérios para ordenar o inventário.
typedef enum {
    POR_NOME = 1,
    POR_TIPO,
    POR_PRIORIDADE
} CriterioOrdenacao;

// Struct Item: Representa um componente na mochila.
typedef struct {
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    int quantidade;
    int prioridade; // De 1 (baixa) a 5 (alta)
} Item;

// --- Variáveis Globais de Controle ---

Item mochila[MAX_ITENS]; // Vetor estático para o inventário
int numItens = 0; // Quantidade atual de itens na mochila
long comparacoes = 0; // Contador de comparações para análise de desempenho
bool ordenadaPorNome = false; // Flag para controle da busca binária

// --- Protótipos das Funções ---

void limparTela();
void exibirMenu();
void inserirItem();
void removerItem();
void listarItens();
void menuDeOrdenacao();
void insertionSort(CriterioOrdenacao criterio);
void buscaBinariaPorNome();

// --- Implementação das Funções ---

// Simula a limpeza da tela (simples para compatibilidade em ambientes diversos)
void limparTela() {
    printf("\n\n----------------------------------------\n\n");
    // Para ambientes Unix/Linux/macOS, pode-se usar system("clear");
    // Para Windows, pode-se usar system("cls");
}

// Apresenta o menu principal e o status da ordenação
void exibirMenu() {
    printf("==================================================\n");
    printf("🎒 Desafio Código da Ilha – Edição Free Fire\n");
    printf("==================================================\n");
    printf("Status da Mochila: %d/%d itens | Ordenada por Nome: %s\n", 
           numItens, MAX_ITENS, ordenadaPorNome ? "SIM ✅" : "NÃO ❌");
    printf("--------------------------------------------------\n");
    printf("1. Adicionar um item\n");
    printf("2. Remover um item (por nome)\n");
    printf("3. Listar todos os itens\n");
    printf("4. Ordenar os itens por critério\n");
    printf("5. Realizar busca binária por nome\n");
    printf("0. Sair\n");
    printf("--------------------------------------------------\n");
    printf("Escolha uma opção: ");
}

// Adiciona um novo item à mochila
void inserirItem() {
    if (numItens >= MAX_ITENS) {
        printf("\n🚫 Mochila cheia! Não é possível adicionar mais itens.\n");
        return;
    }

    Item novoItem;
    printf("\n--- Adicionar Novo Item ---\n");
    
    printf("Nome do Item (max %d): ", TAM_NOME - 1);
    scanf(" %49[^\n]", novoItem.nome); 

    printf("Tipo do Item (ex: Arma, Curativo, Munição): ");
    scanf(" %29[^\n]", novoItem.tipo);

    printf("Quantidade: ");
    while (scanf("%d", &novoItem.quantidade) != 1 || novoItem.quantidade <= 0) {
        printf("Valor inválido. Digite uma quantidade positiva: ");
        // Limpa o buffer de entrada
        while (getchar() != '\n'); 
    }

    printf("Prioridade (1 - Baixa, 5 - Alta): ");
    while (scanf("%d", &novoItem.prioridade) != 1 || novoItem.prioridade < 1 || novoItem.prioridade > 5) {
        printf("Prioridade inválida. Digite um valor entre 1 e 5: ");
        while (getchar() != '\n');
    }
    
    // Insere o item na próxima posição livre e atualiza o contador
    mochila[numItens] = novoItem;
    numItens++;
    
    // A inserção de um novo item desfaz a ordenação por nome
    ordenadaPorNome = false; 

    printf("\n✅ Item '%s' adicionado com sucesso! (Prioridade %d)\n", novoItem.nome, novoItem.prioridade);
}

// Remove um item da mochila pelo nome
void removerItem() {
    if (numItens == 0) {
        printf("\n🚫 A mochila está vazia. Nada a remover.\n");
        return;
    }

    char nomeBusca[TAM_NOME];
    printf("\n--- Remover Item ---\n");
    printf("Digite o nome do item a ser removido: ");
    scanf(" %49[^\n]", nomeBusca);

    int i, encontrado = -1;
    // Busca sequencial pelo nome
    for (i = 0; i < numItens; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado != -1) {
        // Move os itens subsequentes uma posição para trás para preencher a lacuna
        for (i = encontrado; i < numItens - 1; i++) {
            mochila[i] = mochila[i + 1];
        }
        numItens--;
        ordenadaPorNome = false; // A remoção altera a ordem
        printf("\n✅ Item '%s' removido com sucesso!\n", nomeBusca);
    } else {
        printf("\n❌ Item '%s' não encontrado na mochila.\n", nomeBusca);
    }
}

// Lista todos os itens da mochila em formato de tabela
void listarItens() {
    printf("\n--- Itens na Mochila (%d/%d) ---\n", numItens, MAX_ITENS);
    if (numItens == 0) {
        printf("A mochila está vazia.\n");
        return;
    }

    // Cabeçalho da tabela
    printf("+--------------------------------------------------+\n");
    printf("| %-20s | %-15s | %-5s | %-10s |\n", "NOME", "TIPO", "QTD", "PRIORIDADE");
    printf("+--------------------------------------------------+\n");

    // Corpo da tabela
    for (int i = 0; i < numItens; i++) {
        printf("| %-20s | %-15s | %-5d | %-10d |\n", 
               mochila[i].nome, mochila[i].tipo, 
               mochila[i].quantidade, mochila[i].prioridade);
    }
    printf("+--------------------------------------------------+\n");
}

// Menu para escolher o critério de ordenação
void menuDeOrdenacao() {
    if (numItens <= 1) {
        printf("\n🚫 É necessário ter pelo menos 2 itens para ordenar.\n");
        return;
    }
    
    int escolha;
    printf("\n--- Menu de Ordenação ---\n");
    printf("Escolha o critério de ordenação:\n");
    printf("1. Por Nome (A-Z)\n");
    printf("2. Por Tipo (A-Z)\n");
    printf("3. Por Prioridade (5-1) - Mais alta primeiro\n");
    printf("Opção: ");

    if (scanf("%d", &escolha) != 1) {
        printf("\n❌ Opção inválida.\n");
        while (getchar() != '\n'); // Limpa buffer
        return;
    }

    CriterioOrdenacao criterio = (CriterioOrdenacao)escolha;
    
    // Zera o contador de comparações antes de ordenar
    comparacoes = 0;

    switch (criterio) {
        case POR_NOME:
        case POR_TIPO:
        case POR_PRIORIDADE:
            insertionSort(criterio);
            printf("\n✅ Mochila ordenada com sucesso por %s!\n",
                   (criterio == POR_NOME) ? "Nome" : (criterio == POR_TIPO) ? "Tipo" : "Prioridade");
            printf("Relatório de Desempenho: %ld comparações realizadas.\n", comparacoes);
            // Atualiza a flag para a busca binária
            ordenadaPorNome = (criterio == POR_NOME); 
            break;
        default:
            printf("\n❌ Opção de ordenação inválida.\n");
            ordenadaPorNome = false; 
            break;
    }
}

// Implementação do algoritmo Insertion Sort com múltiplos critérios
void insertionSort(CriterioOrdenacao criterio) {
    int i, j;
    Item chave;

    for (i = 1; i < numItens; i++) {
        chave = mochila[i];
        j = i - 1;

        // Loop de comparação para encontrar a posição correta da 'chave'
        while (j >= 0) {
            bool deveTrocar = false; // Flag para indicar se a chave deve ser movida
            comparacoes++; // Conta uma comparação em cada iteração do while

            switch (criterio) {
                case POR_NOME:
                    // Se o item anterior for 'maior' que a chave (strcmp > 0)
                    if (strcmp(mochila[j].nome, chave.nome) > 0) {
                        deveTrocar = true;
                    }
                    break;
                case POR_TIPO:
                    // Se o item anterior for 'maior' que a chave
                    if (strcmp(mochila[j].tipo, chave.tipo) > 0) {
                        deveTrocar = true;
                    }
                    break;
                case POR_PRIORIDADE:
                    // Ordena da maior prioridade para a menor
                    // Se a prioridade for a mesma, usa nome como critério secundário
                    if (mochila[j].prioridade < chave.prioridade) {
                        deveTrocar = true;
                    } else if (mochila[j].prioridade == chave.prioridade && 
                               strcmp(mochila[j].nome, chave.nome) > 0) {
                        comparacoes++; // Mais uma comparação para o desempate
                        deveTrocar = true;
                    }
                    break;
            }

            if (deveTrocar) {
                mochila[j + 1] = mochila[j]; // Move o elemento para a frente
                j = j - 1;
            } else {
                break; // A posição correta foi encontrada
            }
        }
        mochila[j + 1] = chave; // Insere a chave na posição final
    }
}

// Realiza a Busca Binária por nome (exige ordenação prévia por nome)
void buscaBinariaPorNome() {
    if (numItens == 0) {
        printf("\n🚫 A mochila está vazia.\n");
        return;
    }
    
    // Pré-requisito da Busca Binária
    if (!ordenadaPorNome) {
        printf("\n⚠️ ATENÇÃO: A mochila **não está ordenada por nome**.\n");
        printf("Por favor, ordene a mochila por 'Nome' (Opção 4 -> 1) antes de usar a busca binária.\n");
        return;
    }

    char nomeBusca[TAM_NOME];
    printf("\n--- Busca Binária por Nome ---\n");
    printf("Digite o nome do item a ser buscado: ");
    scanf(" %49[^\n]", nomeBusca);

    int inicio = 0;
    int fim = numItens - 1;
    int meio;
    int resultadoComp;
    bool encontrado = false;

    // Algoritmo de Busca Binária
    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        resultadoComp = strcmp(mochila[meio].nome, nomeBusca);

        if (resultadoComp == 0) {
            // Encontrado!
            encontrado = true;
            break;
        } else if (resultadoComp < 0) {
            // O nome buscado está na metade superior
            inicio = meio + 1;
        } else {
            // O nome buscado está na metade inferior
            fim = meio - 1;
        }
    }

    if (encontrado) {
        printf("\n✅ Item '%s' encontrado!\n", nomeBusca);
        printf("+--------------------------------------------------+\n");
        printf("| %-20s | %-15s | %-5s | %-10s |\n", "NOME", "TIPO", "QTD", "PRIORIDADE");
        printf("+--------------------------------------------------+\n");
        printf("| %-20s | %-15s | %-5d | %-10d |\n", 
               mochila[meio].nome, mochila[meio].tipo, 
               mochila[meio].quantidade, mochila[meio].prioridade);
        printf("+--------------------------------------------------+\n");
    } else {
        printf("\n❌ Item '%s' não encontrado na mochila.\n", nomeBusca);
    }
}

// --- Função Principal ---

int main() {
    int opcao;

    // Loop principal do menu
    do {
        limparTela();
        exibirMenu();

        // Leitura da opção e tratamento de erro
        if (scanf("%d", &opcao) != 1) {
            printf("\n❌ Entrada inválida. Por favor, digite um número.\n");
            opcao = -1; // Força uma opção inválida para reiniciar o loop
            while (getchar() != '\n'); // Limpa o buffer de entrada
            continue;
        }
        
        while (getchar() != '\n'); // Limpa o buffer de entrada após a leitura do número

        // Tratamento das opções
        switch (opcao) {
            case 1:
                inserirItem();
                break;
            case 2:
                removerItem();
                break;
            case 3:
                listarItens();
                break;
            case 4:
                menuDeOrdenacao();
                break;
            case 5:
                buscaBinariaPorNome();
                break;
            case 0:
                printf("\n👋 Fuga concluída! O sistema MateCheck está desligando.\n");
                break;
            default:
                printf("\n❌ Opção inválida. Tente novamente.\n");
                break;
        }
        
        if (opcao != 0) {
            printf("\nPressione ENTER para continuar...");
            getchar(); // Espera o usuário pressionar Enter para ver o resultado
        }

    } while (opcao != 0);

    return 0;
}