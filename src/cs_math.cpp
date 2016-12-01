//-----------------------------------------------------------------------------
/*!
* @file		mx_math.cpp
* @brief	���l����
* @author	koma
* @version	$Id$
*
* @note
* �E�X�V����  
*/
//-----------------------------------------------------------------------------
// include
#include <string.h>

#include "mx_strlib.h"
#include "mx_math.h"

#define DOUBLE_TO_SINGLE 1  //!< ���l�𕶎���ɂ���Ƃ��Q�o�C�g�������P�o�C�g�����ɕϊ�����

// �ϊ��p�e�[�u��
static const char* s_conv_1 = "0123456789ABCDEFabcdef+-,.xX ";
static const char* s_conv_2 = "�O�P�Q�R�S�T�U�V�W�X�`�a�b�c�d�e�������������{�|�C�D���w�@";

/*!
* �����𐔒l�ɕϊ�
* @param[in] ch �ϊ��Ώە���
* @return ���l
* 
* @note
* �͈͊O�͂��ׂ� -1 �ɂȂ�܂�<br>
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
* ���l�𕶎��ɕϊ�
* @param[in] n �ϊ��Ώې��l
* @return ����
* 
* @note
* �͈͊O�`�F�b�N�͂��Ă��܂���<br>
*/
static char itoc(int n) {
  // ���l���當���֕ϊ�
  return s_conv_1[n];
}

/*!
* �����ɂ��킹�Ă��낢��ݒ�
* @param[in,out] buf �ҏW�o�b�t�@
* @param[in] toLen �ݒ肷�钷��
* @param[in] option �t���O
* @return �ݒ肵������(toLen)
* 
* @note
* toLen �� 0 ��ݒ肷��ƃo�b�t�@�̒������K�p����܂��B<br>
*/
static int align(char* buf, int toLen, int option) {
  int fromlen = mx::strlen(buf);  // ����

  if (toLen == 0) {  // �����̎w��͎����Ȃ̂� fromlen �ɍ��킹��
    toLen = fromlen;

  } else if (fromlen < toLen) {  // �w�肵�������̂ق�������
    int i;
    char ch;

    // ���̃X�y�[�X���O�ɒu��������
    if (option & mx::math::kFillZero) {
      ch = (char)'0';
    } else {
      ch = (char)' ';
    }

    if (option & mx::math::kAlignLeft) {  // ����
      // �����ړ�
      for (i = 0; i < fromlen + 1; ++i) {
        buf[toLen - i] = buf[fromlen - i];
      }
      // ���߂���
      for (i = 0; i < toLen - fromlen; ++i) {
        buf[i] = ch;
      }

    } else if (option & mx::math::kAlignRight) {  // �E��
      // ���߂���
      for (i = fromlen; i < toLen; ++i) {
        buf[i] = ch;
      }
      buf[toLen] = '\0';
    }

  } else if (toLen < fromlen) {  // �w�肵�������̂ق����Z���̂ŋl�߂�
    // �Ō�� null
    buf[toLen] = '\0';
  }

  return toLen;
}

/*!
* 10 �i���� 16 �i���̕ϊ�����(�ċA���� recursive call)
* @param[in] dec �ϊ��Ώې��l
* @param[out] buf �ϊ��������̂��i�[����o�b�t�@
*/
static void dec2hex_r(int dec, char* buf) {
  static const char* table = "0123456789abcdef";

  // 0 �ɂȂ�����Ō�� null ������
  if (dec == 0) {
    *buf = '\0';
    return;
  }

  *buf = table[dec & 15];  //Table[ dec % 16 ];

  // �ċA�������s��
  dec2hex_r(dec >> 4, buf + 1);  //dec2hex_r(dec / 16, buf + 1);
}

