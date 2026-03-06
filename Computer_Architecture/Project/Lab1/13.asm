.data
prompt_a: .asciiz "Insert a: "
prompt_b: .asciiz "Insert b: "
prompt_c: .asciiz "Insert c: "
prompt_d: .asciiz "Insert d: "
result_msg: .asciiz "F = "
remainder_msg: .asciiz ", remainder "
newline: .asciiz "\n"

.text
.globl main

main:
    li $v0, 4
    la $a0, prompt_a
    syscall
    li $v0, 5
    syscall
    move $t0, $v0

    li $v0, 4
    la $a0, prompt_b
    syscall
    li $v0, 5
    syscall
    move $t1, $v0

    li $v0, 4
    la $a0, prompt_c
    syscall
    li $v0, 5
    syscall
    move $t2, $v0

    li $v0, 4
    la $a0, prompt_d
    syscall
    li $v0, 5
    syscall
    move $t3, $v0

    addi $t4, $t0, 10

    sub $t5, $t1, $t3

    sll $t6, $t0, 1
    sub $t6, $t2, $t6

    mul $t7, $t4, $t5

    mul $t7, $t7, $t6

    add $t8, $t0, $t1
    add $t8, $t8, $t2

    beqz $t8, exit

    div $t7, $t8
    mflo $t9
    mfhi $s0

    li $v0, 4
    la $a0, result_msg
    syscall

    li $v0, 1
    move $a0, $t9
    syscall

    li $v0, 4
    la $a0, remainder_msg
    syscall

    li $v0, 1
    move $a0, $s0
    syscall

    li $v0, 4
    la $a0, newline
    syscall

exit:
    li $v0, 10
    syscall