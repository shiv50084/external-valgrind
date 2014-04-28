
#include <stdio.h>
#include <assert.h>
#include <malloc.h>  // memalign
#include <string.h>  // memset
#include <math.h>    // isnormal

typedef  unsigned char           UChar;
typedef  unsigned short int      UShort;
typedef  unsigned int            UInt;
typedef  signed int              Int;
typedef  unsigned char           UChar;
typedef  unsigned long long int  ULong;

typedef  unsigned char           Bool;
#define False ((Bool)0)
#define True  ((Bool)1)


#define ITERS 10


union _V128 {
   UChar  u8[16];
   UShort u16[8];
   UInt   u32[4];
   ULong  u64[2];
   float  f32[4];
   double f64[2];
};
typedef  union _V128   V128;

static inline UChar randUChar ( void )
{
   static UInt seed = 80021;
   seed = 1103515245 * seed + 12345;
   return (seed >> 17) & 0xFF;
}

static ULong randULong ( void )
{
   Int i;
   ULong r = 0;
   for (i = 0; i < 8; i++) {
      r = (r << 8) | (ULong)(0xFF & randUChar());
   }
   return r;
}

/* Generates a random V128.  Ensures that that it contains normalised
   FP numbers when viewed as either F32x4 or F64x2, so that it is
   reasonable to use in FP test cases. */
static void randV128 ( V128* v )
{
   static UInt nCalls = 0, nIters = 0;
   Int i;
   nCalls++;
   while (1) {
      nIters++;
      for (i = 0; i < 16; i++) {
         v->u8[i] = randUChar();
      }
      if (isnormal(v->f32[0]) && isnormal(v->f32[1]) && isnormal(v->f32[2])
          && isnormal(v->f32[3]) && isnormal(v->f64[0]) && isnormal(v->f64[1]))
        break;
   }
   if (0 == (nCalls & 0xFF))
      printf("randV128: %u calls, %u iters\n", nCalls, nIters);
}

static void showV128 ( V128* v )
{
   Int i;
   for (i = 15; i >= 0; i--)
      printf("%02x", (Int)v->u8[i]);
}

__attribute__((unused))
static void* memalign16(size_t szB)
{
   void* x;
   x = memalign(16, szB);
   assert(x);
   assert(0 == ((16-1) & (unsigned long)x));
   return x;
}


void test_UMINV ( void )
{
  int i;
  V128 block[2];

  /* -- 4s -- */

  for (i = 0; i < 10; i++) {
    memset(&block, 0x55, sizeof(block));
    randV128(&block[0]);
    randV128(&block[1]);
    __asm__ __volatile__(
       "ldr   q7, [%0, #0]   ; "
       "uminv s8, v7.4s   ; "
       "str   q8, [%0, #16] "
       : : "r"(&block[0]) : "memory", "v7", "v8"
                         );
    printf("UMINV v8, v7.4s  ");
    showV128(&block[0]); printf("  ");
    showV128(&block[1]); printf("\n");
  }

  /* -- 8h -- */

  for (i = 0; i < 10; i++) {
    memset(&block, 0x55, sizeof(block));
    randV128(&block[0]);
    randV128(&block[1]);
    __asm__ __volatile__(
       "ldr   q7, [%0, #0]   ; "
       "uminv h8, v7.8h   ; "
       "str   q8, [%0, #16] "
       : : "r"(&block[0]) : "memory", "v7", "v8"
                         );
    printf("UMINV h8, v7.8h  ");
    showV128(&block[0]); printf("  ");
    showV128(&block[1]); printf("\n");
  }

  /* -- 4h -- */

  for (i = 0; i < 10; i++) {
    memset(&block, 0x55, sizeof(block));
    randV128(&block[0]);
    randV128(&block[1]);
    __asm__ __volatile__(
       "ldr   q7, [%0, #0]   ; "
       "uminv h8, v7.4h   ; "
       "str   q8, [%0, #16] "
       : : "r"(&block[0]) : "memory", "v7", "v8"
                         );
    printf("UMINV h8, v7.4h  ");
    showV128(&block[0]); printf("  ");
    showV128(&block[1]); printf("\n");
  }

  /* -- 16b -- */

  for (i = 0; i < 10; i++) {
    memset(&block, 0x55, sizeof(block));
    randV128(&block[0]);
    randV128(&block[1]);
    __asm__ __volatile__(
       "ldr   q7, [%0, #0]   ; "
       "uminv b8, v7.16b   ; "
       "str   q8, [%0, #16] "
       : : "r"(&block[0]) : "memory", "v7", "v8"
                         );
    printf("UMINV b8, v7.16b  ");
    showV128(&block[0]); printf("  ");
    showV128(&block[1]); printf("\n");
  }

  /* -- 8b -- */

  for (i = 0; i < 10; i++) {
    memset(&block, 0x55, sizeof(block));
    randV128(&block[0]);
    randV128(&block[1]);
    __asm__ __volatile__(
       "ldr   q7, [%0, #0]   ; "
       "uminv b8, v7.8b   ; "
       "str   q8, [%0, #16] "
       : : "r"(&block[0]) : "memory", "v7", "v8"
                         );
    printf("UMINV b8, v7.8b  ");
    showV128(&block[0]); printf("  ");
    showV128(&block[1]); printf("\n");
  }

}


void test_UMAXV ( void )
{
  int i;
  V128 block[2];

  /* -- 4s -- */

  for (i = 0; i < 10; i++) {
    memset(&block, 0x55, sizeof(block));
    randV128(&block[0]);
    randV128(&block[1]);
    __asm__ __volatile__(
       "ldr   q7, [%0, #0]   ; "
       "umaxv s8, v7.4s   ; "
       "str   q8, [%0, #16] "
       : : "r"(&block[0]) : "memory", "v7", "v8"
                         );
    printf("UMAXV v8, v7.4s  ");
    showV128(&block[0]); printf("  ");
    showV128(&block[1]); printf("\n");
  }

  /* -- 8h -- */

  for (i = 0; i < 10; i++) {
    memset(&block, 0x55, sizeof(block));
    randV128(&block[0]);
    randV128(&block[1]);
    __asm__ __volatile__(
       "ldr   q7, [%0, #0]   ; "
       "umaxv h8, v7.8h   ; "
       "str   q8, [%0, #16] "
       : : "r"(&block[0]) : "memory", "v7", "v8"
                         );
    printf("UMAXV h8, v7.8h  ");
    showV128(&block[0]); printf("  ");
    showV128(&block[1]); printf("\n");
  }

  /* -- 4h -- */

  for (i = 0; i < 10; i++) {
    memset(&block, 0x55, sizeof(block));
    randV128(&block[0]);
    randV128(&block[1]);
    __asm__ __volatile__(
       "ldr   q7, [%0, #0]   ; "
       "umaxv h8, v7.4h   ; "
       "str   q8, [%0, #16] "
       : : "r"(&block[0]) : "memory", "v7", "v8"
                         );
    printf("UMAXV h8, v7.4h  ");
    showV128(&block[0]); printf("  ");
    showV128(&block[1]); printf("\n");
  }

  /* -- 16b -- */

  for (i = 0; i < 10; i++) {
    memset(&block, 0x55, sizeof(block));
    randV128(&block[0]);
    randV128(&block[1]);
    __asm__ __volatile__(
       "ldr   q7, [%0, #0]   ; "
       "umaxv b8, v7.16b   ; "
       "str   q8, [%0, #16] "
       : : "r"(&block[0]) : "memory", "v7", "v8"
                         );
    printf("UMAXV b8, v7.16b  ");
    showV128(&block[0]); printf("  ");
    showV128(&block[1]); printf("\n");
  }

  /* -- 8b -- */

  for (i = 0; i < 10; i++) {
    memset(&block, 0x55, sizeof(block));
    randV128(&block[0]);
    randV128(&block[1]);
    __asm__ __volatile__(
       "ldr   q7, [%0, #0]   ; "
       "umaxv b8, v7.8b   ; "
       "str   q8, [%0, #16] "
       : : "r"(&block[0]) : "memory", "v7", "v8"
                         );
    printf("UMAXV b8, v7.8b  ");
    showV128(&block[0]); printf("  ");
    showV128(&block[1]); printf("\n");
  }

}


void test_INS_general ( void )
{
  V128 block[3];

  /* -- D[0..1] -- */

  memset(&block, 0x55, sizeof(block));
  block[1].u64[0] = randULong();
  __asm__ __volatile__(
     "ldr q7, [%0, #0]   ; "
     "ldr x19, [%0, #16] ; "
     "ins v7.d[0], x19   ; "
     "str q7, [%0, #32] "
     : : "r"(&block[0]) : "memory", "x19", "v7"
  );
  printf("INS v7.u64[0],x19  ");
  showV128(&block[0]); printf("  %016llx  ", block[1].u64[0]);
  showV128(&block[2]); printf("\n");

  memset(&block, 0x55, sizeof(block));
  block[1].u64[0] = randULong();
  __asm__ __volatile__(
     "ldr q7, [%0, #0]   ; "
     "ldr x19, [%0, #16] ; "
     "ins v7.d[1], x19   ; "
     "str q7, [%0, #32] "
     : : "r"(&block[0]) : "memory", "x19", "v7"
  );
  printf("INS v7.d[1],x19  ");
  showV128(&block[0]); printf("  %016llx  ", block[1].u64[0]);
  showV128(&block[2]); printf("\n");

  /* -- S[0..3] -- */

  memset(&block, 0x55, sizeof(block));
  block[1].u64[0] = randULong();
  __asm__ __volatile__(
     "ldr q7, [%0, #0]   ; "
     "ldr x19, [%0, #16] ; "
     "ins v7.s[0], w19   ; "
     "str q7, [%0, #32] "
     : : "r"(&block[0]) : "memory", "x19", "v7"
  );
  printf("INS v7.s[0],x19  ");
  showV128(&block[0]); printf("  %016llx  ", block[1].u64[0]);
  showV128(&block[2]); printf("\n");

  memset(&block, 0x55, sizeof(block));
  block[1].u64[0] = randULong();
  __asm__ __volatile__(
     "ldr q7, [%0, #0]   ; "
     "ldr x19, [%0, #16] ; "
     "ins v7.s[1], w19   ; "
     "str q7, [%0, #32] "
     : : "r"(&block[0]) : "memory", "x19", "v7"
  );
  printf("INS v7.s[1],x19  ");
  showV128(&block[0]); printf("  %016llx  ", block[1].u64[0]);
  showV128(&block[2]); printf("\n");

  memset(&block, 0x55, sizeof(block));
  block[1].u64[0] = randULong();
  __asm__ __volatile__(
     "ldr q7, [%0, #0]   ; "
     "ldr x19, [%0, #16] ; "
     "ins v7.s[2], w19   ; "
     "str q7, [%0, #32] "
     : : "r"(&block[0]) : "memory", "x19", "v7"
  );
  printf("INS v7.s[2],x19  ");
  showV128(&block[0]); printf("  %016llx  ", block[1].u64[0]);
  showV128(&block[2]); printf("\n");

  memset(&block, 0x55, sizeof(block));
  block[1].u64[0] = randULong();
  __asm__ __volatile__(
     "ldr q7, [%0, #0]   ; "
     "ldr x19, [%0, #16] ; "
     "ins v7.s[3], w19   ; "
     "str q7, [%0, #32] "
     : : "r"(&block[0]) : "memory", "x19", "v7"
  );
  printf("INS v7.s[3],x19  ");
  showV128(&block[0]); printf("  %016llx  ", block[1].u64[0]);
  showV128(&block[2]); printf("\n");

  /* -- H[0..7] -- */

  memset(&block, 0x55, sizeof(block));
  block[1].u64[0] = randULong();
  __asm__ __volatile__(
     "ldr q7, [%0, #0]   ; "
     "ldr x19, [%0, #16] ; "
     "ins v7.h[0], w19   ; "
     "str q7, [%0, #32] "
     : : "r"(&block[0]) : "memory", "x19", "v7"
  );
  printf("INS v7.h[0],x19  ");
  showV128(&block[0]); printf("  %016llx  ", block[1].u64[0]);
  showV128(&block[2]); printf("\n");

  memset(&block, 0x55, sizeof(block));
  block[1].u64[0] = randULong();
  __asm__ __volatile__(
     "ldr q7, [%0, #0]   ; "
     "ldr x19, [%0, #16] ; "
     "ins v7.h[1], w19   ; "
     "str q7, [%0, #32] "
     : : "r"(&block[0]) : "memory", "x19", "v7"
  );
  printf("INS v7.h[1],x19  ");
  showV128(&block[0]); printf("  %016llx  ", block[1].u64[0]);
  showV128(&block[2]); printf("\n");

  memset(&block, 0x55, sizeof(block));
  block[1].u64[0] = randULong();
  __asm__ __volatile__(
     "ldr q7, [%0, #0]   ; "
     "ldr x19, [%0, #16] ; "
     "ins v7.h[2], w19   ; "
     "str q7, [%0, #32] "
     : : "r"(&block[0]) : "memory", "x19", "v7"
  );
  printf("INS v7.h[2],x19  ");
  showV128(&block[0]); printf("  %016llx  ", block[1].u64[0]);
  showV128(&block[2]); printf("\n");

  memset(&block, 0x55, sizeof(block));
  block[1].u64[0] = randULong();
  __asm__ __volatile__(
     "ldr q7, [%0, #0]   ; "
     "ldr x19, [%0, #16] ; "
     "ins v7.h[3], w19   ; "
     "str q7, [%0, #32] "
     : : "r"(&block[0]) : "memory", "x19", "v7"
  );
  printf("INS v7.h[3],x19  ");
  showV128(&block[0]); printf("  %016llx  ", block[1].u64[0]);
  showV128(&block[2]); printf("\n");

  memset(&block, 0x55, sizeof(block));
  block[1].u64[0] = randULong();
  __asm__ __volatile__(
     "ldr q7, [%0, #0]   ; "
     "ldr x19, [%0, #16] ; "
     "ins v7.h[4], w19   ; "
     "str q7, [%0, #32] "
     : : "r"(&block[0]) : "memory", "x19", "v7"
  );
  printf("INS v7.h[4],x19  ");
  showV128(&block[0]); printf("  %016llx  ", block[1].u64[0]);
  showV128(&block[2]); printf("\n");

  memset(&block, 0x55, sizeof(block));
  block[1].u64[0] = randULong();
  __asm__ __volatile__(
     "ldr q7, [%0, #0]   ; "
     "ldr x19, [%0, #16] ; "
     "ins v7.h[5], w19   ; "
     "str q7, [%0, #32] "
     : : "r"(&block[0]) : "memory", "x19", "v7"
  );
  printf("INS v7.h[5],x19  ");
  showV128(&block[0]); printf("  %016llx  ", block[1].u64[0]);
  showV128(&block[2]); printf("\n");

  memset(&block, 0x55, sizeof(block));
  block[1].u64[0] = randULong();
  __asm__ __volatile__(
     "ldr q7, [%0, #0]   ; "
     "ldr x19, [%0, #16] ; "
     "ins v7.h[6], w19   ; "
     "str q7, [%0, #32] "
     : : "r"(&block[0]) : "memory", "x19", "v7"
  );
  printf("INS v7.h[6],x19  ");
  showV128(&block[0]); printf("  %016llx  ", block[1].u64[0]);
  showV128(&block[2]); printf("\n");

  memset(&block, 0x55, sizeof(block));
  block[1].u64[0] = randULong();
  __asm__ __volatile__(
     "ldr q7, [%0, #0]   ; "
     "ldr x19, [%0, #16] ; "
     "ins v7.h[7], w19   ; "
     "str q7, [%0, #32] "
     : : "r"(&block[0]) : "memory", "x19", "v7"
  );
  printf("INS v7.h[7],x19  ");
  showV128(&block[0]); printf("  %016llx  ", block[1].u64[0]);
  showV128(&block[2]); printf("\n");

  /* -- B[0,15] -- */

  memset(&block, 0x55, sizeof(block));
  block[1].u64[0] = randULong();
  __asm__ __volatile__(
     "ldr q7, [%0, #0]   ; "
     "ldr x19, [%0, #16] ; "
     "ins v7.b[0], w19   ; "
     "str q7, [%0, #32] "
     : : "r"(&block[0]) : "memory", "x19", "v7"
  );
  printf("INS v7.b[0],x19  ");
  showV128(&block[0]); printf("  %016llx  ", block[1].u64[0]);
  showV128(&block[2]); printf("\n");

  memset(&block, 0x55, sizeof(block));
  block[1].u64[0] = randULong();
  __asm__ __volatile__(
     "ldr q7, [%0, #0]   ; "
     "ldr x19, [%0, #16] ; "
     "ins v7.b[15], w19   ; "
     "str q7, [%0, #32] "
     : : "r"(&block[0]) : "memory", "x19", "v7"
  );
  printf("INS v7.b[15],x19 ");
  showV128(&block[0]); printf("  %016llx  ", block[1].u64[0]);
  showV128(&block[2]); printf("\n");
}



