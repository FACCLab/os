
.global getFirstStack

getFirstStack:
	xor 	%eax,	%eax
	movl	0(%ebp), 	%eax
	ret
