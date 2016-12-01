//-----------------------------------------------------------------------------
/*!
* @file		mx_math.h
* @brief	���l����
* @author	koma
* @version	$Id$
*
* @note
* �����Ă��镶���͂��ׂ� Shift-JIS �ł�  
*
* @note
* �E�X�V����  
*/
//-----------------------------------------------------------------------------
#ifndef MX_MATH_H
#define MX_MATH_H

// ���l�𕶎���ɕϊ�
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
  kAutoLen = 0,  //!< ��������

  kFillZero = 0x00010000,    //!< 10,16   ���̃X�y�[�X���O�ɒu��������(001234)
  kCutZero = 0x00020000,     //!<       f �]�v�ȂO��؂藎�Ƃ�(12.34000 -> 12.34)
  kDoubleCase = 0x00040000,  //!< 10,16,f �S�p(�P�Q�R�S)
  kAlignLeft = 0x00080000,   //!< 10,16   ����(1234  )
  kAlignRight = 0x00100000,  //!< 10,16   �E��(  1234)
  kBigCase = 0x00200000,     //!<    16   �啶��(AZ)
  kSmallCase = 0x00400000,   //!<    16   ������(az) [default]
  kPrefix = 0x00800000,      //!<    16   ���� 0x ������(0x1234)
  kSigned = 0x01000000,      //!< 10      �{�|�̕���������(+1234)
  kUnsigned = 0x02000000,    //!< 10      �����͂��Ȃ�(�|�̂Ƃ���)
  kComma = 0x04000000,       //!< 10      �J���}������(1,234)
};

char* DecToHex(int dec, char* buf, int len, const char* hexPrefix,
               int option);                               // 10�i�� �� 16�i�������� �ɕϊ�
int HexToDec(const char* buf);                            // 16�i�������� �� 10�i�� �ɕϊ�
char* DecToStr(int dec, char* buf, int len, int option);  // 10�i�� �� 10�i�������� �ɕϊ�
int StrToDec(const char* buf);                            // ���l������ �� 10�i�� �ɕϊ�

int StrToInt(const char* buf, const char* hexPrefix = "0x");   // ���l������ �� ���� �ɕϊ�
char* IntToStr(int value, char* dest, const char* hexPrefix);  // ���� �� ���l������

double StrToFloat(const char* buf);                      // ���l������ �� ���� �ɕϊ�
char* FloatToStr(double value, char* dest, int option);  // ���� �� ���l������ �ɕϊ�

long Pow(int value, int pow);
int GetDigitNumber(int num);

}  //math
}  //mx

#endif  //MX_MATH_H
