[org 0x7c00]
mov [BOOT_DISK], dl ;saving current disk number

;setting up registers
xor ax, ax
mov es, ax
mov ds, ax
mov bp, 0x7000
mov sp, bp 

mov ah, 0
mov al, 0x3
int 0x10

;mov eax, 0x2000
;mov es, eax
;mov di, 0
;
;mov eax, 0xe820
;mov ebx, 0
;mov ecx, 24
;mov edx, 0x534d4150
;int 0x15
;jc error
;
;mov ecx, 20
;scanRAM:
;push ecx
;add di, 24
;mov eax, 0xe820
;mov ecx, 24
;int 0x15
;jc error
;pop ecx
;loop scanRAM
;
;
;
;mov ax, 0x2000
;mov es, ax
;xor bx, bx
;mov cx, 80
;ReadLoop:
;push cx
;push bx
;push bp
;mov bp, sp
;push word [es:bx]
;sub sp, 2
;call hexdump
;mov sp, bp
;pop bp
;pop bx
;add bx, 2
;xor dx, dx
;mov ax, bx
;mov cx, 4
;div cx
;cmp dx, 0
;je .space
;.ret:
;pop cx
;loop ReadLoop
;jmp $

;.space:
;mov ah, 0xe
;mov al, ' '
;int 0x10
;jmp .ret

mov ah, 0x0
int 0x16

;loading second stage of bootloander
xor ax, ax
mov es, ax
mov bx, 0x7e00

mov ah, 2
mov al, 2
mov ch, 0
mov dh, 0
mov cl, 2
mov dl, [BOOT_DISK]
int 0x13
jc error

;loading the kernel
push 0x0000
push 0x3000
push 3
mov cx, 650
loadloop:
push cx

mov ah, 0xe
mov al, '#'
int 0x10

;converting frendly LBA address to unkind CHS address
mov ax, [bp - 6]
push bp
mov bp, sp
push ax
sub sp, 8
call lbatochs
mov ax, [bp - 6] ; sector
mov bx, [bp - 8] ; head
mov cx, [bp - 10]; cylinder
mov sp, bp
pop bp

inc word [bp - 6]

;loading segment
shl cx, 8
add cx, ax
mov dh, bl
mov dl, [BOOT_DISK]
mov bx, [bp - 2]
mov ax, [bp - 4]
mov es, ax
mov ah, 2
mov al, 1
int 0x13
jc error

;setting location for next segment to be loaded
add word [bp - 2], 0x200
jc .addtooffset
pop cx
loop loadloop
jmp .endaddtooffest
.addtooffset:
add word [bp - 4], 0x1000
pop cx
loop loadloop
.endaddtooffest:


xor ax, ax
mov es, ax

;get info about vesa video mode
mov ax, 0x4f01
mov cx, 0x118
mov di, VesaModeInfoBlock
int 0x10
cmp ax, 0x004f
jne error

;set vesa video mode
mov ax, 0x4f02
mov bx, 0x4118
int 0x10
cmp ax, 0x004f
jne error

;set gdt segments
CODE_SEG equ GDT_code - GDT_start
DATA_SEG equ GDT_data - GDT_start

;switch to protected mode
cli
lgdt [GDT_descriptor]
mov eax, cr0
or eax, 1
mov cr0, eax
jmp CODE_SEG:start_protected_mode
exit:
jmp $

error:
mov ah, 0xe
mov al, 'F'
int 0x10
jmp exit


printnum:
mov word [bp - 4], 10000
mov cx, 5
.loop:
mov ax, [bp - 2]
xor dx, dx
mov bx, [bp - 4]
div bx
mov [bp - 2], dx
add al, '0'
mov ah, 0xe
int 0x10
mov ax, [bp - 4]
xor dx, dx
mov bx, 10
div bx
mov [bp - 4], ax
loop .loop
ret

