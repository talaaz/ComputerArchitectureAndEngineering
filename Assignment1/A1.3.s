       # 02155, Assignment1, Exercise A1.3
		.data
#aa:		.word 0x2           #initialize multiplicand       
aa:		.word 0b11111111111111111111111111111110           #initialize multiplicand
bb:		.word 0x3            	#initialize multiplier
pp:		.word 0                 #initialize product
		.text
		.globl main

main:
	lw a0, aa			   		# load multiplicand into a0   A 
	lw a1, bb               	# load multiplier into a1     B
#	lw a2, cc
#	lw a3, dd

	jal myMult 			   		# jump and link myMult
    
    mv a1, a0
	li a0, 1             		# load instruction to print in binary form
	ecall
    mv a0, a1
       
	nop 	
	j end				    	# jump to end
	nop
##########################################################################################################################################
myMult:
	addi sp, sp, -28         	# adjust stack to make room for 8 items of the size 32bit (8*4byte)
    sw ra, 0(sp)				# saves return-address
	sw t0, 4(sp)             	# save temporary register t0 for use afterwards
	sw t1, 8(sp)             	# save temporary register t1 for use afterwards
	sw t2, 12(sp)             	# save temporary register t2 for use afterwards
	sw t3, 16(sp)             	# save temporary register t3 for use afterwards
	sw t4, 20(sp)             	# save temporary register t4 for use afterwards
	sw t5, 24(sp)             	# save temporary register t5 for use afterwards
#	sw t6, 28(sp)             	# save temporary register t6 for use afterwards
	
	li t0, 0				    # initialize temporary register for sign_flag
	li t1, 0				    # initialize temporary register for product
	li t2, 0					# initialize temporary register for test_on_least_significant_bit
	li t3, 0					# initialize temporary register for counter
	li t4, 1					# initialize temporary register for compare value 1
	li t5, 31					# initialize temporary register for compare value 31
#	li t6, 32					# initialize temporary register for compare value 32
	
	jal checkSign		    	#jump and link checkSign which handels signed multiplication
       
	jal testMultiplier       	# jump and link testMultiplier
	
	jal TestSign
	
	lw ra, 0(sp)				# restores return-address
    lw t0, 4(sp)             	# restore register t0 for caller
	lw t1, 8(sp)          		# restore register t1 for caller
	lw t2, 12(sp)           	# restore register t2 for caller
	lw t3, 16(sp)           	# restore register t3 for caller
	lw t4, 20(sp)           	# restore register t4 for caller
	lw t5, 24(sp)           	# restore register t5 for caller
#	lw t6, 28(sp)           	# restore register t6 for caller
	addi sp, sp, 28          	# adjust stack to delete 8 items
    
jr ra						    #jump to the restored return-address
##########################################################################################################################################
checkSign:
	blt a0, x0,converta0		# convert the multiplicand to positive number
    blt a1, x0,converta1		# convert the multiplier to positive number
	xor t0, t0, t1				# xor the sign_flag_multiplicand and sign_flag_multiplier and store it in sign_flag
	li t1, 0				    # initialize temporary register for product
    jr ra

converta0:
	addi a0, a0, -1				# 2 instructions inverting to 2-compliment
	xori a0, a0, -1				#
	li t0, 1					# load the negative sign to t0
    j checkSign

converta1:
	addi a1, a1, -1				# 2 instructions inverting to 2-compliment
	xori a1, a1, -1				#
	li t1, 1					# load the negative sign to t0
    j checkSign
##########################################################################################################################################
testMultiplier:
	andi t2, a1, 1				# load the last bit of the multipler into temp reg t3
	beq t2, t4, multiplier1		# If the least significant bit of the multiplier(t2) is equal 1(t4), go to multiplier1
#    beq t0, x0, shift			# if unsigned (sign_flag(t0) is equal 0(x0)) shift for 32 iterations
	beq t0, t4, shiftSigned		# if signed (sign_flag(t0) is equal 1(t4)) shift for 31 iterations
	jr ra

multiplier1:
	add t1, t1, a0				# add multiplicand(a0) to product(t1) and place the result in product register(t1)   
       
#shift:
#	slli a0, a0, 1				# shift the multiplicand register left 1 bit	
#	srli a1, a1, 1				# shift the multiplier register right 1 bit
#	addi t3, t3, 1				# repetition counter plus one
#	beq t3, t6, Exit			# if its 32th repetition jump to end
#	j testMultiplier			# else jump to testMultiplier
    
shiftSigned:
	slli a0, a0, 1				# shift the multiplicand register left 1 bit	
	srli a1, a1, 1				# shift the multiplier register right 1 bit
	addi t3, t3, 1				# increment counter(t3)
	beq t3, t5, Exit			# if its 31th repetition end loop(counter(t3) equal 31(t5))
	j testMultiplier			# else jump to testMultiplier
Exit:
	jr ra
##########################################################################################################################################
TestSign:
	beq t0, t4, Sign			# if the product should be negativ, make it(sign_flag(t0) equal 1(t4))
    mv a0, t1					# if no sign is needed move product(t1) to a0
    jr ra
    
Sign: 
	xori t1, t1, -1				# 2 instructions getting 2-compliment of the product
	addi a0, t1, 1				# also stores the product in a0
    jr ra
##########################################################################################################################################
end:       
	nop