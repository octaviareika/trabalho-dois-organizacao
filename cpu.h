#ifndef CPU_H
#define CPU_H

#include "instruction.h"
#include "memory.h"

// dentro da maquina 
typedef struct {
    Instruction* instructions; // instruções
    RAM ram; // a RAM
    Cache l1; // cache L1
    Cache l2; // cache L2
    Cache l3; // cache l3
    int hitL1, hitL2, hitL3, hitRAM; // os hits em cada memoria
    int missL1, missL2, missL3; // cache miss = quando um processador pega um dado e não ta na cache 
    int totalCost; // custo toal
} Machine;

void start(Machine*, Instruction*, int*);
void stop(Machine*);
void run(Machine*);
void printMemories(Machine*);

#endif // !CPU_H