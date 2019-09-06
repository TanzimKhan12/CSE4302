.text
add $t0, $zero, $zero        # i = 0
add $t1, $zero, $zero        # initialize the sum to zero
add $t2, $zero, $zero        # for second loop compare 2
add $t3, $zero, $zero
add $t5, $zero, $zero        # initialize temporary register to zero
add $t6, $zero, $zero        # for sw later
add $t7, $zero, $zero

lw $t1, 2048($t0)          # $t1=20
lw $t2, 2048($t1)          # $t2=4
add $t4, $t1, $t2          #   $t4=24
lw $t3, 2048($t4)          # $t3=8
add $t4, $t4, $t3          #   $t4=32
sw $t4, 2048($t0)          # mem[2048]=32
lw $t1, 2048($t0)          # $t1=32 <-- observation of stored value 32 as $t1=0x00000020
lw $t2, 2048($t1)          # $t2=5
add $t4, $t1, $t2          #   $t4=37
sw $t4, 2048($t1)          # mem[2080]=37
lw $t5, 2048($t1)          # $t5=37  <--- observation of stored value 37 as $t5=0x00000025
sll $zero $zero, 0         # execute NOOP ... done


.data
2048: .word 20
2049: .word 32
2050: .word 2
2051: .word 2
2052: .word 3
2053: .word 3
2054: .word 3
2055: .word 3
2056: .word 3
2057: .word 3
2058: .word 3
2059: .word 3
2060: .word 3
2061: .word 3
2062: .word 3
2063: .word 3
2064: .word 3
2065: .word 3
2066: .word 3
2067: .word 3
2068: .word 4
2069: .word 3
2070: .word 3
2071: .word 3
2072: .word 8
2073: .word 3
2074: .word 3
2075: .word 3
2076: .word 3
2077: .word 3
2078: .word 3
2079: .word 3
2080: .word 5


