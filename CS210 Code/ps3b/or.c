	.intel_syntax noprefix

	.section .text

	.global OR_FRAG

OR_FRAG:
	or rax, QWORD PTR [rbx]
	#update rax with bitwise or of the 8 byte quantity at the location of &y
	add rbx, 8
	#rbx should be updated to equal &y +8

	int3