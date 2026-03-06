.data
array: .space 20
prompt1: .asciiz "Please input element "
prompt2: .asciiz ": "
prompt_index: .asciiz "Please enter index: "
output_msg: .asciiz "The value at the selected index is: "
newline: .asciiz "\n"

.text
.globl main

main:
    li $t0, 0
    la $t1, array

input_loop:
    bge $t0, 5, input_done

    li $v0, 4
    la $a0, prompt1
    syscall

    li $v0, 1
    move $a0, $t0
    syscall

    li $v0, 4
    la $a0, prompt2
    syscall

    li $v0, 5
    syscall

    sw $v0, 0($t1)
    addi $t1, $t1, 4
    addi $t0, $t0, 1
    j input_loop

input_done:

    li $v0, 4
    la $a0, prompt_index
    syscall

    li $v0, 5
    syscall
    move $t2, $v0

    li $t3, 4
    mul $t2, $t2, $t3
    la $t1, array
    add $t1, $t1, $t2

    lw $t4, 0($t1)

    li $v0, 4
    la $a0, output_msg
    syscall

    li $v0, 1
    move $a0, $t4
    syscall

    li $v0, 4
    la $a0, newline
    syscall

    li $v0, 10
    syscall