#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ram.h"

//Aloca memória para a RAM com o tamanho especificado
void criarRAM(RAM *ram, int tamanho) {
    ram->memoria = (int *)malloc(tamanho * sizeof(int));
    ram->tamanho = tamanho;
}

//Cria uma RAM com todos os valores inicializados para 0
void criarRAM_vazia(RAM *ram, int tamanho) {
    criarRAM(ram, tamanho);

    for (int i = 0; i < tamanho; ++i) {
        ram->memoria[i] = 0;
    }
}

//Cria uma RAM com valores aleatórios
void criarRAM_aleatoria(RAM *ram, int tamanho) {
    criarRAM(ram, tamanho);

    srand(time(NULL)); // Inicializa a semente para números aleatórios

    for (int i = 0; i < tamanho; ++i) {
        ram->memoria[i] = rand();
    }
}

//Escreve um valor em um endereço específico da RAM, verificando se o endereço está dentro dos limites
void setDado(RAM *ram, int endereco, int conteudo) {
    if (endereco >= 0 && endereco < ram->tamanho) {
        ram->memoria[endereco] = conteudo;
    } else {
        printf("Erro: Endereco fora do limite.\n");
    }
}

//Lê um valor de um endereço específico da RAM, também verificando os limites
int getDado(RAM *ram, int endereco) {
    if (endereco >= 0 && endereco < ram->tamanho) {
        return ram->memoria[endereco];
    } else {
        printf("Erro: Endereco fora do limite.\n");
        return -1; // Valor de erro
    }
}

//Imprime o conteúdo da RAM
void imprimirRAM(RAM *ram) {
    printf("Conteudo da RAM:\n");

    for (int i = 0; i < ram->tamanho; ++i) {
        printf("%d,", ram->memoria[i]);
    }

    printf("\n");
}

// Libera a memória alocada para a RAM  
void liberarRAM(RAM *ram) {
    free(ram->memoria);
    ram->memoria = NULL;
    ram->tamanho = 0;
}

