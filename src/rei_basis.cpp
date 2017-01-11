// -*- mode:c++; coding:utf-8-ws -*-
//====================================================================
//! @file  rei_basis.cpp
//! @brief 基本関数
//!
//! @author
//====================================================================
#include <stdio.h>
#include <stdlib.h>

#include "rei_basis.h"
#include "rei_string.h"

//#define USE_MM

//int heaptype = SYSTEM_HEAP;

#ifdef USE_MM
#include "rei_memory.h"
static void *s_MemoryPtr = 0;
static int s_MemorySize = 0;
#endif  //USE_MM

namespace rei {

//------------------------------------------------------------------
// lib 初期化
//------------------------------------------------------------------
void Initialize(int memory_size) {
#ifdef USE_MM
  // メモリ管理初期化
  if (0 < memory_size) {
    s_MemoryPtr = reinterpret_cast<char *>(malloc_rev(memory_size, SYSTEM_HEAP));  // 確保
    s_MemorySize = memory_size;
    MEM_Initialize(s_MemoryPtr, s_MemorySize);
  } else {
    s_MemoryPtr = 0;
    s_MemorySize = 0;
  }
#else
  (void)memory_size;
#endif  //USE_MM

  //	km::String::BufferInit(64 * 1024);
}

//------------------------------------------------------------------
// lib 後始末
//------------------------------------------------------------------
void Terminate() {
#ifdef USE_MM
  // メモリ管理後始末
  if (s_MemoryPtr) {
    free_(s_MemoryPtr, SYSTEM_HEAP);
    s_MemoryPtr = 0;
  }
#endif  //USE_MM
}

//------------------------------------------------------------------
// メモリアロケート
//------------------------------------------------------------------
void *MemoryAllocate(int size) {
#ifdef USE_CUSTOM_ALLOCATER
  void *p;
  int type = heaptype;

  if (s_MemoryPtr == 0) {
    type = SYSTEM_HEAP;
  }

  switch (type) {
  case SYSTEM_HEAP:  // System
    p = ::malloc(size);
    break;
  case CUSTOM_HEAP:  // Custom
    p = MEM_Allocate(size, 4);
    break;
  default:
    p = 0;
    break;
  }

  return p;
#else
  return ::malloc(size);
#endif  //USE_CUSTOM_ALLOCATER
}

//------------------------------------------------------------------
// メモリフリー
//------------------------------------------------------------------
void MemoryFree(void *p) {
#ifdef USE_CUSTOM_ALLOCATER
  // ポインタの位置で判定
  if (s_MemoryPtr &&
      (p < s_MemoryPtr ||
       reinterpret_cast<char *>(s_MemoryPtr) + s_MemorySize < reinterpret_cast<char *>(p))) {
    ::free(p);
  } else {
    MEM_Free(p);
  }
#else
  ::free(p);
#endif  //USE_CUSTOM_ALLOCATER
}

//------------------------------------------------------------------
// ハッシュ
//------------------------------------------------------------------
unsigned long Hash(const char *data, int length) {
  unsigned long sum = 0;
#if 1
  (void)length;
  //多少時間がかかっても重複しないことが大事
  while (*data) {
    sum = sum * 41 + *data++;
  }
#else
  if (length < 32) {
    while (*data) {
      sum = sum * 37 + *data++;
    }
  } else {
    int step = length / 8;
    int len;
    for (len = length; len > 0; len -= step, data += step) {
      sum = sum * 41 + *data;
    }
  }
#endif
  return sum;
}

}  //rei

#ifdef __cplusplus
// new overload
void *__cdecl operator new(unsigned int size) {
  return rei::MemoryAllocate(size);
}
// delete overload
void __cdecl operator delete(void *p) {
  rei::MemoryFree(p);
}
#endif /* __cplusplus */
/*[EOF]*/
