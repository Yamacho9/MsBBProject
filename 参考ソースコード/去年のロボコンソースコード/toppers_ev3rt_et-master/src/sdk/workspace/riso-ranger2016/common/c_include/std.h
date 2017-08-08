///////////////////////////////////////////////////////////
//  std.h
//  Implementation of the Class std
//  Created on:      2016/04/19 20:14:30
//  Original author: 037789
///////////////////////////////////////////////////////////

#if !defined(EA_AA68E2BB_882A_42d8_A54C_F515E6C7718F__INCLUDED_)
#define EA_AA68E2BB_882A_42d8_A54C_F515E6C7718F__INCLUDED_

typedef signed char    sint8;   //!<   signed 8bit
typedef unsigned char  uint8;   //!< unsigned 8bit
typedef signed short   sint16;  //!<   signed 16bit
typedef unsigned short uint16;  //!< unsigned 16bit
typedef signed long    sint32;  //!<   signed 32bit
typedef unsigned long  uint32;  //!< unsigned 32bit

#ifndef __cplusplus
typedef uint8 bool;             //!< boolean type for C
#define false 0
#define true  1
#endif  // __cplusplus


#ifndef NULL
#define NULL 0
#endif

#define BIT(n) (1UL << (n))

#endif // !defined(EA_AA68E2BB_882A_42d8_A54C_F515E6C7718F__INCLUDED_)