void test_SMINV ( void )
{
  int i;
  V128 block[2];

  /* -- 4s -- */

  for (i = 0; i < 10; i++) {
    memset(&block, 0x55, sizeof(block));
    randV128(&block[0]);
    randV128(&block[1]);
    __asm__ __volatile__(
       "ldr   q7, [%0, #0]   ; "
       "sminv s8, v7.4s   ; "
       "str   q8, [%0, #16] "
       : : "r"(&block[0]) : "memory", "v7", "v8"
                         );
    printf("SMINV v8, v7.4s  ");
    showV128(&block[0]); printf("  ");
    showV128(&block[1]); printf("\n");
  }

  /* -- 8h -- */

  for (i = 0; i < 10; i++) {
    memset(&block, 0x55, sizeof(block));
    randV128(&block[0]);
    randV128(&block[1]);
    __asm__ __volatile__(
       "ldr   q7, [%0, #0]   ; "
       "sminv h8, v7.8h   ; "
       "str   q8, [%0, #16] "
       : : "r"(&block[0]) : "memory", "v7", "v8"
                         );
    printf("SMINV h8, v7.8h  ");
    showV128(&block[0]); printf("  ");
    showV128(&block[1]); printf("\n");
  }

  /* -- 4h -- */

  for (i = 0; i < 10; i++) {
    memset(&block, 0x55, sizeof(block));
    randV128(&block[0]);
    randV128(&block[1]);
    __asm__ __volatile__(
       "ldr   q7, [%0, #0]   ; "
       "sminv h8, v7.4h   ; "
       "str   q8, [%0, #16] "
       : : "r"(&block[0]) : "memory", "v7", "v8"
                         );
    printf("SMINV h8, v7.4h  ");
    showV128(&block[0]); printf("  ");
    showV128(&block[1]); printf("\n");
  }

  /* -- 16b -- */

  for (i = 0; i < 10; i++) {
    memset(&block, 0x55, sizeof(block));
    randV128(&block[0]);
    randV128(&block[1]);
    __asm__ __volatile__(
       "ldr   q7, [%0, #0]   ; "
       "sminv b8, v7.16b   ; "
       "str   q8, [%0, #16] "
       : : "r"(&block[0]) : "memory", "v7", "v8"
                         );
    printf("SMINV b8, v7.16b  ");
    showV128(&block[0]); printf("  ");
    showV128(&block[1]); printf("\n");
  }

  /* -- 8b -- */

  for (i = 0; i < 10; i++) {
    memset(&block, 0x55, sizeof(block));
    randV128(&block[0]);
    randV128(&block[1]);
    __asm__ __volatile__(
       "ldr   q7, [%0, #0]   ; "
       "sminv b8, v7.8b   ; "
       "str   q8, [%0, #16] "
       : : "r"(&block[0]) : "memory", "v7", "v8"
                         );
    printf("SMINV b8, v7.8b  ");
    showV128(&block[0]); printf("  ");
    showV128(&block[1]); printf("\n");
  }

}


void test_SMAXV ( void )
{
  int i;
  V128 block[2];

  /* -- 4s -- */

  for (i = 0; i < 10; i++) {
    memset(&block, 0x55, sizeof(block));
    randV128(&block[0]);
    randV128(&block[1]);
    __asm__ __volatile__(
       "ldr   q7, [%0, #0]   ; "
       "smaxv s8, v7.4s   ; "
       "str   q8, [%0, #16] "
       : : "r"(&block[0]) : "memory", "v7", "v8"
                         );
    printf("SMAXV v8, v7.4s  ");
    showV128(&block[0]); printf("  ");
    showV128(&block[1]); printf("\n");
  }

  /* -- 8h -- */

  for (i = 0; i < 10; i++) {
    memset(&block, 0x55, sizeof(block));
    randV128(&block[0]);
    randV128(&block[1]);
    __asm__ __volatile__(
       "ldr   q7, [%0, #0]   ; "
       "smaxv h8, v7.8h   ; "
       "str   q8, [%0, #16] "
       : : "r"(&block[0]) : "memory", "v7", "v8"
                         );
    printf("SMAXV h8, v7.8h  ");
    showV128(&block[0]); printf("  ");
    showV128(&block[1]); printf("\n");
  }

  /* -- 4h -- */

  for (i = 0; i < 10; i++) {
    memset(&block, 0x55, sizeof(block));
    randV128(&block[0]);
    randV128(&block[1]);
    __asm__ __volatile__(
       "ldr   q7, [%0, #0]   ; "
       "smaxv h8, v7.4h   ; "
       "str   q8, [%0, #16] "
       : : "r"(&block[0]) : "memory", "v7", "v8"
                         );
    printf("SMAXV h8, v7.4h  ");
    showV128(&block[0]); printf("  ");
    showV128(&block[1]); printf("\n");
  }

  /* -- 16b -- */

  for (i = 0; i < 10; i++) {
    memset(&block, 0x55, sizeof(block));
    randV128(&block[0]);
    randV128(&block[1]);
    __asm__ __volatile__(
       "ldr   q7, [%0, #0]   ; "
       "smaxv b8, v7.16b   ; "
       "str   q8, [%0, #16] "
       : : "r"(&block[0]) : "memory", "v7", "v8"
                         );
    printf("SMAXV b8, v7.16b  ");
    showV128(&block[0]); printf("  ");
    showV128(&block[1]); printf("\n");
  }

  /* -- 8b -- */

  for (i = 0; i < 10; i++) {
    memset(&block, 0x55, sizeof(block));
    randV128(&block[0]);
    randV128(&block[1]);
    __asm__ __volatile__(
       "ldr   q7, [%0, #0]   ; "
       "smaxv b8, v7.8b   ; "
       "str   q8, [%0, #16] "
       : : "r"(&block[0]) : "memory", "v7", "v8"
                         );
    printf("SMAXV b8, v7.8b  ");
    showV128(&block[0]); printf("  ");
    showV128(&block[1]); printf("\n");
  }

}

/* Note this also sets the destination register to a known value (0x55..55)
   since it can sometimes be an input to the instruction too. */
#define GEN_BINARY_TEST(INSN,SUFFIX) \
  __attribute__((noinline)) \
  static void test_##INSN##_##SUFFIX ( void ) { \
     Int i; \
     for (i = 0; i < ITERS; i++) { \
        V128 block[3]; \
        memset(block, 0x55, sizeof(block)); \
        randV128(&block[0]); \
        randV128(&block[1]); \
        randV128(&block[2]); \
        __asm__ __volatile__( \
           "ldr   q7, [%0, #0]   ; " \
           "ldr   q8, [%0, #16]   ; " \
           "ldr   q9, [%0, #32]   ; " \
           #INSN " v9." #SUFFIX ", v7." #SUFFIX ", v8." #SUFFIX " ; " \
           "str   q9, [%0, #32] " \
           : : "r"(&block[0]) : "memory", "v7", "v8", "v9" \
        ); \
        printf(#INSN   " v9." #SUFFIX ", v7." #SUFFIX ", v8." #SUFFIX "  "); \
        showV128(&block[0]); printf("  "); \
        showV128(&block[1]); printf("  "); \
        showV128(&block[2]); printf("\n"); \
     } \
  }


/* Note this also sets the destination register to a known value (0x55..55)
   since it can sometimes be an input to the instruction too. */
#define GEN_SHIFT_TEST(INSN,SUFFIXD,SUFFIXN,AMOUNT) \
  __attribute__((noinline)) \
  static void test_##INSN##_##SUFFIXD##_##SUFFIXN##_##AMOUNT ( void ) { \
     Int i; \
     for (i = 0; i < ITERS; i++) { \
        V128 block[2]; \
        memset(block, 0x55, sizeof(block)); \
        randV128(&block[0]); \
        randV128(&block[1]); \
        __asm__ __volatile__( \
           "ldr   q7, [%0, #0]   ; " \
           "ldr   q8, [%0, #16]   ; " \
           #INSN " v8." #SUFFIXD ", v7." #SUFFIXN ", #" #AMOUNT " ; " \
           "str   q8, [%0, #16] " \
           : : "r"(&block[0]) : "memory", "v7", "v8" \
        ); \
        printf(#INSN   " v8." #SUFFIXD ", v7." #SUFFIXN ", #" #AMOUNT "  "); \
        showV128(&block[0]); printf("  "); \
        showV128(&block[1]); printf("\n"); \
     } \
  }

/* Note this also sets the destination register to a known value (0x55..55)
   since it can sometimes be an input to the instruction too. */
#define GEN_UNARY_TEST(INSN,SUFFIXD,SUFFIXN) \
  __attribute__((noinline)) \
  static void test_##INSN##_##SUFFIXD##_##SUFFIXN ( void ) { \
     Int i; \
     for (i = 0; i < ITERS; i++) { \
        V128 block[2]; \
        memset(block, 0x55, sizeof(block)); \
        randV128(&block[0]); \
        randV128(&block[1]); \
        __asm__ __volatile__( \
           "ldr   q7, [%0, #0]   ; " \
           "ldr   q8, [%0, #16]   ; " \
           #INSN " v8." #SUFFIXD ", v7." #SUFFIXN " ; " \
           "str   q8, [%0, #16] " \
           : : "r"(&block[0]) : "memory", "v7", "v8" \
        ); \
        printf(#INSN   " v8." #SUFFIXD ", v7." #SUFFIXN); \
        showV128(&block[0]); printf("  "); \
        showV128(&block[1]); printf("\n"); \
     } \
  }


