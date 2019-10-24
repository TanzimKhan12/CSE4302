.text
lw $t1, 2048($t0)          # $t1=5119 or 0x000013f
add $t0, $zero, $zero        
add $t2, $zero, $zero
add $t3, $zero, $zero
add $t5, $zero, $zero
add $t6, $zero, $zero
add $t7, $zero, $zero
addi, $zero, $zero, 1      # $zero register should never be updated, so detect this change and quit simulator


.data
2048: .word 5119
2049: .word 32


