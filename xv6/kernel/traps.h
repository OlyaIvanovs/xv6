// Intel® 64 and IA-32 Architectures Software Developer’s Manual: Volume 3. CH 10
// Table 5-1. Protected-Mode Exceptions and Interrupts
// x86 trap and interrupt constants.

// Processor-defined:
#define T_DIVIDE 0 // divide error
#define T_DEBUG 1  // debug exception
#define T_NMI 2    // non-maskable interrupt
#define T_BRKPT 3  // breakpoint
#define T_OFLOW 4  // overflow
#define T_BOUND 5  // bounds check
#define T_ILLOP 6  // invalid opcode
#define T_DEVICE 7 // device not available
#define T_DBLFLT 8 // double fault
// #define T_COPROC      9      // reserved (not used since 486)
#define T_TSS 10   // invalid task switch segment
#define T_SEGNP 11 // segment not present
#define T_STACK 12 // stack exception
#define T_GPFLT 13 // general protection fault
#define T_PGFLT 14 // page fault
// #define T_RES        15      // reserved(do not use)
#define T_FPERR 16   // floating point error(math error)
#define T_ALIGN 17   // aligment check
#define T_MCHK 18    // machine check
#define T_SIMDERR 19 // SIMD floating point error