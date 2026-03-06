.data
newline:        .asciiz "\n"
space_char:     .byte ' '
colon_space:    .asciiz ": "
space:          .asciiz " "
vertical_border:  .asciiz "|"
horizontal_border:.asciiz "---"
row_prefix:       .asciiz " |"
header_prefix:  .asciiz "   "          # Indent for column header
digit_space:    .asciiz " "           # Space after each digit
board:          .space 225            # 15x15 grid
prompt_p1_msg:      .asciiz "Player 1, please input your coordinates (x,y): "
prompt_p2_msg:      .asciiz "Player 2, please input your coordinates (x,y): "
player2_win_msg:	.asciiz "Player 2 win!!! FATALITY"
player1_win_msg:	.asciiz "Player 1 win!!! FATALITY"
tie_msg:	.asciiz "No Player win!!! It's a tie"
invalid_move_msg:   .asciiz "Invalid move, try again.\n"
input_buffer:   .space 10           # Buffer to store user input
msg_x: .asciiz "Parsed X: "
msg_y: .asciiz "Parsed Y: "
file_name:      .asciiz "D:/MAR/Project/result.txt"
buffer:         .space 32
.text
.globl main

main:
    # (Step 1) Init board with space ' '
    li $t0, 0
    la $t1, board
    li $t2, 225
init_loop:
    li $t3, 0x20
    sb $t3, 0($t1)
    addi $t1, $t1, 1
    addi $t0, $t0, 1
    blt $t0, $t2, init_loop

    # (Step 2) Start game loop (Player 1 and Player 2 alternating)
    li $s7, 0 # Player turn (0 = Player 1, 1 = Player 2)
game_loop:
    # Print board
    jal print_board

    # Check whose turn it is and prompt accordingly
    beqz $s7, prompt_p1      # If $s7 == 0, it's Player 1's turn
    la $a0, prompt_p2_msg    # Otherwise, it's Player 2's turn
    j get_input
prompt_p1:
    la $a0, prompt_p1_msg

get_input:
    # Print the prompt
    li $v0, 4
    syscall

    # Read input (coordinates)
    li $v0, 8
    la $a0, input_buffer
    li $a1, 10            # Maximum input length
    syscall

    # Parse the input (coordinates)
    jal parse_input

    # Validate the move (check if within bounds and cell is empty)
    blt $v0, $zero, invalid_move  # x < 0
    bge $v0, 15, invalid_move     # x >= 15
    blt $v1, $zero, invalid_move  # y < 0
    bge $v1, 15, invalid_move     # y >= 15

    # Check if the cell is empty
    la $t0, board
    mul $t1, $v1, 15      # y * 15 (row * width)
    add $t1, $t1, $v0     # y * 15 + x (row * width + col)
    add $t0, $t0, $t1     # address = board + index
    lb $t2, 0($t0)        # load the cell value
    li $t3, 0x20          # ' ' in ASCII
    bne $t2, $t3, invalid_move  # If cell is not empty, it's an invalid move

    # Place the symbol (X for Player 1, O for Player 2)
    li $t3, 0x58          # 'X'
    beqz $s7, set_symbol
    li $t3, 0x4F          # 'O'
set_symbol:
    sb $t3, 0($t0)        # Store symbol in board

    # Check if the move results in a win or tie
    jal check_winner

    # Toggle player turn
    xori $s7, $s7, 1      # Toggle $s7 between 0 and 1
    j game_loop

invalid_move:
    # Print the invalid move message
    li $v0, 4
    la $a0, invalid_move_msg
    syscall
    j game_loop

##########################
# Print 15x15 Board with borders
##########################
print_board:
    # Print top border
    li $v0, 4
    la $a0, header_prefix     # "   "
    syscall
    
    li $t0, 0                 # Column counter
print_top_border:
    li $v0, 4
    la $a0, horizontal_border # "---"
    syscall
    
    addi $t0, $t0, 1
    li $t1, 15
    blt $t0, $t1, print_top_border
    
    li $v0, 4
    la $a0, newline
    syscall

    # Print column header
    li $v0, 4
    la $a0, header_prefix     # "   "
    syscall
    
    li $v0, 4
    la $a0, vertical_border   # "|"
    syscall

    li $t0, 0                 # col = 0
print_col_header:
    # Handle single vs double-digit column numbers differently
    li $t4, 10
    blt $t0, $t4, single_digit_col_header
    
    # For double-digit column numbers (10-14)
    li $v0, 1                 # syscall to print int
    move $a0, $t0
    syscall
    
    # No extra space for double-digit numbers
    j after_col_number
    
