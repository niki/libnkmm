// -*- mode:c++; coding:utf-8-ws -*-
#ifndef NKMM_STRLIB_H
#define NKMM_STRLIB_H

// 共通
#define IGNORE_TABSPACE (1 << 30)  // タブスペースを無視する

// 半角英数字記号
#define ANK_ALPHA_BEGIN (0x20)
#define ANK_ALPHA_END (0x7e)

// VbStrConv 列挙体 を参考に StrConv を実装
enum {
  toHiragana =
      0,               // o 文字列内のカタカナをひらがなに変換します。日本語ロケールのみに適用されます。このメンバは、Visual Basic の定数 vbHiragana と等価です。
  toKatakana,          // o 文字列内のひらがなをカタカナに変換します。日本語ロケールのみに適用されます。このメンバは、Visual Basic の定数 vbKatakana と等価です。
  toLinguisticCasing,  // x 文字列に適用する大文字小文字の規則を、ファイル システムの規則から言語学的な規則に変換します。このメンバは、Visual Basic の定数 vbLinguisticCasing と等価です。
  toLowercase,
  toLowerCase,           // o 文字列を小文字に変換します。このメンバは、Visual Basic の定数 vbLowerCase と等価です。
  toNarrow,              // o 文字列内の全角文字を半角文字に変換します。アジア ロケールに適用されます。このメンバは、Visual Basic の定数 vbNarrow と等価です。
  toNone,                // o 変換を実行しません。
  toProperCase,          // x 文字列内の各語の最初の文字を大文字に変換します。このメンバは、Visual Basic の定数 vbProperCase と等価です。
  toSimplifiedChinese,   // x 中国語の繁体字を簡体字に変換します。このメンバは、Visual Basic の定数 vbSimplifiedChinese と等価です。
  toTraditionalChinese,  // x 中国語の簡体字を繁体字に変換します。このメンバは、Visual Basic の定数 vbTraditionalChinese と等価です。
  toUppercase,
  toUpperCase,  // o 文字列を大文字に変換します。このメンバは、Visual Basic の定数 vbUpperCase と等価です。
  toWide,       // o 文字列内の半角 (1 バイト) 文字を全角 (2 バイト) 文字に変換します。アジア ロケールに適用されます。このメンバは、Visual Basic の定数 vbWide と等価です。
};

namespace nkmm {

//@name CRT
//@{
int strlen(const char* s);  //string.h

char* strcpy(char* dst, const char* src);             //string.h
char* strncpy(char* dst, const char* src, int size);  //string.h

int toupper(int ch);  //stdlib.h
int tolower(int ch);  //stdlib.h

char* strupr(char* s);  //extension.h
char* strlwr(char* s);  //extension.h

int strncmp(const char* s1, const char* s2, int len);   //string.h
int strcmp(const char* s1, const char* s2);             //string.h
int strnicmp(const char* s1, const char* s2, int len);  //extension.h
int stricmp(const char* s1, const char* s2);            //string.h

const char* strchr(const char* s, int c);               //string.h
const char* strrchr(const char* s, int c);              //string.h
unsigned long strcspn(const char* s1, const char* s2);  //string.h
unsigned long strspn(const char* s1, const char* s2);   //string.h

char* strcat(char* s1, const char* s2);            //string.h
char* strncat(char* s1, const char* s2, int Len);  //string.h
//@}

}  //nkmm

#include "nkmm_strlib_sjis.h"
//#include "strlib_unicode.h"

#endif  //NKMM_STRLIB_H
