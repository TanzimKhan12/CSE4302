/*
 * riscy-uconn simulator.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "simulator.h"
#include "mipsInstructionMap.h"

int debug = 0;
int pipe_trace = 1;
int forwarding_enabled = 1;
int cache_enabled = 1;
int cycles_to_access_dmem = 10; 

static FILE *fptr_pt;

int main(int argc, char *argv[]) {

  if (argc != 4) {
    printf("Incorrect number of arguments\n");
    exit(1);
  }
  else {
    // Open Input file
    FILE *fp;
    fp = fopen(argv[1], "r");

    sscanf(argv[2],"%d",&forwarding_enabled);
    
    if (forwarding_enabled > 1) {
       printf("Incorrect selection for forwarding_enabled. Use either 0 or 1 to select disable or enable forwarding!\n");
       exit(1);
    }

    printf("\n forwarding_enabled = %d", forwarding_enabled);

    sscanf(argv[3],"%d",&cache_enabled);

    if (cache_enabled > 1) {
       printf("Incorrect selection for cache_enabled. Use either 0 or 1 to select disable or enable cache!\n");
       exit(1);
    }

    printf("\n cache_enabled = %d\n", cache_enabled);

    fptr_pt = fopen("pipe_trace.txt","w");

    // Call initialize function for registers and instruction/data memory
    initialize(fp);

    // Process one instruction at a time
    process_instructions();
 
    printf("\n Printing Output Registers \n");

    // Output registers
    rdump();

    // Output Memory
    mdump();

    // Output Cache
    cdump();

    // Dealloc .data
    free(memory);
    memory = NULL;

    printf("\n Data Memory Stats: data accesses = %d, cache hits = %d\n", dcache_accesses, dcache_hits);

    // Dealloc cache
    free(cache);
    cache = NULL;

    // Close File
    fclose (fp);
    return 0;
  }
}

// Function to take in instruciton to parse
void process_instructions() {
   
   int terminate = 0;
   int instruction_counter = 0; //committed instruction count

   // pipeline state and essential initializations
   unsigned int instruction_fetch, instruction_fetch_n;
   instruction_fetch = 0xffffffff; // initial state to decode stage
   struct Registers decode_out, decode_out_n;
   decode_out.inst = 0xffffffff; // initial state to execute stage
   decode_out.x = 0xffffffff;
   decode_out.y = 0xffffffff;
   struct Registers alu_out, alu_out_n;
   alu_out.inst = 0xffffffff; // initial state to memory stage
   alu_out.x = 0xffffffff;
   alu_out.y = 0xffffffff;
   alu_out.memory_flag = 0;
   struct Registers mem_out, mem_out_n;
   mem_out.inst = 0xffffffff; // initial state to write_back stage
   mem_out.x = 0xffffffff;
   mem_out.y = 0xffffffff;
   unsigned int committed_inst; 

   while(terminate != 1)
   {
     // first update all stages with current state
     committed_inst = write_back_stage(mem_out);
     mem_out_n = memory_stage(alu_out);
     alu_out_n = execute(decode_out);
     decode_out_n = decode(instruction_fetch);
     instruction_fetch_n = fetch(instruction_fetch);
      
     if (pipe_trace == 1) {
        fprintf(fptr_pt, "\ncycle %d, currPC %d, nextPC %d: ", cycle, pc, pc_n);
        inst_dump ("Fetch", instruction_fetch_n);
        inst_dump ("Decode", decode_out_n.inst);
        inst_dump ("Execute", alu_out_n.inst);
        inst_dump ("Memory", mem_out_n.inst);
        inst_dump ("Writeback", committed_inst);
        rdump_pt();
     }

     if(debug==1)
        printf("\n cycle: %d IMEM ADDR:%d instruction 0x%08x", cycle, pc/4, instruction_fetch_n);
     
     if ( (committed_inst != 0xffffffff) & (committed_inst != 0x00000000) ) {
        instruction_counter++;
     }
     
     if(debug==1)
        printf("\n cycle: %d commited inst:  instruction 0x%08x", cycle, committed_inst);

     if (registers[0] != 0)
        terminate = 1; // set terminate flag when $zero is updated     

     // second, update all state for next cycle
     if  (dmem_busy == 1) { 
        
        // add code here to hold fetch, decode, execute and memory stages!
     
     }
     else {
        pc = pc_n;
        instruction_fetch = instruction_fetch_n;
        decode_out = decode_out_n;
        alu_out = alu_out_n;
        mem_out = mem_out_n;
     }

     cycle++;

   }//while pc loop
   printf("\n TOTAL %d INSTRUCTIONS EXECUTED IN %d CYCLES", instruction_counter, cycle);
} //process_instruction

// Advance PC
void advance_pc(int step) {
  pc_n += step;
}

unsigned int fetch(unsigned int instruction_fetch) {
  unsigned int inst = 0;
  
  // your code for fetch stage from lab2 goes here. note that this stage needs no further modifications for lab3

  return inst;
}

struct Registers decode(unsigned int instruction_fetch) {
  struct Registers reg_temp;

  // your code for lab3 goes here. note that this is in addition to your lab2 code 

  return reg_temp;
}


struct Registers execute(struct Registers decode_out)
{
  
  // your code for execute stage from lab2 goes here. note that this stage needs no further modifications for lab3 

  return decode_out;
}


struct Registers memory_stage(struct Registers alu_out)
{

  // your code for lab3 goes here. note that this is in addition to your lab2 code  

  return alu_out;
}

unsigned int write_back_stage(struct Registers mem_out)
{
  
  // your code for write-back stage from lab2 goes here. note that this stage needs no further modifications for lab3

  return mem_out.inst;
}

int dcache_lookup(int addr_mem) {
   int hit = 0;

   // your code for cache lookup operation goes here.

   return hit;
}

void dcache_update(int addr_mem) {

   // your code for cache update logic goes here.

}

// Output reigsters
void rdump()
{
    printf("$0      $zero    0x%08x\n", registers[0]);
    printf("$1      $at      0x%08x\n", registers[1]);
    printf("$2      $v0      0x%08x\n", registers[2]);
    printf("$3      $v1      0x%08x\n", registers[3]);
    printf("$4      $a0      0x%08x\n", registers[4]);
    printf("$5      $a1      0x%08x\n", registers[5]);
    printf("$6      $a2      0x%08x\n", registers[6]);
    printf("$7      $a3      0x%08x\n", registers[7]);
    printf("$8      $t0      0x%08x\n", registers[8]);
    printf("$9      $t1      0x%08x\n", registers[9]);
    printf("$10     $t2      0x%08x\n", registers[10]);
    printf("$11     $t3      0x%08x\n", registers[11]);
    printf("$12     $t4      0x%08x\n", registers[12]);
    printf("$13     $t5      0x%08x\n", registers[13]);
    printf("$14     $t6      0x%08x\n", registers[14]);
    printf("$15     $t7      0x%08x\n", registers[15]);
    printf("$16     $s0      0x%08x\n", registers[16]);
    printf("$17     $s1      0x%08x\n", registers[17]);
    printf("$18     $s2      0x%08x\n", registers[18]);
    printf("$19     $s3      0x%08x\n", registers[19]);
    printf("$20     $s4      0x%08x\n", registers[20]);
    printf("$21     $s5      0x%08x\n", registers[21]);
    printf("$22     $s6      0x%08x\n", registers[22]);
    printf("$23     $s7      0x%08x\n", registers[23]);
    printf("$24     $t8      0x%08x\n", registers[24]);
    printf("$25     $t9      0x%08x\n", registers[25]);
    printf("$26     $k0      0x%08x\n", registers[26]);
    printf("$27     $k1      0x%08x\n", registers[27]);
    printf("$28     $gp      0x%08x\n", registers[28]);
    printf("$29     $sp      0x%08x\n", registers[29]);
    printf("$30     $fp      0x%08x\n", registers[30]);
    printf("$31     $ra      0x%08x\n", registers[31]);
    
    printf("  --> pc  0x%08x\n", pc);
}

void rdump_pt() {
   fprintf(fptr_pt, "\n$0 $zero: %d    $8  $t0: %d    $16 $s0: %d    $24 $t8: %d", registers[0], registers[8], registers[16], registers[24]);
   fprintf(fptr_pt, "\n$1 $at: %d      $9  $t1: %d    $17 $s1: %d    $25 $t9: %d", registers[1], registers[9], registers[17], registers[25]);
   fprintf(fptr_pt, "\n$2 $v0: %d      $10 $t2: %d    $18 $s2: %d    $26 $k0: %d", registers[2], registers[10], registers[18], registers[26]);
   fprintf(fptr_pt, "\n$3 $v1: %d      $11 $t3: %d    $19 $s3: %d    $27 $k1: %d", registers[3], registers[11], registers[19], registers[27]);
   fprintf(fptr_pt, "\n$4 $a0: %d      $12 $t4: %d    $20 $s4: %d    $28 $gp: %d", registers[4], registers[12], registers[20], registers[28]);
   fprintf(fptr_pt, "\n$5 $a1: %d      $13 $t5: %d    $21 $s5: %d    $29 $sp: %d", registers[5], registers[13], registers[21], registers[29]);
   fprintf(fptr_pt, "\n$6 $a2: %d      $14 $t6: %d    $22 $s6: %d    $30 $fp: %d", registers[6], registers[14], registers[22], registers[30]);
   fprintf(fptr_pt, "\n$7 $a3: %d      $15 $t7: %d    $23 $s7: %d    $31 $ra: %d\n", registers[7], registers[15], registers[23], registers[31]);
}

// Output Memory
void mdump() {
    FILE *fptr;
    fptr = fopen("mdump.txt","w");
    int i = 0;
    for(i=0;i<16384;i++)
    {
       fprintf(fptr,"\n Memory[%d] = %d",i,memory[i]);
       //fprintf(fptr,"\n Memory[%d] = %d M[0x%08x]",i,memory[i],i);
    }
}

void cdump() {
   FILE *fptr;
   fptr = fopen("cdump.txt","w");
   int i = 0;
   for (i=0;i<NUM_LINES;i++){
      fprintf(fptr,"\n Cache[%d].v = %d,  Cache[%d].tag = %d",i,cache[i].valid,i,cache[i].tag);
   }
}

void inst_dump(char stage[], unsigned int inst) { 
   //printf("\n %s instr: 0x%08x", stage, inst);
  
   int opcode = inst >> 26;
   
   unsigned int func = inst << 26;
   func = func >> 26;

   int rs = (inst >> 21) & 0x1F;
   int rt = (inst >> 16) & 0x1F;
   int rd = (inst >> 11) & 0x1F;
   int sa = (inst >> 6) & 0x1F;
   int imm = inst & 0xFFFF;
   short shortImm = (short)imm;
   int target = inst & 0x03ffffff;

   if (inst == 0xffffffff)
      fprintf(fptr_pt, "\n %s instr: ", stage);

   if (opcode == RTYPEOP) {
      if (func == ADD)
         fprintf(fptr_pt, "\n %s instr: add $%d, $%d, $%d", stage, rd, rs, rt);
      if (func == SUB)
         fprintf(fptr_pt, "\n %s instr: sub $%d, $%d, $%d", stage, rd, rs, rt);
      if (func == AND)
         fprintf(fptr_pt, "\n %s instr: and $%d, $%d, $%d", stage, rd, rs, rt);
      if (func == OR)
         fprintf(fptr_pt, "\n %s instr: or $%d, $%d, $%d", stage, rd, rs, rt);
      if (func == SLL)
         fprintf(fptr_pt, "\n %s instr: sll $%d, $%d, %d", stage, rd, rt, sa);
      if (func == SRL)
         fprintf(fptr_pt, "\n %s instr: srl $%d, $%d, %d", stage, rd, rt, sa);
      if (func == SLT)
         fprintf(fptr_pt, "\n %s instr: slt $%d, $%d, $%d", stage, rd, rs, rt);
      if (func == JR)
         fprintf(fptr_pt, "\n %s instr: jr $%d", stage, rs);
   }
   
   if (opcode == LW)
      fprintf(fptr_pt, "\n %s instr: lw $%d %d($%d)", stage, rt, imm, rs);
   if (opcode == SW)
      fprintf(fptr_pt, "\n %s instr: sw $%d %d($%d)", stage, rt, imm, rs);
   if (opcode == ANDI)
      fprintf(fptr_pt, "\n %s instr: andi $%d, $%d, %d", stage, rt, rs, imm);
   if (opcode == ADDI)
      fprintf(fptr_pt, "\n %s instr: addi $%d, $%d, %d", stage, rt, rs, imm);
   if (opcode == ORI)
      fprintf(fptr_pt, "\n %s instr: ori $%d, $%d, %d", stage, rt, rs, imm);
   if (opcode == SLTI)
      fprintf(fptr_pt, "\n %s instr: slti $%d, $%d, %d", stage, rt, rs, imm);
   if (opcode == LUI)
      fprintf(fptr_pt, "\n %s instr: lui $%d, %d", stage, rt, imm);
   if (opcode == BEQ)
      fprintf(fptr_pt, "\n %s instr: beq $%d, $%d, %d", stage, rs, rt, shortImm);
   if (opcode == BNE)
      fprintf(fptr_pt, "\n %s instr: bne $%d, $%d, %d", stage, rs, rt, shortImm);

   if (opcode == J)
      fprintf(fptr_pt, "\n %s instr: j %d", stage, target);
   if (opcode == JAL)
      fprintf(fptr_pt, "\n %s instr: jal %d", stage, target);
}

void initialize(FILE *fp)
{
    if (fp == NULL)
    {
      printf("Error opening input file.\n");
      exit(1);
    }

    // Initialize registers values' to 0x0
    for (int i = 0; i < 32; i++)
      registers[i] = 0x0;

    printf("\n Initialized Registers \n");

    // Malloc cache
    cache = (cblock *) malloc(NUM_LINES * sizeof(cblock));
    if (cache == NULL) {
      printf("Not enough memory. Aborting..\n");
      exit(1);
    }
    for (int i=0; i<NUM_LINES; i++) {
      cache[i].valid = 0;
      cache[i].tag = 0;
    }
    printf("\n Initialized Cache \n");

    // Malloc memory
    memory  = (int *)malloc(16384 * sizeof(int));
    if (memory == NULL)
    {
      printf("Not enough memory. Aborting..\n");
      exit(1);
    }

    // Initialize 'memory' array to -1
    for (int i = 0; i < 16384; i++)
    {
      memory[i] = -1;
    }

    printf("\n Initialized Memory \n");

    // Initialize variables for parsing
    char line[MAX_LENGTH+2];
    char *p;
    int i = 0, line_num = 0;
    //int line_num = 0, i = 0, data_line = 0;

    // Copy .text section to memory, breaks at nop
    while (fgets(line, MAX_LENGTH+2, fp) != NULL)
    {
      line_num++;

      // Remove '\n' from 'line'
      p = strchr(line, '\n');
      if (p != NULL)
        *p = '\0';

      memory[i] = getDec(line);
      //printf("\n %d",memory[i]);

      // If 'nop' found, move to 0x8000 / 2048 in memory and break
      //if (strcmp(line, "00000000000000000000000000000000") == 0)
      if (strcmp(line, "11111111111111111111111111111111") == 0)
      {
        memory[i] = 0;
        i = 0x800;
        break;
      }
      else
        i++;
    }

    int j = 2048; //Data Memory Starts at 2048
    for(j=i;j<16384;j++)
    {
      memory[j] = 2;
      //printf("\n %d",memory[i]);
    }

    printf("\n Instructions Read, Memory Offset: %d\n", line_num);

    // Seek fp to first instruction in .data
    char data[MAX_LENGTH+2];
    int bytes = 33 * line_num;
    fseek(fp, bytes, SEEK_SET);

    // Copy .data section to memory
    while (fgets(line, MAX_LENGTH+2, fp) != NULL)
    {
      // Remove '\n' from 'line'
      p = strchr(line, '\n');
      if (p != NULL)
        *p = '\0';

      memory[i] = getDec(line);
      printf("\n Data: %d %d",i,memory[i]);
      i++;
    }

    printf("\n Data put in Memory \n");
}

// Convert a binary string to a decimal value
int getDec(char *bin)
{
    int  b, k, m, n;
    int  len, sum = 0;
    
    // Length - 1 to accomodate for null terminator
    len = strlen(bin) - 1;
    
    // Iterate the string
    for(k = 0; k <= len; k++)
    {
        // Convert char to numeric value
        n = (bin[k] - '0');
        
        // Check the character is binary
        if ((n > 1) || (n < 0))
        {
            return 0;
        }
        
        for(b = 1, m = len; m > k; m--)
            b *= 2;
        
        // sum it up
        sum = sum + n * b;
    }
    return sum;
}