/*!
* 10 �i���� 16 �i���̕ϊ�����(��������)
* @param[out] buf ���ʂ��i�[����o�b�t�@
* @param[in] dec �ϊ��Ώې��l
* @param[in] len 16 �i�̒���
* @param[in] option �t���O
* @return ���ʂ��i�[����o�b�t�@
*/
static char* dec2hex(int dec, char* buf, int len, int option) {
  int hexlen;  // hex char length

  // 0 �Ȃ̂��E�E�E�i���s
  if (dec == 0) {
    buf[0] = '0';
    buf[1] = '\0';
  } else {
    dec2hex_r(dec, buf);
  }

  // ���̒����ݒ�
  hexlen = align(buf, len, option);
  (void)hexlen;

  // �o�b�t�@�𔽓]
  mx::sjis::strrev(buf);

  return buf;
}

/*!
* 16 �i���� 10 �i���̕ϊ�����(�ċA���� recursive call)
* @param[in] buf �ϊ��Ώ� 16 �i����
* @param[in] dec ��Ɨp���l
* @return �ϊ���̐��l
*/
static int hex2dec_r(const char* buf, int dec) {
  // �������琔�l�֕ϊ�
  int value;

  for (;;) {
    // �������琔�l�֕ϊ�
    value = ctoi(*buf);

    // ����ɕϊ��ł��Ȃ��ꍇ�̓X�L�b�v����
    if (value == -1) {
      buf++;
      continue;
    }

    // ����ɕϊ��ł����̂Ŕ�����
    break;
  }

  // �ϊ��������l�����Z����
  dec += value;

  // ���� �� �^�u null �������炱���ŏI���
  if (*(buf + 1) == ' ' || *(buf + 1) == '\t' || *(buf + 1) == '\0') {
    return dec;
  }

  // �ċA�������s��
  return hex2dec_r(buf + 1, dec << 4);
}

/*!
* 10 �i����������̕ϊ�����(�ċA���� recursive call)
* @param[in] dec �ϊ��Ώې��l
* @param[out] buf �ϊ��������̂��i�[����o�b�t�@
*/
static void dec2str_r(int dec, char* buf, int comma) {
  // ���l���當���֕ϊ�
  char c = itoc(dec % 10);

  // �����ݒ�
  *buf = c;

  // �����l�߂�
  dec /= 10;

  // ���l�� 0 �ɂȂ����̂ōŌ�� null ������ďI��
  if (dec == 0) {
    *(buf + 1) = '\0';
    return;
  }

  // �J���}������
  if (0 < comma) {
    if ((comma % 3) == 0) {
      *(++buf) = ',';
    }
    comma++;  // ����
  }

  // �ċA�������s��
  dec2str_r(dec, buf + 1, comma);
}

/*!
* ������ 10 �i���̕ϊ�����(�ċA���� recursive call)
* @param[in] buf �ϊ��Ώ� 16 �i����
* @param[in] dec ��Ɨp���l
* @return �ϊ���̐��l
*/
static int str2dec_r(const char* buf, int dec) {
  int value;

  for (;;) {
    // �������琔�l�֕ϊ�
    value = ctoi(*buf);

    // ����ɕϊ��ł��Ȃ��ꍇ�̓X�L�b�v����
    if (value == -1) {
      buf++;
      continue;
    }

    // ����ɕϊ��ł����̂Ŕ�����
    break;
  }

  // �ϊ��������l�����Z����
  dec += value;

  // ���� �� �^�u null �������炱���ŏI���
  if (*(buf + 1) == ' ' || *(buf + 1) == '\t' || *(buf + 1) == '\0') {
    return dec;
  }

  // �ċA�������s��
  return str2dec_r(buf + 1, dec * 10);
}

