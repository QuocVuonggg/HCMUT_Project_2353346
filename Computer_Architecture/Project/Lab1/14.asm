.data
prompt: .asciiz "Please enter a positive integer less than 16: "
result_msg: .asciiz "Its binary form is: "
newline: .asciiz "\n"
invalid_input_msg: .asciiz "Invalid input\n"

.text
.globl main

main:
    li $v0, 4
    la $a0, prompt
    syscall

    li $v0, 5
    syscall
    move $t0, $v0

    bltz $t0, invalid_input
    li $t1, 16
    bge $t0, $t1, invalid_input

    li $v0, 4
    la $a0, result_msg
    syscall

    li $t1, 8
loop:
        and $t2, $t0, $t1
        bnez $t2, print_one
        li $a0, 48
        li $v0, 11
        syscall
        j next_bit

    print_one:
        li $a0, 49
        li $v0, 11
        syscall

    next_bit:
        srl $t1, $t1, 1
        bnez $t1, loop

    li $v0, 4
    la $a0, newline
    syscall

    li $v0, 10
    syscall

invalid_input:
    li $v0, 4
    la $a0, invalid_input_msg
    syscall
    j exit

exit:
    li $v0, 10
    syscall