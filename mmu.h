#ifndef MMU_H
#define MMU_H

#include "memory.h"
#include "instruction.h"
#include "cpu.h"

typedef enum {
    L1Hit = 1,
    L2Hit,
    L3Hit,
    RAMHit
} WhereWasHit;

typedef enum {
    LRU = 1,
    LFU
} metodoMapeamento;

char* convertToString(WhereWasHit);

Line* MMUSearchOnMemorys(Address, Machine*, Line*, WhereWasHit*, metodoMapeamento);


#endif // !MMU_H