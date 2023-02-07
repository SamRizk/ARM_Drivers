//
// Created by samir on 11/28/2022.
//

#ifndef C_SOLVE_STD_TYPES_H
#define C_SOLVE_STD_TYPES_H

#define  PINS_CONCAT(b7,b6,b5,b4,b3,b2,b1,b0)         CONCAT_HELP(b7,b6,b5,b4,b3,b2,b1,b0)
#define  CONCAT_HELP(b7,b6,b5,b4,b3,b2,b1,b0)         (0b##b7##b6##b5##b4##b3##b2##b1##b0)


#define  TRUE                            1
#define  FALSE                           0
#define  NULL                          (void*)0

typedef  unsigned char                       u8;
typedef  u8*                                pu8;
typedef  volatile u8                        vu8;

typedef  signed char                         s8;
typedef  volatile s8                        vs8;
typedef  s8*                                ps8;

typedef  unsigned  short int                u16;
typedef  volatile u16                      vu16;
typedef  unsigned  short int*              pu16;
typedef  signed  short int                  s16;
typedef  volatile s16                      vs16;
typedef  signed  short int*                ps16;

typedef  unsigned long int                  u32;
typedef  volatile u32                      vu32;
typedef  unsigned long int*                pu32;
typedef  signed long int                    s32;
typedef  volatile s32                      vs32;
typedef  signed long int*                  ps32;

typedef  float                              f32;
typedef  float*                            pf32;

typedef  double                             s64;
typedef  double*                           ps64;
typedef  long double                       s128;
typedef  long double*                     ps128;


#endif //C_SOLVE_STD_TYPES_H
