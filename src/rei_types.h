// -*- mode:c++; coding:utf-8-ws -*-
//====================================================================
//! @file  rei_types.h
//! @brief データ定義
//!
//! @author
//====================================================================
#ifndef REI_TYPES_H
#define REI_TYPES_H

// RAND_MAX
#ifndef WIN32  // Windows は定義済みなので...
#ifndef RAND_MAX
#define RAND_MAX (0x7FFF)  //(0x7FFFFFFF)
#endif                     // RAND_MAX
#endif                     // WIN32

// データ型再定義
#if defined(__cplusplus) && !defined(_CSTDINT_)
#if __cplusplus <= 199711L
typedef char int8_t;
typedef short int16_t;
typedef long int32_t;
typedef long long int64_t;
typedef char int_fast8_t;
typedef short int_fast16_t;
typedef long int_fast32_t;
typedef long long int_fast64_t;
typedef char int_least8_t;
typedef short int_least16_t;
typedef long int_least32_t;
typedef long long int_least64_t;

//intmax_t	//maximum width integer type
//intptr_t	//integer type capable of holding a pointer

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;
typedef unsigned char uint_fast8_t;
typedef unsigned short uint_fast16_t;
typedef unsigned long uint_fast32_t;
typedef unsigned long long uint_fast64_t;
typedef unsigned char uint_least8_t;
typedef unsigned short uint_least16_t;
typedef unsigned long uint_least32_t;
typedef unsigned long long uint_least64_t;

//uintmax_t	//maximum width unsigned integer type
//uintptr_t	//unsigned integer type capable of holding a pointer

#define nullptr ((void *)0)

#else
// C++11 or later
#endif  //__cplusplus <= 199711L
#endif  //defined(__cplusplus)

#define int8_size sizeof(int8_t)
#define int16_size sizeof(int16_t)
#define int32_size sizeof(int32_t)
#define int64_size sizeof(int64_t)
#define uint8_size sizeof(uint8_t)
#define uint16_size sizeof(uint16_t)
#define uint32_size sizeof(uint32_t)
#define uint64_size sizeof(uint64_t)

#ifdef WIN32
#define FALSE 0
#define TRUE 1
#endif

#ifndef WIN32  // Windows は定義済みなので...
typedef long time_t;
typedef unsigned int size_t;
#endif  // WIN32

#endif  //REI_TYPES_H