single_digit_col_header:
    # Print column number
    li $v0, 1                 # syscall to print int
    move $a0, $t0
    syscall
    
    # Print space for padding (only for single-digit numbers)
    li $v0, 11                # Print space for padding
    li $a0, ' '
    syscall
    
after_col_number:
    li $v0, 4                 
    la $a0, vertical_border   # Print "|" after each column
    syscall

    addi $t0, $t0, 1
    li $t1, 15
    blt $t0, $t1, print_col_header
print_separator:
    
    addi $t0, $t0, 1
    li $t1, 15
    blt $t0, $t1, print_separator
    
    li $v0, 4
    la $a0, newline
    syscall

    # Print rows
    li $t2, 0                 # row index
print_row:
    # Print row number with special handling for 2 digits
    li $t4, 10
    blt $t2, $t4, single_digit_row
    
    # For 2 digit row numbers
    li $v0, 1
    move $a0, $t2
    syscall
    j after_row_num
    
single_digit_row:
    # For single digit row numbers, add a space for alignment
    li $v0, 11               # print space
    li $a0, ' '
    syscall
    
    li $v0, 1                # print row number
    move $a0, $t2
    syscall

after_row_num:
    li $v0, 4                # print " |"
    la $a0, row_prefix
    syscall

    li $t3, 0                # col index
print_cell:
    la $t4, board
    mul $t5, $t2, 15         # row offset = row * 15
    add $t5, $t5, $t3        # index = row * 15 + col
    add $t4, $t4, $t5        # address = board + index
    lb $a0, 0($t4)           # load cell value

    li $v0, 11               # syscall: print char
    syscall

    li $v0, 11               # Add space after character for width
    li $a0, ' '
    syscall

    li $v0, 4                # Print vertical border
    la $a0, vertical_border
    syscall

    addi $t3, $t3, 1         # col++
    li $t6, 15
    blt $t3, $t6, print_cell

    li $v0, 4                # newline after row
    la $a0, newline
    syscall

    addi $t2, $t2, 1         # row++
    li $t7, 15
    blt $t2, $t7, print_row

    # Print bottom border
    li $v0, 4
    la $a0, header_prefix    # "   "
    syscall
    
    li $t0, 0                # Column counter
print_bottom_border:
    li $v0, 4
    la $a0, horizontal_border # "---"
    syscall
    
    addi $t0, $t0, 1
    li $t1, 15
    blt $t0, $t1, print_bottom_border
    
    li $v0, 4
    la $a0, newline
    syscall

    jr $ra

##########################
# Check Winner Function
##########################
check_winner:
    # Save registers that need to be preserved
    addi $sp, $sp, -16
    sw $ra, 0($sp)
    sw $s0, 4($sp)
    sw $s1, 8($sp)
    sw $s2, 12($sp)

    # Initialize variables
    la $s0, board         # $s0 points to the board
    li $s1, 0             # $s1 = row index
    li $s2, 0             # $s2 = col index

    # Check for a winner
check_rows:
    li $t0, 0             # Reset column index
check_cols:
    # Load the current cell
    mul $t1, $s1, 15      # row * 15
    add $t1, $t1, $t0     # row * 15 + col
    add $t1, $s0, $t1     # address = board + index
    lb $t2, 0($t1)        # Load cell value

    # Skip empty cells
    li $t3, 0x20          # ASCII for ' '
    beq $t2, $t3, next_cell

    # Check horizontal, vertical, and diagonal sequences
    jal check_horizontal
    jal check_vertical
    jal check_diagonal

next_cell:
    addi $t0, $t0, 1      # Move to the next column
    li $t4, 15
    blt $t0, $t4, check_cols

    # Move to the next row
    addi $s1, $s1, 1
    li $t5, 15
    blt $s1, $t5, check_rows

    # Check for a tie
    jal check_tie

    # Restore registers and return
    lw $ra, 0($sp)
    lw $s0, 4($sp)
    lw $s1, 8($sp)
    lw $s2, 12($sp)
    addi $sp, $sp, 16
    jr $ra

##########################
# Check Horizontal
##########################
check_horizontal:
    # Check for 5 consecutive horizontal symbols from current position (s1, t0)
    li $t4, 1             # Initialize counter to 1 (current cell already counted)
    lb $t5, ($t1)         # Get current cell value
    
    # Skip if current cell is empty
    li $t7, 0x20          # ASCII for ' '
    beq $t5, $t7, horizontal_done
    
    # Check next cells to the right
    li $t6, 1             # Start with next column
