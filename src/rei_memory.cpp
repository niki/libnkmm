// -*- mode:c++; coding:utf-8-ws -*-
//====================================================================
//! @file  rei_memory.cpp
//! @brief メモリマネージャ
//!
//! @author
//====================================================================
/*
空き領域と使用領域を別リストにする。
確保・解放に多少のコストはかかるが、要素の個数に影響しない平均的な速度になる。
解放後に、空き領域リストに繋げる。
そのとき、連続した空き領域であれば結合を行う。
*/
#include <cstdint>

#ifdef WIN32
#ifdef MEM_DEBUG
#include <stdio.h>   // vsprintf
#include <stdarg.h>  // va_list
#endif
#else
#include <nitro.h>
#endif

#include "rei_types.h"
#include "rei_memory.h"

#ifdef WIN32
#endif
#ifdef BCC32
#else             //Athor
#define __inline  //inline
#endif

#define RoundUp(size, alignment) (((size) + (alignment)-1) & ~((alignment)-1))  // アライメント補正

#define BLOCK_SIZE (int)sizeof(BLOCK)
#define INFO_SIZE (int)sizeof(INFO)

//#define REI_R_TYPE       int32_t
#define REI_R_TYPE int64_t
#define REI_R_TYPE_SIZE sizeof(REI_R_TYPE)

//! メモリブロック管理構造体
typedef struct tagBLOCK {
  struct tagBLOCK* prev;  //!< 前の要素
  struct tagBLOCK* next;  //!< 次の要素
  long size;              //!< サイズ  空き領域リスト  ：空きサイズ
                          //           アロケートリスト：使用サイズ
#ifdef MEM_DEBUG
  long checkLINE;  //!< 取得した場所の行番号
#else
  long reserve1;
#endif
} BLOCK;

//! メモリ管理構造体
typedef struct tagINFO {
  BLOCK* freeTop;     //!< 空き領域リスト 先頭
  BLOCK* freeBottom;  //!< 空き領域リスト 終端
  BLOCK* allocTop;    //!< アロケートリスト
  long allocCount;    //!< アロケートカウンタ）

#ifdef MEM_DEBUG
  long debugAllocCount;  //!< カウンタ（Alloc の総数）
  long debugFreeCount;   //!< カウンタ（Free の総数）
  long reserve1;
  long reserve2;
#endif
} INFO;

#ifdef MEM_DEBUG
static int s_CheckLINE = 0;             //!< 行番号
static uint8_t s_CheckFILE = 0;         //!< ファイル名の長さ
static char s_NameCheckFILE[256] = "";  //!< ファイル名
#endif

/*!
* 空きメモリブロックの検索（前方）
* @param[in] block 検索開始ブロック
* @param[in] size 取得サイズ
* @return 0:失敗, BLOCK*:取得可能なメモリブロック
*/
static __inline BLOCK* getFrontBlock(BLOCK* block, int size) {
  for (/* None */; block; block = block->next) {
    // リストの中に納まるサイズの要素があった
    if (size <= block->size) {
      break;
    }
  }

  return block;
}

/*!
* 空きメモリブロックの検索（後方）
* @param[in] block 検索開始ブロック
* @param[in] size 取得サイズ
* @return 0:失敗, BLOCK* :取得可能なメモリブロック
*/
static __inline BLOCK* getRearBlock(BLOCK* block, int size) {
  for (/* None */; block; block = block->prev) {
    // リストの中に納まるサイズの要素があった
    if (size <= block->size) {
      break;
    }
  }

  return block;
}

