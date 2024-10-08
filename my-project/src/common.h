#ifndef COM_H
#define COM_H
#include <stdint.h>


#define M_PII 3.14159265358979323846
#define M_PI_FIX 31415926

#define FIXPOINT_DECIMAL_PLACES 10000000

#define INPUT1      GPIO8 
#define INPUT2      GPIO9 
#define INPUT3      GPIO10 
#define INPUT4      GPIO11 
#define INPUT5      GPIO12 
#define NR_INPUTS   4


extern const uint16_t sin_table[];
extern uint64_t key_assignments[];

static inline uint64_t c_abs64(int64_t a)
{
   if(a<0)
   {
        return -a;
   }
    return a;
}

#endif