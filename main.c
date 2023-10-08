#include "cpu.h"
#include "generator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char**argv) {

    srand(1507);   // Inicializacao da semente para os numeros aleatorios.

    if (argc != 8 && argc != 6) {
        printf("Numero de argumentos invalidos! Sao 5.\n");
        printf("Linha de execucao: ./exe TIPO_INSTRUCAO [TAMANHO_RAM|ARQUIVO_DE_INSTRUCOES] TAMANHO_L1 TAMANHO_L2\n");
        printf("\tExemplo 1 de execucao: ./exe random 10 2 4 7\n");
        printf("\tExemplo 2 de execucao: ./exe file arquivo_de_instrucoes.txt 2 4 8 7\n");
        printf("\tExemplo de instrução para multiplicação: ./exe multi 10 2 3 4 5 6\n");
        return 0;
    }

    int memoriesSize[4];
    Machine machine;
    Instruction *instructions;

    memoriesSize[1] = atoi(argv[3]); /// cache l1
    memoriesSize[2] = atoi(argv[4]); // cache l2
    memoriesSize[3] = atoi(argv[5]); // cache l3 


    if (strcmp(argv[1], "random") == 0) {
        memoriesSize[0] = atoi(argv[2]);
        instructions = generateRandomInstructions(memoriesSize[0]);
    } else if (strcmp(argv[1], "file") == 0) {
        instructions = readInstructions(argv[2], memoriesSize);
    } 
    else if(strcmp(argv[1], "multi") == 0){
        memoriesSize[0] = atoi(argv[2]);

        int valor1 = atoi(argv[6]);
        int valor2 = atoi(argv[7]);

        instructions = generateMultiplicationInstructions(memoriesSize[0], valor1, valor2);
    }

    else if(strcmp(argv[1], "div") == 0){
        memoriesSize[0] = atoi(argv[2]);

        int valor1 = atoi(argv[3]);
        int valor2 = atoi(argv[4]);

        instructions = generateDivInstructions(memoriesSize[0], valor1, valor2);
    }

    else {
        printf("Invalid option.\n");
        return 0;
    }
    
    printf("Starting machine...\n");
    start(&machine, instructions, memoriesSize);
    if (memoriesSize[0] <= 10)
        printMemories(&machine);
    run(&machine);
    if (memoriesSize[0] <= 10)
        printMemories(&machine);
    stop(&machine);
    printf("Stopping machine...\n");

    double tempo_exec;
    clock_t inicio, fim;

    inicio = clock();

    srand(time(NULL));

    fim = clock();
    tempo_exec = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("\nTempo de execucao: %fs\n", tempo_exec);
    return 0;
}