GEN_BINARY_TEST(umax, 4s)
GEN_BINARY_TEST(umax, 8h)
GEN_BINARY_TEST(umax, 4h)
GEN_BINARY_TEST(umax, 16b)
GEN_BINARY_TEST(umax, 8b)

GEN_BINARY_TEST(umin, 4s)
GEN_BINARY_TEST(umin, 8h)
GEN_BINARY_TEST(umin, 4h)
GEN_BINARY_TEST(umin, 16b)
GEN_BINARY_TEST(umin, 8b)

GEN_BINARY_TEST(smax, 4s)
GEN_BINARY_TEST(smax, 8h)
GEN_BINARY_TEST(smax, 4h)
GEN_BINARY_TEST(smax, 16b)
GEN_BINARY_TEST(smax, 8b)

GEN_BINARY_TEST(smin, 4s)
GEN_BINARY_TEST(smin, 8h)
GEN_BINARY_TEST(smin, 4h)
GEN_BINARY_TEST(smin, 16b)
GEN_BINARY_TEST(smin, 8b)

GEN_BINARY_TEST(add, 2d)
GEN_BINARY_TEST(add, 4s)
GEN_BINARY_TEST(add, 2s)
GEN_BINARY_TEST(add, 8h)
GEN_BINARY_TEST(add, 4h)
GEN_BINARY_TEST(add, 16b)
GEN_BINARY_TEST(add, 8b)

GEN_BINARY_TEST(sub, 2d)
GEN_BINARY_TEST(sub, 4s)
GEN_BINARY_TEST(sub, 2s)
GEN_BINARY_TEST(sub, 8h)
GEN_BINARY_TEST(sub, 4h)
GEN_BINARY_TEST(sub, 16b)
GEN_BINARY_TEST(sub, 8b)

GEN_BINARY_TEST(mul, 4s)
GEN_BINARY_TEST(mul, 2s)
GEN_BINARY_TEST(mul, 8h)
GEN_BINARY_TEST(mul, 4h)
GEN_BINARY_TEST(mul, 16b)
GEN_BINARY_TEST(mul, 8b)

GEN_BINARY_TEST(mla, 4s)
GEN_BINARY_TEST(mla, 2s)
GEN_BINARY_TEST(mla, 8h)
GEN_BINARY_TEST(mla, 4h)
GEN_BINARY_TEST(mla, 16b)
GEN_BINARY_TEST(mla, 8b)

GEN_BINARY_TEST(mls, 4s)
GEN_BINARY_TEST(mls, 2s)
GEN_BINARY_TEST(mls, 8h)
GEN_BINARY_TEST(mls, 4h)
GEN_BINARY_TEST(mls, 16b)
GEN_BINARY_TEST(mls, 8b)

GEN_BINARY_TEST(and, 16b)
GEN_BINARY_TEST(and, 8b)

GEN_BINARY_TEST(bic, 16b)
GEN_BINARY_TEST(bic, 8b)

GEN_BINARY_TEST(orr, 16b)
GEN_BINARY_TEST(orr, 8b)

GEN_BINARY_TEST(orn, 16b)
GEN_BINARY_TEST(orn, 8b)

GEN_BINARY_TEST(eor, 16b)
GEN_BINARY_TEST(eor, 8b)

GEN_BINARY_TEST(bsl, 16b)
GEN_BINARY_TEST(bsl, 8b)

GEN_BINARY_TEST(bit, 16b)
GEN_BINARY_TEST(bit, 8b)

GEN_BINARY_TEST(bif, 16b)
GEN_BINARY_TEST(bif, 8b)

GEN_BINARY_TEST(cmeq, 2d)
GEN_BINARY_TEST(cmeq, 4s)
GEN_BINARY_TEST(cmeq, 2s)
GEN_BINARY_TEST(cmeq, 8h)
GEN_BINARY_TEST(cmeq, 4h)
GEN_BINARY_TEST(cmeq, 16b)
GEN_BINARY_TEST(cmeq, 8b)

GEN_BINARY_TEST(cmtst, 2d)
GEN_BINARY_TEST(cmtst, 4s)
GEN_BINARY_TEST(cmtst, 2s)
GEN_BINARY_TEST(cmtst, 8h)
GEN_BINARY_TEST(cmtst, 4h)
GEN_BINARY_TEST(cmtst, 16b)
GEN_BINARY_TEST(cmtst, 8b)

GEN_BINARY_TEST(cmhi, 2d)
GEN_BINARY_TEST(cmhi, 4s)
GEN_BINARY_TEST(cmhi, 2s)
GEN_BINARY_TEST(cmhi, 8h)
GEN_BINARY_TEST(cmhi, 4h)
GEN_BINARY_TEST(cmhi, 16b)
GEN_BINARY_TEST(cmhi, 8b)

GEN_BINARY_TEST(cmgt, 2d)
GEN_BINARY_TEST(cmgt, 4s)
GEN_BINARY_TEST(cmgt, 2s)
GEN_BINARY_TEST(cmgt, 8h)
GEN_BINARY_TEST(cmgt, 4h)
GEN_BINARY_TEST(cmgt, 16b)
GEN_BINARY_TEST(cmgt, 8b)

GEN_BINARY_TEST(cmhs, 2d)
GEN_BINARY_TEST(cmhs, 4s)
GEN_BINARY_TEST(cmhs, 2s)
GEN_BINARY_TEST(cmhs, 8h)
GEN_BINARY_TEST(cmhs, 4h)
GEN_BINARY_TEST(cmhs, 16b)
GEN_BINARY_TEST(cmhs, 8b)

GEN_BINARY_TEST(cmge, 2d)
GEN_BINARY_TEST(cmge, 4s)
GEN_BINARY_TEST(cmge, 2s)
GEN_BINARY_TEST(cmge, 8h)
GEN_BINARY_TEST(cmge, 4h)
GEN_BINARY_TEST(cmge, 16b)
GEN_BINARY_TEST(cmge, 8b)

GEN_SHIFT_TEST(ushr, 2d, 2d, 1)
GEN_SHIFT_TEST(ushr, 2d, 2d, 13)
GEN_SHIFT_TEST(ushr, 2d, 2d, 63)
GEN_SHIFT_TEST(sshr, 2d, 2d, 1)
GEN_SHIFT_TEST(sshr, 2d, 2d, 13)
GEN_SHIFT_TEST(sshr, 2d, 2d, 63)
GEN_SHIFT_TEST(shl,  2d, 2d, 1)
GEN_SHIFT_TEST(shl,  2d, 2d, 13)
GEN_SHIFT_TEST(shl,  2d, 2d, 63)

GEN_SHIFT_TEST(ushr, 4s, 4s, 1)
GEN_SHIFT_TEST(ushr, 4s, 4s, 13)
GEN_SHIFT_TEST(ushr, 4s, 4s, 31)
GEN_SHIFT_TEST(sshr, 4s, 4s, 1)
GEN_SHIFT_TEST(sshr, 4s, 4s, 13)
GEN_SHIFT_TEST(sshr, 4s, 4s, 31)
GEN_SHIFT_TEST(shl,  4s, 4s, 1)
GEN_SHIFT_TEST(shl,  4s, 4s, 13)
GEN_SHIFT_TEST(shl,  4s, 4s, 31)

GEN_SHIFT_TEST(ushr, 2s, 2s, 1)
GEN_SHIFT_TEST(ushr, 2s, 2s, 13)
GEN_SHIFT_TEST(ushr, 2s, 2s, 31)
GEN_SHIFT_TEST(sshr, 2s, 2s, 1)
GEN_SHIFT_TEST(sshr, 2s, 2s, 13)
GEN_SHIFT_TEST(sshr, 2s, 2s, 31)
GEN_SHIFT_TEST(shl,  2s, 2s, 1)
GEN_SHIFT_TEST(shl,  2s, 2s, 13)
GEN_SHIFT_TEST(shl,  2s, 2s, 31)

GEN_SHIFT_TEST(ushr, 8h, 8h, 1)
GEN_SHIFT_TEST(ushr, 8h, 8h, 13)
GEN_SHIFT_TEST(ushr, 8h, 8h, 15)
GEN_SHIFT_TEST(sshr, 8h, 8h, 1)
GEN_SHIFT_TEST(sshr, 8h, 8h, 13)
GEN_SHIFT_TEST(sshr, 8h, 8h, 15)
GEN_SHIFT_TEST(shl,  8h, 8h, 1)
GEN_SHIFT_TEST(shl,  8h, 8h, 13)
GEN_SHIFT_TEST(shl,  8h, 8h, 15)

GEN_SHIFT_TEST(ushr, 4h, 4h, 1)
GEN_SHIFT_TEST(ushr, 4h, 4h, 13)
GEN_SHIFT_TEST(ushr, 4h, 4h, 15)
GEN_SHIFT_TEST(sshr, 4h, 4h, 1)
GEN_SHIFT_TEST(sshr, 4h, 4h, 13)
GEN_SHIFT_TEST(sshr, 4h, 4h, 15)
GEN_SHIFT_TEST(shl,  4h, 4h, 1)
GEN_SHIFT_TEST(shl,  4h, 4h, 13)
GEN_SHIFT_TEST(shl,  4h, 4h, 15)

GEN_SHIFT_TEST(ushr, 16b, 16b, 1)
GEN_SHIFT_TEST(ushr, 16b, 16b, 7)
GEN_SHIFT_TEST(sshr, 16b, 16b, 1)
GEN_SHIFT_TEST(sshr, 16b, 16b, 7)
GEN_SHIFT_TEST(shl,  16b, 16b, 1)
GEN_SHIFT_TEST(shl,  16b, 16b, 7)

GEN_SHIFT_TEST(ushr, 8b, 8b, 1)
GEN_SHIFT_TEST(ushr, 8b, 8b, 7)
GEN_SHIFT_TEST(sshr, 8b, 8b, 1)
GEN_SHIFT_TEST(sshr, 8b, 8b, 7)
GEN_SHIFT_TEST(shl,  8b, 8b, 1)
GEN_SHIFT_TEST(shl,  8b, 8b, 7)

GEN_SHIFT_TEST(ushll,  2d, 2s, 0)
GEN_SHIFT_TEST(ushll,  2d, 2s, 15)
GEN_SHIFT_TEST(ushll,  2d, 2s, 31)
GEN_SHIFT_TEST(ushll2, 2d, 4s, 0)
GEN_SHIFT_TEST(ushll2, 2d, 4s, 15)
GEN_SHIFT_TEST(ushll2, 2d, 4s, 31)

GEN_SHIFT_TEST(sshll,  2d, 2s, 0)
GEN_SHIFT_TEST(sshll,  2d, 2s, 15)
GEN_SHIFT_TEST(sshll,  2d, 2s, 31)
GEN_SHIFT_TEST(sshll2, 2d, 4s, 0)
GEN_SHIFT_TEST(sshll2, 2d, 4s, 15)
GEN_SHIFT_TEST(sshll2, 2d, 4s, 31)

GEN_UNARY_TEST(xtn,  2s, 2d)
GEN_UNARY_TEST(xtn2, 4s, 2d)
GEN_UNARY_TEST(xtn,  4h, 4s)
GEN_UNARY_TEST(xtn2, 8h, 4s)
GEN_UNARY_TEST(xtn,  8b, 8h)
GEN_UNARY_TEST(xtn2, 16b, 8h)


/* Generate a test that involves one integer reg and one vector reg,
   with no bias as towards which is input or output. */
#define GEN_ONEINT_ONEVEC_TEST(TESTNAME,INSN,INTREGNO,VECREGNO) \
  __attribute__((noinline)) \
  static void test_##TESTNAME ( void ) { \
     Int i; \
     for (i = 0; i < ITERS; i++) { \
        V128 block[4]; \
        memset(block, 0x55, sizeof(block)); \
        randV128(&block[0]); \
        randV128(&block[1]); \
        randV128(&block[2]); \
        randV128(&block[3]); \
        __asm__ __volatile__( \
           "ldr   q"#VECREGNO", [%0, #0]  ; " \
           "ldr   x"#INTREGNO", [%0, #16] ; " \
           INSN " ; " \
           "str   q"#VECREGNO", [%0, #32] ; " \
           "str   x"#INTREGNO", [%0, #48] ; " \
           : : "r"(&block[0]) : "memory", "v"#VECREGNO, "x"#INTREGNO \
        ); \
        printf(INSN   "   "); \
        showV128(&block[0]); printf("  "); \
        showV128(&block[1]); printf("  "); \
        showV128(&block[2]); printf("  "); \
        showV128(&block[3]); printf("\n"); \
     } \
  }

GEN_ONEINT_ONEVEC_TEST(umov_01, "umov x9, v10.d[0]", 9, 10)
GEN_ONEINT_ONEVEC_TEST(umov_02, "umov x9, v10.d[1]", 9, 10)
GEN_ONEINT_ONEVEC_TEST(umov_03, "umov w9, v10.s[0]", 9, 10)
GEN_ONEINT_ONEVEC_TEST(umov_04, "umov w9, v10.s[3]", 9, 10)
GEN_ONEINT_ONEVEC_TEST(umov_05, "umov w9, v10.h[0]", 9, 10)
GEN_ONEINT_ONEVEC_TEST(umov_06, "umov w9, v10.h[7]", 9, 10)
GEN_ONEINT_ONEVEC_TEST(umov_07, "umov w9, v10.b[0]", 9, 10)
GEN_ONEINT_ONEVEC_TEST(umov_08, "umov w9, v10.b[15]", 9, 10)

