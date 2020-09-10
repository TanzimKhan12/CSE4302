.text
add $a0, $zero, $zero      # set $a0=0 : 0 --> i

loop:
slti $t0, $a0, 10          # set $t0=1 if $a0 < 10 otherwise $t0=0
beq $t0, $zero, end        # if $t0=0, branch to end label
addi $t0, $zero, 2048
sll $t1, $a0, 2            # $t1 <-- $a0 << 2 : $t1 <-- i*4
add $t0, $t0, $t1          # form address of array[i] in $t0
sw $zero, 0($t0)           # store 32-bits of zero from $zero into array[i]
addi $a0, $a0, 1           # i++
beq $zero, $zero, loop     # branch to label loop -- always branches

end:
addi $zero $zero, 1        # $zero register should never be updated, so detect this change and quit simulator

.data
2048: .word 10
2049: .word 32
2050: .word 2
2051: .word 2
2052: .word 5
2053: .word 5
2054: .word 5
2055: .word 5
2056: .word 5

