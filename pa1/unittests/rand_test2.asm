.text
add $t0, $zero, $zero        
add $t1, $zero, $zero
add $t2, $zero, $zero
add $t3, $zero, $zero
add $t5, $zero, $zero
add $t6, $zero, $zero
add $t7, $zero, $zero


lw $t1, 2048($t0)          # $t1=5119 or 0x000013ff
andi $t2, $t1, 255         # mask 0x000000ff, so $t2=255 or 0x000000ff
sll $t3, $t2, 1            # times 2, so $t3=510 or 0x000001fe
andi $t4, $t3, 63          # mask 0x0000003f, so $t4=62 or 0x0000003e
lui $t5, 65535             # $t5=-65536 or 0xffff0000

addi $zero $zero, 1         # $zero register should never be updated, so detect this change and quit simulator

.data
2048: .word 5119
2049: .word 32


