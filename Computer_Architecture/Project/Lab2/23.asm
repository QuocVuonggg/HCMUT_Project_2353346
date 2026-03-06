.data
prompt:     .asciiz "Please insert an element: "
result1:    .asciiz "Second largest value is "
result2:    .asciiz ", found in index "
comma:      .asciiz ", "
array:      .word 0:10 

.text
.globl main
main:
    li $t0, 0                # i = 0

# 10 elements
input_loop:
    li $v0, 4
    la $a0, prompt
    syscall

    li $v0, 5
    syscall                  # Nh?n input ng??i dùng

    la $t1, array
    mul $t2, $t0, 4
    add $t1, $t1, $t2
    sw $v0, 0($t1)           # L?u vào m?ng

    addi $t0, $t0, 1
    li $t3, 10
    blt $t0, $t3, input_loop

#max
    li $t0, 1
    la $t1, array
    lw $t3, 0($t1)           # max = array[0]

find_max_loop:
    mul $t2, $t0, 4
    la $t1, array
    add $t1, $t1, $t2
    lw $t5, 0($t1)

    slt $t6, $t3, $t5
    bne $t6, $zero, update_max
    j skip_max

update_max:
    move $t3, $t5

skip_max:
    addi $t0, $t0, 1
    li $t7, 10
    blt $t0, $t7, find_max_loop

#second max
    li $t0, 0
    li $t4, -2147483648
second_max_loop:
    mul $t2, $t0, 4
    la $t1, array
    add $t1, $t1, $t2
    lw $t5, 0($t1)

    beq $t5, $t3, skip_second  
    slt $t6, $t4, $t5
    bne $t6, $zero, update_second_max

skip_second:
    addi $t0, $t0, 1
    li $t7, 10
    blt $t0, $t7, second_max_loop
    j print_result

update_second_max:
    move $t4, $t5
    j skip_second


print_result:
    li $v0, 4
    la $a0, result1
    syscall

    li $v0, 1
    move $a0, $t4
    syscall

    li $v0, 4
    la $a0, result2
    syscall

#in các ch? s? là second max
    li $t0, 0
    li $t8, 0              

print_loop:
    mul $t2, $t0, 4
    la $t1, array
    add $t1, $t1, $t2
    lw $t5, 0($t1)

    beq $t5, $t4, print_index
    j next_index

print_index:
    beq $t8, $zero, no_comma
    li $v0, 4
    la $a0, comma
    syscall

no_comma:
    li $v0, 1
    move $a0, $t0
    syscall
    li $t8, 1

next_index:
    addi $t0, $t0, 1
    li $t7, 10
    blt $t0, $t7, print_loop

    li $v0, 10
    syscall
