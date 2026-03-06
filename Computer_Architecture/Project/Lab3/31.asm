.data
prompt_a:    .asciiz "Please insert a: "
prompt_b:    .asciiz "Please insert b: "
prompt_c:    .asciiz "Please insert c: "
msg_two:     .asciiz "x1 = "
msg_and:     .asciiz " and x2 = "
msg_one:     .asciiz "There is one solution, x = "
msg_none:    .asciiz "There is no real solution\n"
newline:     .asciiz "\n"

.text
.globl main

main:
    # Input a
    li $v0, 4
    la $a0, prompt_a
    syscall

    li $v0, 6
    syscall
    mov.s $f2, $f0     # a = $f2

    # Input b
    li $v0, 4
    la $a0, prompt_b
    syscall

    li $v0, 6
    syscall
    mov.s $f4, $f0     # b = $f4

    # Input c
    li $v0, 4
    la $a0, prompt_c
    syscall

    li $v0, 6
    syscall
    mov.s $f6, $f0     # c = $f6

    # Delta
    mul.s $f8, $f4, $f4       # b^2

    li $t0, 4
    mtc1 $t0, $f10
    cvt.s.w $f10, $f10        # f10 = 4.0

    mul.s $f12, $f10, $f2     # 4a
    mul.s $f12, $f12, $f6     # 4ac
    sub.s $f14, $f8, $f12     # delta

    # delta < 0
    li $t1, 0
    mtc1 $t1, $f16
    cvt.s.w $f16, $f16        # f16 = 0.0

    c.lt.s $f14, $f16
    bc1t no_solution

    # delta == 0
    c.eq.s $f14, $f16
    bc1t one_solution

    # delta > 0:
    sqrt.s $f18, $f14         # sqrt(delta)
    neg.s $f20, $f4           # -b
    add.s $f22, $f20, $f18    # -b + sqrt(delta)
    sub.s $f24, $f20, $f18    # -b - sqrt(delta)

    li $t2, 2
    mtc1 $t2, $f26
    cvt.s.w $f26, $f26        # f26 = 2.0

    mul.s $f28, $f2, $f26     # 2a
    div.s $f30, $f22, $f28    # x1
    div.s $f4,  $f24, $f28    # x2 (l?u t?m vào f4 ?? tránh ?è f12)

    # In x1 and x2
    li $v0, 4
    la $a0, msg_two
    syscall

    li $v0, 2
    mov.s $f12, $f30
    syscall

    li $v0, 4
    la $a0, msg_and
    syscall

    li $v0, 2
    mov.s $f12, $f4
    syscall

    j end_program

one_solution:
    # x = -b / 2a
    neg.s $f20, $f4
    li $t3, 2
    mtc1 $t3, $f26
    cvt.s.w $f26, $f26        # 2.0
    mul.s $f28, $f2, $f26
    div.s $f12, $f20, $f28

    li $v0, 4
    la $a0, msg_one
    syscall

    li $v0, 2
    syscall

    j end_program

no_solution:
    li $v0, 4
    la $a0, msg_none
    syscall

end_program:
    li $v0, 10
    syscall
