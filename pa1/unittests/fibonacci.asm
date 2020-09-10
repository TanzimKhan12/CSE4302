.text
addi $t9, $zero, 10
addi $t1, $zero, 0
addi $t2, $zero, 1
j fibonacci

fibonacci:
add $t3, $t2, $t1
addq $t1, $zero, $t2
add $t2, $zero, $t3
addi $t5, $t5, 1
bne $t5, $t9, fibonacci
addi $zero $zero, 1         # $zero register should never be updated, so detect this change and quit simulator

.data
2048: .word 10
2049: .word 10
2050: .word 20
2051: .word 30
2052: .word 40
2053: .word 50
2054: .word 60
2055: .word 70
2056: .word 80
2057: .word 90
2058: .word 100
2059: .word 3
2060: .word 3
2061: .word 3
2062: .word 3
2063: .word 
2064: .word 11
2065: .word 10
2066: .word 20
2067: .word 30
2068: .word 40
2069: .word 50
2070: .word 60
2071: .word 70
2072: .word 80
2073: .word 90
2074: .word 100
2075: .word 3
2076: .word 3
2077: .word 3
2078: .word 3
2079: .word 3
2080: .word 5


