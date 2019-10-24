.text
add $t0, $zero, $zero        # i = 0
add $t1, $zero, $zero        # initialize the sum to zero
add $t2, $zero, $zero        # for second loop compare 2
add $t3, $zero, $zero
add $t5, $zero, $zero        # initialize temporary register to zero
add $t6, $zero, $zero        # for sw later
add $t7, $zero, $zero

lw $t1, 2048($t0)          # $t1=20
lw $t2, 2049($t0)          # $t2=32
add $t4, $t1, $t2
lw $t1, 2050($t0)          # $t1=2
add $t4, $t1, $t4
lw $t1, 2051($t0)          # $t1=2
add $t4, $t1, $t4
lw $t1, 2052($t0)          # $t1=3
add $t4, $t1, $t4
lw $t1, 2053($t0)          # $t1=3
add $t4, $t1, $t4
lw $t1, 2054($t0)          # $t1=3
add $t4, $t1, $t4
lw $t1, 2055($t0)          # $t1=3
add $t4, $t1, $t4
lw $t1, 2056($t0)          # $t1=3
add $t4, $t1, $t4
lw $t1, 2057($t0)          # $t1=3
add $t4, $t1, $t4          # $t4 = 74

addi, $zero, $zero, 1      # $zero register should never be updated, so detect this change and quit simulator


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


