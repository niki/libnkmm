// -*- mode:c++; coding:utf-8-ws -*-
//====================================================================
//! @file  cs_base64.cpp
//! @brief BASE64
//!
//! @author
//! @note
//! http://homepage2.nifty.com/spw/tips/Base64.html
//====================================================================
#include <string.h>
#include "mx_base64.h"

// 変換テーブル
static const char B64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
#define Val(i) (src[i] == '=' ? 0 : (int)(strchr(B64, src[i]) - B64))

namespace mx {
namespace base64 {

//------------------------------------------------------------------
// デコード
//------------------------------------------------------------------
unsigned char* Decode(unsigned char* src, unsigned char* dest, int size) {
  int i, j = 0;
  int l = size;
  //4バイトずつ処理
  for (i = 0; i < l; i += 4) {
    dest[j] = (char)((Val(i) << 2) | ((Val(i + 1) & 0x30) >> 4));
    dest[j + 1] = (char)(((Val(i + 1) & 0x0f) << 4) | ((Val(i + 2) & 0x3c) >> 2));
    dest[j + 2] = (char)(((Val(i + 2) & 0x03) << 6) | (Val(i + 3) & 0x3f));
    j += 3;
  }

  dest[j] = NULL;
  return dest;
}

//------------------------------------------------------------------
//エンコード
//------------------------------------------------------------------
unsigned char* Encode(unsigned char* src, unsigned char* dest, int size) {
  int i, j = 0;
  int l = size;
  //3バイトずつ処理
  for (i = 0; i < l; i += 3) {
    dest[j] = (char)(0 == (src[i] >> 2)) ? 'A' : (B64[src[i] >> 2]);
    dest[j + 1] = (char)(0 == (((src[i] & 0x03) << 4) | (src[i + 1] >> 4)))
                      ? 'A'
                      : (B64[((src[i] & 0x03) << 4) | (src[i + 1] >> 4)]);
    dest[j + 2] = (char)(0 == (((src[i + 1] & 0x0f) << 2) | (src[i + 2] >> 6)))
                      ? 'A'
                      : (B64[((src[i + 1] & 0x0f) << 2) | (src[i + 2] >> 6)]);
    dest[j + 3] = (char)(0 == (src[i + 2] & 0x3f)) ? 'A' : (B64[src[i + 2] & 0x3f]);
    j += 4;
  }
  dest[j] = NULL;
  return dest;
}

}  // namespace base64
}  // namespace mx
