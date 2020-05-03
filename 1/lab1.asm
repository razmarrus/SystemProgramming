model small
.stack 100h         
.data 			

message1 db 'Enter input file name', 13, 10, '$'
message2 db 'Enter output file name', 13, 10, '$'
message3 db 'Error 404 not found', 13, 10, '$'
message4 db 'Warning! This file name is exist', 13, 10, '$'
file_input db 20 dup (0)
file_output db 20 dup (0)
buffer db 257 dup ('$')
read_size dw 0
start_r_w dw 0

.code

start:
	MOV AX, @data
	MOV DS, AX

	LEA SI, file_input
	LEA DX, message1
	MOV AH, 09h
	INT 21h

next_file_in:
	MOV AH, 01h			;ввод имени файла, где лежит текст
	INT 21h
	MOV AH, 0
	CMP AX, 13
	JZ endenter_file_in
	MOV [SI], AL
	INC SI
	JMP next_file_in

endenter_file_in:

	LEA SI, file_output
	LEA DX, message2
	MOV AH, 09h
	INT 21h

next_file_out:			;ввод имени файла, где будет лежать закодированное
	MOV AH, 01h
	INT 21h
	MOV AH, 0
	CMP AX, 13
	JZ endenter_file_out
	MOV [SI], AL
	INC SI
	JMP next_file_out

endenter_file_out:

	mov ah, 3Dh			;открытие файла
	mov al, 0			;для чтения (проверка на то, что он есть)
	lea dx, file_input
	xor cx, cx
	int 21h   
	jnc No_Error_file_name_input	;если ZF == 0, то файл существует (этот флаг устанавливает сам ассэмблер), а если ZF == 1, то что-то не так (например файла нет)
	LEA DX, message3
	MOV AH, 09h
	INT 21h
	jmp the_end_of_file
No_Error_file_name_input:

	mov ah, 3Eh			;закрытие файла
	int 21h

	mov ah, 3Dh			;открытие файла
	mov al, 0			;для чтения (смотрим, есть ли файл для записи, если нет, то создаем его [85-92 строчки])
	lea dx, file_output
	xor cx, cx
	int 21h   
	jc No_File_output_warning	;если флаг ZF == 0, то файл существует, его создавать не надо, но надо выкинуть предупреждение (JC - переход при условии, что ZF == 1)
	LEA DX, message4
	MOV AH, 09h
	INT 21h

	mov ah, 3Eh			;закрытие файла
	int 21h

No_File_output_warning:

	mov ah, 3Ch			;создание файла
	mov al, 1			;для записи
	lea dx, file_output
	xor cx, cx
	int 21h

	mov ah, 3Eh			;закрытие файла
	int 21h

start_of_read:


	lea si, buffer		;очистка буфера (до 104-й строчки кода)
	mov ax, 257
next:
	mov [si], '$'		
	inc si
	dec ax
	cmp ax, 0
	jnz next			

	mov ah, 3Dh			;открытие файла
	mov al, 0			;для чтения
	lea dx, file_input
	xor cx, cx
	int 21h

	mov bx, ax

	mov ax, 4201h		;перемещение курсора на конец предыдущей сессии считывания, где dx - число символов, которые надо пропустить. start_r_w - место, где надо начать считывать 
	mov cx, 0
	mov dx, start_r_w
	int 21h
		
	mov ah, 3Fh			;запись в буфер на 256 байт
	lea dx, buffer
	mov cx, 256
	int 21h
	mov read_size, ax	;во время записи в ах помещается реальное число считанных символов (байт). помещаем его в переменную, чтобы запомнить

	mov ah, 3Eh			;закрытие файла
	int 21h

	lea si, buffer		;кодирование буфера
	mov ax, 257			;постоянно сравниваем ах с 0. это будет означать, что мы закодировали весь буфер и надо переходить к записи буфера в файл
next_shifr:
	xor [si], 'R'	
	inc si
	dec ax
	cmp ax, 0
	jz end_shifr
	xor [si], 'a'	
	inc si
	dec ax
	cmp ax, 0
	jz end_shifr
	xor [si], 'z'	
	inc si
	dec ax
	cmp ax, 0
	jz end_shifr
	xor [si], 'u'	
	inc si
	dec ax
	cmp ax, 0
	jz end_shifr
	xor [si], 'm'	
	inc si
	dec ax
	cmp ax, 0
	jz end_shifr
	xor [si], 'e'	
	inc si
	dec ax
	cmp ax, 0
	xor [si], 'y'	
	inc si
	dec ax
	cmp ax, 0
	jz end_shifr
	xor [si], 'e'	
	inc si
	dec ax
	cmp ax, 0
	jz end_shifr
	xor [si], 'v'	
	inc si
	dec ax
	cmp ax, 0
	jz end_shifr
	
	xor [si], 'a'	
	inc si
	dec ax
	cmp ax, 0
	jz end_shifr

	
	
	jnz next_shifr

end_shifr:

	mov ax, read_size	;если реальное число считываеммых символов равно 0 => мы ничего не считали => файл закончился => пора заканчивать программку
	cmp ax, 0
	jz the_end_of_file

	mov ah, 3Dh			;открытие файла
	mov al, 1			;для записи
	lea dx, file_output
	xor cx, cx
	int 21h

	mov ax, 4201h		;перемещаем курсор, как и раньше
	mov cx, 0
	mov dx, start_r_w
	int 21h

	mov cx, read_size	;запись 
	mov ah, 40h
	lea dx, buffer
	int 21h 

	add cx, start_r_w	;обновляем переменную start_r_w, а в сх хранится число символов, которые мы считали за эту сессию
	mov start_r_w, cx

	mov ah, 3Eh			;закрывам файл
	int 21h

	jmp start_of_read

the_end_of_file:
	
	MOV AH, 4Ch			;закрытие программки
	INT 21h
end start