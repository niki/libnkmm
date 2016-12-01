//-----------------------------------------------------------------------------
/*!
* @file		mx_math.h
* @brief	数値処理
* @author	koma
* @version	$Id$
*
* @note
* 扱っている文字はすべて Shift-JIS です  
*
* @note
* ・更新履歴  
*/
//-----------------------------------------------------------------------------
#ifndef MX_MATH_H
#define MX_MATH_H

// 数値を文字列に変換
#ifdef NCRT
#define MCR_atoi(x) mx::math::StrToInt(x)
#define MCR_itoa(x, dest, radix) mx::math::IntToStr(x, dest, (radix) == 10 ? NULL : "0x")
#define MCR_atof(x) mx::math::StrToFloat(x)
#else
#include <stdlib.h>
#include <math.h>
#define MCR_atoi(x) atoi(x)
#define MCR_itoa(x, dest, radix) itoa(x, dest, radix)
#define MCR_atof(x) atof(x)
#endif

#define MCR_ftoa(x, dest) mx::math::FloatToStr(x, dest, 0)

namespace mx {
namespace math {

enum {
  kAutoLen = 0,  //!< 長さ自動

  kFillZero = 0x00010000,    //!< 10,16   頭のスペースを０に置き換える(001234)
  kCutZero = 0x00020000,     //!<       f 余計な０を切り落とす(12.34000 -> 12.34)
  kDoubleCase = 0x00040000,  //!< 10,16,f 全角(１２３４)
  kAlignLeft = 0x00080000,   //!< 10,16   左寄せ(1234  )
  kAlignRight = 0x00100000,  //!< 10,16   右寄せ(  1234)
  kBigCase = 0x00200000,     //!<    16   大文字(AZ)
  kSmallCase = 0x00400000,   //!<    16   小文字(az) [default]
  kPrefix = 0x00800000,      //!<    16   頭に 0x をつける(0x1234)
  kSigned = 0x01000000,      //!< 10      ＋－の符号をつける(+1234)
  kUnsigned = 0x02000000,    //!< 10      符号はつけない(－のときも)
  kComma = 0x04000000,       //!< 10      カンマをつける(1,234)
};

char* DecToHex(int dec, char* buf, int len, const char* hexPrefix,
               int option);                               // 10進数 を 16進数文字列 に変換
int HexToDec(const char* buf);                            // 16進数文字列 を 10進数 に変換
char* DecToStr(int dec, char* buf, int len, int option);  // 10進数 を 10進数文字列 に変換
int StrToDec(const char* buf);                            // 数値文字列 を 10進数 に変換

int StrToInt(const char* buf, const char* hexPrefix = "0x");   // 数値文字列 を 整数 に変換
char* IntToStr(int value, char* dest, const char* hexPrefix);  // 整数 を 数値文字列

double StrToFloat(const char* buf);                      // 数値文字列 を 小数 に変換
char* FloatToStr(double value, char* dest, int option);  // 小数 を 数値文字列 に変換

long Pow(int value, int pow);
int GetDigitNumber(int num);

}  //math
}  //mx

#endif  //MX_MATH_H
