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
    programa[tamanho++]= (Instrucao){4, 1, 0, 0}; //opcode 4: registrador 1 -> 0
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

    if(base < 0){
        base *=-1;
    }

    for (int i = 0; i < expoente; ++i) {
        multiplicacao(cpu, ram, posBase, posResultado, posResultado, base, getDado(ram, posResultado));
    }
}   

void mdc(CPU *cpu, RAM *ram, int posA, int posB, int a, int b) {
    setDado(ram, posA, a);
    setDado(ram, posB, b);
    Instrucao* programa = alocaInstrucao(4);

    while (getDado(ram, posA) != getDado(ram, posB)) {
        if (getDado(ram, posA) > getDado(ram, posB)) {
            cpu->programa = NULL;
            programa[0] = (Instrucao){3, 1, posA, 0};       //Ram pro registrador 1
            programa[1] = (Instrucao){3, 2, posB, 0};       //Ram pro registrador 2
            programa[2] = (Instrucao){1, 0, 0, 0};          //Subtracao, Registrador 1 - Registrador 2
            programa[3] = (Instrucao){2, 1, posA, 0};       //Resultado da subtracao (Armazenado no registrador 1), de volta para a ram
            setPrograma(cpu, programa, 4);
            iniciarCPU(cpu, ram, 4);
        } else {
            cpu->programa = NULL;
            programa[0] = (Instrucao){3, 2, posA, 0};       //Ram pro Registrador 2
            programa[1] = (Instrucao){3, 1, posB, 0};       //Ram pro registrador 1
            programa[2] = (Instrucao){1, 0, 0, 0};          //Subtracao, Registrador 1 - Registrador 2
            programa[3] = (Instrucao){2, 1, posB, 0};       //Resultado da subtracao (Armazenado no registrador 1), de volta para a ram
            setPrograma(cpu, programa, 4);
            iniciarCPU(cpu, ram, 4);
        }
    }
    free(programa);
}
void divisao(CPU *cpu, RAM *ram, int pos1, int pos2, int posResultado, int numerador, int denominador) {
    if (denominador == 0) {
        printf("Erro: Divisão por zero!\n");
        return;
    }

    setDado(ram, pos1, numerador);
    setDado(ram, pos2, denominador);
    Instrucao* programa = alocaInstrucao(2); // Instruções para divisão
    cpu->programa = NULL;

    programa[0] = (Instrucao){3, 1, pos1, 0}; // opcode 3: RAM (numerador) -> Registrador 1
    programa[1] = (Instrucao){3, 2, pos2, 0}; // opcode 3: RAM (denominador) -> Registrador 2
    setPrograma(cpu, programa, 2);
    iniciarCPU(cpu, ram, 2);

    int quociente = 0;
    while (getDado(ram, pos1) >= getDado(ram, pos2)) {
        cpu->programa = NULL;
        programa[0] = (Instrucao){1, 0, 0, 0};          //Subtracao, Registrador 1 - Registrador 2
        programa[1] = (Instrucao){2, 1, pos1, 0};       //Resultado da subtracao (Armazenado no registrador 1), de volta para a ram
        setPrograma(cpu, programa, 2);
        iniciarCPU(cpu, ram, 2);
        quociente++;
    }

    setDado(ram, posResultado, quociente);
    free(programa);
}

void raiz_quadrada(CPU *cpu, RAM *ram, int pos, int posResultado, int numero) {
    setDado(ram, pos, numero);
    int raiz = 0;

    while (1) {
        multiplicacao(cpu, ram, posResultado, posResultado, posResultado, raiz + 1, raiz + 1);
        if (getDado(ram, posResultado) > getDado(ram, pos)) {
            break;
        }

        raiz++;
    }

    setDado(ram, posResultado, raiz);
}

void hipotenusa(CPU *cpu, RAM *ram, int posA, int posB, int posResultado, int a, int b) {

    int posTemp1 = 5; 
    int posTemp2 = 6;
    Instrucao* programa = alocaInstrucao(4);
    // Calcular a^2 e armazenar em posTemp1
    multiplicacao(cpu, ram, posA, posA, posTemp1, a, a);

    // Calcular b^2 e armazenar em posTemp2
    multiplicacao(cpu, ram, posB, posB, posTemp2, b, b);
    imprimirRAM(ram);

    // Somar a^2 + b^2
    cpu->programa = NULL;
    programa[0] = (Instrucao){3, 1, posTemp1, 0}; // opcode 3: RAM (a^2) -> Registrador 1
    programa[1] = (Instrucao){3, 2, posTemp2, 0}; // opcode 3: RAM (b^2) -> Registrador 2
    programa[2] = (Instrucao){0, 0, 0, 0}; // opcode 0: Soma
    programa[3] = (Instrucao){2, 1, posResultado, 0};
    setPrograma(cpu, programa, 4);
    iniciarCPU(cpu, ram, 4);
    printf("%d %d",cpu->registrador1,cpu->registrador2);

    free(programa);
    // Calcular a raiz quadrada de a^2 + b^2
    raiz_quadrada(cpu, ram, posResultado, posResultado, getDado(ram,posResultado));
    
}


