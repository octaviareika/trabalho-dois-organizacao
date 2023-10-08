#include "instruction.h"

Address getAdd1(Instruction* instruction) {  //manipulação de struct: pega o endereço 1
    return instruction->add1; 
} 

void setAdd1(Instruction* instruction, Address add) { // o endereço 1 vai receber os valores que tem no endereço (palavra e o bloco)
    instruction->add1 = add; 
}

Address getAdd2(Instruction* instruction) { //manipulação de struct: pega o endereço 1
    return instruction->add2; 
}
void setAdd2(Instruction* instruction, Address add) { //o endereço 2 vai receber os valores que tem no endereço (palavra e o bloco)
    instruction->add2 = add; 
}

Address getAdd3(Instruction* instruction) { // anipulação de struct: pega o endereço 3
    return instruction->add3; 
}
void setAdd3(Instruction* instruction, Address add) {// o endereço 2 vai receber os valores que tem no endereço (palavra e o bloco)
     instruction->add3 = add; 
}

int getOpcode(Instruction* instruction) { // retorna a operação 
    return instruction->opcode; 
}

void setOpcode(Instruction* instruction, int opcode) { // operação de codigo vai receber o valor da proṕria operação
    instruction->opcode = opcode; 
}
