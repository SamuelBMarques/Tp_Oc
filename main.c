#include "ram.h"
#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void multiplicacao(CPU *cpu, RAM *ram, int pos1, int pos2, int posResultado,int dado1,int dado2){
    setDado(ram, pos1, dado1);
    setDado(ram, pos2, dado2);
    Instrucao* programa = alocaInstrucao(4+dado2); // Máximo de instruções necessárias
    cpu->programa = NULL;
    int tamanho = 0;
    programa[tamanho++]= (Instrucao){4, 1, 0, 0}; //opcode 4: registrador 1 ->
    programa[tamanho++] = (Instrucao){3, 2, pos1, 0}; // opcode 3: RAM -> Registrador 2
    
    for (int i = 0; i < dado2; ++i) {
        programa[tamanho++] = (Instrucao){0, 0, 0, 0}; // opcode 0: Soma
    }


    programa[tamanho++] = (Instrucao){2, 1, posResultado, 0}; // opcode 2: Registrador -> RAM
    //programa[tamanho++] = (Instrucao){-1, 0, 0, 0}; // opcode 2: Registrador -> RAM
    // Passo 2: Configurar e iniciar a CPU
    setPrograma(cpu, programa, tamanho);
    iniciarCPU(cpu, ram, tamanho);
    free(programa);
}

void fatorial(CPU *cpu, RAM *ram, int pos, int posResultado, int dado){
    setDado(ram, posResultado, 1);

    for (int i = 2; i <= dado; ++i) {
        multiplicacao(cpu, ram, pos, posResultado, posResultado, i, getDado(ram, posResultado));
    }
}

void exponencial(CPU *cpu, RAM *ram, int posBase, int posResultado, int base, int expoente) {
    setDado(ram, posResultado, 1);
    setDado(ram, posBase, base);

    for (int i = 0; i < expoente; ++i) {
        multiplicacao(cpu, ram, posBase, posResultado, posResultado, base, getDado(ram, posResultado));
    }
}   

int main() {
    RAM ram;
    CPU cpu;
    criarRAM_vazia(&ram, 10);

    //multiplicacao(&cpu,&ram, 2, 3,4,9,9);
    //fatorial(&cpu,&ram,5,6,4);
    exponencial(&cpu,&ram,1,2,2,10);
    imprimirRAM(&ram);

    liberarRAM(&ram);
    return 0;
}

