	.intel_syntax noprefix

	.section .text

	.global UPPER_FRAG

UPPER_FRAG:	
	xor rsi, rsi
	mov rcx, QWORD PTR [rbx]
	push rax
	#set the rsi to 0 and mov the rbx value to rcs address. Also push the rax stack

loop_start:
	mov al, BYTE PTR [rcx+rsi]
	cmp al, 0
	je end
	cmp al, 'a
	jb not_letter
	cmp al, 'z
	ja not_letter
	sub al, 32
	mov BYTE PTR [rcx+rsi], al
	inc rsi
	jmp loop_start
	#we are looping through al. If al is equal to 0, then jump to end. If al is lower than char a, jump to not_letter. If al is higher than char z, also jump to not_letter.
	
not_letter:
	inc rsi
	jmp loop_start
	# increment rsi by 1 and jump to loop start
	
end:
	pop rax
	add rax, rsi
	ret
	# pop the rax stack and add rsi value to rax
	