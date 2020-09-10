/*
  
 * simulator.h
 * 
 * Simulator header file declaring variables and methods 
 *
 */

#ifndef SIMULATOR_H_
#define SIMULATOR_H_

// Define MAX_LENGTH for number of registers & instruction length
#define MAX_LENGTH 32

// Array of size MAX_LENGTH to hold registers
int registers[MAX_LENGTH];

// PC Register
unsigned int pc = 0;
unsigned int pc_n = 0;

// Memory Allocated 
//unsigned int *memory;
int *memory;

// CLOCK CYCLE
int cycle = 0;

struct Registers
{
     unsigned int inst;
     int x;
     unsigned int y;
     int reg;
     int rs;
     int rt;
     int rd;
     int sa;
     unsigned short imm;

     int memory_flag;
     int addr_mem;   
     int branch_flag;
     int jmp_out_31; 
  
     //ALU Inputs/Outputs
     int A;
     int B;
     int C;
     int D;
    
};

void initialize(FILE *fp);
void process_instructions();
int getDec(char *bin);
void isZero(int reg);
void advance_pc(int step);
void rdump();
void rdump_pt();
void mdump();
void inst_dump(char [], unsigned int inst);

unsigned int fetch(unsigned int instuction_fetch);
struct Registers decode(unsigned int instuction_fetch);
struct Registers execute(struct Registers decode_out);
struct Registers memory_stage(struct Registers alu_out);
unsigned int write_back_stage(struct Registers memory_out);

#endif /* SIMULATOR_H_ */
