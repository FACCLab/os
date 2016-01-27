
.global getFirstStack
.global getEspRegister

getFirstStack:
	xor 	%eax,	%eax
	movl	(%ebp), %eax
	ret

getEspRegister:
	xor 	%eax, 	%eax
	movl 	%esp, 	%eax
	ret
