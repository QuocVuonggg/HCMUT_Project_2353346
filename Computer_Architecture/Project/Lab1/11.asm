.data
hello_msg: .asciiz "Hello, "
newline: .asciiz "\n"
name_buffer: .space 256

.text
.globl main

main:
    li $v0, 8
    la $a0, name_buffer
    li $a1, 256
    syscall

    li $v0, 4
    la $a0, hello_msg
    syscall

    li $v0, 4
    la $a0, name_buffer
    syscall

    li $v0, 4
    la $a0, newline
    syscall

    li $v0, 10
    syscall