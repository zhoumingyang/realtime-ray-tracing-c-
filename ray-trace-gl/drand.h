#pragma once

#include <stdlib.h>
#include <math.h>

#define RAND48_SEED_0   (0x330e)
#define RAND48_SEED_1   (0xabcd)
#define RAND48_SEED_2   (0x1234)
#define RAND48_MULT_0   (0xe66d)
#define RAND48_MULT_1   (0xdeec)
#define RAND48_MULT_2   (0x0005)
#define RAND48_ADD      (0x000b)

extern unsigned short _rand48_seed[3];
extern unsigned short _rand48_mult[3];
extern unsigned short _rand48_add;

extern void _dorand48(unsigned short xseed[3]);
extern double erand48(unsigned short xseed[3]);
extern double drand48(void);
extern long lrand48(void);
extern long nrand48(unsigned short xseed[3]);
extern long mrand48(void);
extern long jrand48(unsigned short xseed[3]);
extern void srand48(long seed);
extern unsigned short *seed48(unsigned short xseed[3]);
extern void lcong48(unsigned short p[7]);