// -*- mode:c++; coding:utf-8-ws -*-
#ifndef NKMM_BASIS_H
#define NKMM_BASIS_H

#include "nkmm_types.h"  // データ定義

//#define USE_CUSTOM_ALLOCATER

#define __SOURCE__ __FILE__ ":" __FUNCTION__ ":" __LINE__

//extern int heaptype;
//enum HeapType {
//  SYSTEM_HEAP = 0,
//  CUSTOM_HEAP,
//};

// ライブラリ作成タイプ
//   DLL 作成の場合は __declspec(dllexport) をつける必要があります。
//   LIB 作成の場合は特に何もつけなくてＯＫです。
#define __libtype /* LIB */
//#define __libtype __declspec(dllexport) /* DLL */

// オブジェクトの領域を確保
#ifdef __cplusplus
#define SAFE_NEW(Comment, Type) new Type
#define SAFE_NEW_ARRAY(Ccomment, Type, Size) new Type[Size]
#else
#define SAFE_NEW(Comment, Type) (Type *)malloc(sizeof(Type))
#define SAFE_NEW_ARRAY(Ccomment, Type, Size) (Type *)malloc(sizeof(Type) * Size)
#endif

// オブジェクトの領域を解放
#ifdef __cplusplus
#define SAFE_DELETE(P) \
  do {                 \
    if (P) {           \
      delete (P);      \
      (P) = NULL;      \
    }                  \
  } while (0)
#define SAFE_DELETE_ARRAY(P) \
  do {                       \
    if (P) {                 \
      delete[](P);           \
      (P) = NULL;            \
    }                        \
  } while (0)
#else
#define SAFE_DELETE(P) \
  do {                 \
    if (P) {           \
      free(P);         \
      (P) = NULL;      \
    }                  \
  } while (0)
#define SAFE_DELETE_ARRAY(P) \
  do {                       \
    if (P) {                 \
      free(P);               \
      (P) = NULL;            \
    }                        \
  } while (0)
#endif

// Ｃメモリアロケート
//#ifdef _CRT
#include <stdlib.h>
#define malloc_(size) malloc(size)
#define malloc_rev_(size) malloc(size)
#define free_(p) free(p);
//#else
//# define malloc_(size) (__MEM_CHECKPOINT__ suMemoryAllocate(size,  4))
//# define malloc_rev_(size) (__MEM_CHECKPOINT__ suMemoryAllocate(size, -4))
//# define free_(p) suMemoryFree(p);
//#endif

// ランダム値取得( rand() を使用することを前提)
#define RAND() rand()
#define RANDI(n) ((int)RAND() / ((int)RAND_MAX / (n) + 1))
#define RANDF(n) ((double)RAND() / ((double)RAND_MAX + 1) * (n))

// 最小最大値制限
template <class T>
inline T Limit(const T &val, const T &min, const T &max) {
  return ((val) < (min)) ? (min) : (((max) < (val)) ? (max) : (val));
}
//#define Limit(val, min, max) (((val) < (min)) ? (min) : (((max) < (val)) ? (max) : (val)))

// ＡＢ間分割指定位置( a から b までを max 分割した count の位置を求める)
// 式 : a + ( b - a ) * count / max
//#define Split( a, b, count, max ) ((a) + (((b) - (a)) * (count) / (max)))

// パーセント
template <class T>
inline T Percent(const T &num, const T &per) {
  return ((num) * (per) / 100);
}

// 絶対値
template <class T>
inline T Abs(const T &x) {
  return ((x) < (0) ? -(x) : (x));
}

// 大きいほう
template <class T>
inline T Big(const T &x, const T &y) {
  return ((x) > (y) ? (x) : (y));
}

// 小さいほう
template <class T>
inline T Small(const T &x, const T &y) {
  return ((x) < (y) ? (x) : (y));
}

// アライメント
template <class T>
inline T RoundUp(const T &num, const T &x) {
  return (((num) + (x)-1) & ~((x)-1));
}

// 配列の要素数
#define ArrayNum(a) (sizeof(a) / sizeof(a[0]))

// 立っているビットの個数を調べる
// (細分化しているので下位ビットはANDをとる必要はないようなのです)
/*
#define BITC1(n) ((u_int)(n) & 1)				//  1bit
#define BITC2(n) (BITC1 ((n) >>  1) + BITC1 (n))	//  2bit
#define BITC4(n) (BITC2 ((n) >>  2) + BITC2 (n))	//  4bit
#define BITC8(n) (BITC4 ((n) >>  4) + BITC4 (n))	//  8bit
#define BITC16(n) (BITC8 ((n) >>  8) + BITC8 (n))	// 16bit
#define BITC32(n) (BITC16((n) >> 16) + BITC16(n))	// 32bit
#define BITC64(n) (BITC32((n) >> 32) + BITC32(n))	// 64bit
#define BITC128(n) (BITC64((n) >> 64) + BITC64(n))	// 128bit
#define BITC(n, bit) (BITC##_bit##(n))
*/

//#ifdef __cplusplus
//extern "C" {
//#endif /* __cplusplus */

namespace nkmm {

//------------------------------------------------------------------
//! lib 初期化
//! @param[in] memory_size メモリサイズ
//------------------------------------------------------------------
void Initialize(int memory_size);

//------------------------------------------------------------------
//! lib 後始末
//------------------------------------------------------------------
void Terminate();

//------------------------------------------------------------------
//! メモリアロケート
//! @param[in] size 確保するサイズ
//! @return void *
//------------------------------------------------------------------
void *MemoryAllocate(int size);

//------------------------------------------------------------------
//! メモリフリー
//! @param[in] p
//------------------------------------------------------------------
void MemoryFree(void *p);

//------------------------------------------------------------------
//! ハッシュ
//! @param[in] data 求める文字列
//! @return ハッシュ値
//!
//! @note
//! 参考 <br>
//! http://home.att.ne.jp/apple/wizard/ <br>
//! http://www.tokumaru.org/ の「用語集」「ハッシュ法」 <br>
//! <br>
//! ogata_hash を少しいじりました <br>
//! length を指定に、すべてまわす長さを 32 未満に変更 <br>
//------------------------------------------------------------------
unsigned long Hash(const char *data, int length);

double CalcString(const char *exp);  // 逆ポーランド記法による数式の計算
int Eval(const char *exp);           // 式の評価(Evaluation)

}  //nkmm

//#ifdef __cplusplus
//}      /* extern "C" */
//#endif /* __cplusplus */

#ifdef __cplusplus
// new overload
void *__cdecl operator new(unsigned int size);
// delete overload
void __cdecl operator delete(void *p);
#endif /* __cplusplus */

#endif  //NKMM_BASIS_H