horizontal_check:
    add $t3, $t0, $t6     # Get column index: t0 (current col) + t6 (offset)
    bge $t3, 15, horizontal_done  # Out of bounds check
    
    mul $t7, $s1, 15      # row * 15
    add $t7, $t7, $t3     # row * 15 + (col+offset)
    add $t7, $s0, $t7     # address = board + index
    lb $t8, 0($t7)        # Load next cell
    
    bne $t8, $t5, horizontal_done  # If different symbol, stop checking
    addi $t4, $t4, 1      # Increment counter
    li $t7, 5             # Check if we have 5 in a row
    beq $t4, $t7, winner_found
    
    addi $t6, $t6, 1      # Check next position
    j horizontal_check
    
horizontal_done:
    jr $ra

##########################
# Check Vertical
##########################
check_vertical:
    # Check for 5 consecutive vertical symbols from current position (s1, t0)
    li $t4, 1             # Initialize counter to 1 (current cell already counted)
    lb $t5, ($t1)         # Get current cell value
    
    # Skip if current cell is empty
    li $t7, 0x20          # ASCII for ' '
    beq $t5, $t7, vertical_done
    
    # Check cells below
    li $t6, 1             # Start with next row
vertical_check:
    add $t3, $s1, $t6     # Get row index: s1 (current row) + t6 (offset)
    bge $t3, 15, vertical_done  # Out of bounds check
    
    mul $t7, $t3, 15      # (row+offset) * 15
    add $t7, $t7, $t0     # (row+offset) * 15 + col
    add $t7, $s0, $t7     # address = board + index
    lb $t8, 0($t7)        # Load next cell
    
    bne $t8, $t5, vertical_done  # If different symbol, stop checking
    addi $t4, $t4, 1      # Increment counter
    li $t7, 5             # Check if we have 5 in a row
    beq $t4, $t7, winner_found
    
    addi $t6, $t6, 1      # Check next position
    j vertical_check
    
vertical_done:
    jr $ra

##########################
# Check Diagonal
##########################
check_diagonal:
    # First diagonal (top-left to bottom-right)
    li $t4, 1             # Initialize counter to 1 (current cell already counted)
    lb $t5, ($t1)         # Get current cell value
    
    # Skip if current cell is empty
    li $t7, 0x20          # ASCII for ' '
    beq $t5, $t7, diagonal2_start
    
    # Check cells in diagonal (down-right)
    li $t6, 1             # Start with offset 1
diag1_check:
    add $t8, $s1, $t6     # Get row index: s1 (current row) + t6 (offset)
    add $t9, $t0, $t6     # Get col index: t0 (current col) + t6 (offset)
    bge $t8, 15, diagonal2_start  # Out of bounds check
    bge $t9, 15, diagonal2_start  # Out of bounds check
    
    mul $t7, $t8, 15      # (row+offset) * 15
    add $t7, $t7, $t9     # (row+offset) * 15 + (col+offset)
    add $t7, $s0, $t7     # address = board + index
    lb $t3, 0($t7)        # Load next cell
    
    bne $t3, $t5, diagonal2_start  # If different symbol, check second diagonal
    addi $t4, $t4, 1      # Increment counter
    li $t7, 5             # Check if we have 5 in a row
    beq $t4, $t7, winner_found
    
    addi $t6, $t6, 1      # Check next position
    j diag1_check

diagonal2_start:
    # Second diagonal (top-right to bottom-left)
    li $t4, 1             # Reset counter to 1
    lb $t5, ($t1)         # Get current cell value
    
    # Skip if current cell is empty
    li $t7, 0x20          # ASCII for ' '
    beq $t5, $t7, diagonal_done
    
    # Check cells in diagonal (down-left)
    li $t6, 1             # Start with offset 1
diag2_check:
    add $t8, $s1, $t6     # Get row index: s1 (current row) + t6 (offset)
    sub $t9, $t0, $t6     # Get col index: t0 (current col) - t6 (offset)
    bge $t8, 15, diagonal_done  # Out of bounds check
    bltz $t9, diagonal_done    # Out of bounds check
    
    mul $t7, $t8, 15      # (row+offset) * 15
    add $t7, $t7, $t9     # (row+offset) * 15 + (col-offset)
    add $t7, $s0, $t7     # address = board + index
    lb $t3, 0($t7)        # Load next cell
    
    bne $t3, $t5, diagonal_done  # If different symbol, done checking
    addi $t4, $t4, 1      # Increment counter
    li $t7, 5             # Check if we have 5 in a row
    beq $t4, $t7, winner_found
    
    addi $t6, $t6, 1      # Check next position
    j diag2_check
    
