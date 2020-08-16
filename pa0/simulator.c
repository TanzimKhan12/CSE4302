/*
 * riscy-uconn simulator.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "simulator.h"
#include "mipsInstructionMap.h"

int debug = 0;

int main(int argc, char *argv[]) {

  if (argc != 2) {
    printf("Incorrect number of arguments\n");
    exit(1);
  }
  else {
    // Open Input file
    FILE *fp;
    fp = fopen(argv[1], "r");
    
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

// Function to take in instruciton to parse
void process_instructions() {

   int terminate = 0;
   // Cycle PC
   int instruction_counter = 0;

   while(terminate != 1)
   {
     // set terminate flag when SLL $0, $0, 0 (aka NOOP) is executed
     if (memory[pc/4] == 0)
        terminate = 1;
     
     //Fetch Instruction
     unsigned int instruction_fetch;
     instruction_fetch = fetch();
     if(debug==1)
        printf("\n IMEM ADDR:%d instruction 0x%08x", pc/4, instruction_fetch);
     instruction_counter++;

     // Decode Instruction
     struct Registers decode_out;
     decode_out = decode(instruction_fetch);

     // Execute Instruction
     struct Registers alu_out;
     alu_out = execute(decode_out);

     // Memory Stage
     struct Registers mem_out;
     mem_out = memory_stage(alu_out);

     // Write Back Stage
     write_back_stage(mem_out);
   }//while pc loop
   printf("\n TOTAL INSTRUCTIONS EXECUTED: %d", instruction_counter);
} //process_instruction

// Advance PC
void advance_pc(int step) {
  pc += step;
}

unsigned int fetch() {
  unsigned int inst = 0;
  return inst = memory[pc/4];
}

struct Registers decode(unsigned int instruction_fetch) {
  struct Registers reg_temp;

  // your code for decode phase goes here.

  return reg_temp;
}


struct Registers execute(struct Registers decode_out)
{
  return decode_out;
}


struct Registers memory_stage(struct Registers alu_out)
{
  return alu_out;
}

void write_back_stage(struct Registers mem_out)
{
} // WB Done


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

// Output Memory
void mdump()
{
    FILE *fptr;
    fptr = fopen("mdump.txt","w");
    int i = 0;
    for(i=0;i<16384;i++)
    {
        fprintf(fptr,"\n Memory[%d] = %d",i,memory[i]);
    }
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

