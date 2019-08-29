Mips Assembler
==============

An assembler for a subset of the MIPS like instruction set architecture

# How to use
The assembler will take a file written in assembly language as input on the command line and will produce an output file containing the MIPS machine code. The input file should be in ASCII text. Each line in the input assembly file contains either a mnemonic, a section header (such as .data) or a label (jump or branch target. The maximum length of a line is 4 bytes. Section headers such as .data and .text should be in a line by themselves with no other assembly mnemonic. Similarly, branch targets such as loop: will be on a line by themselves with no other assembly mnemonic. The input assembly file should only contain one data section and one text section. The first section in the file will be the .text section, followed by the .data section.

The assembler supports the following instruction set:

R-Type
- add
- sub
- and
- or
- sll
- slt
- srl
- jr

I-Type
- lw
- sw
- andi
- addi
- slti
- ori
- lui
- beq
- bne

J-Type
- j
- jal

# Run
    to compile the assembler
    $ ./make
    
    to run the assembler on a nop.asm assembly file to write machine code in nop.out
    $ ./assembler nop.asm nop.out