GEN_ONEINT_ONEVEC_TEST(smov_01, "smov x9, v10.s[0]", 9, 10)
GEN_ONEINT_ONEVEC_TEST(smov_02, "smov x9, v10.s[3]", 9, 10)

GEN_ONEINT_ONEVEC_TEST(smov_03, "smov x9, v10.h[0]", 9, 10)
GEN_ONEINT_ONEVEC_TEST(smov_04, "smov x9, v10.h[7]", 9, 10)
GEN_ONEINT_ONEVEC_TEST(smov_05, "smov w9, v10.h[0]", 9, 10)
GEN_ONEINT_ONEVEC_TEST(smov_06, "smov w9, v10.h[7]", 9, 10)

GEN_ONEINT_ONEVEC_TEST(smov_07, "smov x9, v10.b[0]", 9, 10)
GEN_ONEINT_ONEVEC_TEST(smov_08, "smov x9, v10.b[15]", 9, 10)
GEN_ONEINT_ONEVEC_TEST(smov_09, "smov w9, v10.b[0]", 9, 10)
GEN_ONEINT_ONEVEC_TEST(smov_10, "smov w9, v10.b[15]", 9, 10)

/* Generate a test that involves two vector regs,
   with no bias as towards which is input or output. */
#define GEN_TWOVEC_TEST(TESTNAME,INSN,VECREG1NO,VECREG2NO) \
  __attribute__((noinline)) \
  static void test_##TESTNAME ( void ) { \
     Int i; \
     for (i = 0; i < ITERS; i++) { \
        V128 block[4]; \
        memset(block, 0x55, sizeof(block)); \
        randV128(&block[0]); \
        randV128(&block[1]); \
        randV128(&block[2]); \
        randV128(&block[3]); \
        __asm__ __volatile__( \
           "ldr   q"#VECREG1NO", [%0, #0]  ; " \
           "ldr   q"#VECREG2NO", [%0, #16] ; " \
           INSN " ; " \
           "str   q"#VECREG1NO", [%0, #32] ; " \
           "str   q"#VECREG2NO", [%0, #48] ; " \
           : : "r"(&block[0]) : "memory", "v"#VECREG1NO, "v"#VECREG2NO \
        ); \
        printf(INSN   "   "); \
        showV128(&block[0]); printf("  "); \
        showV128(&block[1]); printf("  "); \
        showV128(&block[2]); printf("  "); \
        showV128(&block[3]); printf("\n"); \
     } \
  }

GEN_TWOVEC_TEST(fcvtn_01, "fcvtn  v22.2s, v23.2d", 22, 23)
GEN_TWOVEC_TEST(fcvtn_02, "fcvtn2 v22.4s, v23.2d", 22, 23)

GEN_UNARY_TEST(neg, 2d, 2d)
GEN_UNARY_TEST(neg, 4s, 4s)
GEN_UNARY_TEST(neg, 2s, 2s)
GEN_UNARY_TEST(neg, 8h, 8h)
GEN_UNARY_TEST(neg, 4h, 4h)
GEN_UNARY_TEST(neg, 16b, 16b)
GEN_UNARY_TEST(neg, 8b,  8b)
GEN_BINARY_TEST(fadd, 2d)
GEN_BINARY_TEST(fadd, 4s)
GEN_BINARY_TEST(fadd, 2s)
GEN_BINARY_TEST(fsub, 2d)
GEN_BINARY_TEST(fsub, 4s)
GEN_BINARY_TEST(fsub, 2s)
GEN_BINARY_TEST(fmul, 2d)
GEN_BINARY_TEST(fmul, 4s)
GEN_BINARY_TEST(fmul, 2s)
GEN_BINARY_TEST(fdiv, 2d)
GEN_BINARY_TEST(fdiv, 4s)
GEN_BINARY_TEST(fdiv, 2s)
GEN_BINARY_TEST(fmla, 2d)
GEN_BINARY_TEST(fmla, 4s)
GEN_BINARY_TEST(fmla, 2s)
GEN_BINARY_TEST(fmls, 2d)
GEN_BINARY_TEST(fmls, 4s)
GEN_BINARY_TEST(fmls, 2s)
GEN_BINARY_TEST(fabd, 2d)
GEN_BINARY_TEST(fabd, 4s)
GEN_BINARY_TEST(fabd, 2s)

/* Generate a test that involves three vector regs,
   with no bias as towards which is input or output.  It's also OK
   to use v16, v17, v18 as scratch. */
#define GEN_THREEVEC_TEST(TESTNAME,INSN,VECREG1NO,VECREG2NO,VECREG3NO)  \
  __attribute__((noinline)) \
  static void test_##TESTNAME ( void ) { \
     Int i; \
     for (i = 0; i < ITERS; i++) { \
        V128 block[6]; \
        memset(block, 0x55, sizeof(block)); \
        randV128(&block[0]); \
        randV128(&block[1]); \
        randV128(&block[2]); \
        randV128(&block[3]); \
        randV128(&block[4]); \
        randV128(&block[5]); \
        __asm__ __volatile__( \
           "ldr   q"#VECREG1NO", [%0, #0]  ; " \
           "ldr   q"#VECREG2NO", [%0, #16] ; " \
           "ldr   q"#VECREG3NO", [%0, #32] ; " \
           INSN " ; " \
           "str   q"#VECREG1NO", [%0, #48] ; " \
           "str   q"#VECREG2NO", [%0, #64] ; " \
           "str   q"#VECREG3NO", [%0, #80] ; " \
           : : "r"(&block[0]) \
           : "memory", "v"#VECREG1NO, "v"#VECREG2NO, "v"#VECREG3NO, \
             "v16", "v17", "v18" \
        ); \
        printf(INSN   "   "); \
        showV128(&block[0]); printf("  "); \
        showV128(&block[1]); printf("  "); \
        showV128(&block[2]); printf("  "); \
        showV128(&block[3]); printf("  "); \
        showV128(&block[4]); printf("  "); \
        showV128(&block[5]); printf("\n"); \
     } \
  }

GEN_THREEVEC_TEST(add_d_d_d, "add d21, d22, d23", 21, 22, 23)
GEN_THREEVEC_TEST(sub_d_d_d, "sub d21, d22, d23", 21, 22, 23)

/* overkill -- don't need two vecs, only one */
GEN_TWOVEC_TEST(fmov_scalar_imm_01, "fmov d22, #0.125", 22, 23)
GEN_TWOVEC_TEST(fmov_scalar_imm_02, "fmov d22, #-4.0",  22, 23)
GEN_TWOVEC_TEST(fmov_scalar_imm_03, "fmov d22, #1.0",   22, 23)
GEN_TWOVEC_TEST(fmov_scalar_imm_04, "fmov s22, #0.125", 22, 23)
GEN_TWOVEC_TEST(fmov_scalar_imm_05, "fmov s22, #-4.0",  22, 23)
GEN_TWOVEC_TEST(fmov_scalar_imm_06, "fmov s22, #-1.0",   22, 23)

GEN_ONEINT_ONEVEC_TEST(fmov_gen_01, "fmov s7,      w15", 15, 7)
GEN_ONEINT_ONEVEC_TEST(fmov_gen_02, "fmov d7,      x15", 15, 7)
GEN_ONEINT_ONEVEC_TEST(fmov_gen_03, "fmov v7.d[1], x15", 15, 7)
GEN_ONEINT_ONEVEC_TEST(fmov_gen_04, "fmov w15,      s7", 15, 7)
GEN_ONEINT_ONEVEC_TEST(fmov_gen_05, "fmov x15,      d7", 15, 7)
GEN_ONEINT_ONEVEC_TEST(fmov_gen_06, "fmov x15, v7.d[1]", 15, 7)

GEN_TWOVEC_TEST(movi_vector_imm_01, "fmov d22,    #0.125", 22, 23)
GEN_TWOVEC_TEST(movi_vector_imm_02, "fmov d22,    #-4.0",  22, 23)
GEN_TWOVEC_TEST(movi_vector_imm_03, "fmov d22,    #1.0",   22, 23)
GEN_TWOVEC_TEST(movi_vector_imm_04, "fmov v22.2d, #0.125", 22, 23)
GEN_TWOVEC_TEST(movi_vector_imm_05, "fmov v22.2d, #-4.0",  22, 23)
GEN_TWOVEC_TEST(movi_vector_imm_06, "fmov v22.2d, #1.0",   22, 23)

GEN_ONEINT_ONEVEC_TEST(sucvtf_01, "scvtf s7, w15", 15, 7)
GEN_ONEINT_ONEVEC_TEST(sucvtf_02, "scvtf d7, w15", 15, 7)
GEN_ONEINT_ONEVEC_TEST(sucvtf_03, "scvtf s7, x15", 15, 7)
GEN_ONEINT_ONEVEC_TEST(sucvtf_04, "scvtf d7, x15", 15, 7)
GEN_ONEINT_ONEVEC_TEST(sucvtf_05, "ucvtf s7, w15", 15, 7)
GEN_ONEINT_ONEVEC_TEST(sucvtf_06, "ucvtf d7, w15", 15, 7)
GEN_ONEINT_ONEVEC_TEST(sucvtf_07, "ucvtf s7, x15", 15, 7)
GEN_ONEINT_ONEVEC_TEST(sucvtf_08, "ucvtf d7, x15", 15, 7)

GEN_THREEVEC_TEST(fadd_d,  "fadd d2, d11, d29", 2, 11, 29)
GEN_THREEVEC_TEST(fadd_s,  "fadd s2, s11, s29", 2, 11, 29)
GEN_THREEVEC_TEST(fsub_d,  "fsub d2, d11, d29", 2, 11, 29)
GEN_THREEVEC_TEST(fsub_s,  "fsub s2, s11, s29", 2, 11, 29)
GEN_THREEVEC_TEST(fmul_d,  "fmul d2, d11, d29", 2, 11, 29)
GEN_THREEVEC_TEST(fmul_s,  "fmul s2, s11, s29", 2, 11, 29)
GEN_THREEVEC_TEST(fdiv_d,  "fdiv d2, d11, d29", 2, 11, 29)
GEN_THREEVEC_TEST(fdiv_s,  "fdiv s2, s11, s29", 2, 11, 29)
GEN_THREEVEC_TEST(fnmul_d, "fnmul d2, d11, d29", 2, 11, 29)
GEN_THREEVEC_TEST(fnmul_s, "fnmul s2, s11, s29", 2, 11, 29)

GEN_THREEVEC_TEST(fabd_d,  "fabd d2, d11, d29", 2, 11, 29)
GEN_THREEVEC_TEST(fabd_s,  "fabd s2, s11, s29", 2, 11, 29)

GEN_TWOVEC_TEST(fmov_d,  "fmov d22, d23",   22, 23)
GEN_TWOVEC_TEST(fmov_s,  "fmov s22, s23",   22, 23)
GEN_TWOVEC_TEST(fabs_d,  "fabs d22, d23",   22, 23)
GEN_TWOVEC_TEST(fabs_s,  "fabs s22, s23",   22, 23)
GEN_TWOVEC_TEST(fneg_d,  "fneg d22, d23",   22, 23)
GEN_TWOVEC_TEST(fneg_s,  "fneg s22, s23",   22, 23)
GEN_TWOVEC_TEST(fsqrt_d, "fsqrt d22, d23",   22, 23)
GEN_TWOVEC_TEST(fsqrt_s, "fsqrt s22, s23",   22, 23)

GEN_UNARY_TEST(fneg, 2d, 2d)
GEN_UNARY_TEST(fneg, 4s, 4s)
GEN_UNARY_TEST(fneg, 2s, 2s)
GEN_UNARY_TEST(fabs, 2d, 2d)
GEN_UNARY_TEST(fabs, 4s, 4s)
GEN_UNARY_TEST(fabs, 2s, 2s)

GEN_BINARY_TEST(fcmeq,  2d)
GEN_BINARY_TEST(fcmeq,  4s)
GEN_BINARY_TEST(fcmeq,  2s)
GEN_BINARY_TEST(fcmge,  2d)
GEN_BINARY_TEST(fcmge,  4s)
GEN_BINARY_TEST(fcmge,  2s)
GEN_BINARY_TEST(fcmgt,  2d)
GEN_BINARY_TEST(fcmgt,  4s)
GEN_BINARY_TEST(fcmgt,  2s)
GEN_BINARY_TEST(facge,  2d)
GEN_BINARY_TEST(facge,  4s)
GEN_BINARY_TEST(facge,  2s)
GEN_BINARY_TEST(facgt,  2d)
GEN_BINARY_TEST(facgt,  4s)
GEN_BINARY_TEST(facgt,  2s)

// Uses v15 as the first table entry
GEN_THREEVEC_TEST(
   tbl_16b_1reg, "tbl v21.16b, {v15.16b}, v23.16b", 21, 15, 23)
// and v15 ^ v21 as the second table entry
GEN_THREEVEC_TEST(
   tbl_16b_2reg, "eor v16.16b, v15.16b, v21.16b ; "
                 "tbl v21.16b, {v15.16b, v16.16b}, v23.16b", 21, 15, 23)