lbatochs:
;input LBA [bp - 2]
;temp [bp - 4]
;sector [bp - 6]
;head [bp - 8]
;cylinder [bp - 10]
mov ah, 8
mov dl, [BOOT_DISK]
int 0x13
inc dh
and cl, 0x3f
mov word ax, [bp - 2]
div cl
mov word [bp - 6], ax
shr word [bp - 6], 8
inc word [bp - 6]
and ax, 0x0f
mov word [bp - 4], ax
mov ax, [bp - 4]
div dh
mov word [bp - 8], ax
shr word [bp - 8], 8
mov word [bp - 10], ax
and word [bp - 10], 0x0f
ret


hexdump:
mov cx, 4
.loop:
xor dx, dx
mov ax, [bp - 2]
mov bx, 0x10
div bx
mov [bp - 2], ax
mov ax, dx
cmp al, 0xa
jl .num
add al, 0x57
jmp .exit
.num:
add al, '0'
.exit:
mov ah, 0xe
int 0x10
loop .loop
ret

rsdpID:
db 'RSD PTR '

BOOT_DISK:
db 0                                    
 
times 510-($-$$) db 0              
dw 0xaa55
[bits 32]
start_protected_mode:
;jmp $
mov ax, DATA_SEG
mov ds, ax
mov ss, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ebp, 0xffff
mov esp, ebp


mov eax, [VesaModeInfoBlock.BitsPerPixel]
and eax, 0x000000ff
xor edx, edx
mov ebx, 8
div ebx
push eax
mov eax, [VesaModeInfoBlock.BytesPerScanLine]
and eax, 0x0000ffff
push eax
mov eax, [VesaModeInfoBlock.LFBAddress]
push eax
jmp 0x30000
times 512-(($-$$)-512) db 0

GDT_start:                          ; must be at the end of real mode code
    GDT_null:
        dd 0x0
        dd 0x0

    GDT_code:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10011010
        db 0b11001111
        db 0x0

    GDT_data:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10010010
        db 0b11001111
        db 0x0

GDT_end:

GDT_descriptor:
    dw GDT_end - GDT_start - 1
    dd GDT_start



VesaModeInfoBlock:				;	VesaModeInfoBlock_size = 256 bytes
	.ModeAttributes		resw 1
	.FirstWindowAttributes	resb 1
	.SecondWindowAttributes	resb 1
	.WindowGranularity	resw 1		;	in KB
	.WindowSize		resw 1		;	in KB
	.FirstWindowSegment	resw 1		;	0 if not supported
	.SecondWindowSegment	resw 1		;	0 if not supported
	.WindowFunctionPtr	resd 1
	.BytesPerScanLine	resw 1

	;	Added in Revision 1.2
	.Width			resw 1		;	in pixels(graphics)/columns(text)
	.Height			resw 1		;	in pixels(graphics)/columns(text)
	.CharWidth		resb 1		;	in pixels
	.CharHeight		resb 1		;	in pixels
	.PlanesCount		resb 1
	.BitsPerPixel		resb 1
	.BanksCount		resb 1
	.MemoryModel		resb 1		;	http://www.ctyme.com/intr/rb-0274.htm#Table82
	.BankSize		resb 1		;	in KB
	.ImagePagesCount	resb 1		;	count - 1
	.Reserved1		resb 1		;	equals 0 in Revision 1.0-2.0, 1 in 3.0

	.RedMaskSize		resb 1
	.RedFieldPosition	resb 1
	.GreenMaskSize		resb 1
	.GreenFieldPosition	resb 1
	.BlueMaskSize		resb 1
	.BlueFieldPosition	resb 1
	.ReservedMaskSize	resb 1
	.ReservedMaskPosition	resb 1
	.DirectColorModeInfo	resb 1

	;	Added in Revision 2.0
	.LFBAddress		resd 1
	.OffscreenMemoryOffset	resd 1
	.OffscreenMemorySize	resw 1		;	in KB
	.Reserved2		resb 206	;	available in Revision 3.0, but useless for now
times 512-(($-$$)-1024) db 0