diagonal_done:
    jr $ra

##########################
# Check Tie
##########################
check_tie:
    # Traverse the board to check if all cells are filled
    la $t1, board         # Start at beginning of board
    li $t0, 0             # Counter for cells
    li $t2, 225           # Total cells
check_tie_loop:
    lb $t3, 0($t1)        # Load cell value
    li $t4, 0x20          # ASCII for ' '
    beq $t3, $t4, not_tie # If any cell is empty, not a tie
    
    addi $t1, $t1, 1      # Move to next cell
    addi $t0, $t0, 1      # Increment counter
    blt $t0, $t2, check_tie_loop
    
    # All cells are filled and no winner, it's a tie
    jal print_board       # Print final board state
    
    li $v0, 4
    la $a0, tie_msg
    syscall
    
    jal write_tie_to_file
    j exit_game

not_tie:
    jr $ra

##########################
# Write Tie to File
##########################
write_tie_to_file:
    # Save registers
    addi $sp, $sp, -12
    sw $ra, 0($sp)
    sw $s0, 4($sp)
    sw $s1, 8($sp)
    
    # Open file for writing
    li $v0, 13           # syscall: open file
    la $a0, file_name    # file name
    li $a1, 1            # flags: 1-write
    li $a2, 0            # mode: ignored
    syscall
    move $s0, $v0        # save file descriptor
    
    # Write top border
    li $v0, 15
    move $a0, $s0
    la $a1, header_prefix    # "   "
    li $a2, 3
    syscall
    
    li $t0, 0                 # Column counter
write_tie_top_border:
    li $v0, 15
    move $a0, $s0
    la $a1, horizontal_border # "---"
    li $a2, 3
    syscall
    
    addi $t0, $t0, 1
    li $t1, 15
    blt $t0, $t1, write_tie_top_border
    
    # Write newline
    li $v0, 15
    move $a0, $s0
    la $a1, newline
    li $a2, 1
    syscall
    
    # Write column header
    li $v0, 15
    move $a0, $s0
    la $a1, header_prefix     # "   "
    li $a2, 3
    syscall
    
    li $v0, 15
    move $a0, $s0
    la $a1, vertical_border   # "|" 
    li $a2, 1
    syscall
    
    # Write column numbers
    li $t0, 0                 # col = 0
write_tie_col_header:
    # Handle single vs double-digit column numbers differently
    li $t4, 10
    blt $t0, $t4, write_tie_single_digit_col
    
    # For double-digit column numbers (10-14)
    la $t1, buffer
    
    # Convert number to ASCII chars
    div $t0, $t4        # Divide by 10
    mflo $t2            # Get tens digit
    addi $t2, $t2, 48   # Convert to ASCII
    sb $t2, 0($t1)      # Store tens digit
    
    mfhi $t2            # Get ones digit
    addi $t2, $t2, 48   # Convert to ASCII
    sb $t2, 1($t1)      # Store ones digit
    
    # Write the number to file
    li $v0, 15
    move $a0, $s0
    la $a1, buffer
    li $a2, 2
    syscall
    
    j write_tie_col_separator
    
write_tie_single_digit_col:
    # Write single digit number
    la $t1, buffer
    addi $t2, $t0, 48   # Convert to ASCII
    sb $t2, 0($t1)      # Store digit
    
    # Add space for padding (for single-digit numbers)
    li $t2, 32          # space
    sb $t2, 1($t1)      # Store space
    
    # Write number and padding
    li $v0, 15
    move $a0, $s0
    la $a1, buffer
    li $a2, 2
    syscall
    
write_tie_col_separator:
    # Write vertical separator
    li $v0, 15
    move $a0, $s0
    la $a1, vertical_border
    li $a2, 1
    syscall
    
    addi $t0, $t0, 1
    li $t1, 15
    blt $t0, $t1, write_tie_col_header
    
    # Write newline
    li $v0, 15
    move $a0, $s0
    la $a1, newline
    li $a2, 1
    syscall
    
    # Write separator line
    li $v0, 15
    move $a0, $s0
    la $a1, header_prefix    # "   "
    li $a2, 3
    syscall
    
    li $t0, 0                # Column counter