void mmc(CPU *cpu, RAM *ram, int pos1, int pos2, int posResultado, int a, int b) {
    int posMDC = 7;
    // Calcular o MDC
    mdc(cpu, ram, pos1, pos2, a,b); // Chamando a função mdc para calcular o MDC
    int mdc = ram->memoria[pos1];
    // Calcular o MMC usando a fórmula: MMC(a, b) = (a * b) / MDC(a, b)
    multiplicacao(cpu,ram,pos1,pos2,posResultado,a,b);
    imprimirRAM(ram);
    // Calculando o MMC
    divisao(cpu,ram,posResultado,posMDC,posResultado,getDado(ram,posResultado),mdc);
    imprimirRAM(ram);
}

void dist(CPU *cpu, RAM *ram, int pos1, int pos2, int pos3, int pos4, int posResultado ,int dado1, int dado2, int dado3, int dado4){
    setDado(ram, posResultado, 0);

    setDado(ram, pos1, dado1);
    setDado(ram, pos2, dado2);
    setDado(ram, pos3, dado3);
    setDado(ram, pos4, dado4);

    Instrucao* programa = alocaInstrucao(4);
    cpu->programa = NULL;
    int tamanho = 0;

    programa[tamanho++]= (Instrucao){3, 1, pos1, 0}; // opcode 3: RAM -> Registrador 2
    programa[tamanho++] = (Instrucao){3, 2, pos2, 0}; // opcode 3: RAM -> Registrador 2

    programa[tamanho++] = (Instrucao){1, 0, 0, 0}; // opcode 1: Subtração
    programa[tamanho++] = (Instrucao){2, 1, pos1, 0};

    cpu->programa = NULL;
    setPrograma(cpu, programa, tamanho);
    iniciarCPU(cpu, ram, tamanho);
    free(programa);

    exponencial(cpu, ram, pos1, posResultado, getDado(ram, pos1), 2);

    programa = alocaInstrucao(1);
    cpu->programa = NULL;
    tamanho = 0;

    programa[tamanho++] = (Instrucao){2, 1, pos1, 0}; // opcode 1: Subtração

    cpu->programa = NULL;
    setPrograma(cpu, programa, tamanho);
    iniciarCPU(cpu, ram, tamanho);
    free(programa);

    programa = alocaInstrucao(4);
    cpu->programa = NULL;
    tamanho = 0;

    programa[tamanho++]= (Instrucao){3, 1, pos3, 0}; // opcode 3: RAM -> Registrador 2
    programa[tamanho++] = (Instrucao){3, 2, pos4, 0}; // opcode 3: RAM -> Registrador 2
    programa[tamanho++] = (Instrucao){1, 0, 0, 0}; // opcode 0: Soma
    programa[tamanho++] = (Instrucao){2, 1, pos3, 0}; // opcode 1: Subtração

    cpu->programa = NULL;
    setPrograma(cpu, programa, tamanho);
    iniciarCPU(cpu, ram, tamanho);
    free(programa);

    exponencial(cpu, ram, pos3, posResultado, getDado(ram, pos3), 2);

    programa = alocaInstrucao(1);
    cpu->programa = NULL;
    tamanho = 0;

    programa[tamanho++] = (Instrucao){2, 1, pos3, 0}; // opcode 1: Subtração

    cpu->programa = NULL;
    setPrograma(cpu, programa, tamanho);
    iniciarCPU(cpu, ram, tamanho);
    free(programa);

    programa = alocaInstrucao(4);
    cpu->programa = NULL;
    tamanho = 0;

    programa[tamanho++] = (Instrucao){4, 2, getDado(ram, posResultado), 0};
    programa[tamanho++] = (Instrucao){4, 1, getDado(ram, pos1), 0};
    programa[tamanho++] = (Instrucao){0, 0, 0, 0}; // opcode 0: Soma
    programa[tamanho++] = (Instrucao){2, 1, pos3, 0};

    cpu->programa = NULL;
    setPrograma(cpu, programa, tamanho);
    iniciarCPU(cpu, ram, tamanho);
    free(programa);

    raiz_quadrada(cpu, ram, pos3, posResultado, getDado(ram, pos3));

}


int main() {
    RAM ram;
    CPU cpu;
    criarRAM_vazia(&ram, 10);

    //multiplicacao(&cpu,&ram, 2, 3,4,9,9);
    //fatorial(&cpu,&ram,5,6,4);
    //exponencial(&cpu,&ram,1,2,2,10);
    //mdc(&cpu,&ram,1,2,3,4);
    //divisao(&cpu, &ram, 1, 2, 3, 20, 5);
    //raiz_quadrada(&cpu, &ram, 1, 3, 24);
    //hipotenusa(&cpu, &ram, 1, 2, 3, 3, 4);
    //mmc(&cpu,&ram,1,2,3,3,4);
    dist(&cpu, &ram, 0, 1, 2, 3, 4, 4, 10, 2, 4);
    imprimirRAM(&ram);

    liberarRAM(&ram);
    return 0;
}

