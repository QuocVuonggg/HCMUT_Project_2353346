.data
    prompt_a: .asciiz "a = "
    prompt_b: .asciiz "b = "
    msg_gcd:  .asciiz "GCD = "
    msg_lcm:  .asciiz "LCM = "
    newline:  .asciiz "\n"
    error:    .asciiz "Error: Inputs must be positive integers.\n"

.text
.globl main
main:
    li $v0, 4
    la $a0, prompt_a
    syscall

    # a
    li $v0, 5
    syscall
    move $t0, $v0         # $t0 = a

    blez $t0, invalid_input

    li $v0, 4
    la $a0, prompt_b
    syscall

    #b
    li $v0, 5
    syscall
    move $t1, $v0         # $t1 = b

    blez $t1, invalid_input

    #Gi? bi?n
    move $s0, $t0         # $s0 = a
    move $s1, $t1         # $s1 = b

    #GCD(a, b)
    move $a0, $t0         # a
    move $a1, $t1         # b
    jal gcd
    move $t2, $v0         # $t2 = GCD

    #LCM
    mul $t3, $s0, $s1     # $t3 = a * b
    move $t5, $t2         # $t5 = GCD
    div $t3, $t5          # $t3 / GCD
    mflo $t4              # $t4 = LCM

    li $v0, 4
    la $a0, msg_gcd
    syscall

    li $v0, 1
    move $a0, $t2
    syscall

    li $v0, 4
    la $a0, newline
    syscall

    li $v0, 4
    la $a0, msg_lcm
    syscall

    li $v0, 1
    move $a0, $t4
    syscall

    li $v0, 4
    la $a0, newline
    syscall

    li $v0, 10
    syscall

gcd:
    addi $sp, $sp, -8       # t?o ch? trên stack
    sw $ra, 4($sp)          # l?u ??a ch? tr? v?
    sw $a0, 0($sp)          # l?u a 

    beqz $a1, gcd_end       #b = 0 thì tr? v? a

    move $t6, $a0
    move $t7, $a1
    rem $t8, $t6, $t7       # a % b

    move $a0, $t7           # a = b
    move $a1, $t8           # b = a % b
    jal gcd

    lw $ra, 4($sp)          # khôi ph?c ??a ch? tr? v?
    addi $sp, $sp, 8        # thu g?n stack
    jr $ra

gcd_end:
    move $v0, $a0
    lw $ra, 4($sp)
    addi $sp, $sp, 8
    jr $ra

invalid_input:
    li $v0, 4
    la $a0, error
    syscall
    li $v0, 10
    syscall