write_tie_separator_line:
    li $v0, 15
    move $a0, $s0
    la $a1, horizontal_border # "---"
    li $a2, 3
    syscall
    
    addi $t0, $t0, 1
    li $t1, 15
    blt $t0, $t1, write_tie_separator_line
    
    # Write newline
    li $v0, 15
    move $a0, $s0
    la $a1, newline
    li $a2, 1
    syscall
    
    # Write rows
    li $t2, 0                 # row index
write_tie_board_row:
    # Print row number with special handling for 2 digits
    li $t4, 10
    blt $t2, $t4, write_tie_single_digit_row
    
    # For 2 digit row numbers
    la $t1, buffer
    
    # Convert number to ASCII chars
    div $t2, $t4        # Divide by 10
    mflo $t3            # Get tens digit
    addi $t3, $t3, 48   # Convert to ASCII
    sb $t3, 0($t1)      # Store tens digit
    
    mfhi $t3            # Get ones digit
    addi $t3, $t3, 48   # Convert to ASCII
    sb $t3, 1($t1)      # Store ones digit
    
    # Write row number
    li $v0, 15
    move $a0, $s0
    la $a1, buffer
    li $a2, 2
    syscall
    
    j after_tie_row_num_write
    
write_tie_single_digit_row:
    # For single digit row numbers, add a space for alignment
    la $t1, buffer
    li $t3, 32          # space
    sb $t3, 0($t1)      # Store space
    
    addi $t3, $t2, 48   # Convert to ASCII
    sb $t3, 1($t1)      # Store digit
    
    # Write padded row number
    li $v0, 15
    move $a0, $s0
    la $a1, buffer
    li $a2, 2
    syscall

after_tie_row_num_write:
    # Write row prefix " |"
    li $v0, 15
    move $a0, $s0
    la $a1, row_prefix
    li $a2, 2
    syscall
    
    # Write cells for this row
    li $t3, 0            # col index
write_tie_board_cell:
    la $s1, board
    mul $t5, $t2, 15     # row offset = row * 15
    add $t5, $t5, $t3    # index = row * 15 + col
    add $t4, $s1, $t5    # address = board + index
    lb $t6, 0($t4)       # load cell value
    
    # Write cell to buffer
    la $t1, buffer
    sb $t6, 0($t1)       # store cell character
    
    # Add space for width
    li $t6, 32          # space
    sb $t6, 1($t1)      # store space
    
    # Write cell content
    li $v0, 15
    move $a0, $s0
    la $a1, buffer
    li $a2, 2           # character + space
    syscall
    
    # Write vertical border
    li $v0, 15
    move $a0, $s0
    la $a1, vertical_border
    li $a2, 1
    syscall
    
    addi $t3, $t3, 1     # col++
    li $t6, 15
    blt $t3, $t6, write_tie_board_cell
    
    # Write newline after row
    li $v0, 15
    move $a0, $s0
    la $a1, newline
    li $a2, 1
    syscall
    
    addi $t2, $t2, 1     # row++
    li $t7, 15
    blt $t2, $t7, write_tie_board_row
    
    # Write bottom border
    li $v0, 15
    move $a0, $s0
    la $a1, header_prefix    # "   "
    li $a2, 3
    syscall
    
    li $t0, 0                # Column counter
write_tie_bottom_border:
    li $v0, 15
    move $a0, $s0
    la $a1, horizontal_border # "---"
    li $a2, 3
    syscall
    
    addi $t0, $t0, 1
    li $t1, 15
    blt $t0, $t1, write_tie_bottom_border
    
    # Write newline
    li $v0, 15
    move $a0, $s0
    la $a1, newline
    li $a2, 1
    syscall
    
    # Write tie message
    li $v0, 15
    move $a0, $s0
    la $a1, tie_msg
    li $t5, 0            # Length counter
tie_len_loop:
    add $t6, $a1, $t5
    lb $t7, 0($t6)
    beqz $t7, tie_len_done
    addi $t5, $t5, 1
    j tie_len_loop
tie_len_done:
    move $a2, $t5        # Length of message
    syscall
    
    # Close the file
    li $v0, 16           # syscall: close file
    move $a0, $s0        # file descriptor
    syscall
    
    # Restore registers
    lw $ra, 0($sp)
    lw $s0, 4($sp)
    lw $s1, 8($sp)
    addi $sp, $sp, 12
    jr $ra

##########################
# Winner Found
##########################
winner_found:
    # Check which player won - need to check the symbol
    lb $t8, ($t1)         # Load the winning symbol
    li $t9, 0x58          # 'X' (Player 1)
    beq $t8, $t9, player1_wins
    
    # Otherwise it's Player 2
    jal print_board
    
    # Print to console
    li $v0, 4
    la $a0, player2_win_msg
    syscall
    
    # Write to file
    jal write_result_to_file
    j exit_game

