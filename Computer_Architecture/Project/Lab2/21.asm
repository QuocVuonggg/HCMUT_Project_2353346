.data
    input:   .space 100          # Nh?p chu?i
    freq:    .space 256          # M?ng t?n su?t
    chars:   .space 256          # M?ng ký t? t??ng ?ng v?i freq
    colon:   .asciiz ": "
    comma:   .asciiz ", "
    newline: .asciiz "\n"

.text
.globl main
main:
    li $v0, 8
    la $a0, input
    li $a1, 100
    syscall

#loop
    li $t0, 0
init_chars:
    la $t1, chars
    add $t2, $t1, $t0
    sb $t0, 0($t2) 		#l?u giá tr? $t0 vào chars[i]

    addi $t0, $t0, 1
    li $t3, 256
    blt $t0, $t3, init_chars

    # ??m t?n su?t
    la $t0, input
    la $t1, freq

count_loop:
    lb $t2, 0($t0)
    beq $t2, 10, sort 		#xu?ng dòng ho?c null -> sort
    beqz $t2, sort

    add $t3, $t1, $t2
    lb $t4, 0($t3)
    addi $t4, $t4, 1
    sb $t4, 0($t3)

    addi $t0, $t0, 1		#t?ng s? ??m lên 1
    j count_loop

#Bubble sort
sort:
    li $t5, 0                  # i
outer_loop:
    li $t6, 0                  # j
    li $t7, 255
    sub $t7, $t7, $t5

inner_loop:
    la $t1, freq
    la $t2, chars
    add $t8, $t1, $t6          # addr_freq[j]
    add $t9, $t2, $t6          # addr_char[j]

    addi $s0, $t6, 1
    add $s1, $t1, $s0          # addr_freq[j+1]
    add $s2, $t2, $s0          # addr_char[j+1]

    lb $s3, 0($t8)             # freq[j]
    lb $s4, 0($s1)             # freq[j+1]

    bgt $s3, $s4, do_swap
    beq $s3, $s4, check_ascii
    j skip_swap

check_ascii:
    lb $s5, 0($t9)             # char[j]
    lb $s6, 0($s2)             # char[j+1]
    bgt $s5, $s6, do_swap
    j skip_swap

do_swap:
    # swap freq
    sb $s4, 0($t8)
    sb $s3, 0($s1)

    # swap char
    lb $s5, 0($t9)
    lb $s6, 0($s2)
    sb $s6, 0($t9)
    sb $s5, 0($s2)

skip_swap:
    addi $t6, $t6, 1
    blt $t6, $t7, inner_loop

    addi $t5, $t5, 1
    li $t3, 255
    blt $t5, $t3, outer_loop

# In k?t qu?
print:
    li $t0, 0
print_loop:
    la $t1, freq
    la $t2, chars
    add $t3, $t1, $t0
    lb $t4, 0($t3)
    beqz $t4, next_char

    # l?y ký t?
    add $t5, $t2, $t0
    lb $a0, 0($t5)

    li $v0, 11     # in ký t?
    syscall

    li $v0, 4      # in ": "
    la $a0, colon
    syscall

    li $v0, 1      # in s?
    move $a0, $t4
    syscall

    li $v0, 4      # in ", "
    la $a0, comma
    syscall

next_char:
    addi $t0, $t0, 1
    li $t6, 256
    blt $t0, $t6, print_loop

    li $v0, 4
    la $a0, newline
    syscall

    li $v0, 10
    syscall
