[bits 32]
[extern main]
;global framebuffer
global bbl
global bbp

;pop dword [framebuffer]
pop dword [bbl]
pop dword [bbp]
mov esp, ebp

call main
jmp $
;framebuffer:
;dd 0
bbp:
dd 0
bbl:
dd 0
