.text
main:
    addiu $sp, $sp, -4
    sw $ra, 0($sp)

    li $v0, 4
    la $a0, Number
    syscall
    li $v0, 5
    syscall
    move $a0, $v0

    jal function

    move $a0, $v0
    li $v0, 1
    syscall
    la $a0, newline
    li $v0, 4
    syscall

    lw $ra, 0($sp)
    addiu $sp, $sp, 4 

    jr $ra

function:
    addiu $sp, $sp, -8
    sw $ra, 0($sp)
    sw $s0, 4($sp)
    
    beqz $a0, _basecase
    move $s0, $a0
    addi $a0, $a0, -1

    jal function

    li $t1, 3
    addi $s0, $s0, -1
    mul $t1, $t1, $s0
    addi $t1, $t1, 1
    add $v0, $t1, $v0 

    lw $ra, 0($sp)
    lw $s0, 4($sp)
    addiu $sp, $sp, 8

    jr $ra

_basecase: 
    li $v0, 2

    lw $ra, 0($sp)
    lw $s0, 4($sp)
    addiu $sp, $sp, 8

    jr $ra


.data
newline: .asciiz "\n"
Number: .asciiz "N: "




