
.global getFirstStack

getFirstStack:
	xor 	%eax,	%eax
	movl	(%ebp), %eax
	ret
