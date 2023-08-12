	.intel_syntax noprefix

	.section .data

	.global SUM_POSITIVE
	.global SUM_NEGATIVE
SUM_POSITIVE:		.quad 0 #set aside 8bytes
SUM_NEGATIVE:		.quad 0 #set aside 8bytes

	.section .text

	.global SUM_FRAG

SUM_FRAG:

	add rax, QWORD PTR [rbx]
	#update rax by adding y quantity at the location of &y
	mov r8, QWORD PTR [SUM_POSITIVE]
	#r8 is holding the value in SUM_POSITIVE
	cmp QWORD PTR [rbx], 0
	#compare 8 byte value at the address in rbx to 0
	jl is_neg
	#if negative (y<0) jump to negative case
	add r8, QWORD PTR [rbx]
	#Positive Case
	mov QWORD PTR [SUM_POSITIVE], r8
	#set SUM_POSITIVE to r8
	jmp done_cond
	#end of positive case goto condition end

is_neg:
	mov r9, QWORD PTR [SUM_NEGATIVE]
	#set r9 to the value in SUM_NEGATIVE
	add r9, QWORD PTR [rbx]
	#Negative Case
	mov QWORD PTR [SUM_NEGATIVE], r9
	#set back the stored value sotred in SUM_NEGATIVE to r9
done_cond:
	add rbx, 8
	#update rbx to point the next value

	ret
	