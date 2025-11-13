	.file	"calc.c"
	.text
	.globl	signalHandler
	.type	signalHandler, @function
signalHandler:
	endbr32
	pushl	%ebp
	movl	%esp, %ebp
	movl	$0, %ebx
	movl	$1, %eax
	int	$0x80
	.size	signalHandler,	.-signalHandler
	.section	.rodata
	
.LC0:
	.string "\"<num1> <op> <num2>\"\n"
.LC1:
	.string "%d %c %d"
.LC2: 
	.string "Result : %d\n"
.LC3:
	.string "Error : Arithmetic Operation Error!\n"
.LC4:
	.string "Error : Division by zero!\n"
	.text
	.globl	main
	.type	main, @function
	
main:
	endbr32
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	pushl	$signalHandler
	pushl	$2
	call	signal
	addl	$8, %esp

.L1 : 
	pushl	$.LC0
	call	printf
	addl	$4, %esp
	leal	-8(%ebp), %eax
	pushl	%eax
	leal	-13(%ebp), %eax
	pushl	%eax
	leal	-4(%ebp), %eax
	pushl	%eax
	pushl	$.LC1
	call	scanf
	addl	$16, %esp
	movl	-13(%ebp), %eax
	cmpb	$43, %al
	jne	.L2
	movl	-4(%ebp), %eax
	movl	-8(%ebp), %edx
	addl	%edx, %eax
	movl	%eax, -12(%ebp)
	pushl	-12(%ebp)
	pushl	$.LC2
	call	printf
	addl	$8, %esp
	jmp	.L1
	
.L2 :
	cmpb	$45, %al
	jne	.L3
	movl	-4(%ebp), %eax
	movl	-8(%ebp), %edx
	subl	%edx, %eax
	movl	%eax, -12(%ebp)
	pushl	-12(%ebp)
	pushl	$.LC2
	call	printf
	addl	$8, %esp
	jmp	.L1
	
.L3 :
	cmpb	$42, %al
	jne	.L4
	movl	-4(%ebp), %eax
	movl	-8(%ebp), %edx
	imull	%edx
	movl	%eax, -12(%ebp)
	pushl	-12(%ebp)
	pushl	$.LC2
	call	printf
	addl	$8, %esp
	jmp	.L1
	
.L4 :
	cmpb	$47, %al
	jne	.L5
	movl	-4(%ebp), %eax
	cdq
	movl	-8(%ebp), %ecx
	testl	%ecx, %ecx
	je	.L6
	idivl	%ecx
	movl	%eax, -12(%ebp)
	pushl	-12(%ebp)
	pushl	$.LC2
	call	printf
	addl	$8, %esp
	jmp	.L1

.L5 : 
	pushl	$.LC3
	call	printf
	addl	$4, %esp
	movl	$1, %ebx
	movl	$1, %eax
	int	$0x80

.L6 :
	pushl	$.LC4
	call	printf
	addl	$4, %esp
	movl	$1, %ebx
	movl	$1, %eax
	int	$0x80
	
	leave
	ret
	.size	main,	.-main
	

	
	
