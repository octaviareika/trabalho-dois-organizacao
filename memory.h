#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>
#include <stdbool.h>

#include "constants.h"

typedef struct {
    int words[WORDS_SIZE]; // dentro do bloco da memoria RAM vai ter os numeros (tamanho do numero vai ate 4 ex: 1000) 
} MemoryBlock;

typedef struct {
    MemoryBlock* blocks;// vai ter o blocos }
    int size; // A RAM vai ter o tamanho 
} RAM;

typedef struct {
    MemoryBlock block; // tem o bloco na cache
    int tag; // tag na cache
    bool updated; // n sei oq eh isso
    int vezesDeUso; //lfu
    int tempoCache; // lru
} Line;

typedef struct {
    Line* lines; // cache tem linhas
    int size; // tamanho da cache 
} Cache;

void startCache(Cache*, int);
void stopCache(Cache*);

void startRAM(RAM*, int);
void stopRAM(RAM*);

#endif // !MEMORY_H