// -*- mode:c++; coding:utf-8-ws -*-
//====================================================================
//! @file  rei_buffer.cpp
//! @brief バッファ操作 (f*()のバッファ版)
//!
//! @author
//====================================================================
#include <stdio.h>
//#include <malloc.h>

#include "rei_buffer.h" /* Buffer operation */
#include "rei_basis.h"

// バッファを閉じます
int bclose(BUFF* bp) {
  free_(bp->buffer);
  free_(bp);
  return 0;
}

// バッファをフラッシュします
int bflush(BUFF* bp) {
  (void)bp;
  return 0;
}

// バッファから 1 文字取得、バッファポインタを進める
int bgetc(BUFF* bp) {
  unsigned char ch = *bp->curp++;
  return (int)ch;
}

// バッファから文字列を取得します
char* bgets(char* dest, int n, BUFF* bp) {
  char* t_dest = dest;

  while (n--) {
    char ch = *(char*)bp->curp++;

    // バッファオーバー
    if (bp->buffer + bp->bsize < bp->curp) {
      return NULL;
    }

    if (ch == '\r' /*CR(0x0d)*/ || ch == '\n' /*LF(0x0a)*/ || ch == '\0') {
      *dest = '\0';

      // CRLF と判断
      if (ch == '\r' && *bp->curp == '\n') {
        bp->curp++;
      }

      return t_dest;
    } else {
      *dest++ = ch;
    }
  }

  return NULL;
}

// バッファのオープン
BUFF* bopen(const char* path, const char* mode) {
  FILE* fp;
  BUFF* bp;
  int fsize;
  unsigned char* buffer;

  // ファイルを開く
  if ((fp = fopen(path, "rb")) == NULL) {
#ifdef __debugprint
    printf("file open error(%s)\n", path);
#endif            // __debugprint
    return NULL;  //_B_OPEN_ERR;
  }

  // バッファ取得のためにファイルサイズをチェックする
  fseek(fp, 0L, B_SEEK_END);  // ファイルポインタを最後尾へ移動
  fsize = ftell(fp);          // ファイルサイズを取得
  fseek(fp, 0L, B_SEEK_SET);  // ファイルポインタを先頭へ移動

  // ファイルサイズが 0 以下なのでエラーで返す
  if (fsize <= 0) {
#ifdef __debugprint
    printf("file size is zero(%s)\n", path);
#endif            // __debugprint
    fclose(fp);   /* file close */
    return NULL;  //_B_OPEN_ERR;
  }

  // バッファ取得
  buffer = (unsigned char*)malloc_(sizeof(BUFF) + fsize);
  fread(buffer + sizeof(BUFF), fsize, 1, fp);
  fclose(fp); /* file close */

  // バッファ設定
  bp = (BUFF*)buffer;
  bp->curp = buffer + sizeof(BUFF);
  bp->buffer = bp->curp;
  bp->bsize = fsize;

#if _B_USE_NAME
  {
    int i;
    for (i = 0; path[i]; i++) {
      bp->name[i] = path[i];
    }
    bp->name[i] = '\0';
  }
#endif  // _B_USE_NAME

  (void)mode;
  return bp;
}

// バッファから１文字出力、バッファポインタを進める
int bputc(int ch, BUFF* bp) {
  *bp->curp++ = (unsigned char)ch;
  return ch;
}

// バッファに文字列を書き込みます
int bputs(char* str, BUFF* bp) {
  char* str_ = str;

  while (*str) {
    *bp->curp++ = *str++;
  }

  return str - str_ - 1;
}

// バッファから指定サイズのデータを指定した個数分だけ取得する
int bread(void* ptr, int size, int n, BUFF* bp) {
  int i, j;

  for (i = 0; i < n; i++) {
    for (j = 0; j < size; j++) {
      ((unsigned char*)ptr)[i * size + j] = *bp->curp++;
    }
  }

  return size;
}

// バッファのシーク
int bseek(BUFF* bp, long offset, int whence) {
  if (bp == 0) {
    return 1;
  }

  switch (whence) {
  case B_SEEK_CUR:  // 現在地から
    bp->curp += offset;
    break;

  case B_SEEK_END:  // 終端から
    bp->curp = (bp->buffer + bp->bsize) + offset;
    break;

  case B_SEEK_SET:  // 先頭から
    bp->curp = bp->buffer + offset;
    break;
  }

  return 0;
}

// バッファから指定サイズのデータを指定した個数分だけ出力する
int bwrite(const void* ptr, int size, int n, BUFF* bp) {
  int i, j;

  for (i = 0; i < n; i++) {
    for (j = 0; j < size; j++) {
      *bp->curp++ = ((unsigned char*)ptr)[i * size + j];

      if ((bp->buffer + bp->bsize) < bp->curp) {
        return i;
      }
    }
  }

  return n;
}

// バッファポインタからオフセットを取得する
int btell(BUFF* bp) {
  return bp->curp - bp->buffer;
}
