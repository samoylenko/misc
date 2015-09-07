model tiny
ideal
p386
assume cs:@code, ds:@code, ss:@code

codeseg

	org 100h
start:
	mov ax, cs
	mov ds, ax

	mov ah, 09h
	mov dx, offset banner
	int 21h

input_loop:
	mov ax, 0800h
	int 21h

	cmp al, 30h
	jc  not_number
	cmp al, 3ah
	jnc not_number
	call echo

	sub al, 30h
	and eax, 0ffh
	mov ebx, [dword number]
	imul ebx, [dword ten]
	add ebx, eax
	mov [dword number], ebx

	jmp input_loop

not_number:
	cmp al, '='
	jnz not_eq
	call echo
	call complete_operation
	mov [byte operat], 0

	cmp [dword answer], 0
	jnl more_zero

	mov al, '-'
	call echo

	neg [dword answer]

more_zero:
	mov di, offset buffer
	mov ecx, 0
	mov eax, [dword answer]
bin2asc:
	mov edx, 0
	div [dword ten]
	add dl, 30h
	mov [byte ds:di], dl
	inc di
	inc ecx
	cmp eax, [dword ten]
	jnc bin2asc
	cmp al, 0
	jz no_lead_zero
	add al, 30h
	stosb
	inc ecx
no_lead_zero:
out_num:
	dec di
	mov al, [byte ds:di]
	push ecx
	call echo
	pop ecx
	loop out_num

	mov ah, 09h
	mov dx, offset prompt
	int 21h
	mov [dword number], 0
	mov [dword answer], 0
	jmp input_loop

not_eq:
	cmp al, '+'
	jz operation
	cmp al, '-'
	jz operation
	cmp al, '/'
	jz operation
	cmp al, '*'
	jnz not_operation

operation:
	call echo
	call complete_operation
	mov [byte operat], al
	jmp input_loop

not_operation:
	cmp al, 1bh
	jnz input_loop

	mov ah, 09h
	mov dx, offset endban
	int 21h

	ret

echo:
	push eax
	push edx

	mov dl, al
	mov ah, 02h
	int 21h

	pop edx
	pop eax
	ret

complete_operation:
	push eax

	mov eax, [dword answer]
	cmp [byte operat], 0
	jnz o_not_null
	add eax, [dword number]
	jmp exit_c_o
o_not_null:
	cmp [byte operat], '+'
	jnz o_not_plus
	add eax, [dword number]
	jmp exit_c_o
o_not_plus:
	cmp [byte operat], '-'
	jnz o_not_minus
	sub eax, [dword number]
	jmp exit_c_o
o_not_minus:
	cmp [byte operat], '*'
	jnz o_not_mul
	imul eax, [dword number]
	jmp exit_c_o
o_not_mul:
	cmp [byte operat], '/'
	jnz o_not_div
	mov edx, 0
	idiv [dword number]
	jmp exit_c_o
o_not_div:
exit_c_o:
	mov [dword answer], eax
	mov [dword number], 0
	pop eax
	ret

banner	db "anton + bratan = wazzap", 0dh, 0ah, "input here:"
prompt	db 0dh, 0ah, "> $"
number	dd 0
answer  dd 0
operat  db 0
endban  db 0dh, 0ah, "bye!", 0dh, 0ah, '$'
ten 	dd 10	; may be hex, oct etc. (need minor changes)
buffer  db 256 dup (?)

end start
