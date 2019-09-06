.text
add $a0, $zero, $zero      # set $a0=0 : 0 --> i

loop:
#addi $t0, $zero, 2048     # set $t0 to 2048
ori $t0, $zero, 2048       # set $t0 to 2048
#lui $t0, 0                # $t0 <-- 0x00000000
#ori $t0, $t0, 0           # $t0 <-- $t0 | 0x0800 : $t0 = 0x00000800
sll $t1, $a0, 2            # $t1 <-- $a0 << 2 : $t1 <-- i*4
add $t0, $t0, $t1          # form address of array[i] in $t0
sw $zero, 0($t0)           # store 32-bits of zero from $zero into array[i]
addi $a0, $a0, 1           # i++
slti $t0, $a0, 10          # set $t0=1 if $a0 < 10 otherwise $t0=0
bne $t0, $zero, loop       # if $t0=0, branch to end label
sll $zero $zero, 0         # execute NOOP ... done

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

