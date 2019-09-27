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
andi $t4, $t3, 63          # mask 0x0000003f, so $t3=62 or 0x0000003e
#lui $t5, 65535             # $t5=-65536 or 0xffff0000

srl $t5, $t4, 1            # div 2, so $t5=31 or 0x0000001f
srl $t6, $t4, 2            # div 4, so $t6=15 or 0x0000000f
srl $t7, $t4, 3            # div 8, so $t7=7 or 0x00000007

sub $a0, $t5, $t6          # $a0=16 or 0x00000010
and $a1, $t5, $t6          # $a1=15 or 0x0000000f
or $a2, $t5, $t6           # $a2=31 or 0x0000001f


ori $a3, $a0, 3            # $a3=19 or 0x00000013

slti $t0, $a0, 17          # $t0=1 
addi $t0, $t0, 2           # $t0=3 or 0x00000003

sll $zero $zero, 0         # execute NOOP ... done


.data
2048: .word 5119
2049: .word 32


