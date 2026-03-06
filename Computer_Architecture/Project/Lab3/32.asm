.data
prompt_u: .asciiz "Please insert u: "
prompt_v: .asciiz "Please insert v: "
prompt_a: .asciiz "Please insert a: "
prompt_b: .asciiz "Please insert b: "
prompt_c: .asciiz "Please insert c: "
prompt_d: .asciiz "Please insert d: "
prompt_e: .asciiz "Please insert e: "
result_msg: .asciiz "The result is: "

float_7: .float 7.0
float_6: .float 6.0
float_2: .float 2.0

.text
.globl main

main:
    # Input u
    li $v0, 4
    la $a0, prompt_u
    syscall
    li $v0, 6
    syscall
    mov.s $f20, $f0  # u

    # Input v
    li $v0, 4
    la $a0, prompt_v
    syscall
    li $v0, 6
    syscall
    mov.s $f22, $f0  # v

    # Input a
    li $v0, 4
    la $a0, prompt_a
    syscall
    li $v0, 6
    syscall
    mov.s $f2, $f0  # a

    # Input b
    li $v0, 4
    la $a0, prompt_b
    syscall
    li $v0, 6
    syscall
    mov.s $f4, $f0  # b

    # Input c
    li $v0, 4
    la $a0, prompt_c
    syscall
    li $v0, 6
    syscall
    mov.s $f6, $f0  # c

    # Input d
    li $v0, 4
    la $a0, prompt_d
    syscall
    li $v0, 6
    syscall
    mov.s $f8, $f0  # d

    # Input e
    li $v0, 4
    la $a0, prompt_e
    syscall
    li $v0, 6
    syscall
    mov.s $f10, $f0  # e

    # Compute denominator = d^4 + e^3
    mov.s $f12, $f8        # f12 = d
    mul.s $f12, $f12, $f8  # d^2
    mul.s $f12, $f12, $f8  # d^3
    mul.s $f12, $f12, $f8  # d^4

    mov.s $f14, $f10       # f14 = e
    mul.s $f14, $f14, $f10 # e^2
    mul.s $f14, $f14, $f10 # e^3

    add.s $f16, $f12, $f14 # f16 = d^4 + e^3

    # Compute F(u)
    mov.s $f18, $f20
    jal compute_integral
    mov.s $f24, $f0  # store F(u)

    # Compute F(v)
    mov.s $f18, $f22
    jal compute_integral
    sub.s $f24, $f24, $f0  # F(u) - F(v)

    # Divide by denominator
    div.s $f12, $f24, $f16

    # Print result
    li $v0, 4
    la $a0, result_msg
    syscall

    li $v0, 2
    mov.s $f12, $f12
    syscall

    li $v0, 10
    syscall

# Subroutine compute_integral
# Input: $f18 = x; $f2 = a, $f4 = b, $f6 = c
# Output: $f0 = ax^7/7 + bx^6/6 + cx^2/2
compute_integral:
    # ax^7
    mov.s $f1, $f18
    mul.s $f1, $f1, $f18  # x^2
    mul.s $f1, $f1, $f18  # x^3
    mul.s $f1, $f1, $f18  # x^4
    mul.s $f1, $f1, $f18  # x^5
    mul.s $f1, $f1, $f18  # x^6
    mul.s $f1, $f1, $f18  # x^7
    mul.s $f1, $f1, $f2   # ax^7

    l.s $f11, float_7
    div.s $f1, $f1, $f11  # ax^7 / 7

    # bx^6
    mov.s $f3, $f18
    mul.s $f3, $f3, $f18
    mul.s $f3, $f3, $f18
    mul.s $f3, $f3, $f18
    mul.s $f3, $f3, $f18
    mul.s $f3, $f3, $f18
    mul.s $f3, $f3, $f4

    l.s $f11, float_6
    div.s $f3, $f3, $f11  # bx^6 / 6

    # cx^2
    mov.s $f5, $f18
    mul.s $f5, $f5, $f18
    mul.s $f5, $f5, $f6

    l.s $f11, float_2
    div.s $f5, $f5, $f11  # cx^2 / 2

    # total
    add.s $f0, $f1, $f3
    add.s $f0, $f0, $f5

    jr $ra
