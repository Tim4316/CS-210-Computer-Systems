	.intel_syntax noprefix
	.section .data
x:	.quad 0
	.section .text

	.global _start

_start:
	xor rax, rax
	#Initializes the RAX register to zero
	mov rbx, OFFSET [CALC_DATA_BEGIN]
	#Initializes RBX to the address of CALC_DATA_BEGIN
loop_start:
	cmp BYTE PTR [rbx], 0
	#check if first byte of current command is zero
	je done_cond
	#if it is then exit your loop and your program
	cmp BYTE PTR [rbx], '|
	#check if second byte of current command is in OR_FRAG
	je or_command
	#if it is then jump to or_command
	cmp BYTE PTR [rbx], '&
	#check if the byte of current command is in AND_FRAG
	je and_command
	#if it is then jum to and_command
	cmp BYTE PTR [rbx], 'S
	#check if the byte of current command is in SUM_FRAG
	je sum_command
	#if it is then jum to sum_command
	cmp BYTE PTR [rbx], 'U
	je upper_command
	#if it is then jump to upper_command
	add rbx, 8
	jmp loop_start
	#jump to loop start
	
or_command:
	add rbx, 8
	#advances RBX by 8
	call OR_FRAG
	#call and return OR_FRAG
	jmp loop_start
	#jump to loop_start
	
and_command:
	add rbx, 8
	#advances RBX by 8
	call AND_FRAG
	#call and return AND_FRAG
	jmp loop_start
	#jump to loop_start
	
sum_command:
	add rbx, 8
	#advances RBX by 8
	call SUM_FRAG
	#call and return SUM_FRAG
	jmp loop_start
	#jum to loop_start

upper_command:
	add rbx, 8
	#advances RBX by 8
	call UPPER_FRAG
	#call and return SUM_FRAG
	jmp loop_start
	#jum to loop_start
	
done_cond:
	mov QWORD PTR [x], rax
	mov rax, 1
	mov rdi, 1
	mov rsi, OFFSET [x]
	mov rdx, 8
	syscall
	#writing to standard out for x
	
	mov rax, 1
	mov rdi, 1
	mov rsi, OFFSET [SUM_POSITIVE]
	mov rdx, 8
	syscall
	#writing to standard out for SUM_POSITIVE

	mov rax, 1
	mov rdi, 1
	mov rsi, OFFSET [SUM_NEGATIVE]
	mov rdx, 8
	syscall
	#writing to standard out for SUM_POSITIVE
	
	mov rax, 1
	mov rdi, 1
	mov r10, OFFSET [CALC_DATA_END]
	mov r9, OFFSET [CALC_DATA_BEGIN]
	sub r10, r9
	mov rsi, OFFSET [CALC_DATA_BEGIN]
	mov rdx, r10 
	syscall
	#writing to standard out for CALC_DATA_BEGIN/END
	
	mov rax, 60
	#rax = 60 os exit system call number
	mov rdi, 0
	#rdi = 0 exit status 0 success
	syscall
	#call OS system call
	