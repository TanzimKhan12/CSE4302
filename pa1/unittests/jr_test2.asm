.text
addi $a0, $zero, 2         # argument 0 = 2
addi $a1, $zero, 3         # argument 1 = 3
addi $a2, $zero, 4         # argument 2 = 4
addi $a3, $zero, 5         # argument 3 = 5
add $t4, $zero, $zero
addi $t4, $t4, 36
jr $t4                     # call procedure
add $s0, $v0, $zero        # y = returned value
addi $zero $zero, 1        # $zero register should never be updated, so detect this change and quit simulator
add $t0, $a0, $a1          # $t0 = f + g
add $t1, $a2, $a3          # $t1 = h + i
sub $s0, $t0, $t1          # result = (f+g)-(h+i)
add $v0, $s0, $zero        # put return value in $v0
add $t6, $zero, $zero
addi $t6, $t6, 28
jr $t6                    # return to caller

.data
2048: .word 10
2049: .word 10
