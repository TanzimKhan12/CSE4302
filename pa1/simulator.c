/*
 * riscy-uconn simulator.c --- Non pipelined simulator
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "simulator.h"
#include "mipsInstructionMap.h"

int debug = 1;
int pipe_trace = 1;

static FILE *fptr_pt;

int main(int argc, char *argv[]) {

  if (argc != 2) {
    printf("Incorrect number of arguments\n");
    exit(1);
  }
  else {
    // Open Input file
    FILE *fp;
    fp = fopen(argv[1], "r");
    
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

    // Dealloc .data
    free(memory);
    memory = NULL;

    // Close File
    fclose (fp);
    return 0;
  }
}

void process_instructions() {

   int terminate = 0;
   int instruction_counter = 0; //committed instruction count

   while(terminate != 1)
   {
     //Fetch Instruction
     unsigned int instruction_fetch;
     instruction_fetch = 0xffffffff; // initial state
     instruction_fetch = fetch(instruction_fetch);
     cycle++;

     if (pipe_trace == 1) {
         fprintf(fptr_pt, "\ncycle %d, PC %d: ", cycle, pc);
         inst_dump ("Fetch", instruction_fetch);
     }
       
     // Decode Instruction
     struct Registers decode_out;
     decode_out = decode(instruction_fetch);
     cycle++;
       
     if (pipe_trace == 1) {
         fprintf(fptr_pt, "\ncycle %d, PC %d: ", cycle, pc);
         inst_dump ("Decode", decode_out.inst);
     }

     // Execute Instruction
     struct Registers alu_out;
     alu_out = execute(decode_out);
     cycle++;
       
     if (pipe_trace == 1) {
         fprintf(fptr_pt, "\ncycle %d, PC %d: ", cycle, pc);
         inst_dump ("Execute", alu_out.inst);
     }

     // Memory Stage
     struct Registers mem_out;
     mem_out = memory_stage(alu_out);
     cycle++;
       
     if (pipe_trace == 1) {
         fprintf(fptr_pt, "\ncycle %d, PC %d: ", cycle, pc);
         inst_dump ("Memory", mem_out.inst);
     }

     // Write Back Stage
     unsigned int committed_inst;
     committed_inst = write_back_stage(mem_out);
     cycle++;
       
     if (pipe_trace == 1) {
         fprintf(fptr_pt, "\ncycle %d, PC %d: ", cycle, pc);
         inst_dump ("Writeback", committed_inst);
         rdump_pt();
     }
       
     if(debug==1)
         printf("\n cycle: %d IMEM ADDR:%d instruction 0x%08x", cycle, pc/4, instruction_fetch);
    
     if ( (committed_inst != 0xffffffff) & (committed_inst != 0x00000000) ) {
         instruction_counter++;
     }
       
     if (registers[0] != 0)
        terminate = 1; // set terminate flag when $zero is updated
       
   }//while pc loop
   
   printf("\n TOTAL %d INSTRUCTIONS EXECUTEDIN %d CYCLES", instruction_counter, cycle);

} //process_instruction

// Advance PC
void advance_pc(int step) {
  pc += step;
}

unsigned int fetch(unsigned int instruction_fetch) {
  unsigned int inst = 0;
  return inst = memory[pc/4];
}

struct Registers decode(unsigned int instruction_fetch) {
  struct Registers reg_temp;
  // your code for decode phase goes here.

  reg_temp.x = instruction_fetch >> 26;
  reg_temp.y = instruction_fetch & 0x3F;
  reg_temp.rs = (instruction_fetch >> 21) & 0x1F;
  reg_temp.rt = (instruction_fetch >> 16) & 0x1F;
  reg_temp.rd = (instruction_fetch >> 11) & 0x1F;
  reg_temp.sa = (instruction_fetch >> 6) & 0x1F;
  reg_temp.imm = instruction_fetch & 0xFFFF;

  if ((reg_temp.x == LW) | (reg_temp.x == SW))
  {
    reg_temp.memory_flag = 1;
  }
  else
  {
    reg_temp.memory_flag = 0;
  }

  reg_temp.addr_mem = 0x03FFFFFF;
  reg_temp.jmp_out_31 = pc+4;

  reg_temp.A = registers[reg_temp.rs];
  reg_temp.B = registers[reg_temp.rt];
  reg_temp.C = registers[reg_temp.rd];
  if (0x8000 & reg_temp.imm)
  {
     reg_temp.D = 0xFFFF0000 | reg_temp.imm;
  }
  else
  {
     reg_temp.D = reg_temp.imm;
  }
  //Program Counter

  //R-type PC
  if (reg_temp.x == RTYPEOP)
  {
    if (reg_temp.y == JR)
    {
      pc = reg_temp.A;
    }
    else
    {
      advance_pc(4);
    }
  }
  //I-type PC
  else if ((reg_temp.x == LW) | (reg_temp.x == SW) | (reg_temp.x == ANDI) | (reg_temp.x == ADDI) | (reg_temp.x == ORI) | (reg_temp.x == SLTI) | (reg_temp.x == LUI))
  {
    advance_pc(4);
  }
  else if (reg_temp.x == BEQ)
    {
       if (reg_temp.A == reg_temp.B)
       { 
          pc = pc + 4 + reg_temp.D;
       }
       else
       {
         advance_pc(4);
       }
    }
  else if (reg_temp.x == BNE)
   {
      if (reg_temp.A != reg_temp.B)
       {
          pc = pc + 4 + reg_temp.D;
       }
       else
       {
         advance_pc(4);
       }
    }
  else if ((reg_temp.x == J) | (reg_temp.x == JAL))
    {
      pc = (instruction_fetch & 0x03FFFFFF);
    } 
  return reg_temp;
}


struct Registers execute(struct Registers decode_out)
{
  // your code for execute phase goes here.
  //R-Type Functions
  if(decode_out.x == RTYPEOP)
  {
      switch(decode_out.y)
      {
         case ADD :
            decode_out.C = decode_out.A + decode_out.B;
            break;
         case SUB :
            decode_out.C = decode_out.A - decode_out.B;
	    break;
         case AND :
            decode_out.C = decode_out.A & decode_out.B;
	    break;
         case OR :
            decode_out.C = decode_out.A | decode_out.B;
            break;
         case SLL :
            decode_out.C = decode_out.A << decode_out.sa;
            break;
         case SRL :
            decode_out.C = decode_out.A >> decode_out.sa;
            break;
         case SLT :
	    if(decode_out.A < decode_out.B)
	    {
 		decode_out.C = 1;
            }
	    else
            {
                decode_out.C = 0;
	    }
            break;
        default :
            break;
      } 
  }

  //I-Type Functions
  switch(decode_out.x)
  {
     case LW:
         decode_out.addr_mem = decode_out.A + decode_out.D;
         break;
     case SW:
         decode_out.addr_mem = decode_out.A + decode_out.D;
         break;
     case ANDI:
         decode_out.B = decode_out.A & decode_out.D;
         break;
     case ADDI:
         decode_out.B = decode_out.A + decode_out.D;
         break;
     case ORI:
         decode_out.B = decode_out.A | decode_out.D;
         break;
     case SLTI:
         if(decode_out.A < decode_out.D)
         {
             decode_out.B = 1;
         }
         else
         {
             decode_out.B = 0;
         }
         break;
     case LUI:
         decode_out.B = decode_out.D << 16;
         break;
     default:
         break;
  }

   return decode_out;
}


struct Registers memory_stage(struct Registers alu_out)
{
  
  // your code for memory phase goes here.
  //I-Type
  switch(alu_out.x)
  {
    case LW:
       alu_out.B = memory[alu_out.addr_mem];
       break;
    case SW:
       memory[alu_out.addr_mem] = alu_out.B;
       break;
    default:
       break;
  }

  
  return alu_out;
}

unsigned int write_back_stage(struct Registers mem_out)
{
  // your code for write back phase goes here.
  //R-Type
  if((mem_out.x == RTYPEOP) & (mem_out.y != JR))
  {
       registers[mem_out.rd] = mem_out.C;
  }

  //I-type
  if((mem_out.x != SW) & (mem_out.x != BEQ) & (mem_out.x != BNE) & (mem_out.x != RTYPEOP))
  {
      registers[mem_out.rt] = mem_out.B;
  }

  //J-Type
  if(mem_out.x == JAL)
  {
     registers[31] = mem_out.jmp_out_31;
  }


  return mem_out.inst;
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
    }
}

void inst_dump(char stage[], unsigned int inst) {
  
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