// and v15 ^ v23 as the third table entry
GEN_THREEVEC_TEST(
   tbl_16b_3reg, "eor v16.16b, v15.16b, v21.16b ; "
                 "eor v17.16b, v15.16b, v23.16b ; "
                 "tbl v21.16b, {v15.16b, v16.16b, v17.16b}, v23.16b",
                 21, 15, 23)
// and v21 ^ v23 as the fourth table entry
GEN_THREEVEC_TEST(
   tbl_16b_4reg, "eor v16.16b, v15.16b, v21.16b ; "
                 "eor v17.16b, v15.16b, v23.16b ; "
                 "eor v18.16b, v21.16b, v23.16b ; "
                 "tbl v21.16b, {v15.16b, v16.16b, v17.16b, v18.16b}, v23.16b",
                 21, 15, 23)

// Same register scheme for tbl .8b, tbx .16b, tbx.8b
GEN_THREEVEC_TEST(
   tbl_8b_1reg, "tbl v21.8b, {v15.16b}, v23.8b", 21, 15, 23)
GEN_THREEVEC_TEST(
   tbl_8b_2reg, "eor v16.16b, v15.16b, v21.16b ; "
                "tbl v21.8b, {v15.16b, v16.16b}, v23.8b", 21, 15, 23)
GEN_THREEVEC_TEST(
   tbl_8b_3reg, "eor v16.16b, v15.16b, v21.16b ; "
                "eor v17.16b, v15.16b, v23.16b ; "
                "tbl v21.8b, {v15.16b, v16.16b, v17.16b}, v23.8b",
                21, 15, 23)
GEN_THREEVEC_TEST(
   tbl_8b_4reg, "eor v16.16b, v15.16b, v21.16b ; "
                "eor v17.16b, v15.16b, v23.16b ; "
                "eor v18.16b, v21.16b, v23.16b ; "
                "tbl v21.8b, {v15.16b, v16.16b, v17.16b, v18.16b}, v23.8b",
                21, 15, 23)

GEN_THREEVEC_TEST(
   tbx_16b_1reg, "tbx v21.16b, {v15.16b}, v23.16b", 21, 15, 23)
GEN_THREEVEC_TEST(
   tbx_16b_2reg, "eor v16.16b, v15.16b, v21.16b ; "
                 "tbx v21.16b, {v15.16b, v16.16b}, v23.16b", 21, 15, 23)
GEN_THREEVEC_TEST(
   tbx_16b_3reg, "eor v16.16b, v15.16b, v21.16b ; "
                 "eor v17.16b, v15.16b, v23.16b ; "
                 "tbx v21.16b, {v15.16b, v16.16b, v17.16b}, v23.16b",
                 21, 15, 23)
GEN_THREEVEC_TEST(
   tbx_16b_4reg, "eor v16.16b, v15.16b, v21.16b ; "
                 "eor v17.16b, v15.16b, v23.16b ; "
                 "eor v18.16b, v21.16b, v23.16b ; "
                 "tbx v21.16b, {v15.16b, v16.16b, v17.16b, v18.16b}, v23.16b",
                 21, 15, 23)

// Same register scheme for tbx .8b, tbx .16b, tbx.8b
GEN_THREEVEC_TEST(
   tbx_8b_1reg, "tbx v21.8b, {v15.16b}, v23.8b", 21, 15, 23)
GEN_THREEVEC_TEST(
   tbx_8b_2reg, "eor v16.16b, v15.16b, v21.16b ; "
                "tbx v21.8b, {v15.16b, v16.16b}, v23.8b", 21, 15, 23)
GEN_THREEVEC_TEST(
   tbx_8b_3reg, "eor v16.16b, v15.16b, v21.16b ; "
                "eor v17.16b, v15.16b, v23.16b ; "
                "tbx v21.8b, {v15.16b, v16.16b, v17.16b}, v23.8b",
                21, 15, 23)
GEN_THREEVEC_TEST(
   tbx_8b_4reg, "eor v16.16b, v15.16b, v21.16b ; "
                "eor v17.16b, v15.16b, v23.16b ; "
                "eor v18.16b, v21.16b, v23.16b ; "
                "tbx v21.8b, {v15.16b, v16.16b, v17.16b, v18.16b}, v23.8b",
                21, 15, 23)

GEN_TWOVEC_TEST(cmge_zero_2d,  "cmge v5.2d,  v22.2d,  #0", 5, 22)
GEN_TWOVEC_TEST(cmge_zero_4s,  "cmge v5.4s,  v22.4s,  #0", 5, 22)
GEN_TWOVEC_TEST(cmge_zero_2s,  "cmge v5.2s,  v22.2s,  #0", 5, 22)
GEN_TWOVEC_TEST(cmge_zero_8h,  "cmge v5.8h,  v22.8h,  #0", 5, 22)
GEN_TWOVEC_TEST(cmge_zero_4h,  "cmge v5.4h,  v22.4h,  #0", 5, 22)
GEN_TWOVEC_TEST(cmge_zero_16b, "cmge v5.16b, v22.16b, #0", 5, 22)
GEN_TWOVEC_TEST(cmge_zero_8b,  "cmge v5.8b,  v22.8b,  #0", 5, 22)

GEN_TWOVEC_TEST(cmgt_zero_2d,  "cmgt v5.2d,  v22.2d,  #0", 5, 22)
GEN_TWOVEC_TEST(cmgt_zero_4s,  "cmgt v5.4s,  v22.4s,  #0", 5, 22)
GEN_TWOVEC_TEST(cmgt_zero_2s,  "cmgt v5.2s,  v22.2s,  #0", 5, 22)
GEN_TWOVEC_TEST(cmgt_zero_8h,  "cmgt v5.8h,  v22.8h,  #0", 5, 22)
GEN_TWOVEC_TEST(cmgt_zero_4h,  "cmgt v5.4h,  v22.4h,  #0", 5, 22)
GEN_TWOVEC_TEST(cmgt_zero_16b, "cmgt v5.16b, v22.16b, #0", 5, 22)
GEN_TWOVEC_TEST(cmgt_zero_8b,  "cmgt v5.8b,  v22.8b,  #0", 5, 22)

GEN_TWOVEC_TEST(cmle_zero_2d,  "cmle v5.2d,  v22.2d,  #0", 5, 22)
GEN_TWOVEC_TEST(cmle_zero_4s,  "cmle v5.4s,  v22.4s,  #0", 5, 22)
GEN_TWOVEC_TEST(cmle_zero_2s,  "cmle v5.2s,  v22.2s,  #0", 5, 22)
GEN_TWOVEC_TEST(cmle_zero_8h,  "cmle v5.8h,  v22.8h,  #0", 5, 22)
GEN_TWOVEC_TEST(cmle_zero_4h,  "cmle v5.4h,  v22.4h,  #0", 5, 22)
GEN_TWOVEC_TEST(cmle_zero_16b, "cmle v5.16b, v22.16b, #0", 5, 22)
GEN_TWOVEC_TEST(cmle_zero_8b,  "cmle v5.8b,  v22.8b,  #0", 5, 22)

GEN_TWOVEC_TEST(cmeq_zero_2d,  "cmeq v5.2d,  v22.2d,  #0", 5, 22)
GEN_TWOVEC_TEST(cmeq_zero_4s,  "cmeq v5.4s,  v22.4s,  #0", 5, 22)
GEN_TWOVEC_TEST(cmeq_zero_2s,  "cmeq v5.2s,  v22.2s,  #0", 5, 22)
GEN_TWOVEC_TEST(cmeq_zero_8h,  "cmeq v5.8h,  v22.8h,  #0", 5, 22)
GEN_TWOVEC_TEST(cmeq_zero_4h,  "cmeq v5.4h,  v22.4h,  #0", 5, 22)
GEN_TWOVEC_TEST(cmeq_zero_16b, "cmeq v5.16b, v22.16b, #0", 5, 22)
GEN_TWOVEC_TEST(cmeq_zero_8b,  "cmeq v5.8b,  v22.8b,  #0", 5, 22)

GEN_TWOVEC_TEST(cmlt_zero_2d,  "cmlt v5.2d,  v22.2d,  #0", 5, 22)
GEN_TWOVEC_TEST(cmlt_zero_4s,  "cmlt v5.4s,  v22.4s,  #0", 5, 22)
GEN_TWOVEC_TEST(cmlt_zero_2s,  "cmlt v5.2s,  v22.2s,  #0", 5, 22)
GEN_TWOVEC_TEST(cmlt_zero_8h,  "cmlt v5.8h,  v22.8h,  #0", 5, 22)
GEN_TWOVEC_TEST(cmlt_zero_4h,  "cmlt v5.4h,  v22.4h,  #0", 5, 22)
GEN_TWOVEC_TEST(cmlt_zero_16b, "cmlt v5.16b, v22.16b, #0", 5, 22)
GEN_TWOVEC_TEST(cmlt_zero_8b,  "cmlt v5.8b,  v22.8b,  #0", 5, 22)



/* IMPORTANT: keep the tests in here in the same order as the
   implementations are in guest_arm64_toIR.c. */
