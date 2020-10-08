.text
addi $t9, $zero, 10
loop:
lw $t2, 2049($t1)               #t2 = mem[2049 + t1]     
lw $t3, 2065($t1)               #t3 = mem[2065 + t1]
addi $t1, $t1, 1                #t1++
lw $t4, 2049($t1)               #t4 = mem[2049 + t1]
lw $t5, 2065($t1)               #t5 = mem[2065 + t1]
addi $t1, $t1, 1                #t1++

add $t6, $t2, $t3               #t6 = 20, 60, 100, 140, 180
add $t7, $t4, $t5               #t7 = 40, 80, 120, 160, 200

sw  $t6, 2048($t8)
addi $t8, $t8, 1
sw  $t7, 2048($t8) 
addi $t8, $t8, 1

bne $t1, $t9, loop

addi $zero, $zero, 1        


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