player1_wins:
    jal print_board
    
    # Print to console
    li $v0, 4
    la $a0, player1_win_msg
    syscall
    
    # Write to file
    jal write_result_to_file
    j exit_game

##########################
# Write Result to File
##########################
write_result_to_file:
    # Save registers
    addi $sp, $sp, -16
    sw $ra, 0($sp)
    sw $s0, 4($sp)
    sw $s1, 8($sp)
    sw $s3, 12($sp)      # Save winning symbol
    
    move $s3, $a0        # Save winning symbol
    
    # Open file for writing
    li $v0, 13           # syscall: open file
    la $a0, file_name    # file name
    li $a1, 1            # flags: 1-write
    li $a2, 0            # mode: ignored
    syscall
    move $s0, $v0        # save file descriptor
    
    # Write top border
    li $v0, 15
    move $a0, $s0
    la $a1, header_prefix    # "   "
    li $a2, 3
    syscall
    
    li $t0, 0                 # Column counter
write_top_border:
    li $v0, 15
    move $a0, $s0
    la $a1, horizontal_border # "---"
    li $a2, 3
    syscall
    
    addi $t0, $t0, 1
    li $t1, 15
    blt $t0, $t1, write_top_border
    
    # Write newline
    li $v0, 15
    move $a0, $s0
    la $a1, newline
    li $a2, 1
    syscall
    
    # Write column header
    li $v0, 15
    move $a0, $s0
    la $a1, header_prefix     # "   "
    li $a2, 3
    syscall
    
    li $v0, 15
    move $a0, $s0
    la $a1, vertical_border   # "|" 
    li $a2, 1
    syscall
    
    # Write column numbers
    li $t0, 0                 # col = 0
write_col_header:
    # Handle single vs double-digit column numbers differently
    li $t4, 10
    blt $t0, $t4, write_single_digit_col
    
    # For double-digit column numbers (10-14)
    la $t1, buffer
    
    # Convert number to ASCII chars
    div $t0, $t4        # Divide by 10
    mflo $t2            # Get tens digit
    addi $t2, $t2, 48   # Convert to ASCII
    sb $t2, 0($t1)      # Store tens digit
    
    mfhi $t2            # Get ones digit
    addi $t2, $t2, 48   # Convert to ASCII
    sb $t2, 1($t1)      # Store ones digit
    
    # Write the number to file
    li $v0, 15
    move $a0, $s0
    la $a1, buffer
    li $a2, 2
    syscall
    
    j write_col_separator
    
write_single_digit_col:
    # Write single digit number
    la $t1, buffer
    addi $t2, $t0, 48   # Convert to ASCII
    sb $t2, 0($t1)      # Store digit
    
    # Add space for padding (for single-digit numbers)
    li $t2, 32          # space
    sb $t2, 1($t1)      # Store space
    
    # Write number and padding
    li $v0, 15
    move $a0, $s0
    la $a1, buffer
    li $a2, 2
    syscall
    
write_col_separator:
    # Write vertical separator
    li $v0, 15
    move $a0, $s0
    la $a1, vertical_border
    li $a2, 1
    syscall
    
    addi $t0, $t0, 1
    li $t1, 15
    blt $t0, $t1, write_col_header
    
    # Write newline
    li $v0, 15
    move $a0, $s0
    la $a1, newline
    li $a2, 1
    syscall
    
    # Write separator line
    li $v0, 15
    move $a0, $s0
    la $a1, header_prefix    # "   "
    li $a2, 3
    syscall
    
    li $t0, 0                # Column counter
write_separator_line:
    li $v0, 15
    move $a0, $s0
    la $a1, horizontal_border # "---"
    li $a2, 3
    syscall
    
    addi $t0, $t0, 1
    li $t1, 15
    blt $t0, $t1, write_separator_line
    
    # Write newline
    li $v0, 15
    move $a0, $s0
    la $a1, newline
    li $a2, 1
    syscall
    
    # Write rows
    li $t2, 0                 # row index