int main ( void )
{
   assert(sizeof(V128) == 16);

   printf("BEGIN: FMOV (general)\n");
   test_fmov_gen_01();
   test_fmov_gen_02();
   test_fmov_gen_03();
   test_fmov_gen_04();
   test_fmov_gen_05();
   test_fmov_gen_06();
   printf("END:   FMOV (general)\n\n");

   printf("BEGIN: FMOV (scalar, immediate)\n");
   test_fmov_scalar_imm_01();
   test_fmov_scalar_imm_02();
   test_fmov_scalar_imm_03();
   test_fmov_scalar_imm_04();
   test_fmov_scalar_imm_05();
   test_fmov_scalar_imm_06();
   printf("END:   FMOV (scalar, immediate)\n\n");

   printf("BEGIN: {FMOV,MOVI} (vector, immediate)\n");
   test_movi_vector_imm_01();
   test_movi_vector_imm_02();
   test_movi_vector_imm_03();
   test_movi_vector_imm_04();
   test_movi_vector_imm_05();
   test_movi_vector_imm_06();
   printf("END:   {FMOV,MOVI} (vector, immediate)\n\n");

   printf("BEGIN: {S,U}CVTF (scalar, integer)\n");
   test_sucvtf_01();
   test_sucvtf_02();
   test_sucvtf_03();
   test_sucvtf_04();
   test_sucvtf_05();
   test_sucvtf_06();
   test_sucvtf_07();
   test_sucvtf_08();
   printf("END:   {S,U}CVTF (scalar, integer)\n\n");

   printf("BEGIN: F{ADD,SUB,MUL,DIV,NMUL} (scalar)\n");
   test_fadd_d();
   test_fadd_s();
   test_fsub_d();
   test_fsub_s();
   test_fmul_d();
   test_fmul_s();
   test_fdiv_d();
   test_fdiv_s();
   test_fnmul_d();
   test_fnmul_s();
   printf("END:   F{ADD,SUB,MUL,DIV,NMUL} (scalar)\n\n");

   printf("BEGIN: F{MOV,ABS,NEG,SQRT} D/D or S/S\n");
   test_fmov_d();
   test_fmov_s();
   test_fabs_d();
   test_fabs_s();
   test_fneg_d();
   test_fneg_s();
   test_fsqrt_d();
   test_fsqrt_s();
   printf("END:   F{MOV,ABS,NEG,SQRT} D/D or S/S\n\n");

   printf("BEGIN: F{ABS,NEG} (vector)\n");
   test_fabs_2d_2d();
   test_fabs_4s_4s();
   test_fabs_2s_2s();
   test_fneg_2d_2d();
   test_fneg_4s_4s();
   test_fneg_2s_2s();
   printf("END:   F{ABS,NEG} (vector)\n\n");

   printf("FCMP,FCMPE MISSING\n\n");

   printf("F{N}M{ADD,SUB} MISSING\n\n");

   printf("FCVT{N,P,M,Z}{S,U} (scalar, integer) MISSING\n\n");

   printf("FRINT{I,M,P,Z} (scalar) MISSING\n\n");

   printf("FCVT (scalar) MISSING\n\n");

   printf("BEGIN: FABD (scalar) MISSING\n");
   test_fabd_d();
   test_fabd_s();
   printf("END:   FABD (scalar) MISSING\n\n");

   printf("{S,U}CVTF (vector, integer) MISSING\n\n");

   printf("BEGIN: F{ADD,SUB,MUL,DIV,MLA,MLS,ABD} (vector)\n");
   test_fadd_2d();
   test_fadd_4s();
   test_fadd_2s();
   test_fsub_2d();
   test_fsub_4s();
   test_fsub_2s();
   test_fmul_2d();
   test_fmul_4s();
   test_fmul_2s();
   test_fdiv_2d();
   test_fdiv_4s();
   test_fdiv_2s();
   test_fmla_2d();
   test_fmla_4s();
   test_fmla_2s();
   test_fmls_2d();
   test_fmls_4s();
   test_fmls_2s();
   test_fabd_2d();
   test_fabd_4s();
   test_fabd_2s();
   printf("END:   F{ADD,SUB,MUL,DIV,MLA,MLS,ABD} (vector)\n\n");

   printf("BEGIN: FCM{EQ,GE,GT}, FAC{GE,GT} (vector)\n");
   test_fcmeq_2d();
   test_fcmeq_4s();
   test_fcmeq_2s();
   test_fcmge_2d();
   test_fcmge_4s();
   test_fcmge_2s();
   test_fcmgt_2d();
   test_fcmgt_4s();
   test_fcmgt_2s();
   test_facge_2d();
   test_facge_4s();
   test_facge_2s();
   test_facgt_2d();
   test_facgt_4s();
   test_facgt_2s();
   printf("END:   FCM{EQ,GE,GT}, FAC{GE,GT} (vector)\n");

   printf("BEGIN: FCVTN (MISSING 16F <- 32F cases)\n");
   test_fcvtn_01();
   test_fcvtn_02();
   printf("END:   FCVTN (MISSING 16F <- 32F cases)\n\n");

   printf("BEGIN: ADD/SUB (vector)\n");
   test_add_2d();
   test_add_4s();
   test_add_2s();
   test_add_8h();
   test_add_4h();
   test_add_16b();
   test_add_8b();
   test_sub_2d();
   test_sub_4s();
   test_sub_2s();
   test_sub_8h();
   test_sub_4h();
   test_sub_16b();
   test_sub_8b();
   printf("END:   ADD/SUB (vector)\n\n");

   printf("BEGIN: ADD/SUB (scalar)\n");
   test_add_d_d_d();
   test_sub_d_d_d();
   printf("END:   ADD/SUB (scalar)\n\n");

   printf("BEGIN: MUL/PMUL/MLA/MLS (vector)\n");
   test_mul_4s();
   test_mul_2s();
   test_mul_8h();
   test_mul_4h();
   test_mul_16b();
   test_mul_8b();
   test_mla_4s();
   test_mla_2s();
   test_mla_8h();
   test_mla_4h();
   test_mla_16b();
   test_mla_8b();
   test_mls_4s();
   test_mls_2s();
   test_mls_8h();
   test_mls_4h();
   test_mls_16b();
   test_mls_8b();
   printf("END:   MUL/PMUL/MLA/MLS (vector) (MISSING PMUL)\n\n");

   printf("BEGIN: {S,U}{MIN,MAX} (vector)\n");
   test_umax_4s();
   test_umax_8h();
   test_umax_4h();
   test_umax_16b();
   test_umax_8b();
   test_umin_4s();
   test_umin_8h();
   test_umin_4h();
   test_umin_16b();
   test_umin_8b();
   test_smax_4s();
   test_smax_8h();
   test_smax_4h();
   test_smax_16b();
   test_smax_8b();
   test_smin_4s();
   test_smin_8h();
   test_smin_4h();
   test_smin_16b();
   test_smin_8b();
   printf("END:   {S,U}{MIN,MAX} (vector)\n\n");

   printf("BEGIN: {S,U}{MIN,MAX}V\n");
   test_UMINV();
   test_UMAXV();
   test_SMINV();
   test_SMAXV();
   printf("END:   {S,U}{MIN,MAX}V\n\n");

   printf("BEGIN: {AND,BIC,ORR,ORN} (vector)\n");
   test_and_16b();
   test_and_8b();
   test_bic_16b();
   test_bic_8b();
   test_orr_16b();
   test_orr_8b();
   test_orn_16b();
   test_orn_8b();
   printf("END:   {AND,BIC,ORR,ORN} (vector)\n\n");

   printf("BEGIN: CM{EQ,HI,HS,GE,GT,TST,LE,LT} (vector)\n\n");
   test_cmeq_2d();
   test_cmeq_4s();
   test_cmeq_2s();
   test_cmeq_8h();
   test_cmeq_4h();
   test_cmeq_16b();
   test_cmeq_8b();
   test_cmtst_2d();
   test_cmtst_4s();
   test_cmtst_2s();
   test_cmtst_8h();
   test_cmtst_4h();
   test_cmtst_16b();
   test_cmtst_8b();
   test_cmhi_2d();
   test_cmhi_4s();
   test_cmhi_2s();
   test_cmhi_8h();
   test_cmhi_4h();
   test_cmhi_16b();
   test_cmhi_8b();
   test_cmgt_2d();
   test_cmgt_4s();
   test_cmgt_2s();
   test_cmgt_8h();
   test_cmgt_4h();
   test_cmgt_16b();
   test_cmgt_8b();
   test_cmhs_2d();
   test_cmhs_4s();
   test_cmhs_2s();
   test_cmhs_8h();
   test_cmhs_4h();
   test_cmhs_16b();
   test_cmhs_8b();
   test_cmge_2d();
   test_cmge_4s();
   test_cmge_2s();
   test_cmge_8h();
   test_cmge_4h();
   test_cmge_16b();
   test_cmge_8b();
   test_cmge_zero_2d();
   test_cmge_zero_4s();
   test_cmge_zero_2s();
   test_cmge_zero_8h();
   test_cmge_zero_4h();
   test_cmge_zero_16b();
   test_cmge_zero_8b();
   test_cmgt_zero_2d();
   test_cmgt_zero_4s();
   test_cmgt_zero_2s();
   test_cmgt_zero_8h();
   test_cmgt_zero_4h();
   test_cmgt_zero_16b();
   test_cmgt_zero_8b();
   test_cmle_zero_2d();
   test_cmle_zero_4s();
   test_cmle_zero_2s();
   test_cmle_zero_8h();
   test_cmle_zero_4h();
   test_cmle_zero_16b();
   test_cmle_zero_8b();
   test_cmeq_zero_2d();
   test_cmeq_zero_4s();
   test_cmeq_zero_2s();
   test_cmeq_zero_8h();
   test_cmeq_zero_4h();
   test_cmeq_zero_16b();
   test_cmeq_zero_8b();
   test_cmlt_zero_2d();
   test_cmlt_zero_4s();
   test_cmlt_zero_2s();
   test_cmlt_zero_8h();
   test_cmlt_zero_4h();
   test_cmlt_zero_16b();
   test_cmlt_zero_8b();
   printf("END:   CM{EQ,HI,HS,GE,GT,TST,LE,LT} (vector)\n\n");

   printf("BEGIN: {EOR,BSL,BIT,BIF} (vector)\n");
   test_eor_16b();
   test_eor_8b();
   test_bsl_16b();
   test_bsl_8b();
   test_bit_16b();
   test_bit_8b();
   test_bif_16b();
   test_bif_8b();
   printf("END:   {EOR,BSL,BIT,BIF} (vector)\n\n");

   printf("BEGIN: {USHR,SSHR,SHL} (vector, immediate)\n");
   test_ushr_2d_2d_1();
   test_ushr_2d_2d_13();
   test_ushr_2d_2d_63();
   test_sshr_2d_2d_1();
   test_sshr_2d_2d_13();
   test_sshr_2d_2d_63();
   test_shl_2d_2d_1();
   test_shl_2d_2d_13();
   test_shl_2d_2d_63();

   test_ushr_4s_4s_1();
   test_ushr_4s_4s_13();
   test_ushr_4s_4s_31();
   test_sshr_4s_4s_1();
   test_sshr_4s_4s_13();
   test_sshr_4s_4s_31();
   test_shl_4s_4s_1();
   test_shl_4s_4s_13();
   test_shl_4s_4s_31();

   test_ushr_2s_2s_1();
   test_ushr_2s_2s_13();
   test_ushr_2s_2s_31();
   test_sshr_2s_2s_1();
   test_sshr_2s_2s_13();
   test_sshr_2s_2s_31();
   test_shl_2s_2s_1();
   test_shl_2s_2s_13();
   test_shl_2s_2s_31();

   test_ushr_8h_8h_1();
   test_ushr_8h_8h_13();
   test_ushr_8h_8h_15();
   test_sshr_8h_8h_1();
   test_sshr_8h_8h_13();
   test_sshr_8h_8h_15();
   test_shl_8h_8h_1();
   test_shl_8h_8h_13();
   test_shl_8h_8h_15();

   test_ushr_4h_4h_1();
   test_ushr_4h_4h_13();
   test_ushr_4h_4h_15();
   test_sshr_4h_4h_1();
   test_sshr_4h_4h_13();
   test_sshr_4h_4h_15();
   test_shl_4h_4h_1();
   test_shl_4h_4h_13();
   test_shl_4h_4h_15();

   test_ushr_16b_16b_1();
   test_ushr_16b_16b_7();
   test_sshr_16b_16b_1();
   test_sshr_16b_16b_7();
   test_shl_16b_16b_1();
   test_shl_16b_16b_7();

   test_ushr_8b_8b_1();
   test_ushr_8b_8b_7();
   test_sshr_8b_8b_1();
   test_sshr_8b_8b_7();
   test_shl_8b_8b_1();
   test_shl_8b_8b_7();
   printf("END:   {USHR,SSHR,SHL} (vector, immediate)\n\n");

   printf("BEGIN: {U,S}SHLL{,2}\n");
   test_ushll_2d_2s_0();
   test_ushll_2d_2s_15();
   test_ushll_2d_2s_31();
   test_ushll2_2d_4s_0();
   test_ushll2_2d_4s_15();
   test_ushll2_2d_4s_31();
   test_sshll_2d_2s_0();
   test_sshll_2d_2s_15();
   test_sshll_2d_2s_31();
   test_sshll2_2d_4s_0();
   test_sshll2_2d_4s_15();
   test_sshll2_2d_4s_31();
   printf("END:   {U,S}SHLL{,2} (MISSING h_b and s_h versions)\n\n");

   printf("BEGIN: XTN{,2}\n");
   test_xtn_2s_2d();
   test_xtn2_4s_2d();
   test_xtn_4h_4s();
   test_xtn2_8h_4s();
   test_xtn_8b_8h();
   test_xtn2_16b_8h();
   printf("END:   XTN{,2}\n\n");

   printf("DUP (element, vector) COMPLETELY MISSING\n\n");

   printf("DUP (general, vector) COMPLETELY MISSING\n\n");

   printf("BEGIN: {S,U}MOV\n");
   test_umov_01();
   test_umov_02();
   test_umov_03();
   test_umov_04();
   test_umov_05();
   test_umov_06();
   test_umov_07();
   test_umov_08();
   test_smov_01();
   test_smov_02();
   test_smov_03();
   test_smov_04();
   test_smov_05();
   test_smov_06();
   test_smov_07();
   test_smov_08();
   test_smov_09();
   test_smov_10();
   printf("END:   {S,U}MOV\n\n");

   printf("BEGIN: INS (general)\n");
   test_INS_general();
   printf("END:   INS (general)\n\n");

   printf("BEGIN: NEG (vector)\n");
   test_neg_2d_2d();
   test_neg_4s_4s();
   test_neg_2s_2s();
   test_neg_8h_8h();
   test_neg_4h_4h();
   test_neg_16b_16b();
   test_neg_8b_8b();
   printf("END:   NEG (vector)\n\n");

   printf("BEGIN: TBL, TBX\n");
   test_tbl_16b_1reg();
   test_tbl_16b_2reg();
   test_tbl_16b_3reg();
   test_tbl_16b_4reg();
   test_tbl_8b_1reg();
   test_tbl_8b_2reg();
   test_tbl_8b_3reg();
   test_tbl_8b_4reg();
   test_tbx_16b_1reg();
   test_tbx_16b_2reg();
   test_tbx_16b_3reg();
   test_tbx_16b_4reg();
   test_tbx_8b_1reg();
   test_tbx_8b_2reg();
   test_tbx_8b_3reg();
   test_tbx_8b_4reg();
   printf("END:   TBL, TBX\n");

   return 0;
}

