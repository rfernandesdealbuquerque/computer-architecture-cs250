.text

start:

    addi $r1, $r0, 5
    addi $r2, $r0, 1
    add $r3, $r1, $r2

    jr $r2
    j start


