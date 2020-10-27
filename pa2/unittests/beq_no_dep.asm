.text
addi $t0, $zero, 10
addi $t1, $zero, 10
addi $t2, $zero, 20
addi $t3, $zero, 21
addi $t4, $zero, 22
addi $t5, $zero, 23
beq $t0, $t1, loop
sw $t0, 2048($zero)
sw $t1, 2049($zero)
sw $t2, 2050($zero)
addi $t0, $zero, 30

loop:
lw $t0, 2048($zero)
addi, $zero, $zero, 1      # $zero register should never be updated, so detect this change and quit simulator

.data
2048: .word 5119
2049: .word 32


