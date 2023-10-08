#include "generator.h"
#include <stdio.h>
#include <stdlib.h>

Instruction* generateRandomInstructions(int ramSize) {
    // 01|22|13|45 => this is an instruction
    // 02|33|12|01 => this is another instruction
            
    // 0 => opcode => move
    // 1 => opcode => sum
    // 2 => opcode => subtract
    //-1 => halt
    
    int n = 9;

    Instruction* instructions = (Instruction*) malloc((n + 1) * sizeof(Instruction));

    for (int i=0; i<n; i++) {
        instructions[i].add1.block = rand() % ramSize; // numero de 1 a 10 por exemplo
        instructions[i].add1.word = rand() % WORDS_SIZE; // numero de 1 a 4 
        instructions[i].add2.block = rand() % ramSize;
        instructions[i].add2.word = rand() % WORDS_SIZE;
        instructions[i].add3.block = rand() % ramSize; // endereço 3 do bloco escolhido de maneira aleatória 
        instructions[i].add3.word = rand() % WORDS_SIZE; // a informção que vai ter dentro do e
        instructions[i].opcode = rand() % 3;
    }
    instructions[n].opcode = -1;
    
    return instructions;
}

void lacoRepeticao(int valorEndereco1, int valorEndereco2, int valorEndereco3, Instruction *instruction, int tamanho){
    for(int i = 2; i < tamanho -1; i++){
        instruction[i].opcode = 1; 
        instruction[i].add1.word = valorEndereco1; instruction[i].add1.block = 0;
        instruction[i].add2.word = valorEndereco2; instruction[i].add2.block = 0;
        instruction[i].add3.word = valorEndereco3; instruction[i].add3.block = 0;
    }
}

Instruction* generateMultiplicationInstructions(int ramSize, int multiplicando, int multiplicador){
    int tamanho = 3 + multiplicador; //Calcula o tamanho que sera alocado para o vetor de instruction

    Instruction* multiplicacao = (Instruction*) malloc(tamanho * sizeof(Instruction));

    //Define o endereco de memoria randomicamente para as variaveis que farao a operacao
    int valorEndereco1 = 0;
    int valorEndereco2 = 1;

    int valorEndereco3 = valorEndereco2;

    //Chama a funcao que gera instrucoes de somas sucetivas dentro de um for da terceira posicao a penultima
    lacoRepeticao(valorEndereco1, valorEndereco2, valorEndereco3, multiplicacao, tamanho);

    //Define as informacoes iniciais para as linhas 0 e 1 do vetor instruction
    multiplicacao[0].opcode = 0; 
    multiplicacao[0].add1.word = 0; 
    multiplicacao[0].add1.block = multiplicando;
    multiplicacao[0].add2.word = valorEndereco1; 
    multiplicacao[0].add2.block = 0;
    multiplicacao[0].add3.word = 0; 
    multiplicacao[0].add3.block = 0;

    multiplicacao[1].opcode = 0; 
    multiplicacao[1].add1.word = 0; 
    multiplicacao[1].add1.block = 0;
    multiplicacao[1].add2.word = valorEndereco2; 
    multiplicacao[1].add2.block = 0;
    multiplicacao[1].add3.word = 0; 
    multiplicacao[1].add3.block = 0;

    //Define a ultima instrucao que desligara a maquina
    multiplicacao[tamanho - 1].opcode = -1;

    return multiplicacao;
}

Instruction *generateDivInstructions(int ramSize, int valor1, int valor2){
    Instruction *divisao;
    int contador = 0;
    
    // o tamanho da RAM mais 4, pois vai ter sido usado a primeira posição das instruções, a segunda posição, a posição que vai armazenar o quociente e a poisição final que é o HALT
    divisao = malloc((ramSize + 4) * sizeof(Instruction)); 

    divisao[0].opcode = 0;
    divisao[0].add1.word = valor1; // o valor do dividendo
    divisao[0].add1.block = rand()%ramSize;

    divisao[1].opcode = 0;
    divisao[1].add1.block = valor2; //divisor

    do {
        divisao[1].add2.block = rand()%ramSize;
    }while(divisao[1].add2.block == divisao[0].add2.block);

        while (valor2 * (contador + 1) <= valor1 ){ // enquanto o valor do dividendo eh maior que o divisor 
            
            divisao[contador + 2].opcode = 2; // é usado o (contador + 2) pois usamos as duas primeiras posições do vetor para opcode 0
            divisao[contador + 2].add1.word = divisao[0].add2.word;
            divisao[contador + 2].add2.word = divisao[1].add2.word;
            divisao[contador + 2].add3.word = divisao[0].add2.word;
    
            contador++; // contar quantas vezes esta rolando a subtração 
        }

    //salvando o quociente(contador) na RAM


    divisao[contador + 2].opcode = 0; // atribuir um valor para endereço da RAM
    divisao[contador + 2].add1.block = contador;

    // O numero da RAM(endereço)  não pode ter valor igual ao da RAM da primeira posição do vetor de instruções e nem da segunda
    do {
        divisao[contador + 2].add2.block = rand()%ramSize;
    }while(divisao[contador + 2].add2.block == divisao[1].add2.block && divisao[contador + 2].add2.block == divisao[0].add2.block);


    // ultima posição com halt (por isso o +1)
    divisao[contador + 3].opcode = -1;

    return divisao;

}


Instruction* readInstructions(char* fileName, int* memoriesSize) {
    Instruction* instructions = NULL;

    printf("FILE -> %s\n", fileName);
    FILE* file = fopen(fileName, "r"); // Open file in read mode
    
    if (file == NULL) {
        printf("Arquivo nao pode ser aberto.\n");
        exit(1);
    }

    int n, i = 0;
    fscanf(file, "%d %d", &n, &memoriesSize[0]);
    instructions = (Instruction*) malloc(n * sizeof(Instruction));
    while (i < n) {
        fscanf(file, "%d %d %d %d %d %d %d", 
            &instructions[i].opcode, 
            &instructions[i].add1.block, &instructions[i].add1.word,
            &instructions[i].add2.block, &instructions[i].add2.word,
            &instructions[i].add3.block, &instructions[i].add3.word);
        i++;
    }
    fclose(file); // Close file

    return instructions;
}