/*!
* 1 �o�C�g���l������� 2 �o�C�g���l������ɕϊ�
* @param[in] src �ϊ����� 1 �o�C�g���l������
* @param[out] dest �ϊ��������l��������i�[����o�b�t�@
* @return �ϊ���̕�����
* 
* @note
* �E1 �o�C�g���l�� 2 �o�C�g���l�����݂��Ă��Ă����܂��܂���B<br>
* �E���l�Ɠ���̕����ȊO�̃L�����N�^�����������ꍇ�͂��̏�ŏI�����܂��B<br>
* �E�e���|�����o�b�t�@��p�ӂ��Ă���̂ŁA�ϊ����ƕϊ��悪�����ł����܂��܂���B<br>
*/
static char* strValue2Wide(const char* src, char* dest) {
  int i;
  char t_buff[32];

  const char* rp = src;
  char* wp = t_buff;

  while (*rp) {
    char ch = *rp++;

    if (mx::sjis::IsKanji1(ch)) {
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

  // ������R�s�[
  mx::strcpy(dest, t_buff);

  return dest;
}

/*!
* 2 �o�C�g���l������� 1 �o�C�g���l������ɕϊ�
* @param[in] src �ϊ����� 2 �o�C�g���l������
* @param[out] dest �ϊ��������l��������i�[����o�b�t�@
* @return �ϊ���̕�����
* 
* @note
* �E1 �o�C�g���l�� 2 �o�C�g���l�����݂��Ă��Ă����܂��܂���B<br>
* �E������ src < dest �ɂȂ邱�Ƃ͂Ȃ��̂ŁA�ϊ����ƕϊ��悪�����ł����܂��܂���B<br>
*/
static char* strValue2Single(const char* src, char* dest) {
  int i;

  const char* rp = src;
  char* wp = dest;

  while (*rp) {
    char ch = *rp++;

    if (mx::sjis::IsKanji1(ch)) {
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

namespace mx {
namespace math {

/*!
* 10 �i���� 16 �i���̕ϊ�����(�f�t�H���g�̏o�͂͏�����)
* @param[in] dec �ϊ��Ώې��l
* @param[out] buf ���ʂ��i�[����o�b�t�@
* @param[in] len ����
* @param[in] hexPrefix 16�i���̃v���t�B�b�N�X
* @param[in] option �t���O
* @return ���ʂ��i�[����o�b�t�@
* 
* @note
* �啶���ɂ���ꍇ�̓t���O�� kBigCase ���w�肵�Ă�������<br>
*/
char* DecToHex(int dec, char* buf, int len, const char* hexPrefix, int option) {
  dec2hex(dec, buf, len, option);

  // �o�͂͑啶������������
  if (option & kBigCase) {
    for (int i = 0; buf[i]; ++i) {
      if (0x61 <= buf[i] && buf[i] <= 0x66) {
        buf[i] -= 0x20;  // �啶���ɕϊ�
      }
    }
  }

  // ���Ƀv���t�B�b�N�X������
  if (option & kPrefix) {
    int i;
    char temp[256];
    mx::strcpy(temp, buf);

    for (i = 0; hexPrefix[i]; ++i) {
      buf[i] = hexPrefix[i];
    }

    mx::strcpy(buf + i, temp);
  }

  // �_�u���o�C�g�ɕϊ�
  if (option & kDoubleCase) {
    strValue2Wide(buf, buf);
  }

  return buf;
}

/*!
* 16 �i���� 10 �i���̕ϊ�����
* @param[in] buf �ϊ��Ώ� 16 �i����
* @return �ϊ���̐��l
*/
int HexToDec(const char* buf) {
  char* p = (char*)buf;

  // 0x �͊܂߂Ȃ�
  //	if (*(unsigned short*)p == *(unsigned short*)"0x") {
  //		p += 2;
  //	}

  // �Ƃ肠�����ŏ��� 0 �͎�菜��
  //	while (*p == '0') {
  //		p++;
  //	}

  return hex2dec_r(p, 0);
}

/*!
* 10 �i����������̕ϊ�����
* @param[in] dec �ϊ��Ώې��l
* @param[out] buf �ϊ��������̂��i�[����o�b�t�@
* @param[in] len ����
* @param[in] option �t���O
* @return �ϊ��������̂��i�[����o�b�t�@
*/
char* DecToStr(int dec, char* buf, int len, int option) {
  int slen;  // char length
  int sign = 1;

  // �����ݒ�
  if (dec < 0) {
    sign = -1;
    dec = dec * -1;
  }

  // 0 �Ȃ̂��E�E�E�i���s
  if (dec == 0) {
    buf[0] = '0';
    buf[1] = '\0';
  } else if (option & kComma) {
    // 3 �����ƂɃJ���}������
    dec2str_r(dec, buf, 1);
  } else {
    // �J���}�����Ȃ�
    dec2str_r(dec, buf, 0);
  }

  // ���̒����ݒ�
  slen = align(buf, len, option);

  // �����t��
  if (option & kSigned) {
    buf[slen + 0] = (sign == 1) ? (char)'+' : (char)'-';  // �����t��
    buf[slen + 1] = '\0';                                 // �Ō�� null

  } else if (option & kUnsigned) {  // �����Ȃ�
                                    // ���̂܂�

  } else {  // �ʏ�
    // �����́|
    if (sign == -1) {
      buf[slen + 0] = '-';   // �����t��
      buf[slen + 1] = '\0';  // �Ō�� null
    }
  }

  // �o�b�t�@�𔽓]
  mx::sjis::strrev(buf);

  // �_�u���o�C�g�ɕϊ�
  if (option & kDoubleCase) {
    strValue2Wide(buf, buf);
  }

  return buf;
}

/*!
* ������ 10 �i���̕ϊ�����
* @param[in] buf �ϊ��Ώ� 10 �i����
* @return �ϊ���̐��l
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
  val *= sign;            // �����ݒ�

  return val;
}

/*!
* ������𐔒l�ɕϊ�
* @param[in] buf �ϊ����鐔�l������
* @param[in] hexPrefix 16�i���̃v���t�B�b�N�X
* @return �ϊ���̐��l
*/
int StrToInt(const char* buf, const char* hexPrefix) {
  int value;
  int radix = 10;  // �f�t�H���g�� 10 �i��

  char* p;

#if DOUBLE_TO_SINGLE == 1
  char t_buff[64];          // �ϊ��p�o�b�t�@
  p = t_buff;               // �Q�Ɛ�ύX
  strValue2Single(buf, p);  // �啶���͏������ɕϊ����Ă���
#else                       // DOUBLE_TO_SINGLE
  p = (char*)buf;
#endif                      // DOUBLE_TO_SINGLE

  // �����v���t�B�b�N�X�̂Ƃ���16�i���Ƃ���
  {
    int len = mx::strlen(hexPrefix);

    if (len > 0 && mx::strncmp(p, hexPrefix, len) == 0) {
      radix = 16;  // 16 �i��
      p += len;
    }
  }

  switch (radix) {
  default:
  case 10:
    value = StrToDec(p);
    break;  // 10 �i��
  case 16:
    value = HexToDec(p);
    break;  // 16 �i��
  }

  return value;
}

/*!
* ���l�𕶎���ɕϊ�
* @param[in] value �ϊ����鐔�l
* @param[out] dest �ϊ��������l��������i�[����o�b�t�@
* @param[in] hexPrefix 16�i���̃v���t�B�b�N�X(NULL�̂Ƃ���10�i���ŏo��)
* @return �ϊ��������l��������i�[����o�b�t�@
*/
char* IntToStr(int value, char* dest, const char* hexPrefix) {
  // 16 �i��
  if (hexPrefix) {
    DecToHex(value, dest, kAutoLen, hexPrefix, 0 /*option*/);
  } else {  // 10 �i��
    DecToStr(value, dest, kAutoLen, 0 /*option*/);
  }

  return dest;
}

/*!
* ������𐔒l(double)�ɕϊ�
* @param[in] buf �ϊ����鐔�l������
* @return �ϊ���̐��l
* 
* @note
* 10 �i�\�L�̂ݑΉ��B<br>
* AGB ���Ƃ����Ɠ����Ȃ��i���s<br>
*/
double StrToFloat(const char* buf) {
  double val1;  // ������
  double val2;  // ������

  char* p;

#if DOUBLE_TO_SINGLE == 1
  char t_buff[32];          // �ϊ��p�o�b�t�@
  p = t_buff;               // �Q�Ɛ�ύX
  strValue2Single(buf, p);  // �啶���͏������ɕϊ����Ă���
#else                       // DOUBLE_TO_SINGLE
  p = (char*)buf;
#endif                      // DOUBLE_TO_SINGLE

  // �������쐬
  {
    char buff1[16];
    char buff2[8] = "000000";  // ������
    char* wp1 = buff1;
    char* wp2 = buff2;

    // ��������(.(�����_) �� null �܂�)
    for (;;) {
      if (*p == '.' || *p == '\0') {
        *wp1 = '\0';
        break;
      }

      *wp1++ = *p++;
    }

    // ��������(p �� wp2 �� null �܂�)
    if (*p == '.') {
      p++;

      for (;;) {
        if (*p == '\0' || *wp2 == '\0') {
          break;
        }

        *wp2++ = *p++;
      }
    }

    // �Ƃ肠���������ŕϊ�
    val1 = (double)StrToDec(buff1);
    val2 = (double)StrToDec(buff2);
  }

  return val1 + (val2 / 1000000.0);
}

/*!
* ���l(double)�𕶎���ɕϊ�
* @param[in] value �ϊ����鐔�l
* @param[out] dest �ϊ��������l��������i�[����o�b�t�@
* @param[in] option �t���O
* @return �ϊ��������l��������i�[����o�b�t�@
* 
* @note
* 10 �i�\�L�̂ݑΉ��B<br>
*/
char* FloatToStr(double value, char* dest, int option) {
  int i;
  int val1;  // ������
  int val2;  // ������
  char* p = dest;

  // �����������o��
  val1 = (int)value;

  // �����������o��
  val2 = (int)((value - (double)val1) * 1000000.0) % 1000000;

  // �]�v�� 0 ��؂藎�Ƃ�
  if ((0 < val2) && (option & kCutZero)) {
    for (i = 0; i < 6; ++i) {
      if ((val2 % 10) == 0) {
        val2 /= 10;
      } else {
        break;
      }
    }
  }

  // �������̕ϊ�
  DecToStr(val1, p, 0, option);

  // �����_�̕t��
  p += mx::strlen(p);
  if (option & kDoubleCase) {
    *p++ = (char)0x81;  // '�D'
    *p++ = 0x44;
  } else {
    *p++ = 0x2e;  // '.'
  }

  // �������̕ϊ�
  DecToStr(val2, p, 0, option);

  return dest;
}

/*!
* �ׂ���
* @param[in] value ���l
* @param[in] pow ���悷�邩
* @return ����
*/
long Pow(int value, int pow) {
  long n;

  // 0 �͂Ȃ�
  if (value == 0) {
    return 0;
  }

  // ���̒l�ɂ���
  if (value < 0) {
    value = -value;
  }

  // ���
  n = value;

  // �ׂ���
  for (int i = 0; i < pow - 1; ++i) {
    n *= value;
  }

  return n;
}

/*!
* �����̎Z�o
* @param[in] num �擾���鐔�l
* @return ����
*/
int GetDigitNumber(int num) {
  int keta = 0;

  // 0 �̏ꍇ
  if (num == 0) {
    keta++;
  } else {  // 0 �łȂ�
    // ���l�������Ă����Č���������o��
    int num2 = num;

    while (0 < num2) {
      num2 /= 10;
      keta++;
    }
  }

  return keta;
}

}  //math
}  //mx
