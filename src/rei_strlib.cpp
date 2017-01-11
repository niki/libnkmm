// -*- mode:c++; coding:utf-8-ws -*-
//====================================================================
//! @file  rei_strlib.cpp
//! @brief 文字列処理
//!
//! @author
//====================================================================
#include <ctype.h>
#include <string.h>
#include <stdio.h>
//#include <extension.h>

#include "rei_strlib.h"

#define NCRT  // No C Runtime

namespace rei {

/*!
	文字列の長さを取得
	@param[in] s 文字列
	@return 長さ
*/
int strlen(const char* s) {
#ifdef NCRT
  const char* top = s;
  while (*s++ != '\0') {
  }  // null 文字まで進める
  return s - top - 1;
#else
  return ::strlen(s);
#endif
}

/*!
	文字列をコピー
	@param[out] dst 出力先
	@param[in] src 入力元
	@return 出力先
*/
char* strcpy(char* dst, const char* src) {
#ifdef NCRT
  int len = strlen(src);
  return strncpy(dst, src, len);
#else
  return ::strcpy(dst, src);
#endif
}

/*!
	文字列をn個コピー
	@param[out] dst 出力先
	@param[in] drc 入力元
	@param[in] size 長さ
	@return 出力先
	
	@note
	size 分だけ忠実にコピーします。途中で null(\0) 文字になっても続けます <br>
	本来の動作は null までなんですが； <br>
	dst に収まりきらない size を指定するほうがどうかと思うよ？ <br>
*/
char* strncpy(char* dst, const char* src, int size) {
#ifdef NCRT
  // 最後に null 文字つけとく
  dst[size] = '\0';

  // long(4byte) 単位で処理
  while (4 <= size) {
    size -= 4;
    *(long*)(dst + size) = *(long*)(src + size);
  }

  // 残りがあれば 1 バイト単位で処理
  while (0 < size) {
    size -= 1;
    *(char*)(dst + size) = *(char*)(src + size);
  }

  return dst;
#else
  return ::strncpy(dst, src, size);
#endif
}

/*!
	小文字を大文字に変換する(a->A)
	@param[in] ch 文字
	@return 文字
*/
int toupper(int ch) {
#ifdef NCRT
  if ('a' <= ch && ch <= 'z') {
    return ch - ANK_ALPHA_BEGIN;
  }

  return ch;
#else
  return ::toupper(ch);
#endif
}

/*!
	大文字を小文字に変換する(A->a)
	@param[in] ch 文字
	@return 文字
*/
int tolower(int ch) {
#ifdef NCRT
  if ('A' <= ch && ch <= 'Z') {
    return ch + ANK_ALPHA_BEGIN;
  }

  return ch;
#else
  return ::tolower(ch);
#endif
}

/*!
	小文字を大文字に変換する(az->AZ)
	@param[in,out] s 文字列
	@return 文字列
*/
char* strupr(char* s) {
#ifdef NCRT
  char* t_s = s;

  while (*s) {
    *s = toupper(*s);
    ++s;
  }

  return t_s;
#else
  return ::strupr(s);
#endif
}

/*!
	大文字を小文字に変換する(AZ->az)
	@param[in,out] s 文字列
	@return 文字列
*/
char* strlwr(char* s) {
#ifdef NCRT
  char* t_s = s;

  while (*s) {
    *s = tolower(*s);
    ++s;
  }

  return t_s;
#else
  return ::strlwr(s);
#endif
}

/*!
	２つの文字列をn個比較する
	@param[in] s1 文字列１
	@param[in] s2 文字列２
	@param[in] len 長さ
	@return 0:s1 = s2, -1:s1 < s2, 1:s1 > s2
*/
int strncmp(const char* s1, const char* s2, int len) {
#ifdef NCRT
  int ret = 0;

  // 結果が違うか len が 0 になるまで
  while (0 < len) {
    ret = *s1 - *s2;

    if (ret) {
      break;
    }

    // 比較サイズを減らす
    --len;

    // 文字列のポインタを進める
    ++s1;
    ++s2;
  }

  return ret;  // diff ok
#else
  return ::strncmp(s1, s2, len);
#endif
}

/*!
	２つの文字列を比較する
	@param[in] s1 文字列１
	@param[in] s2 文字列２
	@return 0:s1 = s2, -1:s1 < s2, 1:s1 > s2
*/
int strcmp(const char* s1, const char* s2) {
#ifdef NCRT
  int len1 = strlen(s1);
  int len2 = strlen(s2);

  return strncmp(s1, s2, (len1 < len2) ? len2 : len1);
#else
  return ::strcmp(s1, s2);
#endif
}

/*!
	２つの文字列をn個小文字として比較する
	@param[in] s1 文字列１
	@param[in] s2 文字列２
	@param[in] len 長さ
	@return 0:s1 = s2, -1:s1 < s2, 1:s1 > s2
*/
int strnicmp(const char* s1, const char* s2, int len) {
// CRTを使用した場合、うまく動作しない
// 調査中 05/13/2011 12:47 PM

#if 1  //def NCRT
  int ret = 0;

  // 結果が違うか len が 0 になるまで
  while (0 < len) {
    ret = tolower(*s1) - tolower(*s2);

    if (ret) {
      break;
    }

    // 比較サイズを減らす
    --len;

    // 文字列のポインタを進める
    ++s1;
    ++s2;
  }

  return ret;  // diff ok
#else
  return ::strnicmp(s1, s2, len);
#endif
}

/*!
	２つの文字列を小文字として比較する
	@param[in] s1 文字列１
	@param[in] s2 文字列２
	@return 0:s1 = s2, -1:s1 < s2, 1:s1 > s2
*/
int stricmp(const char* s1, const char* s2) {
#ifdef NCRT
  int len1 = strlen(s1);
  int len2 = strlen(s2);

  return strnicmp(s1, s2, (s1 < s2) ? len2 : len1);
#else
  return ::stricmp(s1, s2);
#endif
}

/*!
	文字列をスキャンして、指定された文字が最初に出現する位置を見つけます
	@param[in] s 文字列
	@param[in] ch 検索する文字
	@return 見つかった位置, 0:見つからなかった
*/
const char* strchr(const char* s, int ch) {
#ifdef NCRT
  const char* t_s = 0;

  for (;;) {
    char s_ch = *s++;

    if (s_ch == '\0') {
      break;
    } else if (s_ch == (char)ch) {
      t_s = s - 1;
      break;
    }
  }

  return t_s;
#else
  return ::strchr(s, ch);
#endif
}

/*!
	文字列をスキャンして、指定された文字が最後に出現する位置を見つけます
	@param[in] s 文字列
	@param[in] ch 検索する文字
	@return 見つかった位置, 0:見つからなかった
*/
const char* strrchr(const char* s, int ch) {
#ifdef NCRT
  const char* t_s = 0;

  for (;;) {
    char ch2 = *s++;

    if (ch2 == '\0') {
      break;
    } else if (ch2 == (char)ch) {
      t_s = s - 1;
    }
  }

  return t_s;
#else
  return ::strrchr(s, ch);
#endif
}

/*!
	文字列をスキャンして、指定された文字群が最初に出現する位置を見つけます
	@param[in] s1 文字列１
	@param[in] s2 文字列２
	@return 見つかった位置までの長さ。見つからない場合は s1 の長さ
*/
unsigned long strcspn(const char* s1, const char* s2) {
#ifdef NCRT
  const char* t_s1 = s1;

  for (;;) {
    char ch = *s1++;

    if (strchr(s2, ch)) {
      break;
    }
  }

  return s1 - t_s1 - 1;
#else
  return ::strcspn(s1, s2);
#endif
}

/*!
	文字列をスキャンして、指定された文字群以外が最初に出現する位置を見つけます
	@param[in] s1 文字列１
	@param[in] s2 文字列２
	@return 見つかった位置までの長さ。見つからない場合は s1 の長さ
*/
unsigned long strspn(const char* s1, const char* s2) {
#ifdef NCRT
  const char* t_s1 = s1;

  for (;;) {
    char ch = *s1++;

    if (!strchr(s2, ch)) {
      break;
    }
  }

  return s1 - t_s1 - 1;
#else
  return ::strspn(s1, s2);
#endif
}

/*!
	文字列を繋げる(s1 に s2 を繋げる)
	@param[in,out] s1 文字列１
	@param[in] s2 文字列２
	@return char* 
*/
char* strcat(char* s1, const char* s2) {
#ifdef NCRT
  return strcpy(s1 + strlen(s1), s2);
#else
  return ::strcat(s1, s2);
#endif
}

/*!
	文字列を繋げる(s1 に s2 を len 繋げる)
	@param[in,out] s1 文字列１
	@param[in] s2 文字列２
	@param[in] len 長さ
	@return char*
*/
char* strncat(char* s1, const char* s2, int len) {
#ifdef NCRT
  return strncpy(s1 + strlen(s1), s2, len);
#else
  return ::strncat(s1, s2, len);
#endif
}

}  //rei
