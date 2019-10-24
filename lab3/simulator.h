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

// Global metadata to support pipelining
int pipe_stall = 0;
int br_taken = 0;
int lw_in_exe = 0;
int we_exe, ws_exe, dout_exe = 0;
int we_mem, ws_mem, dout_mem = 0;
int we_wb,  ws_wb,  dout_wb = 0;

int dmem_busy = 0;
int dmem_cycles = 0;

#define NUM_LINES 16 // 1024 bytes (cache size) / 64 bytes (block size)

typedef struct {
   int valid;
   int tag;
} cblock;

cblock *cache;

int dcache_accesses = 0;
int dcache_hits = 0;

void initialize(FILE *fp);
void process_instructions();
int getDec(char *bin);
void isZero(int reg);
void advance_pc(int step);
void rdump();
void rdump_pt();
void mdump();
void cdump();
void inst_dump(char [], unsigned int inst);

unsigned int fetch(unsigned int instuction_fetch);
struct Registers decode(unsigned int instuction_fetch);
struct Registers execute(struct Registers decode_out);
struct Registers memory_stage(struct Registers alu_out);
unsigned int write_back_stage(struct Registers memory_out);

int dcache_lookup(int addr_mem);
void dcache_update(int addr_mem);

#endif /* SIMULATOR_H_ */
