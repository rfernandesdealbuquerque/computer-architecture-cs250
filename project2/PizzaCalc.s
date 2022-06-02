.text
##### STRING == DONE FUNCTION #####
string_equals:
    la $t0, done
    move $t1, $s2
_loop1:
    lb $t2, 0($t0) 
    lb $t3, 0($t1)
    beqz $t2, _endloop1
    seq $t4, $t2, $t3
    beqz $t4, _endloop1
    addi $t0, 1
    addi $t1, 1
    j _loop1

_endloop1:
    move $v0, $t4
    jr $ra

##### PRINT LIST FUNCTION #####
print_list:
    addiu $sp, $sp, -4
    sw $ra, 0($sp)
    
_loop2:
    jal remove_newline

    #print name
    li $v0, 4    
    syscall

    #print float
    l.s $f12, 64($a0)
    li $v0, 2
    syscall

   #print \n
    move $t0, $a0 
    la $a0, newline
    li $v0, 4
    syscall
    move $a0, $t0 #restoring $a0

    lw $a0, 68($a0) #load the address of next node in $a0
    beq $0, $a0, _endloop2 #if $a0 == 0, then we reached the end, so end loop 
    j _loop2

_endloop2:
    lw $ra, 0($sp)
    addiu $sp, $sp, 4
    jr $ra


##### REMOVE \n CHARACTER FUNCTION #####
remove_newline:
    move $t0, $a0
    li $t2, 10
    li $t4, 32

_loop3:
    lb $t1, 0($t0)
    beq $t1, $t2, _endloop3
    addi $t0, $t0, 1
    j _loop3

_endloop3:
    sb $t4, 0($t0)
    jr $ra

##### PIZZA COMPARE FUNCTION ######
#if temp comes before current, function returns 1
pizza_compare:
    addiu $sp, $sp, -4
    sw $ra, 0($sp)
    #current pizza in a0
    #temp pizza in a1

    l.s $f4, 64($a0)
    l.s $f5, 64($a1)

    c.eq.s $f4, $f5
    bc1t _compare_names
    c.lt.s $f4, $f5 #if current.ppd < temp.ppd, then flag == 1
    bc1t _temp_before
    bc1f _temp_after

_compare_names:

    lb $t0, 0($a0)
    lb $t1, 0($a1)

    seq $t2, $t0, $t1
    addi $a0, $a0, 1
    addi $a1, $a1, 1
    bnez $t2, _compare_names

    slt $t2, $t1, $t0 #if temp.name comes before current.name, $t2 == 1
    move $v0, $t2

    lw $ra, 0($sp)
    addiu $sp, $sp, 4
    jr $ra

_temp_before:

    li $v0, 1
    lw $ra, 0($sp)
    addiu $sp, $sp, 4
    jr $ra

_temp_after:

    li $v0, 0

    lw $ra, 0($sp)
    addiu $sp, $sp, 4
    jr $ra

##### MAIN #####

main:
    addiu $sp, $sp, -20
    sw $ra, 0($sp)
    sw $s1, 4($sp)
    sw $s2, 8($sp)
    sw $s3, 12($sp)
    sw $s4, 16($sp)

    # $s1 is HEAD NODE
    # $s2 is TEMP NODE
    # $s3 is CURRENT NODE
    # $s4 is PREV NODE

_loopmain:

#allocating 72 bytes of memory on heap = name(64) + ppd(4) + pointer to next(4)
    li $v0, 9 #code for malloc
    li $a0, 72 #size of struct
    syscall

    move $s2, $v0 #move address of name of pizza to $s2 (making temp node)

    la $a0, name
    li $v0, 4
    syscall

    move $a0, $s2
    li $a1, 64
    li $v0, 8 #code for reading string input
    syscall #read input string and put it on address stored in $a0, which is the same address stored by $s2

    jal string_equals #check if string = DONE
    bnez $v0, _endloopmain #if $v0 = 1, then string == DONE and we end the loop

    la $a0, diameter
    li $v0, 4
    syscall
    li $v0, 6
    syscall
    mov.s $f4, $f0

    la $a0, price
    li $v0, 4
    syscall 
    li $v0, 6
    syscall
    mov.s $f5, $f0

    li.s $f6, 0.0
    c.eq.s $f5, $f6
    s.s $f5, 64($s2) #temp.ppd = 0
    bc1t _skip_float_operations

_not_zero_price:

    la $t0, PI
    l.s $f6, 0($t0)
    li.s $f8 4.0

    mul.s $f7, $f4, $f4
    mul.s $f7, $f6, $f7
    div.s $f7, $f7, $f8
    div.s $f7, $f7, $f5

    s.s $f7, 64($s2) #temp.ppd = result

_skip_float_operations:
    
    sw $0, 68($s2) #temp.next = NULL

    move $s3, $s1 #current = head
    move $s4, $0 #prev = NULL

    bnez $s1, _add_new_head #if head != NULL, jump to see if we add a new head

_add_head:
    
    move $s1, $s2 #setting the head

    j _loopmain
    
_add_new_head:

    move $a0, $s1
    move $a1, $s2

    jal pizza_compare

    beqz $v0, _prepare

    sw $s1, 68($s2) #temp.next = head
    move $s1, $s2 #head = temp

    j _loopmain

_prepare:
    
    move $s4, $s1 #prev = head 
    lw $t0, 68($s1)
    move $s3, $t0 #current = head.next

_while_loop:

    bnez $s3, _endif_2
    sw $s2, 68($s4) #prev.next = temp
    j _loopmain

_endif_2:

    move $a0, $s3 #t0 = current.ppd
    move $a1, $s2 #t1 = temp.ppd

    jal pizza_compare

    beqz $v0, _endif_3
    sw $s2, 68($s4) #prev.next = temp
    sw $s3, 68($s2) #temp.next = current

    j _loopmain

_endif_3:

    move $s4, $s3 #prev = current
    lw $s3, 68($s3) #current = current.next
    j _while_loop

_endloopmain:

#calling print list function
    move $a0, $s1
    jal print_list

    lw $ra, 0($sp)
    lw $s1, 4($sp)
    lw $s2, 8($sp)
    lw $s3, 12($sp)
    lw $s4, 16($sp)
    addiu $sp, $sp, 20

    jr $ra

.data
name: .asciiz "Pizza Name: "
diameter: .asciiz "Diameter: "
price: .asciiz "Price: "
done: .asciiz "DONE"
buffer: .asciiz " "
PI: .float 3.14159265358979323846
numberfour: .float 4.0
newline: .asciiz "\n"
space: .asciiz " "