write_board_row:
    # Print row number with special handling for 2 digits
    li $t4, 10
    blt $t2, $t4, write_single_digit_row
    
    # For 2 digit row numbers
    la $t1, buffer
    
    # Convert number to ASCII chars
    div $t2, $t4        # Divide by 10
    mflo $t3            # Get tens digit
    addi $t3, $t3, 48   # Convert to ASCII
    sb $t3, 0($t1)      # Store tens digit
    
    mfhi $t3            # Get ones digit
    addi $t3, $t3, 48   # Convert to ASCII
    sb $t3, 1($t1)      # Store ones digit
    
    # Write row number
    li $v0, 15
    move $a0, $s0
    la $a1, buffer
    li $a2, 2
    syscall
    
    j after_row_num_write
    
write_single_digit_row:
    # For single digit row numbers, add a space for alignment
    la $t1, buffer
    li $t3, 32          # space
    sb $t3, 0($t1)      # Store space
    
    addi $t3, $t2, 48   # Convert to ASCII
    sb $t3, 1($t1)      # Store digit
    
    # Write padded row number
    li $v0, 15
    move $a0, $s0
    la $a1, buffer
    li $a2, 2
    syscall

after_row_num_write:
    # Write row prefix " |"
    li $v0, 15
    move $a0, $s0
    la $a1, row_prefix
    li $a2, 2
    syscall
    
    # Write cells for this row
    li $t3, 0            # col index
write_board_cell:
    la $s1, board
    mul $t5, $t2, 15     # row offset = row * 15
    add $t5, $t5, $t3    # index = row * 15 + col
    add $t4, $s1, $t5    # address = board + index
    lb $t6, 0($t4)       # load cell value
    
    # Write cell to buffer
    la $t1, buffer
    sb $t6, 0($t1)       # store cell character
    
    # Add space for width
    li $t6, 32          # space
    sb $t6, 1($t1)      # store space
    
    # Write cell content
    li $v0, 15
    move $a0, $s0
    la $a1, buffer
    li $a2, 2           # character + space
    syscall
    
    # Write vertical border
    li $v0, 15
    move $a0, $s0
    la $a1, vertical_border
    li $a2, 1
    syscall
    
    addi $t3, $t3, 1     # col++
    li $t6, 15
    blt $t3, $t6, write_board_cell
    
    # Write newline after row
    li $v0, 15
    move $a0, $s0
    la $a1, newline
    li $a2, 1
    syscall
    
    addi $t2, $t2, 1     # row++
    li $t7, 15
    blt $t2, $t7, write_board_row
    
    # Write bottom border
    li $v0, 15
    move $a0, $s0
    la $a1, header_prefix    # "   "
    li $a2, 3
    syscall
    
    li $t0, 0                # Column counter
write_bottom_border:
    li $v0, 15
    move $a0, $s0
    la $a1, horizontal_border # "---"
    li $a2, 3
    syscall
    
    addi $t0, $t0, 1
    li $t1, 15
    blt $t0, $t1, write_bottom_border
    
    # Write newline
    li $v0, 15
    move $a0, $s0
    la $a1, newline
    li $a2, 1
    syscall
    
    # Write result message
    li $v0, 15
    move $a0, $s0
    
    # Determine which message to write
    li $t9, 0x58          # 'X' (Player 1)
    beq $t8, $t9, write_p1_msg
    
    # Player 2 wins message
    la $a1, player2_win_msg
    li $t5, 0            # Length counter
p2_len_loop:
    add $t6, $a1, $t5
    lb $t7, 0($t6)
    beqz $t7, p2_len_done
    addi $t5, $t5, 1
    j p2_len_loop
p2_len_done:
    move $a2, $t5        # Length of message
    syscall
    j close_file
    
write_p1_msg:
    # Player 1 wins message
    la $a1, player1_win_msg
    li $t5, 0            # Length counter
p1_len_loop:
    add $t6, $a1, $t5
    lb $t7, 0($t6)
    beqz $t7, p1_len_done
    addi $t5, $t5, 1
    j p1_len_loop
p1_len_done:
    move $a2, $t5        # Length of message
    syscall
    
close_file:
    # Close the file
    li $v0, 16           # syscall: close file
    move $a0, $s0        # file descriptor
    syscall
    
    # Restore registers
    lw $ra, 0($sp)
    lw $s0, 4($sp)
    lw $s1, 8($sp)
    lw $s3, 12($sp)
    addi $sp, $sp, 16
    jr $ra

# Integer to string conversion helper
int_to_string:
    addi $sp, $sp, -20
    sw $ra, 0($sp)
    sw $t0, 4($sp)
    sw $t1, 8($sp)
    sw $t2, 12($sp)
    sw $t3, 16($sp)
    
    # $t0 = integer to convert
    # $t1 = buffer address
    
    # Handle special case for 0
    bnez $t0, not_zero
    li $t2, '0'
    sb $t2, 0($t1)
    sb $zero, 1($t1)
    j int_to_string_done
    