/*!
* 空き領域リストのリンク
* @param[in] info 管理ポインタ
* @param[in] block 接続対象ブロック
*/
static __inline void linkFreeList(INFO* info, BLOCK* block) {
  BLOCK* pos;

  // block に一番近い要素を探す
  for (pos = info->freeTop; pos; pos = pos->next) {
    if (block < pos) {
      break;
    }
  }

  // block が先頭
  if (pos == 0) {
    pos = info->freeTop;  // 現在地を freeTop にする

    // 空きリストは block だけになる
    if (pos == 0) {
      // 空き領域リスト作成
      block->prev = 0;
      block->next = 0;

      // リスト設定
      info->freeTop = block;
      info->freeBottom = block;

    } else {
      // block と pos は隣り合わせ
      if ((char*)block + block->size == (char*)pos) {
        // block と pos を結合
        BLOCK* next = pos->next;

        block->prev = 0;
        block->next = next;

        if (next) {
          next->prev = block;

        } else /*if (next == 0)*/ {
          // リスト設定
          info->freeBottom = block;
        }

      } else {  // block と pos は隣り合っていない
        // block と pos をつなぐ
        block->prev = 0;
        block->next = pos;

        pos->prev = block;

        // リスト設定
        info->freeTop = block;
      }
    }

  } else {  // pos <-> block <-> pos->next でつなぐ
    if (pos->next) {
      // pos と block は隣り合わせ
      if ((char*)pos + pos->size == (char*)block) {
        // pos と block と pos->next は隣り合わせ。
        if ((char*)block + block->size == (char*)pos->next) {
          // pos と block と pos->next を結合
          BLOCK* next = pos->next->next;

          pos->size = pos->size + block->size + pos->next->size;
          pos->next = next;

          if (next) {
            next->prev = pos;

          } else /*if (next == 0)*/ {
            // リスト設定
            info->freeBottom = pos;
          }

        } else {  // pos と block だけ隣り合わせ
          // pos と block を結合
          pos->size = pos->size + block->size;
        }

      } else if ((char*)block + block->size ==
                 (char*)pos->next) {  // block と pos->next は隣り合わせ
        // block と pos->next を結合
        BLOCK* next = pos->next->next;

        block->size = block->size + pos->next->size;
        block->next = next;

        if (next) {
          next->prev = block;

        } else /*if (next == 0)*/ {
          // リスト設定
          info->freeBottom = block;
        }
      }

    } else /*if (pos->next == 0)*/ {
      // pos と block は隣り合わせ
      if ((char*)pos + pos->size == (char*)block) {
        // pos と block を結合
        pos->size = pos->size + block->size;

      } else {  // pos と block は隣り合っていない
        // pos と block とつなげる
        pos->next = block;

        block->prev = pos;
        block->next = 0;

        // リスト設定
        info->freeBottom = block;
      }
    }
  }
}

/*!
* 空き領域リストのアンリンク
* @param[in] info 管理ポインタ
* @param[in] block 切断対象ブロック
*/
static __inline void unlinkFreeList(INFO* info, BLOCK* block) {
  BLOCK* prev = block->prev;
  BLOCK* next = block->next;

  if (prev) {
    // ○ prev ○ next
    if (next) {
      prev->next = next;
      next->prev = prev;

    } else /*if (next == 0)*/ {  // ○ prev × next
      prev->next = 0;
      info->freeBottom = prev;
    }

  } else /*if (prev == 0)*/ {
    // × prev ○ next
    if (next) {
      next->prev = 0;
      info->freeTop = next;

    } else /*if (next == 0)*/ {  // × prev × next
      // ここにくるとしたら
      // 空きがちょうどなくなったとき
      info->freeTop = 0;
      info->freeBottom = 0;
    }
  }
}

/*!
* アロケートリストの接続（先頭に追加していく）
* @param[in] info 管理ポインタ
* @param[in] block 接続対象ブロック
*/
static __inline void linkAllocList(INFO* info, BLOCK* block) {
  // アロケートリスト追加
  block->prev = 0;
  block->next = info->allocTop;

  // allocTop があれば設定
  if (info->allocTop) {
    info->allocTop->prev = block;
  }

  info->allocTop = block;

  // アロケートカウンタを増やす
  info->allocCount++;
}

/*!
* アロケートリストの切断
* @param[in] info 管理ポインタ
* @param[in] block 切断対象ブロック
*/
static __inline void unlinkAllocList(INFO* info, BLOCK* block) {
  BLOCK* prev = block->prev;
  BLOCK* next = block->next;

  if (prev) {
    if (next) {
      prev->next = next;
      next->prev = prev;

    } else /*if (next == 0)*/ {
      prev->next = 0;
    }

  } else /*if (prev == 0)*/ {
    if (next) {
      next->prev = 0;
      info->allocTop = next;

    } else {
      info->allocTop = 0;
    }
  }

  // アロケートカウンタを減らす
  info->allocCount--;
}

