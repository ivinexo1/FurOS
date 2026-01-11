global enablePaging
global reloadPaging
extern kernel_size
extern kernel_loc

enablePaging:
mov eax, 0x100000
mov cr3, eax

mov eax, cr0
or eax, 0x80000001
mov cr0, eax
ret

reloadPaging:
mov eax, cr3
mov cr3, eax
ret
