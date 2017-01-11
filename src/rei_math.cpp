// -*- mode:c++; coding:utf-8-ws -*-
//====================================================================
//! @file  rei_math.cpp
//! @brief 数値処理
//!
//! @author
//====================================================================
#include <string.h>

#include "rei_strlib.h"
#include "rei_math.h"

#define DOUBLE_TO_SINGLE 1  //!< 数値を文字列にするとき２バイト文字を１バイト文字に変換する

// 変換用テーブル
static const char* s_conv_1 = "0123456789ABCDEFabcdef+-,.xX ";
static const char* s_conv_2 = "０１２３４５６７８９ＡＢＣＤＥＦａｂｃｄｅｆ＋－，．ｘＸ　";

/*!
* 文字を数値に変換
* @param[in] ch 変換対象文字
* @return 数値
* 
* @note
* 範囲外はすべて -1 になります<br>
*/
static int ctoi(char ch) {
  if ('0' <= ch && ch <= '9') {
    return (int)ch - 0x30;
  } else if ('A' <= ch && ch <= 'F') {
    return (int)ch - 0x41 + 0x0a;
  } else if ('a' <= ch && ch <= 'f') {
    return (int)ch - 0x61 + 0x0a;
  }

  return -1;
}

/*!
* 数値を文字に変換
* @param[in] n 変換対象数値
* @return 文字
* 
* @note
* 範囲外チェックはしていません<br>
*/
static char itoc(int n) {
  // 数値から文字へ変換
  return s_conv_1[n];
}

/*!
* 長さにあわせていろいろ設定
* @param[in,out] buf 編集バッファ
* @param[in] toLen 設定する長さ
* @param[in] option フラグ
* @return 設定した長さ(toLen)
* 
* @note
* toLen に 0 を設定するとバッファの長さが適用されます。<br>
*/
static int align(char* buf, int toLen, int option) {
  int fromlen = rei::strlen(buf);  // 長さ

  if (toLen == 0) {  // 長さの指定は自動なので fromlen に合わせる
    toLen = fromlen;

  } else if (fromlen < toLen) {  // 指定した桁数のほうが長い
    int i;
    char ch;

    // 頭のスペースを０に置き換える
    if (option & rei::math::kFillZero) {
      ch = (char)'0';
    } else {
      ch = (char)' ';
    }

    if (option & rei::math::kAlignLeft) {  // 左寄せ
      // 文字移動
      for (i = 0; i < fromlen + 1; ++i) {
        buf[toLen - i] = buf[fromlen - i];
      }
      // うめうめ
      for (i = 0; i < toLen - fromlen; ++i) {
        buf[i] = ch;
      }

    } else if (option & rei::math::kAlignRight) {  // 右寄せ
      // うめうめ
      for (i = fromlen; i < toLen; ++i) {
        buf[i] = ch;
      }
      buf[toLen] = '\0';
    }

  } else if (toLen < fromlen) {  // 指定した桁数のほうが短いので詰める
    // 最後は null
    buf[toLen] = '\0';
  }

  return toLen;
}

/*!
* 10 進数→ 16 進数の変換処理(再帰処理 recursive call)
* @param[in] dec 変換対象数値
* @param[out] buf 変換したものを格納するバッファ
*/
static void dec2hex_r(int dec, char* buf) {
  static const char* table = "0123456789abcdef";

  // 0 になったら最後に null を入れる
  if (dec == 0) {
    *buf = '\0';
    return;
  }

  *buf = table[dec & 15];  //Table[ dec % 16 ];

  // 再帰処理を行う
  dec2hex_r(dec >> 4, buf + 1);  //dec2hex_r(dec / 16, buf + 1);
}

/*!
* 10 進数→ 16 進数の変換処理(内部処理)
* @param[out] buf 結果を格納するバッファ
* @param[in] dec 変換対象数値
* @param[in] len 16 進の長さ
* @param[in] option フラグ
* @return 結果を格納するバッファ
*/
static char* dec2hex(int dec, char* buf, int len, int option) {
  int hexlen;  // hex char length

  // 0 なのだ・・・（つｗＴ
  if (dec == 0) {
    buf[0] = '0';
    buf[1] = '\0';
  } else {
    dec2hex_r(dec, buf);
  }

  // 桁の長さ設定
  hexlen = align(buf, len, option);
  (void)hexlen;

  // バッファを反転
  rei::sjis::strrev(buf);

  return buf;
}

