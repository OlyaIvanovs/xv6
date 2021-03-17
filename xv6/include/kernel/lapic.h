#ifndef XV6_LAPIC_H
#define XV6_LAPIC_H

void lapic_init();
int find_lapic_id();

void microdelay(int us);

#endif // XV6_LAPIC_H