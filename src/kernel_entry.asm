[bits 32]
[extern main]
global kernel_loc
global kernel_size
global framebuffer
global bbl
global bbp

pop dword [kernel_size]
pop dword [kernel_loc]
pop dword [framebuffer]
pop dword [bbl]
pop dword [bbp]
mov esp, ebp

call main
jmp $
kernel_loc:
dd 0
kernel_size:
dd 0
framebuffer:
dd 0
bbp:
dd 0
bbl:
dd 0