/*!
* 16 進数→ 10 進数の変換処理(再帰処理 recursive call)
* @param[in] buf 変換対象 16 進文字
* @param[in] dec 作業用数値
* @return 変換後の数値
*/
static int hex2dec_r(const char* buf, int dec) {
  // 文字から数値へ変換
  int value;

  for (;;) {
    // 文字から数値へ変換
    value = ctoi(*buf);

    // 正常に変換できない場合はスキップする
    if (value == -1) {
      buf++;
      continue;
    }

    // 正常に変換できたので抜ける
    break;
  }

  // 変換した数値を加算する
  dec += value;

  // 次が 空白 タブ null だったらここで終わり
  if (*(buf + 1) == ' ' || *(buf + 1) == '\t' || *(buf + 1) == '\0') {
    return dec;
  }

  // 再帰処理を行う
  return hex2dec_r(buf + 1, dec << 4);
}

/*!
* 10 進数→文字列の変換処理(再帰処理 recursive call)
* @param[in] dec 変換対象数値
* @param[out] buf 変換したものを格納するバッファ
*/
static void dec2str_r(int dec, char* buf, int comma) {
  // 数値から文字へ変換
  char c = itoc(dec % 10);

  // 文字設定
  *buf = c;

  // 桁を詰める
  dec /= 10;

  // 数値が 0 になったので最後に null をいれて終了
  if (dec == 0) {
    *(buf + 1) = '\0';
    return;
  }

  // カンマをつける
  if (0 < comma) {
    if ((comma % 3) == 0) {
      *(++buf) = ',';
    }
    comma++;  // 桁数
  }

  // 再帰処理を行う
  dec2str_r(dec, buf + 1, comma);
}

/*!
* 文字列→ 10 進数の変換処理(再帰処理 recursive call)
* @param[in] buf 変換対象 16 進文字
* @param[in] dec 作業用数値
* @return 変換後の数値
*/
static int str2dec_r(const char* buf, int dec) {
  int value;

  for (;;) {
    // 文字から数値へ変換
    value = ctoi(*buf);

    // 正常に変換できない場合はスキップする
    if (value == -1) {
      buf++;
      continue;
    }

    // 正常に変換できたので抜ける
    break;
  }

  // 変換した数値を加算する
  dec += value;

  // 次が 空白 タブ null だったらここで終わり
  if (*(buf + 1) == ' ' || *(buf + 1) == '\t' || *(buf + 1) == '\0') {
    return dec;
  }

  // 再帰処理を行う
  return str2dec_r(buf + 1, dec * 10);
}

/*!
* 1 バイト数値文字列を 2 バイト数値文字列に変換
* @param[in] src 変換する 1 バイト数値文字列
* @param[out] dest 変換した数値文字列を格納するバッファ
* @return 変換後の文字列
* 
* @note
* ・1 バイト数値と 2 バイト数値が混在していてもかまいません。<br>
* ・数値と特定の文字以外のキャラクタが見つかった場合はその場で終了します。<br>
* ・テンポラリバッファを用意しているので、変換元と変換先が同じでもかまいません。<br>
*/
static char* strValue2Wide(const char* src, char* dest) {
  int i;
  char t_buff[32];

  const char* rp = src;
  char* wp = t_buff;

  while (*rp) {
    char ch = *rp++;

    if (rei::sjis::IsKanji1(ch)) {
      *wp++ = ch;
      *wp++ = *rp++;
    } else {
      for (i = 0; s_conv_1[i] != '\0'; ++i) {
        if (ch == s_conv_1[i]) {
          *wp++ = s_conv_2[i * 2];
          *wp++ = s_conv_2[i * 2 + 1];
          break;
        }
      }
    }
  }

  *wp = '\0';

  // 文字列コピー
  rei::strcpy(dest, t_buff);

  return dest;
}

/*!
* 2 バイト数値文字列を 1 バイト数値文字列に変換
* @param[in] src 変換する 2 バイト数値文字列
* @param[out] dest 変換した数値文字列を格納するバッファ
* @return 変換後の文字列
* 
* @note
* ・1 バイト数値と 2 バイト数値が混在していてもかまいません。<br>
* ・長さが src < dest になることはないので、変換元と変換先が同じでもかまいません。<br>
*/
static char* strValue2Single(const char* src, char* dest) {
  int i;

  const char* rp = src;
  char* wp = dest;

  while (*rp) {
    char ch = *rp++;

    if (rei::sjis::IsKanji1(ch)) {
      char ch2 = *rp++;

      for (i = 0; s_conv_2[i] != '\0'; i += 2) {
        if (ch == s_conv_2[i] && ch2 == s_conv_2[i + 1]) {
          *wp++ = s_conv_1[i / 2];
          break;
        }
      }
    } else {
      *wp++ = ch;
    }
  }

  *wp = '\0';

  return dest;
}

