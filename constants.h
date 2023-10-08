#include <stdbool.h>

#define WORDS_SIZE 4
#define INVALID_ADD -1
#define COST_ACCESS_L1 1 // custo para acessa l1
#define COST_ACCESS_L2 10 // custo para acessar l2
#define COST_ACCESS_L3 100 // custo para acessar l3
#define COST_ACCESS_RAM 1000
#define PRINT_INTERMEDIATE_RAM
#define PRINT_LOG

#define METODOMAPEAMENTO 2 
// mapeamento direto 3
// LFU 1
// LRU 2