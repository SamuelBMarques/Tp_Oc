#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "ram.h"

//Aloca memória para instruções de um programa
Instrucao* alocaInstrucao(int n){
    Instrucao* inst = (Instrucao *)malloc(n * sizeof(Instrucao));
    return inst;
}

//Configura um programa (conjunto de instruções) na CPU
void setPrograma(CPU *cpu, Instrucao *programa, int tamanho) {
    if(cpu->programa == NULL){
        cpu->programa = (Instrucao *)malloc(tamanho * sizeof(Instrucao));
        for (int i = 0; i < tamanho; ++i) {
            cpu->programa[i] = programa[i];
        }
    }
    
}

//Executa o programa carregado, processando instruções baseadas em um código de operação (opcode)
void iniciarCPU(CPU *cpu, RAM *ram, int programaTamanho) {
    cpu->opcode = 0;
    for (cpu->PC = 0; cpu->PC < programaTamanho && cpu->opcode != -1; ++cpu->PC) {
        Instrucao inst = cpu->programa[cpu->PC];
        cpu->opcode =  cpu->programa[cpu->PC].opcode;
        switch (cpu->opcode) {
            case -1:  //Termina o Programa
                printf("Programa terminou!!\n");
                imprimirRAM(ram);
                break;

            case 0: { // Soma
                cpu->registrador1 += cpu->registrador2;
                //printf("Inst sum -> RAM posicao %d com conteudo %d\n", inst.add3, cpu->registrador1);
                break;
            }

            case 1: { // Subtração
                cpu->registrador1 -= cpu->registrador2;
                //printf("Inst sub -> RAM posicao %d com conteudo %d\n", inst.add3, cpu->registrador1);
                break;
            }

            case 2: { // Copiar registrador para RAM
                if (inst.add1 == 1) {
                    setDado(ram, inst.add2, cpu->registrador1);
                    //printf("Inst copy_reg_ram -> RAM posicao %d com conteudo %d\n", inst.add2, cpu->registrador1);
                } else if (inst.add1 == 2) {
                    setDado(ram, inst.add2, cpu->registrador2);
                    //printf("Inst copy_reg_ram -> RAM posicao %d com conteudo %d\n", inst.add2, cpu->registrador2);
                }
                break;
            }

            case 3: { // Copiar RAM para registrador
                if (inst.add1 == 1) {
                    cpu->registrador1 = getDado(ram, inst.add2);
                    //printf("Inst copy_ram_reg -> Registrador1 com conteudo %d\n", cpu->registrador1);
                } else if (inst.add1 == 2) {
                    cpu->registrador2 = getDado(ram, inst.add2);
                    //printf("Inst copy_ram_reg -> Registrador2 com conteudo %d\n", cpu->registrador2);
                }
                break;
            }

            case 4: { // Copiar valor externo para registrador
                if (inst.add1 == 1) {
                    cpu->registrador1 = inst.add2;
                    //printf("Inst copy_ext_reg -> Registrador1 com conteudo %d\n", cpu->registrador1);
                } else if (inst.add1 == 2) {
                    cpu->registrador2 = inst.add2;
                    //printf("Inst copy_ext_reg -> Registrador2 com conteudo %d\n", cpu->registrador2);
                }
                break;
            }

            case 5: { // Obter valor do registrador para instrução
                if (inst.add1 == 1) {
                    inst.add2 = cpu->registrador1;
                    //printf("Inst obtain_reg -> Registrador1 com conteudo %d\n", cpu->registrador1);
                } else if (inst.add1 == 2) {
                    inst.add2 = cpu->registrador2;
                    //printf("Inst obtain_reg -> Registrador2 com conteudo %d\n", cpu->registrador2);
                }
                break;
            }
        }
    }
}

void liberarCPU(CPU *cpu) {
    free(cpu->programa);
    cpu->programa = NULL;
}
