#include "memory.h"

// WORDS_SIZE =  tamanho da palavra 


void startRAM(RAM* ram, int size) { 
    ram->blocks = (MemoryBlock*) malloc(sizeof(MemoryBlock) * size); // o bloco da RAM vai ter uma alocação (vai ser um vetor com o tamanho que a gente quer inserir)
    ram->size = size; // o tamanho da RAM vai ser o tamanho que colocarmos

    // memoria = b blocos, memoria cache = l linhas
    for (int i=0;i<size;i++) {  // a memoria vai possuir blocos com o tamanho que escolhemos
        for (int j=0;j<WORDS_SIZE;j++) // contador que vai de 0 até o tamanho da palavra
            ram->blocks[i].words[j] = rand() % 20;   // em cada bloco da ram vai ter numeros (que vai de 0 até 100 dependendo da aleatoriedade) (como se fosse uma matriz)        
     // exe RAM com 3 blocos e em cada bloco vai ter numeros de 1 a 100
    }
}

void stopRAM(RAM *ram) { // liberar a memoria ram alocada
    free(ram->blocks);
}

/// inicia a memoria cache (inicia com valor inválido = -1)
void startCache(Cache* cache, int size) { // tamanho do cache como um dos paramentros
    cache->lines = (Line*) malloc(sizeof(Line) * size); // a memoria cache vai ter um determinado numero de linhas 
    cache->size = size; // tamanho do cache vai receber o valor do cache inserido

    for (int i=0;i<size;i++) {// 
        cache->lines[i].tag = INVALID_ADD; // cada linha da memoria cache vai receber uma tag (-1) (inicial)
        cache->lines[i].tempoCache = 0; // inicializa o tempo na cache com 0
        cache->lines[i].vezesDeUso = 0; 
    // colocar algo aq 
    }
}

// libera a memoria cache
void stopCache(Cache *cache) {
    free(cache->lines);
}
