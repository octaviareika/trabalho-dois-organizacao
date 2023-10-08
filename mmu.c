#include "mmu.h"

#include <stdio.h>

bool canOnlyReplaceBlock(Line);

int memoryCacheMapping(int, Cache*, metodoMapeamento);

int lineWhichWillLeave(int, Cache*);

void updateMachineInfos(Machine*, WhereWasHit*, Line*, int);

void moveLine(Line *, int, Line*, int);

char* convertToString(WhereWasHit whereWasHit) {
    switch (whereWasHit) {
        case L1Hit:
            return "CL1";
        case L2Hit:
            return "CL2";

        case L3Hit:
            return "CL3";
        case RAMHit:
            return "RAM";
    }

    return "";
}

Line* MMUSearchOnMemorys(Address add, Machine* machine, Line *line, WhereWasHit* whereWasHit, metodoMapeamento metodos) { // procura nas memórias
    // Strategy => write back
    
    // Direct memory map
    int l1posicao = memoryCacheMapping(add.block, &machine->l1, metodos); // vai fazer o mapeamento associativo em cache1, vai pegar a posição que foi menos usada ou que ficou mais tempo na cache
    int l2posicao = memoryCacheMapping(add.block, &machine->l2, metodos); // posicão da cache 2 que vai mapear 
    int l3posicao = memoryCacheMapping(add.block, &machine->l3, metodos); // posição da cache 3 que vai mapear

    Line* cache1 = machine->l1.lines; // quantidade de linhas dentro do cache 1
    Line* cache2 = machine->l2.lines; // quantidade de linhas dentro da cache 2
    Line* cache3 = machine->l3.lines;
    MemoryBlock* RAM = machine->ram.blocks; // a RAM vai ter os blocos dentro da maquina
    int cost = 0; // custo inicializa com 0

    if (cache1[l1posicao].tag == add.block) { // se o valor da tag posição da linha da memoria cache 1 for igual ao endereço do bloco 

        
        cost = COST_ACCESS_L1; // custo foi 1 
        cache1[l1posicao].updated = false;
        *whereWasHit = L1Hit;
        cache1[l1posicao].vezesDeUso++;
        updateMachineInfos(machine, whereWasHit, &(cache1[l1posicao]), cost);


        return &(cache1[l1posicao]);

    } 
    else if (cache2[l2posicao].tag == add.block) { // se a tag na linha da cache 2 tiver o msm numero do endereço do bloco (o numero do bloco é o numero da tag na cache )
        /* Bloco está na memoria cache l2 (em cada linha)*/

        cache2[l2posicao].tag = add.block;  // uma posição especifica da cache l2
        cache2[l2posicao].updated = false; // não atualizou

        cost = COST_ACCESS_L1 + COST_ACCESS_L2; // vai somar ambos os custos ja que precisa passar pela l1 e dps pela l2 
        cache2[l2posicao].vezesDeUso++;

        
        *whereWasHit = L2Hit;
        updateMachineInfos(machine, whereWasHit, &(cache3[l2posicao]), cost); // hit em l2
        moveLine(cache2, l2posicao, cache1, l1posicao);
        return &(cache2[l2posicao]);
    }

    else if (cache3[l3posicao].tag == add.block){

        /*Bloco está na memória cache 3*/
        cache3[l3posicao].tag = add.block;
        cache3[l3posicao].updated = false; // nao vai ser atualizado 
        cost = COST_ACCESS_L1 + COST_ACCESS_L2 + COST_ACCESS_L3; // custo para acessar foi maior
        *whereWasHit = L3Hit;
        cache3[l3posicao].vezesDeUso++;
        updateMachineInfos(machine, whereWasHit, &(cache3[l3posicao]), cost); // hit em l3
        moveLine(cache3, l3posicao, cache2, l2posicao); // move l3 para l2
        moveLine(cache2, l2posicao, cache1, l1posicao); // move l2 para l1
        return &(cache3[l3posicao]);

    }

    // Vai pegar da RAM e trazer esse dado para L1

    else {

        if (canOnlyReplaceBlock(cache1[l1posicao]) == false) { // bloco da cache 1 não pode ser substituído
            if (canOnlyReplaceBlock(cache2[l2posicao]) == false) { // bloco da cache 2 não pode ser substituído
                if (canOnlyReplaceBlock(cache3[l3posicao]) == false){ // bloco da cache 3 não pode ser substituído
                    RAM[cache3[l3posicao].tag] = cache3[l3posicao].block;
                }

                cache3[l3posicao] = cache2[l2posicao]; // mudar isso aq (da l2 vai para l3)
                cache3[l3posicao].tempoCache = 0; // Como a antiga posição da cache 2 foi transferida ou movimentada para a cache 3, o tempo que ele fica reseta ja que mudou de lugar
            }
        
            cache2[l2posicao] = cache1[l1posicao]; // da l1 vai para l2
            cache2[l2posicao].tempoCache = 0;
         
        }

        cache1[l1posicao].block = RAM[add.block];
        cache1[l1posicao].tag = add.block;
        cache1[l1posicao].updated = false;
        cost = COST_ACCESS_L1 + COST_ACCESS_L2 + COST_ACCESS_L3 + COST_ACCESS_RAM;
        cache1[l1posicao].tempoCache = 0;
        *whereWasHit = RAMHit;
    

    

    }

    updateMachineInfos(machine, whereWasHit, &(cache1[l1posicao]), cost);
    return &(cache1[l1posicao]);


}

