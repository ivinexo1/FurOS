#include "../include/vga.h"
#include "../include/terminal.h"
#include "../include/idt.h"
#include "../include/isr.h"
#include "../include/ports.h"

isr_t interrupt_handlers[256];

void isr_install() {
  set_idt_gate(0, (uint32_t) isr0);
  set_idt_gate(1, (uint32_t) isr1);
  set_idt_gate(2, (uint32_t) isr2);
  set_idt_gate(3, (uint32_t) isr3);
  set_idt_gate(4, (uint32_t) isr4);
  set_idt_gate(5, (uint32_t) isr5);
  set_idt_gate(6, (uint32_t) isr6);
  set_idt_gate(7, (uint32_t) isr7);
  set_idt_gate(8, (uint32_t) isr8);
  set_idt_gate(9, (uint32_t) isr9);
  set_idt_gate(10, (uint32_t) isr10);
  set_idt_gate(11, (uint32_t) isr11);
  set_idt_gate(12, (uint32_t) isr12);
  set_idt_gate(13, (uint32_t) isr13);
  set_idt_gate(14, (uint32_t) isr14);
  set_idt_gate(15, (uint32_t) isr15);
  set_idt_gate(16, (uint32_t) isr16);
  set_idt_gate(17, (uint32_t) isr17);
  set_idt_gate(18, (uint32_t) isr18);
  set_idt_gate(19, (uint32_t) isr19);
  set_idt_gate(20, (uint32_t) isr20);
  set_idt_gate(21, (uint32_t) isr21);
  set_idt_gate(22, (uint32_t) isr22);
  set_idt_gate(23, (uint32_t) isr23);
  set_idt_gate(24, (uint32_t) isr24);
  set_idt_gate(25, (uint32_t) isr25);
  set_idt_gate(26, (uint32_t) isr26);
  set_idt_gate(27, (uint32_t) isr27);
  set_idt_gate(28, (uint32_t) isr28);
  set_idt_gate(29, (uint32_t) isr29);
  set_idt_gate(30, (uint32_t) isr30);
  set_idt_gate(31, (uint32_t) isr31);
  // ... asi tu treba definovat aj tie ostatne isr [myslim]  

  // defautsky je prvych 8 IRQ mapped na prvych 8 gates
  // takze kvoli tomu aby tam neboli issues vraj musime
  // remappnut PIC alebo daco :V

  // main PIC ~ 0x20 command ~ 0x21 data
  // sec. PIC ~ 0xA0 command ~ 0xA1 data
  outb(0x20, 0x11);
  outb(0xA0, 0x11);
  outb(0x21, 0x20);
  outb(0xA1, 0x28);
  outb(0x21, 0x04);
  outb(0xA1, 0x02);
  outb(0x21, 0x01);
  outb(0xA1, 0x01);
  outb(0x21, 0x0);
  outb(0xA1, 0x0);
  // initialization commands [ICW] : https://www.thesatya.com/8259.html

  // main PIC
  set_idt_gate(32, (uint32_t)irq0);
  set_idt_gate(33, (uint32_t)irq1);
  set_idt_gate(34, (uint32_t)irq2);
  set_idt_gate(35, (uint32_t)irq3);
  set_idt_gate(36, (uint32_t)irq4);
  set_idt_gate(37, (uint32_t)irq5);
  set_idt_gate(38, (uint32_t)irq6);
  set_idt_gate(39, (uint32_t)irq7);

  // secondary PIC
  set_idt_gate(40, (uint32_t)irq8);
  set_idt_gate(41, (uint32_t)irq9);
  set_idt_gate(42, (uint32_t)irq10);
  set_idt_gate(43, (uint32_t)irq11);
  set_idt_gate(44, (uint32_t)irq12);
  set_idt_gate(45, (uint32_t)irq13);
  set_idt_gate(46, (uint32_t)irq14);
  set_idt_gate(47, (uint32_t)irq15);

  load_idt();
}

char *exeption_msg[] = {
  "DIVISION BY 0",
  "DEBUG",
  "NON MASKABLE INTERRUPT",
  "BREAKPOINT",
  "INTO DETECTED OVERFLOW",
  "OUT OF BOUNDS",
  "INVALID OPCODE",
  "NO COPROCESSOR",

  "DOUBLE FAULT",
  "COPROCESSOR SEGMENT OVERRUN",
  "BAD TSS",
  "SEGMENT NOT PRESENT",
  "STACK FAULT",
  "GENERAL PROTECTION FAULT",
  "PAGE FAULT",
  "UNKNOWN INTERRUPT",

  "COPROCESSOR FAULT",
  "ALIGNMENT CHECK",
  "MACHINE CHECK",
  "RESERVED",
  "RESERVED",
  "RESERVED",
  "RESERVED",

  "RESERVED",
  "RESERVED",
  "RESERVED",
  "RESERVED",
  "RESERVED",
  "RESERVED",
  "RESERVED",
  "RESERVED",
  "RESERVED",
};

void isr_handler(registers_t *r){
  for (int y = 0; y < 768; y++) {
    for (int x = 0; x < 1024; x++) {
      printPixel(x, y, 0xff0000);
    }
  }
  for (uint32_t i = 0; i < r->int_no; i++) {
    switch (i % 2) {
      case 0:
        printPixel(i, 0, 0xffffff);
        break;
      case 1:
        printPixel(i, 0, 0x000000);
    }
  }

  resetTerminal();
  printString(exeption_msg[r->int_no]);
  asm volatile("hlt");
}



void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

void irq_handler(registers_t *r) {
    /* Handle the interrupt in a more modular way */
    if (interrupt_handlers[r->int_no] != 0) {
        isr_t handler = interrupt_handlers[r->int_no];
        handler(r);
    }
    // EOI
    if (r->int_no >= 40) {
        outb(0xA0, 0x20); /* follower */
    }
    outb(0x20, 0x20); /* leader */
}