namespace rei {

Memory::Memory() : addr_(nullptr), size_(0) {}

void Memory::initialize(void* addr, int size) {
  INFO* info = (INFO*)addr;
  BLOCK* block;

#ifdef MEM_DEBUG
  // アドレス不正
  if (addr == 0) {
    printf("アドレスが不正\n");
    return;
  }
#endif

  // 空き領域リスト作成（空きとして最初に１つ作成しておく）
  block = (BLOCK*)((char*)addr + INFO_SIZE);
  block->prev = 0;
  block->next = 0;
  block->size = size - INFO_SIZE;

  // バッファの設定
  info->freeTop = block;
  info->freeBottom = block;
  info->allocTop = 0;
  info->allocCount = 0;  // アロケートカウンタ

#ifdef MEM_DEBUG
  info->debugAllocCount = 0;
  info->debugFreeCount = 0;
#endif

  // 配置
  addr_ = addr;
  size_ = size;

#ifdef MEM_DEBUG
  printf("=======================================================\n");
  printf("MEMOEY CREATE!!\n");
  printf("  Area: 0x%.8x - 0x%.8x\n", addr, (char*)addr + size - 1);
  printf("  size: %d\n", size);
  printf("=======================================================\n");
#endif
}

void Memory::clear() {
  initialize(addr_, size_);
}

void* Memory::allocate(int size, int alignment) {
  INFO* info;
  BLOCK* block;  // 現在の要素
  BLOCK* add;

  // サイズが不正
  if (size < 0) {
#ifdef MEM_DEBUG
    printf("サイズの指定が不正です\n");
#endif
    return (void*)0;
  }

  info = (INFO*)addr_;

// 取得サイズの設定
#ifdef MEM_DEBUG
  size = RoundUp(size + BLOCK_SIZE + s_CheckFILE, alignment);
#else
  size = RoundUp(size + BLOCK_SIZE, alignment);
#endif

  // メモリブロックの取得（前から）
  if (alignment > 0) {
    block = getFrontBlock(info->freeTop, size);

    // 取得失敗
    if (block == 0) {
#ifdef MEM_DEBUG
      printf("FRONT:有効なメモリブロックが見つかりません(%d)\n", size);
#endif
      return (void*)0;
    }

    // 追加ブロック設定
    add = block;

    if (block->size == size) {
      /*
			* 取得した空きリストがサイズ0になったため，空き領域として使えない
			* そのため，空き領域リストから切断する必要がある
			*/

      // 空き領域リストから切断（サイズが０になったらもう使えない）
      unlinkFreeList(info, block);

    } else {
      /*
			* 前方からの取得に伴い，先頭アドレスが変化してしまうため
			* 取得した空きリストの残りサイズ分のブロックを再構築し
			* 再度，空き領域リストに接続しなおす
			*/

      int rem_size = block->size - size;  // 残りサイズ

      // 空き領域リストから切断
      unlinkFreeList(info, block);

      // ブロック再設定
      block = (BLOCK*)((char*)block + size);
      block->size = rem_size;  // 残りサイズ更新

      // 空き領域リストに接続
      linkFreeList(info, block);
    }

  } else /*if (alignment < 0)*/ {  // メモリブロックの取得（後ろから）
    block = getRearBlock(info->freeBottom, size);

    // 取得失敗
    if (block == 0) {
#ifdef MEM_DEBUG
      printf("REAR:有効なメモリブロックが見つかりません(%d)\n", size);
#endif
      return (void*)0;
    }

    // 追加ブロック設定
    add = (BLOCK*)((char*)block + block->size - size);

    if (block->size == size) {
      // 空き領域リストから切断（サイズが０になったらもう使えない）
      unlinkFreeList(info, block);

    } else {
      /*
			* 後方からの取得なので空き領域リストに登録されているブロックサイズの更新だけでOK
			* 新たに接続しなおす必要がない
			*/

      // 残りサイズ更新
      block->size = block->size - size;
    }
  }

  // アロケートリスト更新
  linkAllocList(info, add);

  // ブロックサイズ設定
  add->size = size;

#ifdef MEM_DEBUG
  // チェックポイント設定
  SetCheckPoint(add);

  // カウンタを増やす
  info->debugAllocCount++;
#endif

// 返すポインタに情報は含まず
#ifdef MEM_DEBUG
  return (char*)add + BLOCK_SIZE + s_CheckFILE;
#else
  return (char*)add + BLOCK_SIZE;
#endif
}

void Memory::free(void* p) {
  INFO* info;
  BLOCK* block;

  // null 解放の保証
  if (p == 0) {
    return;
  }

  info = (INFO*)addr_;

// ブロック設定
#ifdef MEM_DEBUG
  block = (BLOCK*)((char*)p - BLOCK_SIZE - *((uint8_t*)P - 1));
#else
  block = (BLOCK*)((char*)p - BLOCK_SIZE);
#endif

  // アロケートリスト更新
  unlinkAllocList(info, block);

  // 空き領域リスト更新
  linkFreeList(info, block);

#ifdef MEM_DEBUG
  // カウンタを減らす
  info->debugFreeCount++;
#endif
}

int Memory::getMaxFreeSize() const {
  INFO* info = (INFO*)addr_;
  BLOCK* pos;
  int size = 0;

  for (pos = info->freeTop; pos; pos = pos->next) {
    if (size < pos->size) {
      size = pos->size;
    }
  }

  return size;
}

int Memory::getTotalFreeSize() const {
  INFO* info = (INFO*)addr_;
  BLOCK* pos;
  int size = 0;

  for (pos = info->freeTop; pos; pos = pos->next) {
    size += pos->size;
  }

  return size;
}

void Memory::dump() {
#ifdef MEM_DEBUG
  INFO* info = (INFO*)addr_;
  BLOCK* pos;
  int total, count;

  printf("\n---------------------\n");
  printf(".debugAllocCount  : %d\n", info->debugAllocCount);
  printf(".debugFreeCount : %d\n", info->debugFreeCount);

  printf("----- space list -----\n");
  total = 0;
  count = 0;
  for (pos = info->freeTop; pos; pos = pos->next) {
    count++;
    printf("%5d: 0x%08x - 0x%08x %d\n", count, pos, (char*)pos + pos->size - 1, pos->size);
    total += pos->size;
  }
  printf("== Total      : %d(%dKB)\n", total, total / 1024);
  printf("== Count      : %d\n", count);

  printf("----- alloc list ------\n");
  total = 0;
  count = 0;
  for (pos = info->allocTop; pos; pos = pos->next) {
    count++;
    if (0 < pos->checkLINE) {
      printf("%5d: 0x%08x - 0x%08x %d, %s(%d)\n", count, pos, (char*)pos + pos->size - 1, pos->size,
             (char*)pos + BLOCK_SIZE, pos->checkLINE);
    } else {
      printf("%5d: 0x%08x - 0x%08x %d, %s(%d)\n", count, pos, (char*)pos + pos->size - 1, pos->size,
             (char*)pos + BLOCK_SIZE, pos->checkLINE);
    }
    total += pos->size;
  }
  printf("== Total      : %d(%dKB)\n", total, total / 1024);
  printf("== Count      : %d\n", count);
  printf("== .allocCount: %d\n", info->allocCount);

  printf("---------------------\n");
#endif
}

// チェックポイント設定（ローカル）
void Memory::SetCheckPoint(char* checkFILE, int checkLINE) {
#ifdef MEM_DEBUG
  char* dst = s_NameCheckFILE;

  if (!(*checkFILE)) {
    s_CheckFILE = 0;
    s_CheckLINE = checkLINE;
    return;
  }

  // 文字列をコピー
  while (*checkFILE) {
    *dst++ = *checkFILE++;
  }
  *dst++ = '\0';

  s_CheckFILE = (uint8_t)RoundUp((dst - s_NameCheckFILE) + 1, 4);  // ４バイト境界になるように
  // +1 バイトは文字列の長さ記録用
  s_CheckLINE = checkLINE;  // 行番号
#else
  (void)checkFILE;
  (void)checkLINE;
#endif
}

// メモリの内容を比較
int Memory::Compare(void* A, void* B, int size) {
  REI_R_TYPE* a1 = (REI_R_TYPE*)A;
  REI_R_TYPE* b1 = (REI_R_TYPE*)B;

  // KX_R_TYPE 単位で処理
  while (REI_R_TYPE_SIZE <= size) {
    size -= REI_R_TYPE_SIZE;

    if (*a1 != *b1) {
      return int(*a1 - *b1);
    }

    a1++;
    b1++;
  }

  int8_t* a8 = (int8_t*)a1;
  int8_t* b8 = (int8_t*)b1;

  // 残りがあれば 1 バイト単位で処理
  while (0 < size) {
    size -= 1;

    if (*a8 != *b8) {
      return int(*a8 - *b8);
    }

    a8++;
    b8++;
  }

  return 0;
}

// メモリの内容をコピー
void* Memory::Copy(void* A, void* B, int size) {
  REI_R_TYPE* a1 = (REI_R_TYPE*)A;
  REI_R_TYPE* b1 = (REI_R_TYPE*)B;

  // KX_R_TYPE 単位で処理
  while (REI_R_TYPE_SIZE <= size) {
    size -= REI_R_TYPE_SIZE;
    *a1++ = *b1++;
  }

  int8_t* a8 = (int8_t*)a1;
  int8_t* b8 = (int8_t*)b1;

  // 残りがあれば 1 バイト単位で処理
  while (0 < size) {
    size -= 1;
    *a8++ = *b8++;
  }

  return A;
}

// メモリの内容を設定
void* Memory::Fill(void* A, int ch, int size) {
  const int32_t ch_ = ((ch & 0xff) << 24) | ((ch & 0xff) << 16) | ((ch & 0xff) << 8) | (ch & 0xff);

  int32_t* a1 = (int32_t*)A;

  // int32_t(4byte) 単位で処理
  while (int32_size <= size) {
    size -= int32_size;
    *a1++ = ch_;
  }

  int8_t* a8 = (int8_t*)a1;

  // 残りがあれば 1 バイト単位で処理
  while (0 < size) {
    size -= 1;
    *a8++ = ch_ & 0xff;
  }

  return A;
}

// メモリの内容に０を設定
void* Memory::Zero(void* A, int size) {
  return Fill(A, 0, size);
}

}  //rei
