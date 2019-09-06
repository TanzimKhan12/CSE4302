.text
addi $a0, $zero, 2         # argument 0 = 2
addi $a1, $zero, 3         # argument 1 = 3
addi $a2, $zero, 4         # argument 2 = 4
addi $a3, $zero, 5         # argument 3 = 5
add $t4, $zero, $zero
jal diffofsums             # call procedure
add $s0, $v0, $zero        # y = returned value
sll $zero $zero, 0         # execute NOOP ... done

diffofsums:
add $t0, $a0, $a1          # $t0 = f + g
add $t1, $a2, $a3          # $t1 = h + i
sub $s0, $t0, $t1          # result = (f+g)-(h+i)
add $v0, $s0, $zero        # put return value in $v0
jr $ra                     # return to caller

.data
2048: .word 10
2049: .word 10
