#ifndef CPU_H
#define CPU_H

#include "ram.h"

typedef struct {
    int opcode;
    int add1;
    int add2;
    int add3;
} Instrucao;

typedef struct {
    int registrador1;
    int registrador2;
    int PC;
    Instrucao *programa;
    int opcode;
} CPU;


void setPrograma(CPU *cpu, Instrucao *programa, int tamanho);
void iniciarCPU(CPU *cpu, RAM *ram, int programaTamanho);
void liberarCPU(CPU *cpu);
Instrucao* alocaInstrucao(int n);

#endif