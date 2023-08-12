	.intel_syntax noprefix

	.section .text

	.global AND_FRAG

AND_FRAG:
	and rax, QWORD PTR [rbx]
	#update rax with bit wise and of the 8 byte quantity at the location of &y
	add rbx, 8
	#rbx should be updated to equal &y + 8

	int3