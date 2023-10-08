#ifndef INSTRUCTION_H
#define INSTRUCTION_H

typedef struct {
    int block; // bloco 
    int word;
} Address; // endereço contem o numero e o bloco 

/*exemplo; RAM possui um bloco e palavras */

typedef struct {
    Address add1; // endereço
    Address add2; // endereço 2
    Address add3; // endereço 3
    int opcode; // opção 
} Instruction;

Address getAdd1(Instruction* instruction);
void setAdd1(Instruction* instruction, Address add);

Address getAdd2(Instruction* instruction);
void setAdd2(Instruction* instruction, Address add);

Address getAdd3(Instruction* instruction);
void setAdd3(Instruction* instruction, Address add);

int getOpcode(Instruction* instruction);
void setOpcode(Instruction* instruction, int opcode);

#endif // !INSTRUCTION_H