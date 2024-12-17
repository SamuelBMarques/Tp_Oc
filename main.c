#include "ram.h"
#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void aleatorio(CPU *cpu,RAM *ram, int pos1, int pos2,int posResultado, int dado1,int dado2){
    srand(time(NULL));
    int aleatorio = rand()%2;
    setDado(ram, pos1, dado1);
    setDado(ram, pos2, dado2);
    Instrucao* programa = alocaInstrucao(4);
    cpu->programa = NULL;
    programa[0]= (Instrucao){3, 1, pos1, 0}; //opcode 4: registrador 1 -> 0
    programa[1] = (Instrucao){3, 2, pos2, 0}; // opcode 3: RAM -> Registrador 2
    programa[2] = (Instrucao){aleatorio, 0, 0, 0};
    programa[3] = (Instrucao){2, 1, posResultado, 0};
    setPrograma(cpu, programa, 4);
    iniciarCPU(cpu, ram,4);

}

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

void fibonacci(CPU *cpu, RAM *ram, int pos1,int pos2, int n) {
    // Inicializa Fibonacci(0) = 0 e Fibonacci(1) = 1
    setDado(ram, pos1, 0);       // F(0)
    setDado(ram, pos2, 1);   // F(1)

    if (n == 0 || n == 1) return; // Se n for 0 ou 1, já está pronto na RAM.

    // Aloca instruções para o cálculo
    Instrucao* programa = alocaInstrucao(5);
    
    for (int i = 0; i <n-1; ++i) {
        cpu->programa = NULL;
        programa[0] = (Instrucao){3, 1, pos1, 0};       // RAM -> Registrador1 (F(n-2))
        programa[1] = (Instrucao){3, 2, pos2, 0};   // RAM -> Registrador2 (F(n-1))
        programa[2] = (Instrucao){0, 0, 0, 0};   // Soma Registrador1 + Registrador2 -> RAM (F(n))
        programa[3] = (Instrucao){2, 1, pos2, 0}; // Atualiza F(n-1)
        programa[4] = (Instrucao){2, 2, pos1, 0};   // Atualiza F(n-2)
        setPrograma(cpu, programa, 5);
        iniciarCPU(cpu, ram,5);
    }
    free(programa);
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
    //hipotenusa(&cpu, &ram, 1, 2, 3, 3, 4);            COM FALHA!
    //mmc(&cpu,&ram,1,2,3,3,4);
    //fibonacci(&cpu,&ram,2,3,6);
    aleatorio(&cpu,&ram,1,2,3,5,7);
    imprimirRAM(&ram);

    liberarRAM(&ram);
    return 0;
}

