mov ax 100
jmp [22]
mov bx 1

mov dx 600
add ax ax
add ax 100
add ax [1]
add ax [bx]
add ax [bx+2]

cmp ax [bx+5]

halt
halt