// não mexer nisso
bool canOnlyReplaceBlock(Line line) {
    // verificar se pode substituir o bloco
    // Or the block is empty or (se o bloco está vazio) (com o valor -1 em tag)
    // the block is equal to the one in memory RAM and can be replaced (se o bloco for igual na memória RAM E pode ser substuída )
    // ou se a linha da cache não tiver o -1, porem não tiver sido atualizada 
    if (line.tag == INVALID_ADD || (line.tag != INVALID_ADD && !line.updated))
        return true;
    return false; 
}

// mudar essa função aq 
int memoryCacheMapping(int address, Cache* cache, metodoMapeamento metodos) { // mapear memoria cache 
// A memória cache tem muito menos linhas comparado ao número de blocos da
// memória principal. É necessário que linhas saiam para que outras entrem.
// A escolha de quem sai e entra define a organização da cache.

    int posicao = 0; // inicializa uma posição qualquer com 0
    cache->lines[0].tempoCache++;

    switch (metodos){
        case LRU: 
        //LRU = bloco que está mais tempo na cache vai ser substituído por outro
        //Um dado ou instrução acessado recentemente tem maior probabilidade de ser acessado novamente, do que um dado ou instrução acessado há mais tempo.
        for (int i =0; i < cache->size; i++){
            if (cache->lines[i].tag != INVALID_ADD){
    
            
            cache->lines[i].tempoCache = cache->lines[i].tempoCache + 1;

                if (cache->lines[i].tempoCache > cache->lines[posicao].tempoCache){ // retira a linha que estava a mais tempo na cache, que foi menos recentemente usada
                    posicao = i; // pega a posição com o maior tempo na cache, é a linha menos recentemente usada até agora 
                }
                

                if (cache->lines[i].tag == address)///verifica-se se o endereço de uma determinada linha tem msm numero do endereço q estamos procurando, se tem o endereço que queremos
                    return i; // retorna a tag da cache que tem o mesmo endereço do bloco 
                
            }
            
        }

        break;

        case LFU:
        //LFU = substitui o bloco que menos foi usado na cache.
        for (int i =0; i < cache->size; i++){
            if (cache->lines[i].vezesDeUso < cache->lines[posicao].vezesDeUso){
                posicao = i;
            }


            if (cache->lines[i].tag == address){
                return i; 
            }

        }

        break;

    }

    
    return address % cache->size; // retorna o resto da divisão do numero do endereço com o tamanho da cache (a posição da linha da cache ) - mapeamento direto
}

void moveLine(Line *linha1, int posicao1, Line *linha2, int posicao2){
    Line auxiliar = linha1[posicao1];
    linha1[posicao1] = linha2[posicao2];
    linha2[posicao2] = auxiliar;
}


int lineWhichWillLeave(int address, Cache* cache) {

    return address % cache->size;
}

void updateMachineInfos(Machine* machine, WhereWasHit* whereWasHit, Line *line, int cost) { // achar 
    // Se o processador precisar de um dado ele estiver na cache, tem-se um cache hit 
    // Ao buscar um dado na memória, um bloco com várias palavras é salvo na cache 
    switch (*whereWasHit) { 
        case L1Hit: // se o hit for na l1, incrementa
            machine->hitL1 += 1;
            break;

        case L2Hit:
            machine->hitL2 += 1; // se foi na l2 incrementa o hit porem incrementa o hit miss pq não foi achado na l1
            machine->missL1 += 1;
            break;
        
        case L3Hit:
            machine->hitL3 += 1; // se foi achado em L3, marca hit...
            machine->missL1 += 1;
            machine->missL2 += 1;
            break;

        case RAMHit:
            machine->hitRAM += 1; // se foi achado na RAM
            machine->missL1 += 1;
            machine->missL2 += 1;
            machine->missL3 += 1;
            break;
        
    }

    line->tempoCache = 0; // volta com zero ja que a determinada posição foi usada/acessada
    line->vezesDeUso = line->vezesDeUso + 1; // vezes de uso = quantas vezes foi acessado tal posição da cache 
    machine->totalCost += cost;
}