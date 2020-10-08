.text
addi $t0, $zero, 10
addi $t1, $zero, 25
addi $t2, $zero, 20
addi $t3, $zero, 19
addi $t4, $zero, 18
sw $t0, 2048($zero)
addi, $zero, $zero, 1      # $zero register should never be updated, so detect this change and quit simulator

.data
2048: .word 5119
2049: .word 32


