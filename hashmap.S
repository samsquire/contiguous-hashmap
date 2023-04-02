	.file	"hashmap.c"
	.text
	.globl	hash
	.type	hash, @function
hash:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	$5381, -8(%rbp)
	jmp	.L2
.L3:
	movq	-8(%rbp), %rax
	salq	$5, %rax
	movq	%rax, %rdx
	movq	-8(%rbp), %rax
	addq	%rax, %rdx
	movl	-12(%rbp), %eax
	cltq
	addq	%rdx, %rax
	movq	%rax, -8(%rbp)
.L2:
	movq	-24(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -24(%rbp)
	movzbl	(%rax), %eax
	movzbl	%al, %eax
	movl	%eax, -12(%rbp)
	cmpl	$0, -12(%rbp)
	jne	.L3
	movq	-8(%rbp), %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	hash, .-hash
	.globl	set_hashmap_string
	.type	set_hashmap_string, @function
set_hashmap_string:
.LFB7:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	hash
	andl	$1023, %eax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rdx
	movq	%rdx, %rax
	salq	$8, %rax
	addq	%rdx, %rax
	salq	$2, %rax
	movq	-24(%rbp), %rdx
	leaq	(%rax,%rdx), %rcx
	movq	-32(%rbp), %rax
	movl	$1024, %edx
	movq	%rax, %rsi
	movq	%rcx, %rdi
	call	memcpy@PLT
	movq	-8(%rbp), %rdx
	movq	%rdx, %rax
	salq	$7, %rax
	addq	%rdx, %rax
	salq	$3, %rax
	leaq	1052672(%rax), %rdx
	movq	-24(%rbp), %rax
	leaq	(%rdx,%rax), %rcx
	movq	-40(%rbp), %rax
	movl	$1024, %edx
	movq	%rax, %rsi
	movq	%rcx, %rdi
	call	memcpy@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	set_hashmap_string, .-set_hashmap_string
	.globl	get_hashmap
	.type	get_hashmap, @function
get_hashmap:
.LFB8:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$40, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	hash
	andl	$1023, %eax
	movq	%rax, -8(%rbp)
	movq	-24(%rbp), %rcx
	movq	-32(%rbp), %rsi
	movq	-8(%rbp), %rdx
	movq	%rdx, %rax
	salq	$7, %rax
	addq	%rdx, %rax
	salq	$3, %rax
	addq	%rsi, %rax
	addq	$1052672, %rax
	movq	%rcx, %rdx
	movl	$1032, %ecx
	movq	(%rax), %rsi
	movq	%rsi, (%rdx)
	movl	%ecx, %esi
	addq	%rdx, %rsi
	leaq	8(%rsi), %rdi
	movl	%ecx, %esi
	addq	%rax, %rsi
	addq	$8, %rsi
	movq	-16(%rsi), %rsi
	movq	%rsi, -16(%rdi)
	leaq	8(%rdx), %rdi
	andq	$-8, %rdi
	subq	%rdi, %rdx
	subq	%rdx, %rax
	addl	%edx, %ecx
	andl	$-8, %ecx
	shrl	$3, %ecx
	movl	%ecx, %edx
	movl	%edx, %edx
	movq	%rax, %rsi
	movq	%rdx, %rcx
	rep movsq
	movq	-24(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	get_hashmap, .-get_hashmap
	.section	.rodata
.LC0:
	.string	"Using %luGB for test.\n"
	.text
	.globl	clone_benchmark
	.type	clone_benchmark, @function
clone_benchmark:
.LFB9:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, -16(%rbp)
	movl	$0, -20(%rbp)
	movl	$2109440, %esi
	movl	$1024, %edi
	call	calloc@PLT
	movq	%rax, -8(%rbp)
	movl	$2, %esi
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L9
.L10:
	movq	-16(%rbp), %rax
	movq	(%rax), %rcx
	movl	-20(%rbp), %eax
	leal	1(%rax), %edx
	movl	%edx, -20(%rbp)
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-8(%rbp), %rax
	addq	%rdx, %rax
	movl	$2109440, %edx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	memcpy@PLT
	movl	-20(%rbp), %eax
	cltd
	shrl	$22, %edx
	addl	%edx, %eax
	andl	$1023, %eax
	subl	%edx, %eax
	movl	%eax, -20(%rbp)
	movq	-16(%rbp), %rax
	movl	12(%rax), %eax
	leal	1(%rax), %edx
	movq	-16(%rbp), %rax
	movl	%edx, 12(%rax)
.L9:
	movq	-16(%rbp), %rax
	movl	8(%rax), %eax
	cmpl	$1, %eax
	je	.L10
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	clone_benchmark, .-clone_benchmark
	.section	.rodata
.LC1:
	.string	"Created array of hashmaps"
.LC2:
	.string	"world"
.LC3:
	.string	"hello"
.LC4:
	.string	"Set hashmap hello"
.LC5:
	.string	"nested"
.LC6:
	.string	"from inner"
.LC7:
	.string	"inside"
.LC8:
	.string	"Found item in hashmap: %s\n"
	.align 8
.LC9:
	.string	"Found item in nested hashmap: %s\n"
.LC10:
	.string	"Size of hashmap: %lu\n"
.LC11:
	.string	"Benchmark run: %ldGB\n\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB10:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$4096, %rsp
	orq	$0, (%rsp)
	subq	$96, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$8, %esi
	movl	$10, %edi
	call	calloc@PLT
	movq	%rax, -4184(%rbp)
	movl	$16, %edi
	call	malloc@PLT
	movq	%rax, -4176(%rbp)
	movq	-4176(%rbp), %rax
	movl	$1, 8(%rax)
	movq	-4184(%rbp), %rax
	movq	(%rax), %rdx
	movq	-4176(%rbp), %rax
	movq	%rdx, (%rax)
	movq	-4176(%rbp), %rax
	movl	$0, 12(%rax)
	movq	-4184(%rbp), %rax
	movq	(%rax), %rax
	leaq	.LC2(%rip), %rdx
	leaq	.LC3(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	set_hashmap_string
	leaq	.LC4(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-4184(%rbp), %rax
	movq	(%rax), %rcx
	leaq	-4160(%rbp), %rax
	leaq	.LC5(%rip), %rdx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	get_hashmap
	movl	-3132(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-4184(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	leaq	.LC6(%rip), %rdx
	leaq	.LC7(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	set_hashmap_string
	movq	-4184(%rbp), %rax
	movq	(%rax), %rcx
	leaq	-3120(%rbp), %rax
	leaq	.LC3(%rip), %rdx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	get_hashmap
	leaq	-3120(%rbp), %rax
	addq	$4, %rax
	movq	%rax, %rsi
	leaq	.LC8(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-4184(%rbp), %rax
	movq	(%rax), %rcx
	leaq	-2080(%rbp), %rax
	leaq	.LC5(%rip), %rdx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	get_hashmap
	movl	-1052(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-4184(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rcx
	leaq	-1040(%rbp), %rax
	leaq	.LC7(%rip), %rdx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	get_hashmap
	leaq	-1040(%rbp), %rax
	addq	$4, %rax
	movq	%rax, %rsi
	leaq	.LC9(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-4176(%rbp), %rdx
	leaq	-4192(%rbp), %rax
	movq	%rdx, %rcx
	leaq	clone_benchmark(%rip), %rdx
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_create@PLT
	movq	$8, -4168(%rbp)
	movq	-4168(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC10(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$5, %edi
	call	sleep@PLT
	movq	-4176(%rbp), %rax
	movl	$0, 8(%rax)
	movq	-4176(%rbp), %rax
	movl	12(%rax), %eax
	cltq
	imulq	-4168(%rbp), %rax
	shrq	$30, %rax
	movq	%rax, %rsi
	leaq	.LC11(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-4192(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_join@PLT
	movl	$600, %edi
	call	sleep@PLT
	movl	$0, %eax
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L13
	call	__stack_chk_fail@PLT
.L13:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
