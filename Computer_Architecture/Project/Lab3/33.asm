.data
    # File names
    raw_input_filename:  .asciiz "raw_input.txt"
    formatted_filename: .asciiz "formatted_result.txt"

    # Messages for printing
    info_header:       .asciiz "-Student personal information-\n"
    name_label:        .asciiz "Name: "
    id_label:          .asciiz "ID: "
    address_label:     .asciiz "Address: "
    age_label:         .asciiz "Age: "
    religion_label:    .asciiz "Religion: "
    newline:           .asciiz "\n"
    comma:             .asciiz ","

    buffer_read:       .space 256  # Buffer to read from file (adjust size as needed)

.text
.globl main

main:
    # Open the input file for reading
    li   $v0, 13          # system call for open file
    la   $a0, raw_input_filename  # input file name
    li   $a1, 0          # Open for reading
    li   $a2, 0          # mode is ignored
    syscall
    move $s0, $v0          # Save the file descriptor

    # Read from the input file
    li   $v0, 14          # system call for read
    move $a0, $s0          # file descriptor
    la   $a1, buffer_read   # address of buffer to read into
    li   $a2, 255         # Maximum number of bytes to read (adjust as needed)
    syscall

    # Close the input file
    li   $v0, 16          # system call for close file
    move $a0, $s0          # file descriptor to close
    syscall

    # Allocate memory on the heap for the string
    li   $v0, 9          # system call for memory allocation
    li   $a0, 256         # Allocate enough space (adjust as needed)
    syscall
    move $s1, $v0          # Save the address of the allocated memory (our string)
    move $s3, $v0          # Store the *start* of allocated memory

    # Copy the data from the read buffer to the allocated memory
    li   $t0, 0          # index for buffer_read
    li   $t1, 0          # index for allocated memory
copy_loop:
    lb   $t2, buffer_read($t0)   # load a byte from buffer
    sb   $t2, ($s1)        # store the byte in allocated memory
    beqz $t2, copy_done    # if byte is null, end loop
    addi $t0, $t0, 1
    addi $t1, $t1, 1
    addi $s1, $s1, 1       # Increment $s1 *after* storing
    j    copy_loop
copy_done:
    sub  $s4, $s1, $s3    # $s4 = allocated memory size
    move $s1, $s3          # Restore $s1 to start of allocated memory

    # Open the output file for writing
    li   $v0, 13          # system call for open file
    la   $a0, formatted_filename # output file name
    li   $a1, 1          # Open for writing
    li   $a2, 0          # mode is ignored
    syscall
    move $s2, $v0          # Save the file descriptor

    # Print the formatted output to the console and write to the file
    # Print header
    li   $v0, 4
    la   $a0, info_header
    syscall
    li   $v0, 15           #write to file
    move $a0, $s2
    la   $a1, info_header
    li   $a2, 31 #length of info_header
    syscall

    # Initialize address for parsing
    move $t0, $s3

    # Print Name
    li   $v0, 4
    la   $a0, name_label
    syscall
    li   $v0, 15
    move $a0, $s2
    la   $a1, name_label
    li   $a2, 6
    syscall
    jal print_value

    # Print ID
    li   $v0, 4
    la   $a0, id_label
    syscall
    li   $v0, 15
    move $a0, $s2
    la   $a1, id_label
    li   $a2, 4
    syscall
    jal print_value

    # Print Address
    li   $v0, 4
    la   $a0, address_label
    syscall
    li   $v0, 15
    move $a0, $s2
    la   $a1, address_label
    li   $a2, 9
    syscall
    jal print_value

    # Print Age
    li   $v0, 4
    la   $a0, age_label
    syscall
    li   $v0, 15
    move $a0, $s2
    la   $a1, age_label
    li   $a2, 5
    syscall
    jal print_value

    # Print Religion
    li   $v0, 4
    la   $a0, religion_label
    syscall
    li   $v0, 15
    move $a0, $s2
    la   $a1, religion_label
    li   $a2, 10
    syscall
    jal print_value

    # Close the output file
    li   $v0, 16          # system call for close file
    move $a0, $s2          # file descriptor to close
    syscall

    li   $v0, 10          # Exit the program
    syscall

# Function to print a value until a comma or end of string, and write to file
print_value:
    # Save registers
    addi $sp, $sp, -8
    sw   $ra, 0($sp)
    sw   $s0, 4($sp)

    move $s0, $a0      # Save file descriptor

print_value_loop:
    lb   $a0, ($t0)       # Load a byte from the value string
    beqz $a0, print_newline # End of string
    beq  $a0, 44, print_newline # If it's a comma, go to newline

    # Print character to console
    li   $v0, 11
    syscall

    # Write character to file
    li   $v0, 15
    move $a1, $t0
    li   $a2, 1
    move $a0, $s0
    syscall

    addi $t0, $t0, 1      # Move to the next character
    j    print_value_loop

print_newline:
    # Print newline to console
    li   $v0, 4
    la   $a0, newline
    syscall
    li   $v0, 15
    move $a0, $s0
    la   $a1, newline
    li   $a2, 1
    syscall
    addi $t0, $t0, 1      # Skip the comma for the next value

    # Restore registers
    lw   $ra, 0($sp)
    lw   $s0, 4($sp)
    addi $sp, $sp, 8
    jr   $ra