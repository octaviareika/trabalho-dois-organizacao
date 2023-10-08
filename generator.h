#ifndef GENERATOR_H
#define GENERATOR_H

#include "cpu.h"

Instruction* generateRandomInstructions(int);
Instruction* readInstructions(char*, int*);
Instruction *generateMultiplicationInstructions(int, int, int);
Instruction *generateDivInstructions(int, int, int);


#endif // !GENERATOR_H