.text
    addi $r1, $r0, 20
    addi $r2, $r0, 27
    add $r2, $r1, $r2
    addi $r4, $r0, 10
_loop:
    addi $r2, $r2, 1
    output $r2
    addi $r3, $r3, 1
    beq $r3, $r4, _end_loop
    beq $r0, $r0, _loop

_end_loop:
    halt