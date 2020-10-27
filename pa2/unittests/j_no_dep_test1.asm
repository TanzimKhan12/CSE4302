.text
add $t0, $zero, $zero        # iterator i = 0
addi $t2, $zero, 1            # init t2 = 1
add $t3, $zero, $zero        # initialize temporary register to zero
add $t4, $zero, $zero        # initialize temporary register to zero
j jump_test1                # Jump to procedure "jump_test1"

jump_test2:
lw $a1, 2048($t0)            # Load a1 = 2, Mem[2048] = 2, 2 in simulator
lw $a2, 2048($t2)            # Load a2 = 10, Mem[2049] = 10, 10 in simulator
j end                        # Jump to procedure "end"

jump_test1:
add $t5, $zero, $zero        # initialize temporary register to zero
add $t6, $zero, $zero        # initialize temporary register to zero
j jump_test2                # Jump to procedure "jump_test2"

end:
addi, $zero, $zero, 1      # $zero register should never be updated, so detect this change and quit simulator

.data
2048: .word 2
2049: .word 10