not_zero:
    move $t2, $t1        # Copy buffer address
    li $t3, 0            # Digit counter
    
    # Convert digits in reverse order
int_loop:
    beqz $t0, reverse_digits
    rem $t4, $t0, 10     # Get last digit
    div $t0, $t0, 10     # Remove last digit
    addi $t4, $t4, '0'   # Convert to ASCII
    sb $t4, 0($t2)       # Store in buffer
    addi $t2, $t2, 1     # Next buffer position
    addi $t3, $t3, 1     # Increment counter
    j int_loop
    
reverse_digits:
    # Null-terminate the string
    sb $zero, 0($t2)
    
    # Reverse the digits
    move $t2, $t1        # Start of buffer
    add $t4, $t1, $t3    # End of buffer
    addi $t4, $t4, -1    # Adjust to last character
    
reverse_loop:
    bge $t2, $t4, int_to_string_done
    lb $t5, 0($t2)       # Load from start
    lb $t6, 0($t4)       # Load from end
    sb $t6, 0($t2)       # Store end at start
    sb $t5, 0($t4)       # Store start at end
    addi $t2, $t2, 1     # Move start forward
    addi $t4, $t4, -1    # Move end backward
    j reverse_loop
    
int_to_string_done:
    lw $ra, 0($sp)
    lw $t0, 4($sp)
    lw $t1, 8($sp)
    lw $t2, 12($sp)
    lw $t3, 16($sp)
    addi $sp, $sp, 20
    jr $ra

##########################
# Exit Game
##########################
exit_game:
    li $v0, 10
    syscall

##########################
# Parse Input: string -> (x, y)
##########################
parse_input:
    # Save registers that need to be preserved
    addi $sp, $sp, -12
    sw $ra, 0($sp)
    sw $s0, 4($sp)
    sw $s1, 8($sp)
    
    # Assume input format is always correct (x,y)
    # Where 'x' and 'y' are numeric values, we need to extract them
    # Load address of input string
    la $t0, input_buffer   # $t0 -> address of input_buffer
    # Initialize x and y to 0
    li $s0, 0   # $s0 = x (initialize to 0)
    li $s1, 0   # $s1 = y (initialize to 0)
    
    # Parse x (before comma)
parse_x:
    lb $t2, 0($t0)     # Load byte from input_buffer
    beqz $t2, done_parsing  # If null terminator is reached, stop parsing
    beq $t2, 0x2C, parse_y_start  # If comma (',') is found, move to parsing y
    
    # Check if character is a digit
    blt $t2, 0x30, not_a_digit_x  # If less than '0', not a digit
    bgt $t2, 0x39, not_a_digit_x  # If greater than '9', not a digit
    
    # Convert character to integer and accumulate in x
    sub $t2, $t2, 0x30  # Convert ASCII to integer (subtract '0')
    mul $s0, $s0, 10    # Multiply previous value of x by 10
    add $s0, $s0, $t2   # Add the current digit to x
    
not_a_digit_x:
    addi $t0, $t0, 1     # Move to the next character
    j parse_x            # Continue parsing x

# Start parsing y (after comma)
parse_y_start:
    addi $t0, $t0, 1     # Skip over the comma
    
# Parse y (after comma)
parse_y:
    lb $t2, 0($t0)       # Load next character of y
    beqz $t2, done_parsing  # If null terminator is reached, stop parsing
    beq $t2, 0x0A, done_parsing  # If newline is reached, stop parsing
    
    # Check if character is a digit
    blt $t2, 0x30, not_a_digit_y  # If less than '0', not a digit
    bgt $t2, 0x39, not_a_digit_y  # If greater than '9', not a digit
    
    # Convert character to integer and accumulate in y
    sub $t2, $t2, 0x30  # Convert ASCII to integer (subtract '0')
    mul $s1, $s1, 10    # Multiply previous value of y by 10
    add $s1, $s1, $t2   # Add the current digit to y
    
not_a_digit_y:
    addi $t0, $t0, 1     # Move to the next character
    j parse_y            # Continue parsing y
    
done_parsing:
    # Set return values
    move $v0, $s0   # Return x coordinate in $v0
    move $v1, $s1   # Return y coordinate in $v1
    
    # Restore saved registers
    lw $ra, 0($sp)
    lw $s0, 4($sp)
    lw $s1, 8($sp)
    addi $sp, $sp, 12
    
    jr $ra
