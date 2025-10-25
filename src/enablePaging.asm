global enablePaging

enablePaging:
mov eax, 0x7000000
mov cr3, eax

mov eax, cr0
or eax, 0x80000001
mov cr0, eax
ret
