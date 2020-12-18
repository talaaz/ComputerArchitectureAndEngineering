	# 02155, Assignment1, Exercise 234
	.data
hello:
#	 .asciiz "120"
#    .asciiz "-180"
	.asciiz "+300"
	.text
	.globl main

main:
	la a0, hello			   	# Load x10 with a string

	jal Convert 			   	# Jump and link Convert
    
    mv a1, a0
	li a0, 1             		# Load instruction to print in binary form
	ecall
    mv a0, a1
       
	nop 	
	j end				    	# Jump to end
	nop
##########################################################################################################################################
Convert:
	addi sp, sp, -36         	# adjust stack to make room for 8 items of the size 32bit (8*4byte)
    sw ra, 0(sp)				# saves return-address
	sw t0, 4(sp)             	# save temporary register t0 for use afterwards
	sw t1, 8(sp)             	# save temporary register t1 for use afterwards
	sw t2, 12(sp)             	# save temporary register t2 for use afterwards
	sw t3, 16(sp)             	# save temporary register t3 for use afterwards
	sw t4, 20(sp)             	# save temporary register t4 for use afterwards
	sw t5, 24(sp)             	# save temporary register t5 for use afterwards
	sw t6, 28(sp)             	# save temporary register t6 for use afterwards
    sw a1, 32(sp)				# 10
	
	li t0, 0				    # initialize temporary register for byte
	li t1, 0				    # initialize temporary register for temp byte
	li t2, 0					# initialize temporary register for sign
	li t3, 0x2D					# initialize temporary register for compare value(Ascii "-")
	li t4, 0x2B					# initialize temporary register for compare value(Ascii "+")
	li t5, 0x30					# initialize temporary register for compare value(hex 30)
	li t6, 0x40					# initialize temporary register for compare value(hex 40)
    li a1, 10
	
	jal TestSign		    	#
       
	jal TestByte       			#
    
    jal Sign					#
    
    mv a0, t0
	
	lw ra, 0(sp)				# restores return-address
    lw t0, 4(sp)             	# restore register t0 for caller
	lw t1, 8(sp)          		# restore register t1 for caller
	lw t2, 12(sp)           	# restore register t2 for caller
	lw t3, 16(sp)           	# restore register t3 for caller
	lw t4, 20(sp)           	# restore register t4 for caller
	lw t5, 24(sp)           	# restore register t5 for caller
	lw t6, 28(sp)           	# restore register t6 for caller
    lw a1, 32(sp)
	addi sp, sp, 36          	# adjust stack to delete 8 items
    
jr ra						    #jump to the restored return-address
##########################################################################################################################################
TestSign:
	lb t1, 0(a0)				# Load first byte
    beq t1, t3, NumNeg			# If the loaded byte is equal to ascii "-"
    beq t1, t4, NumPos			# If the loaded byte is equal to ascii "+"
    jr ra
NumNeg:
	addi t2, t2, 1				# set t2 to 1 because the string is a negativ number
#    addi t1, t1, 1				# Set sign_flag high
    addi a0, a0, 1				# Prepare byte
    lb t1, 0(a0)				# Load first byte
    jr ra
NumPos:
#	addi t1, t1, 1				# Set sign_flag high
    addi a0, a0, 1				# Prepare for next byte
    lb t1, 0(a0)				# Load byte
    jr ra
##########################################################################################################################################
TestByte:
	beq t1, x0, Exit
	blt t1, t5, Fail
    bge t1, t6, Fail
    
    mul t0, t0, a1
    sub t1, t1, t5
    add t0, t0, t1

    addi a0, a0, 1				# Prepare for next byte
    lb t1, 0(a0)				# Load byte
    
    j TestByte
Fail:
	addi a0, a0, -1
    j end
##########################################################################################################################################
Sign:
	bne t2, x0, AddSign
    jr ra
AddSign:
	xori t0, t0, -1
	addi t0, t0, 1
    mv t2, x0
    j Sign
##########################################################################################################################################
Exit:       
	jr ra
##########################################################################################################################################
end:       
	nop