namespace rei {
namespace math {

/*!
* 10 進数→ 16 進数の変換処理(デフォルトの出力は小文字)
* @param[in] dec 変換対象数値
* @param[out] buf 結果を格納するバッファ
* @param[in] len 長さ
* @param[in] hexPrefix 16進数のプレフィックス
* @param[in] option フラグ
* @return 結果を格納するバッファ
* 
* @note
* 大文字にする場合はフラグに kBigCase を指定してください<br>
*/
char* DecToHex(int dec, char* buf, int len, const char* hexPrefix, int option) {
  dec2hex(dec, buf, len, option);

  // 出力は大文字か小文字か
  if (option & kBigCase) {
    for (int i = 0; buf[i]; ++i) {
      if (0x61 <= buf[i] && buf[i] <= 0x66) {
        buf[i] -= 0x20;  // 大文字に変換
      }
    }
  }

  // 頭にプレフィックスをつける
  if (option & kPrefix) {
    int i;
    char temp[256];
    rei::strcpy(temp, buf);

    for (i = 0; hexPrefix[i]; ++i) {
      buf[i] = hexPrefix[i];
    }

    rei::strcpy(buf + i, temp);
  }

  // ダブルバイトに変換
  if (option & kDoubleCase) {
    strValue2Wide(buf, buf);
  }

  return buf;
}

/*!
* 16 進数→ 10 進数の変換処理
* @param[in] buf 変換対象 16 進文字
* @return 変換後の数値
*/
int HexToDec(const char* buf) {
  char* p = (char*)buf;

  // 0x は含めない
  //	if (*(unsigned short*)p == *(unsigned short*)"0x") {
  //		p += 2;
  //	}

  // とりあえず最初の 0 は取り除く
  //	while (*p == '0') {
  //		p++;
  //	}

  return hex2dec_r(p, 0);
}

/*!
* 10 進数→文字列の変換処理
* @param[in] dec 変換対象数値
* @param[out] buf 変換したものを格納するバッファ
* @param[in] len 長さ
* @param[in] option フラグ
* @return 変換したものを格納するバッファ
*/
char* DecToStr(int dec, char* buf, int len, int option) {
  int slen;  // char length
  int sign = 1;

  // 符号設定
  if (dec < 0) {
    sign = -1;
    dec = dec * -1;
  }

  // 0 なのだ・・・（つｗＴ
  if (dec == 0) {
    buf[0] = '0';
    buf[1] = '\0';
  } else if (option & kComma) {
    // 3 桁ごとにカンマをつける
    dec2str_r(dec, buf, 1);
  } else {
    // カンマをつけなし
    dec2str_r(dec, buf, 0);
  }

  // 桁の長さ設定
  slen = align(buf, len, option);

  // 符号付き
  if (option & kSigned) {
    buf[slen + 0] = (sign == 1) ? (char)'+' : (char)'-';  // 符号付加
    buf[slen + 1] = '\0';                                 // 最後は null

  } else if (option & kUnsigned) {  // 符号なし
                                    // このまま

  } else {  // 通常
    // 符号は－
    if (sign == -1) {
      buf[slen + 0] = '-';   // 符号付加
      buf[slen + 1] = '\0';  // 最後は null
    }
  }

  // バッファを反転
  rei::sjis::strrev(buf);

  // ダブルバイトに変換
  if (option & kDoubleCase) {
    strValue2Wide(buf, buf);
  }

  return buf;
}

/*!
* 文字列→ 10 進数の変換処理
* @param[in] buf 変換対象 10 進文字
* @return 変換後の数値
*/
int StrToDec(const char* buf) {
  int sign = 1;
  int val;

  const char* p = buf;

  switch (*p) {
  case '+':
    sign = 1;
    p++;
    break;
  case '-':
    sign = -1;
    p++;
    break;
  }

  val = str2dec_r(p, 0);  // str -> dec conv
  val *= sign;            // 符号設定

  return val;
}

/*!
* 文字列を数値に変換
* @param[in] buf 変換する数値文字列
* @param[in] hexPrefix 16進数のプレフィックス
* @return 変換後の数値
*/
int StrToInt(const char* buf, const char* hexPrefix) {
  int value;
  int radix = 10;  // デフォルトは 10 進数

  char* p;

#if DOUBLE_TO_SINGLE == 1
  char t_buff[64];          // 変換用バッファ
  p = t_buff;               // 参照先変更
  strValue2Single(buf, p);  // 大文字は小文字に変換しておく
#else                       // DOUBLE_TO_SINGLE
  p = (char*)buf;
#endif                      // DOUBLE_TO_SINGLE

  // 頭がプレフィックスのときは16進数とする
  {
    int len = rei::strlen(hexPrefix);

    if (len > 0 && rei::strncmp(p, hexPrefix, len) == 0) {
      radix = 16;  // 16 進数
      p += len;
    }
  }

  switch (radix) {
  default:
  case 10:
    value = StrToDec(p);
    break;  // 10 進数
  case 16:
    value = HexToDec(p);
    break;  // 16 進数
  }

  return value;
}

/*!
* 数値を文字列に変換
* @param[in] value 変換する数値
* @param[out] dest 変換した数値文字列を格納するバッファ
* @param[in] hexPrefix 16進数のプレフィックス(NULLのときは10進数で出力)
* @return 変換した数値文字列を格納するバッファ
*/
char* IntToStr(int value, char* dest, const char* hexPrefix) {
  // 16 進数
  if (hexPrefix) {
    DecToHex(value, dest, kAutoLen, hexPrefix, 0 /*option*/);
  } else {  // 10 進数
    DecToStr(value, dest, kAutoLen, 0 /*option*/);
  }

  return dest;
}

/*!
* 文字列を数値(double)に変換
* @param[in] buf 変換する数値文字列
* @return 変換後の数値
* 
* @note
* 10 進表記のみ対応。<br>
* AGB だとちゃんと動かない（つｗＴ<br>
*/
double StrToFloat(const char* buf) {
  double val1;  // 整数部
  double val2;  // 小数部

  char* p;

#if DOUBLE_TO_SINGLE == 1
  char t_buff[32];          // 変換用バッファ
  p = t_buff;               // 参照先変更
  strValue2Single(buf, p);  // 大文字は小文字に変換しておく
#else                       // DOUBLE_TO_SINGLE
  p = (char*)buf;
#endif                      // DOUBLE_TO_SINGLE

  // 小数を作成
  {
    char buff1[16];
    char buff2[8] = "000000";  // 少数部
    char* wp1 = buff1;
    char* wp2 = buff2;

    // 整数部分(.(小数点) か null まで)
    for (;;) {
      if (*p == '.' || *p == '\0') {
        *wp1 = '\0';
        break;
      }

      *wp1++ = *p++;
    }

    // 少数部分(p か wp2 が null まで)
    if (*p == '.') {
      p++;

      for (;;) {
        if (*p == '\0' || *wp2 == '\0') {
          break;
        }

        *wp2++ = *p++;
      }
    }

    // とりあえず整数で変換
    val1 = (double)StrToDec(buff1);
    val2 = (double)StrToDec(buff2);
  }

  return val1 + (val2 / 1000000.0);
}

/*!
* 数値(double)を文字列に変換
* @param[in] value 変換する数値
* @param[out] dest 変換した数値文字列を格納するバッファ
* @param[in] option フラグ
* @return 変換した数値文字列を格納するバッファ
* 
* @note
* 10 進表記のみ対応。<br>
*/
char* FloatToStr(double value, char* dest, int option) {
  int i;
  int val1;  // 整数部
  int val2;  // 小数部
  char* p = dest;

  // 整数部抜き出し
  val1 = (int)value;

  // 小数部抜き出し
  val2 = (int)((value - (double)val1) * 1000000.0) % 1000000;

  // 余計な 0 を切り落とす
  if ((0 < val2) && (option & kCutZero)) {
    for (i = 0; i < 6; ++i) {
      if ((val2 % 10) == 0) {
        val2 /= 10;
      } else {
        break;
      }
    }
  }

  // 整数部の変換
  DecToStr(val1, p, 0, option);

  // 小数点の付加
  p += rei::strlen(p);
  if (option & kDoubleCase) {
    *p++ = (char)0x81;  // '．'
    *p++ = 0x44;
  } else {
    *p++ = 0x2e;  // '.'
  }

  // 小数部の変換
  DecToStr(val2, p, 0, option);

  return dest;
}

/*!
* べき乗
* @param[in] value 数値
* @param[in] pow 何乗するか
* @return 結果
*/
long Pow(int value, int pow) {
  long n;

  // 0 はなし
  if (value == 0) {
    return 0;
  }

  // 正の値にする
  if (value < 0) {
    value = -value;
  }

  // 代入
  n = value;

  // べき乗
  for (int i = 0; i < pow - 1; ++i) {
    n *= value;
  }

  return n;
}

/*!
* 桁数の算出
* @param[in] num 取得する数値
* @return 桁数
*/
int GetDigitNumber(int num) {
  int keta = 0;

  // 0 の場合
  if (num == 0) {
    keta++;
  } else {  // 0 でない
    // 数値を割っていって桁数を割り出す
    int num2 = num;

    while (0 < num2) {
      num2 /= 10;
      keta++;
    }
  }

  return keta;
}

}  //math
}  //rei
