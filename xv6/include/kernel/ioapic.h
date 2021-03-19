#ifndef XV6_IOAPIC_H
#define XV6_IOAPIC_H

void ioapic_init();
void ioapic_enable(int irq, int cpu_num);

#endif