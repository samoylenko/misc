	bits 32
	org  0x6cc70c

haspmain:
	cmp bh, 0x01
	jne short .not_01

	xor eax, eax
	mov ecx, eax
	inc eax
	mov ebx, eax
	mov edx, eax

	jmp 0x6dba75

.not_01:
	cmp bh, 0x05
	jne short .not_05

	xor eax, eax
	inc eax
	mov ebx, eax
	mov ecx, eax
	mov edx, 0x1d4c

	jmp 0x6dba75

.not_05:
	cmp bh, 0x06
	jne short .not_06

	xor eax, eax
        mov ebx, eax
	mov ecx, eax
	mov edx, eax

	jmp 0x6dba75

.not_06:
	cmp bh, 0x28
	jne short .not_28

	xor eax, eax
	mov ecx, eax
	mov ebx, eax
	mov edx, 0x29a
	              
	jmp 0x6dba75

.not_28:
	cmp bh, 0x32
	jne short .not_32

	mov cl, [bfirst]
	and cl, cl
	jz .f32_notfirst

	xor ecx, ecx
	mov [bfirst], cl
	jmp .f32_ecx0

.f32_notfirst:
	mov ecx, esi
.f32_ecx0:
	mov edx, haspmemory
	mov esi, edi
	shl esi, 1
	add esi, edx
	mov edi, eax

	rep movsw

	mov edx, edi
	mov ebx, ecx
	mov eax, ecx

	jmp 0x6dba75

.not_32:
	cmp bh, 0x33
	jne short .not_33

	mov ecx, esi
	mov edx, haspmemory
	shl edi, 1
	add edi, edx
	mov esi, eax
	add eax, ecx
	mov edx, eax

	rep movsw

	mov ebx, ecx
	mov edx, edi
	mov eax, ecx

	jmp 0x6dba75

.not_33:
	cmp bh, 0x4e
	jne short .default

	mov ecx, 0xffffffe3
	xor eax, eax
	mov ebx, eax
	mov edx, eax

	jmp 0x6dba75

.default:
	xor eax, eax
	mov ecx, eax
	mov ebx, eax
	mov edx, eax

	jmp 0x6dba75

bfirst:
	db 1

haspmemory:
	db 0xe0, 0x7f, 0x61, 0x00, 0x8c, 0x6a, 0x84, 0x00