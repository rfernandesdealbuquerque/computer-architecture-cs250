.text

main:

    li $v0, 4
    la $a0, Number
    syscall
    li $v0, 5
    syscall
    addi $t5, $v0, 1 #$t5 holds input N+1
    li $t0, 1  #$t0 = i
    li $t1, 7

_loop:
    slt $t2, $t0, $t5  #if $t0 < $N+1, $t2 will be 1
    beqz $t2, _endloop
    mul $t3, $t1, $t0 # 7*i, result in t3
    li $v0, 1
    addi, $a0, $t3, 0
    syscall
    li $v0, 4
    la $a0, newline
    syscall
    addi $t0, $t0, 1
    j _loop

_endloop:
    jr $ra


.data

newline: .asciiz "\n" 
Number: .asciiz "N: "
