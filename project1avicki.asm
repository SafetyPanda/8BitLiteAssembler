mov bx 100
mov cx 2
get
mov [bx] ax
mov ax -1
mov [bx+1] ax
add bx cx
get
cmp ax 0
je [24]
mov dx bx
add bx cx
mov [bx] ax
mov [bx+1] dx
jmp [5]
mov [99] bx

halt