/*
   abs      d
   abs      2d,4s,2s,8h,4h,16b,8b
   add      d
   add      2d,4s,2s,8h,4h,16b,8b
   addhn    2s.2d.2d, 4s.2d.2d, h_from_s and b_from_h (add and get high half)
   addp     d (add pairs, across)
   addp     2d,4s,2s,8h,4h,16b,8b
   addv     4s,8h,4h,16b,18b (reduce across vector)
   aesd     16b (aes single round decryption)
   aese     16b (aes single round encryption)
   aesimc   16b (aes inverse mix columns)
   aesmc    16b (aes mix columns)
   and      16b,8b

   bic      4s,2s,8h,4h (vector, imm)
   also movi, mvni, orr

   bic      16b,8b (vector,reg) (bit clear)
   bif      16b,8b (vector) (bit insert if false)
   bit      16b,8b (vector) (bit insert if true)
   bsl      16b,8b (vector) (bit select)

   cls      4s,2s,8h,4h,16b,8b (count leading sign bits)
   clz      4s,2s,8h,4h,16b,8b (count leading zero bits)

   cmeq     d
   cmeq     2d,4s,2s,8h,4h,16b,8b
   cmeq_z   d
   cmeq_z   2d,4s,2s,8h,4h,16b,8b

   cmge     d
   cmge     2d,4s,2s,8h,4h,16b,8b
   cmge_z   d
   cmge_z   2d,4s,2s,8h,4h,16b,8b

   cmgt     d
   cmgt     2d,4s,2s,8h,4h,16b,8b
   cmgt_z   d
   cmgt_z   2d,4s,2s,8h,4h,16b,8b

   cmhi     d
   cmhi     2d,4s,2s,8h,4h,16b,8b

   cmhs     d
   cmhs     2d,4s,2s,8h,4h,16b,8b

   cmle_z   d
   cmle_z   2d,4s,2s,8h,4h,16b,8b

   cmlt_z   d
   cmlt_z   2d,4s,2s,8h,4h,16b,8b

   cmtst    d
   cmtst    2d,4s,2s,8h,4h,16b,8b

   cnt      16b,8b (population count per byte)

   dup      d,s,h,b (vec elem to scalar)
   dup      2d,4s,2s,8h,4h,16b,8b (vec elem to vector)
   dup      2d,4s,2s,8h,4h,16b,8b (general reg to vector)

   eor      16b,8b (vector)
   ext      16b,8b,#imm4 (concat 2 vectors, then slice)

   fabd     d,s
   fabd     2d,4s,2s

   fabs     d,s
   fabs     2d,4s,2s

   facge    s,d  (floating abs compare GE)
   facge    2d,4s,2s

   facgt    s,d  (floating abs compare GE)
   facgt    2d,4s,2s

   fadd     d,s
   fadd     2d,4s,2s

   faddp    d,s (floating add pair)
   faddp    2d,4s,2s

   fccmp    d,s (floating point conditional quiet compare)
   fccmpe   d,s (floating point conditional signaling compare)

   fcmeq    d,s
   fcmeq    2d,4s,2s
   fcmeq_z  d,s
   fcmeq_z  2d,4s,2s

   fcmge    d,s
   fcmge    2d,4s,2s
   fcmge_z  d,s
   fcmge_z  2d,4s,2s

   fcmgt    d,s
   fcmgt    2d,4s,2s
   fcmgt_z  d,s
   fcmgt_z  2d,4s,2s

   fcmle_z  d,s
   fcmle_z  2d,4s,2s

   fcmlt_z  d,s
   fcmlt_z  2d,4s,2s

   fcmp     d,s (floating point quiet, set flags)
   fcmp_z   d,s
   fcmpe    d,s (floating point signaling, set flags)
   fcmpe_z  d,s

   fcsel    d,s (fp cond select)

   fcvt     s_h,d_h,h_s,d_s,h_d,s_d (fp convert, scalar)

   fcvtas   d,s  (fcvt to signed int, nearest, ties away)
   fcvtas   2d,4s,2s
   fcvtas   w_s,x_s,w_d,x_d

   fcvtau   d,s  (fcvt to unsigned int, nearest, ties away)
   fcvtau   2d,4s,2s
   fcvtau   w_s,x_s,w_d,x_d

   fcvtl{2} 4s/4h, 4s/8h, 2d/2s, 2d/4s (float convert to longer form)

   fcvtms   d,s  (fcvt to signed int, minus inf)
   fcvtms   2d,4s,2s
   fcvtms   w_s,x_s,w_d,x_d

   fcvtmu   d,s  (fcvt to unsigned int, minus inf)
   fcvtmu   2d,4s,2s
   fcvtmu   w_s,x_s,w_d,x_d

   fcvtn{2} 4h/4s, 8h/4s, 2s/2d, 4s/2d (float convert to narrower form)

   fcvtns   d,s  (fcvt to signed int, nearest)
   fcvtns   2d,4s,2s
   fcvtns   w_s,x_s,w_d,x_d

   fcvtnu   d,s  (fcvt to unsigned int, nearest)
   fcvtnu   2d,4s,2s
   fcvtnu   w_s,x_s,w_d,x_d

   fcvtps   d,s  (fcvt to signed int, plus inf)
   fcvtps   2d,4s,2s
   fcvtps   w_s,x_s,w_d,x_d

   fcvtpu   d,s  (fcvt to unsigned int, plus inf)
   fcvtpu   2d,4s,2s
   fcvtpu   w_s,x_s,w_d,x_d

   fcvtxn   s_d (fcvt to lower prec narrow, rounding to odd)
   fcvtxn   2s_2d,4s_2d

   fcvtzs   s,d (fcvt to signed fixedpt, to zero) (w/ #fbits)
   fcvtzs   2d,4s,2s

   fcvtzs   s,d (fcvt to signed integer, to zero)
   fcvtzs   2d,4s,2s

   fcvtzs   w_s,x_s,w_d,x_d (fcvt to signed fixedpt, to zero) (w/ #fbits)

   fcvtzs   w_s,x_s,w_d,x_d (fcvt to signed integer, to zero)

   fcvtzu   s,d (fcvt to unsigned fixedpt, to zero) (w/ #fbits)
   fcvtzu   2d,4s,2s

   fcvtzu   s,d (fcvt to unsigned integer, to zero)
   fcvtzu   2d,4s,2s

   fcvtzu   w_s,x_s,w_d,x_d (fcvt to unsigned fixedpt, to zero) (w/ #fbits)

   fcvtzu   w_s,x_s,w_d,x_d (fcvt to unsigned integer, to zero)

   fdiv     d,s
   fdiv     2d,4s,2s

   fmadd    d,s
   fnmadd   d,s
   fnmsub   d,s
   fnmul    d,s

   fmax     d,s
   fmin     d,s

   fmax     2d,4s,2s
   fmin     2d,4s,2s

   fmaxnm   d,s ("max number")
   fminnm   d,s

   fmaxnm   2d,4s,2s
   fminnm   2d,4s,2s

   fmaxnmp  d_2d,s_2s ("max number pairwise")
   fminnmp  d_2d,s_2s

   fmaxnmp  2d,4s,2s
   fminnmp  2d,4s,2s

   fmaxnmv  s_4s (maxnum across vector)
   fminnmv  s_4s

   fmaxp    d_2d,s_2s (max of a pair)
   fminp    d_2d,s_2s (max of a pair)

   fmaxp    2d,4s,2s  (max pairwise)
   fminp    2d,4s,2s

   fmaxv    s_4s (max across vector)
   fminv    s_4s

   fmla     d_d_d[],s_s_s[] (by element)
   fmla     2d_2d_d[],4s_4s_s[],2s_2s_s[]

   fmla     2d,4s,2s

   fmls     d_d_d[],s_s_s[] (by element)
   fmls     2d_2d_d[],4s_4s_s[],2s_2s_s[]

   fmls     2d,4s,2s

   fmov     2d,4s,2s #imm (part of the MOVI/MVNI/ORR/BIC imm group)

   fmov     d_d,s_s

   fmov     s_w,w_s,d_x,d[1]_x,x_d,x_d[1]

   fmov     d,s #imm

   fmsub    d,s

   fmul     d_d_d[],s_s_s[]
   fmul     2d_2d_d[],4s_4s_s[],2s_2s_s[]

   fmul     2d,4s,2s
   fmul     d,s

   fmulx    d_d_d[],s_s_s[]
   fmulx    2d_2d_d[],4s_4s_s[],2s_2s_s[]

   fmulx    d,s
   fmulx    2d,4s,2s

   fneg     d,s
   fneg     2d,4s,2s

   frecpe   d,s (recip estimate)
   frecpe   2d,4s,2s

   frecps   d,s (recip step)
   frecps   2d,4s,2s

   frecpx   d,s (recip exponent)

   frinta   2d,4s,2s (round to integral, nearest away)
   frinta   d,s

   frinti   2d,4s,2s (round to integral, per FPCR)
   frinti   d,s

   frintm   2d,4s,2s (round to integral, minus inf)
   frintm   d,s

   frintn   2d,4s,2s (round to integral, nearest, to even)
   frintn   d,s

   frintp   2d,4s,2s (round to integral, plus inf)
   frintp   d,s

   frintx   2d,4s,2s (round to integral exact, per FPCR)
   frintx   d,s

   frintz   2d,4s,2s (round to integral, zero)
   frintz   d,s

   frsqrte  d,s (est)
   frsqrte  2d,4s,2s

   frsqrts  d,s (step)
   frsqrts  2d,4s,2s

   fsqrt    d,s
   fsqrt    2d,4s,2s

   fsub     d,s
   fsub     2d,4s,2s

   ins      d[]_d[],s[]_s[],h[]_h[],b[]_b[]

   ins      d[]_x, s[]_w, h[]_w, b[]_w

   ld1  (multiple 1-element structures to 1/2/3/4 regs)
   ld1  (single 1-element structure to one lane of 1 reg)
   ld1r (single 1-element structure and rep to all lanes of 1 reg)

   ld2  (multiple 2-element structures to 2 regs)
   ld2  (single 2-element structure to one lane of 2 regs)
   ld2r (single 2-element structure and rep to all lanes of 2 regs)

   ld3  (multiple 3-element structures to 3 regs)
   ld3  (single 3-element structure to one lane of 3 regs)
   ld3r (single 3-element structure and rep to all lanes of 3 regs)

   ld4  (multiple 4-element structures to 4 regs)
   ld4  (single 4-element structure to one lane of 4 regs)
   ld4r (single 4-element structure and rep to all lanes of 4 regs)

   ldnp  q_q_addr,d_d_addr,s_s_addr  (load pair w/ non-temporal hint)
         addr = reg + uimm7 * reg_size

   ldp   q_q_addr,d_d_addr,s_s_addr  (load pair)
         addr = [Xn|SP],#imm   or [Xn|SP,#imm]!  or [Xn|SP,#imm]

   ldr   q,d,s,h,b from addr
         addr = [Xn|SP],#imm   or [Xn|SP,#imm]!  or [Xn|SP,#imm]

   ldr   q,d,s from  pc+#imm19

   ldr   q,d,s,h,b from addr
         addr = [Xn|SP, R <extend> <shift]

   ldur  q,d,s,h,b from addr
         addr = [Xn|SP,#imm] (unscaled offset)

   mla   4s_4s_s[],2s_2s_s[],8h_8h_h[],4h_4h_h[]
   mla   4s,2s,8h,4h,16b,8b

   mls   4s_4s_s[],2s_2s_s[],8h_8h_h[],4h_4h_h[]
   mls   4s,2s,8h,4h,16b,8b

   movi  16b,8b   #imm8, LSL #0
   movi  8h,4h    #imm8, LSL #0 or 8
   movi  4s,2s    #imm8, LSL #0, 8, 16, 24
   movi  4s,2s    #imm8, MSL #8 or 16
   movi  d,       #imm64
   movi  2d,      #imm64

   mul   4s_4s_s[],2s_2s_s[],8h_8h_h[],4h_4h_h[]
   mul   4s,2s,8h,4h,16b,8b

   mvni  8h,4h    #imm8, LSL #0 or 8
   mvni  4s,2s    #imm8, LSL #0, 8, 16, 24
   mvni  4s,2s    #imm8, MSL #8 or 16

   neg   d
   neg   2d,4s,2s,8h,4h,16b,8b

   not   16b,8b

   orn   16b,8b

   orr   8h,4h   #imm8, LSL #0 or 8
   orr   4s,2s   #imm8, LSL #0, 8, 16 or 24

   orr   16b,8b

   pmul  16b,8b

   pmull{2}  8h_8b_8b,8h_16b_16b,1q_1d_1d,1d_2d_2d

   raddhn{2}  2s/4s_2d_2d, 4h/8h_4s_4s, 8b/16b_8h_8h

   rbit    16b,8b
   rev16   16b,8b
   rev32   16b,8b,8h,4h
   rev64   16b,8b,8h,4h,4s,2s

   rshrn{2}  2s/4s_2d, 8h/4h_4s, 2s/4s_2d,   #imm in 1 .. elem_bits

   rsubhn{2}  2s/4s_2d_2d, 4h/8h_4s_4s, 8b/16b_8h_8h

   saba      16b,8b,8h,4h,4s,2s
   sabal{2}  2d_(2s_2s)/(4s_4s), 4s_(4h_4h)/(8h_8h), 8h_(8b_8b)/(16b_16b)

   sabd      16b,8b,8h,4h,4s,2s
   sabdl{2}  2d_(2s_2s)/(4s_4s), 4s_(4h_4h)/(8h_8h), 8h_(8b_8b)/(16b_16b)

   sadalp    4h_8b,8h_16b,2s_4h,4s_8h,1d_2s,2d_4s

   saddl{2}  2d_(2s_2s)/(4s_4s), 4s_(4h_4h)/(8h_8h), 8h_(8b_8b)/(16b_16b)

   saddlp    4h_8b,8h_16b,2s_4h,4s_8h,1d_2s,2d_4s

   saddlv    h_16b/8b, s_8h/4h, d_4s

   saddw{2}  8h_8h_16b/8b, 4s_4s_8h/4h, 2d_2d_2s/4s

   scvtf     d,s        _#fbits
   scvtf     2d,4s,2s   _#fbits

   scvtf     d,s
   scvtf     2d,4s,2s

   scvtf     s_w, d_w, s_x, d_x,   _#fbits
   scvtf     s_w, d_w, s_x, d_x

   sha1c       q_s_4s
   sha1h       s_s
   sha1m       q_s_4s
   sha1p       q_s_4s
   sha1su0     4s_4s_4s
   sha1su1     4s_4s
   sha256h2    q_q_4s
   sha256h     q_q_4s
   sha256su0   4s_4s
   sha256su1   4s_4s_4s

   shadd       16b,8b,8h,4h,4s,2s

   shl         d_#imm
   shl         16b,8b,8h,4h,4s,2s,2d  _#imm

   shll{2}   8h_8b/16b_#8, 4s_4h/8h_#16, 2d_2s/4s_#32

   shrn{2}  2s/4s_2d, 8h/4h_4s, 2s/4s_2d,   #imm in 1 .. elem_bits

   shsub       16b,8b,8h,4h,4s,2s

   sli         d_#imm
   sli         2d,4s,2s,8h,4h,16b,8b  _#imm

   smax        4s,2s,8h,4h,16b,8b

   smaxp       4s,2s,8h,4h,16b,8b

   smaxv       s_4s,h_8h,h_4h,b_16b,b_8b

   smin        4s,2s,8h,4h,16b,8b

   sminp       4s,2s,8h,4h,16b,8b

   sminv       s_4s,h_8h,h_4h,b_16b,b_8b

   smlal{2}    2d_2s/4s_s[], 4s_4h/8h_h[]
   smlal{2}    2d_(2s_2s)/(4s_4s), 4s_(4h_4h)/(8h_8h), 8h_(8b_8b)/(16b_16b)

   smlsl{2}    2d_2s/4s_s[], 4s_4h/8h_h[]
   smlsl{2}    2d_(2s_2s)/(4s_4s), 4s_(4h_4h)/(8h_8h), 8h_(8b_8b)/(16b_16b)

   smov        w_b[], w_h[], x_b[], x_h[], x_s[]

   smull{2}    2d_2s/4s_s[]. 4s_4h/8h_h[]
   smull{2}    2d_(2s_2s)/(4s_4s), 4s_(4h_4h)/(8h_8h), 8h_(8b_8b)/(16b_16b)

   sqabs       d,s,h,b
   sqabs       2d,4s,2s,8h,4h,16b,8b

   sqadd       d,s,h,b
   sqadd       2d,4s,2s,8h,4h,16b,8b

   sqdmlal     d_s_s[], s_h_h[]
   sqdmlal{2}  2d_2s/4s_s[], 4s_4h/8h_h[]

   sqdmlal     d_s_s, s_h_h
   sqdmlal{2}  2d_(2s_2s)/(4s_4s), 4s_(4h_4h)/(8h_8h)

   sqdmlsl     d_s_s[], s_h_h[]
   sqdmlsl{2}  2d_2s/4s_s[], 4s_4h/8h_h[]

   sqdmlsl     d_s_s, s_h_h
   sqdmlsl{2}  2d_(2s_2s)/(4s_4s), 4s_(4h_4h)/(8h_8h)

   sqdmulh     s_s_s[], h_h_h[]
   sqdmulh     4s_4s_s[], 2s_2s_s[], 8h_8h_h[], 4h_4h_h[]

   sqdmulh     h,s
   sqdmulh     4s,2s,8h,4h

   sqdmull     d_s_s[], s_h_h[]
   sqdmull{2}  2d_2s/4s_s[], 4s_4h/2h_h[]

   sqdmull     d_s_s,s_h_h
   sqdmull{2}  2d_(2s_2s)/(4s_4s), 4s_(4h_4h)/(8h_8h)

   sqneg       d,s,h,b
   sqneg       2d,4s,2s,8h,4h,16b,8b

   sqrdmulh    s_s_s[], h_h_h[]
   sqrdmulh    4s_4s_s[], 2s_2s_s[], 8h_8h_h[], 4h_4h_h[]

   sqrdmulh    h,s
   sqrdmulh    4s,2s,8h,4h

   sqrshl      d,s,h,b
   sqrshl      2d,4s,2s,8h,4h,16b,8b

   sqrshrn     s_d, h_s, b_h   #imm
   sqrshrn{2}  2s/4s_2d, 4h/8h_4s, 8b/16b_8h,  #imm

   sqrshrun     s_d, h_s, b_h   #imm
   sqrshrun{2}  2s/4s_2d, 4h/8h_4s, 8b/16b_8h,  #imm

   sqshl        d,s,h,b   _#imm
   sqshl        2d,4s,2s,8h,4h,16b,8b   _#imm

   sqshl        d,s,h,b
   sqshl        2d,4s,2s,8h,4h,16b,8b

   sqshlu       d,s,h,b  _#imm
   sqshlu       2d,4s,2s,8h,4h,16b,8b  _#imm

   sqshrn       s_d, h_s, b_h   #imm
   sqshrn{2}    2s/4s_2d, 4h/8h_4s, 8b/16b_8h,  #imm

   sqshrun      s_d, h_s, b_h   #imm
   sqshrun{2}   2s/4s_2d, 4h/8h_4s, 8b/16b_8h,  #imm

   sqsub       d,s,h,b
   sqsub       2d,4s,2s,8h,4h,16b,8b

   sqxtn       s_d,h_s,b_h
   sqxtn{2}    2s/4s_2d, 4h/8h_4s, 8b/16b_8h

   sqxtun      s_d,h_s,b_h
   sqxtun{2}   2s/4s_2d, 4h/8h_4s, 8b/16b_8h

   srhadd      4s,2s,8h,4h,16b,8b

   sri         d_#imm
   sri         2d,4s,2s,8h,4h,16b,8b  _#imm

   srshl (reg) d
   srshl       2d,4s,2s,8h,4h,16b,8b

   srshr (imm) d
   srshr       2d,4s,2s,8h,4h,16b,8b

   srsra (imm) d
   srsra       2d,4s,2s,8h,4h,16b,8b

   sshl (reg)  d
   sshl        2d,4s,2s,8h,4h,16b,8b

   sshll{2} (imm)  2d_2s/4s  4s_4h/8h, 8h_8b/16b

   sshr (imm)  d
   sshr        2d,4s,2s,8h,4h,16b,8b

   ssra (imm)  d
   ssra        2d,4s,2s,8h,4h,16b,8b

   ssubl{2}  2d_(2s_2s)/(4s_4s), 4s_(4h_4h)/(8h_8h), 8h_(8b_8b)/(16b_16b)

   ssubw{2}  8h_8h_16b/8b, 4s_4s_8h/4h, 2d_2d_2s/4s

   st1 (multiple 1-element structures from 1/2/3/4 regs)
   st1 (single 1-element structure for 1 lane of 1 reg)

   st2 (multiple 2-element structures from 2 regs)
   st2 (single 2-element structure from 1 lane of 2 regs)

   st3 (multiple 3-element structures from 3 regs)
   st3 (single 3-element structure from 1 lane of 3 regs)

   st4 (multiple 4-element structures from 4 regs)
   st4 (single 4-element structure from one lane of 4 regs)

   stnp q_q_addr, d_d_addr, s_s_addr
        addr = [Xn|SP, #imm]

   stp  q_q_addr, d_d_addr, s_s_addr
        addr = [Xn|SP], #imm  or [Xn|SP, #imm]!  or [Xn|SP, #imm]

   str  q,d,s,h,b_addr
        addr = [Xn|SP], #simm  or [Xn|SP, #simm]!  or [Xn|SP, #pimm]

   str   q,d,s,h,b_addr
         addr = [Xn|SP, R <extend> <shift]

   stur  q,d,s,h,b_addr
         addr = [Xn|SP,#imm] (unscaled offset)

   sub   d
   sub   2d,4s,2s,8h,4h,16b,8b

   subhn{2}  2s/4s_2d_2d, 4h/8h_4s_4s, 8b/16b_8h_8h

   suqadd  d,s,h,b
   suqadd  2d,4s,2s,8h,4h,16b,8b

   tbl     8b_{16b}_8b, 16b_{16b}_16b
   tbl     8b_{16b,16b}_8b, 16b_{16b,16b}_16b
   tbl     8b_{16b,16b,16b}_8b, 16b_{16b,16b,16b}_16b
   tbl     8b_{16b,16b,16b,16b}_8b, 16b_{16b,16b,16b,16b}_16b

   tbx     8b_{16b}_8b, 16b_{16b}_16b
   tbx     8b_{16b,16b}_8b, 16b_{16b,16b}_16b
   tbx     8b_{16b,16b,16b}_8b, 16b_{16b,16b,16b}_16b
   tbx     8b_{16b,16b,16b,16b}_8b, 16b_{16b,16b,16b,16b}_16b

   trn1    2d,4s,2s,8h,4h,16b,8b
   trn2    2d,4s,2s,8h,4h,16b,8b

   uaba      16b,8b,8h,4h,4s,2s
   uabal{2}  2d_(2s_2s)/(4s_4s), 4s_(4h_4h)/(8h_8h), 8h_(8b_8b)/(16b_16b)

   uabd      16b,8b,8h,4h,4s,2s
   uabdl{2}  2d_(2s_2s)/(4s_4s), 4s_(4h_4h)/(8h_8h), 8h_(8b_8b)/(16b_16b)

   uadalp    4h_8b,8h_16b,2s_4h,4s_8h,1d_2s,2d_4s

   uaddl{2}  2d_(2s_2s)/(4s_4s), 4s_(4h_4h)/(8h_8h), 8h_(8b_8b)/(16b_16b)

   uaddlp    4h_8b,8h_16b,2s_4h,4s_8h,1d_2s,2d_4s

   uaddlv    h_16b/8b, s_8h/4h, d_4s

   uaddw{2}  8h_8h_16b/8b, 4s_4s_8h/4h, 2d_2d_2s/4s

   ucvtf     d,s        _#fbits
   ucvtf     2d,4s,2s   _#fbits

   ucvtf     d,s
   ucvtf     2d,4s,2s

   ucvtf     s_w, d_w, s_x, d_x,   _#fbits
   ucvtf     s_w, d_w, s_x, d_x

   uhadd       16b,8b,8h,4h,4s,2s

   uhsub       16b,8b,8h,4h,4s,2s

   umax        4s,2s,8h,4h,16b,8b

   umaxp       4s,2s,8h,4h,16b,8b

   umaxv       s_4s,h_8h,h_4h,b_16b,b_8b

   umin        4s,2s,8h,4h,16b,8b

   uminp       4s,2s,8h,4h,16b,8b

   uminv       s_4s,h_8h,h_4h,b_16b,b_8b

   umlal{2}    2d_2s/4s_s[], 4s_4h/8h_h[]
   umlal{2}    2d_(2s_2s)/(4s_4s), 4s_(4h_4h)/(8h_8h), 8h_(8b_8b)/(16b_16b)

   umlsl{2}    2d_2s/4s_s[], 4s_4h/8h_h[]
   umlsl{2}    2d_(2s_2s)/(4s_4s), 4s_(4h_4h)/(8h_8h), 8h_(8b_8b)/(16b_16b)

   umov        w_b[], w_h[], x_b[], x_h[], x_s[]

   umull{2}    2d_2s/4s_s[]. 4s_4h/8h_h[]
   umull{2}    2d_(2s_2s)/(4s_4s), 4s_(4h_4h)/(8h_8h), 8h_(8b_8b)/(16b_16b)

   uqadd       d,s,h,b
   uqadd       2d,4s,2s,8h,4h,16b,8b

   uqrshl      d,s,h,b
   uqrshl      2d,4s,2s,8h,4h,16b,8b

   uqrshrn     s_d, h_s, b_h   #imm
   uqrshrn{2}  2s/4s_2d, 4h/8h_4s, 8b/16b_8h,  #imm

   uqshl        d,s,h,b   _#imm
   uqshl        2d,4s,2s,8h,4h,16b,8b   _#imm

   uqshl        d,s,h,b
   uqshl        2d,4s,2s,8h,4h,16b,8b

   uqshrn       s_d, h_s, b_h   #imm
   uqshrn{2}    2s/4s_2d, 4h/8h_4s, 8b/16b_8h,  #imm

   uqsub       d,s,h,b
   uqsub       2d,4s,2s,8h,4h,16b,8b

   uqxtn       s_d,h_s,b_h
   uqxtn{2}    2s/4s_2d, 4h/8h_4s, 8b/16b_8h

   urecpe      4s,2s

   urhadd      4s,2s,8h,4h,16b,8b

   urshl (reg) d
   urshl       2d,4s,2s,8h,4h,16b,8b

   urshr (imm) d
   urshr       2d,4s,2s,8h,4h,16b,8b

   ursqrte     4s,2s

   ursra (imm) d
   ursra       2d,4s,2s,8h,4h,16b,8b

   ushl (reg)  d
   ushl        2d,4s,2s,8h,4h,16b,8b

   ushll{2} (imm)  2d_2s/4s  4s_4h/8h, 8h_8b/16b

   ushr (imm)  d
   ushr        2d,4s,2s,8h,4h,16b,8b

   usqadd      d,s,h,b
   usqadd      2d,4s,2s,8h,4h,16b,8b

   usra (imm)  d
   usra        2d,4s,2s,8h,4h,16b,8b

   usubl{2}  2d_(2s_2s)/(4s_4s), 4s_(4h_4h)/(8h_8h), 8h_(8b_8b)/(16b_16b)

   usubw{2}  8h_8h_16b/8b, 4s_4s_8h/4h, 2d_2d_2s/4s

   uzp1      2d,4s,2s,8h,4h,16b,8b
   uzp2      2d,4s,2s,8h,4h,16b,8b

   xtn{2}    2s/4s_2d, 4h/8h_4s, 8b/16b_8h

   zip1      2d,4s,2s,8h,4h,16b,8b
   zip2      2d,4s,2s,8h,4h,16b,8b
*/
