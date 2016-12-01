//-----------------------------------------------------------------------------
/*!
* @file		mx_str_sjis.cpp
* @brief	�����񏈗�
* @author	koma
* @version	$Id$
* 
* @note
* �E�X�V����  
* 2005.05.20	�|�C���^�̃`�F�b�N���͂����Ă݂�  
*/
//-----------------------------------------------------------------------------

// include
#include <ctype.h>
#include <string.h>
#include <stdio.h>
//#include <extension.h>

#include "mx_types.h"
#include "mx_strlib.h"
#include "mx_strlib_sjis.h"
#include "font_code.ci"

// ���p�p�����L��
#define SJIS_ANK_ALPHA_BEGIN ANK_ALPHA_BEGIN
#define SJIS_ANK_ALPHA_END ANK_ALPHA_END

// ���p�J�^�J�i
#define SJIS_ANK_KANA_BEGIN 0xa1
#define SJIS_ANK_KANA_END 0xdf

// ���p�J�^�J�i(����)
#define SJIS_ANK_KANA_SP1 0xde  //�
#define SJIS_ANK_KANA_SP2 0xdf  //�

// �����P�o�C�g��
#define SJIS_KANJI11_BEGIN 0x81
#define SJIS_KANJI11_END 0x9f
#define SJIS_KANJI12_BEGIN 0xe0
#define SJIS_KANJI12_END 0xfc

// �����Q�o�C�g��
#define SJIS_KANJI21_BEGIN 0x40
#define SJIS_KANJI21_END 0x7e
#define SJIS_KANJI22_BEGIN 0x80
#define SJIS_KANJI22_END 0xfc

namespace mx {

/*!
* �S�p��������S�p�啶���ɕϊ�����(��->�`)
* @param[in] cc ����
* @return ����
*/
int sjis::toupper(int cc) {
  if ((uint16_t)(cc & 0xff00) == 0x8200 &&
      (0x81 <= (uint16_t)(cc & 0xff) && (uint16_t)(cc & 0xff) <= 0x9a)) {
    return cc - 0x21;
  }

  return cc;
}

/*!
* �S�p�啶����S�p�������ɕϊ�����(�`->��)
* @param[in] cc ����
* @return ����
*/
int sjis::tolower(int cc) {
  if ((uint16_t)(cc & 0xff00) == 0x8200 &&
      (0x60 <= (uint16_t)(cc & 0xff) && (uint16_t)(cc & 0xff) <= 0x79)) {
    return cc - 0x21;
  }

  return cc;
}

/*!
* ��������啶���ɕϊ�����(az->AZ)
* @param[in,out] s ������
* @return ������
*/
char* sjis::strupr(char* s) {
  char* t_s = s;

  while (*s) {
    if (sjis::IsKanji1(*s)) {
      *(short*)s = sjis::toupper(*(short*)s);
      ++s;
      ++s;
    } else /*if (!sjis::IsKanji1(*s))*/ {
      *s = toupper(*s);
      ++s;
    }
  }

  return t_s;
}

/*!
* �啶�����������ɕϊ�����(AZ->az)
* @param[in,out] s ������
* @return ������
*/
char* sjis::strlwr(char* s) {
  char* t_s = s;

  while (*s) {
    if (sjis::IsKanji1(*s)) {
      *(short*)s = sjis::tolower(*(short*)s);
      ++s;
      ++s;
    } else /*if (!sjis::IsKanji1(*s))*/ {
      *s = tolower(*s);
      ++s;
    }
  }

  return t_s;
}

/*!
* �Q�̕������n��r����(���{��`�F�b�N����)
* @param[in] s1 ������P
* @param[in] s2 ������Q
* @param[in] len ����
* @return 0  s1 = s2, -1  s1 < s2, 1  s1 > s2
*/
int sjis::strncmp(const char* s1, const char* s2, int len) {
  int ret = 0;

  // ���ʂ��Ⴄ�� len �� 0 �ɂȂ�܂�
  while (0 < len) {
    if (sjis::IsKanji1(*s1)) {
      if (sjis::IsKanji1(*s2)) {
        ret = (int)(*(uint16_t*)s1 - (int)*(uint16_t*)s2);

        if (ret) {
          break;
        }

        // ��r�T�C�Y�����炷
        len -= 2;

        // ������̃|�C���^��i�߂�
        s1 += 2;
        s2 += 2;
      } else /*if (!sjis::IsKanji1(*s2))*/ {
        return 1;  // s1 > s2
      }
    } else /*if (!sjis::IsKanji1(*s1))*/ {
      if (!sjis::IsKanji1(*s2)) {
        ret = *s1 - *s2;

        if (ret) {
          break;
        }

        // ��r�T�C�Y�����炷
        --len;

        // ������̃|�C���^��i�߂�
        ++s1;
        ++s2;
      } else /*if (sjis::IsKanji1(*s2))*/ {
        return -1;  // s1 < s2
      }
    }
  }

  return ret;  // diff ok
}

/*!
* �Q�̕�������r����(���{��`�F�b�N����)
* @param[in] s1 ������P
* @param[in] s2 ������Q
* @return 0  s1 = s2, -1  s1 < s2, 1  s1 > s2
*/
int sjis::strcmp(const char* s1, const char* s2) {
  int len1 = strlen(s1);
  int len2 = strlen(s2);

  return sjis::strncmp(s1, s2, (len1 < len2) ? len2 : len1);
}

/*!
* �Q�̕������n�������Ƃ��Ĕ�r����(���{��`�F�b�N����)
* @param[in] s1 ������P
* @param[in] s2 ������Q
* @param[in] len ����
* @return 0  s1 = s2, -1  s1 < s2, 1  s1 > s2
*/
int sjis::strnicmp(const char* s1, const char* s2, int len) {
  int ret = 0;

  // ���ʂ��Ⴄ�� len �� 0 �ɂȂ�܂�
  while (0 < len) {
    if (sjis::IsKanji1(*s1)) {
      if (sjis::IsKanji1(*s2)) {
        ret = (int)(*(uint16_t*)s1 - (int)*(uint16_t*)s2);

        if (ret) {
          break;
        }

        // ��r�T�C�Y�����炷
        len -= 2;

        // ������̃|�C���^��i�߂�
        s1 += 2;
        s2 += 2;
      } else /*if (!sjis::IsKanji1(*s2))*/ {
        return 1;  // s1 > s2
      }
    } else /*if (!sjis::IsKanji1(*s1))*/ {
      if (!sjis::IsKanji1(*s2)) {
        ret = tolower(*s1) - tolower(*s2);

        if (ret) {
          break;
        }

        // ��r�T�C�Y�����炷
        --len;

        // ������̃|�C���^��i�߂�
        ++s1;
        ++s2;
      } else /*if (sjis::IsKanji1(*s2))*/ {
        return -1;  // s1 < s2
      }
    }
  }

  return ret;  // diff ok
}

/*!
* �Q�̕�������������Ƃ��Ĕ�r����(���{��`�F�b�N����)
* @param[in] s1 ������P
* @param[in] s2 ������Q
* @return 0  s1 = s2, -1  s1 < s2, 1  s1 > s2
*/
int sjis::stricmp(const char* s1, const char* s2) {
  int len1 = strlen(s1);
  int len2 = strlen(s2);

  return sjis::strnicmp(s1, s2, (s1 < s2) ? len2 : len1);
}

/*!
* �������؂���
* @param[out] dst �o�͐�
* @param[in] src ���͌�
* @param[in] index �؂���ʒu
* @param[in] len �؂��蒷��
* @return char*
*/
char* sjis::strsub(char* dst, const char* src, int index, int len) {
  return strncpy(dst, src + index, len);
}

/*!
* �o�b�t�@�𔽓]
* @param[in,out] s ������
* @return ������
*/
char* sjis::strrev(char* s) {
  int i, j;

  // �o�b�t�@�𔽓]
  for (i = 0, j = strlen(s) - 1; i < j; ++i, --j) {
    char ch = s[i];
    s[i] = s[j];
    s[j] = ch;
  }

  return s;
}

/*!
* �^�u���X�y�[�X���`�F�b�N
* @param[in] ch �L�����N�^
* @return 0:�Ⴄ, 1:�^�u���X�y�[�X
*/
int sjis::IsTabSpace(char ch) {
  return (ch == '\t' || ch == ' ');
}

/*!
* ���{��P�o�C�g�ڂ̕��������ׂ�
* @param[in] ch ����
* @return 1:���{��P�o�C�g��, 0:�Ⴄ
* 
* @note
* http://www.st.rim.or.jp/~phinloda/cqa/cqa15.html#Q4
*/
int sjis::IsKanji1(char ch) {
  //return ((uint8_t)ch ^ 0x20) - 0xa1 < 0x3c;
  if ((uint8_t)ch >= SJIS_KANJI11_BEGIN && (uint8_t)ch <= SJIS_KANJI11_END) return true;
  if ((uint8_t)ch >= SJIS_KANJI12_BEGIN && (uint8_t)ch <= SJIS_KANJI12_END) return true;
  return false;
}

/*!
* ���{��Q�o�C�g�ڂ̕��������ׂ�
* @param[in] ch ����
* @return 1:���{��Q�o�C�g��, 0:�Ⴄ
*/
int sjis::IsKanji2(char ch) {
  if ((uint8_t)ch >= SJIS_KANJI21_BEGIN && (uint8_t)ch <= SJIS_KANJI21_END) return true;
  if ((uint8_t)ch >= SJIS_KANJI22_BEGIN && (uint8_t)ch <= SJIS_KANJI22_END) return true;
  return false;
}

/*!
* ���p�J�i�����ׂ�
* @param[in] ch ����
* @return 1:���p�J�i, 0:�Ⴄ
*/
int sjis::IsKana(char ch) {
  if (SJIS_ANK_KANA_BEGIN <= (uint8_t)ch && (uint8_t)ch <= SJIS_ANK_KANA_END) return true;
  return false;
}

/*!
* �����񂩂�w��̕�������菜��
* @param[out] s ������
* @param[in] dels �폜�����Q
* @param[in] len ����
*/
void sjis::RemoveNString(char* s, const char* dels, int len) {
  char* rp = s;

  while (len > 0) {
    if (sjis::IsKanji1(*rp)) {
      int rm = 0;
      const char* p = dels;

      while (*p) {
        if (sjis::IsKanji1(*p)) {
          if (*p == *rp && *(p + 1) == *(rp + 1)) {
            //�폜�Ώ�
            rm = 1;
            break;
          }
        }

        p += 2;
      }

      if (!rm) {
        *s++ = *(rp + 0);
        *s++ = *(rp + 1);
      }

      ++rp;
      ++rp;

      --len;
      --len;
    } else /*if (!sjis::IsKanji1(*rp))*/ {
      int rm = 0;
      const char* p = dels;

      while (*p) {
        if (!sjis::IsKanji1(*p)) {
          if (*p == *rp) {
            //�폜�Ώ�
            rm = 1;
            break;
          }
        }

        ++p;
      }

      if (!rm) {
        *s++ = *rp;
      }

      ++rp;

      --len;
    }
  }

  *s = '\0';
}

/*!
* �����񂩂�w��̂P�o�C�g��������菜��
* @param[out] s ������
* @param[in] dels �폜�����Q
*/
void sjis::RemoveString(char* s, const char* dels) {
  sjis::RemoveNString(s, dels, strlen(s));
}

/*!
* len �̒����������`�F�b�N�� nil �̕������炷
* @param[in,out] s ������
* @param[in] len ������̒���
* @return ������
*/
static __inline char* excludeNull(char* s, int len) {
  int i;
  char* t_s = s;

  // nil ���l�߂�
  for (i = 0; i < len; ++i) {
    // nil����Ȃ��Ƃ�����
    if (t_s[i] != '\0') {
      *s++ = t_s[i];
    }
  }

  *s = '\0';  // �Ō�� null ����
  return t_s;
}

/*!
* �擪����A�z��Ŏw�肳�ꂽ�����Z�b�g�����ׂč폜���܂�
* @param[in,out] s ������
* @param[in] len ������̒���
* @param[in] dels ��菜���Ώە����̂Ȃ��
* 
* @note
* �P�o�C�g������̂ݑΉ��B�Ⴄ�����������炻���ŏI���<br>
*/
static __inline void trimStart(char* s, int len, const char* dels) {
  (void)len;

  while (*s) {
    const char* p = strchr(dels, *s);

    if (p == 0) {
      break;
    }

    *s++ = '\0';  // �Ώە����� '\0' �Ŗ��߂�
  }
}

/*!
* ��������A�z��Ŏw�肳�ꂽ�����Z�b�g�����ׂč폜���܂�
* @param[in,out] s ������
* @param[in] len ������̒���
* @param[in] dels ��菜���Ώە����̂Ȃ��
* 
* @note
* �P�o�C�g������̂ݑΉ��B�Ⴄ�����������炻���ŏI���<br>
*/
static __inline void trimEnd(char* s, int len, const char* dels) {
  s += (len - 1);

  while (len--) {
    const char* p = strchr(dels, *s);

    if (p == 0) {
      break;
    }

    *s-- = '\0';  // �Ώە����� '\0' �Ŗ��߂�
  }
}

/*!
* �擪�Ɩ����ŁA�z��Ŏw�肳�ꂽ�����Z�b�g�����ׂč폜���܂�
* @param[in,out] s ������
* @param[in] dels ��菜�������Q
*/
void sjis::TrimString(char* s, const char* dels) {
  /*if (s)*/ {
    int len = strlen(s);

    if (len == 0) {
      return;
    }

    trimStart(s, len, dels);  // ��������
    trimEnd(s, len, dels);    // �E������
    excludeNull(s, len);      // nil ���l�߂�
  }
}

/*!
* �擪����A�z��Ŏw�肳�ꂽ�����Z�b�g�����ׂč폜���܂�
* @param[in,out] s ������
* @param[in] dels ��菜�������Q
*/
void sjis::TrimStringStart(char* s, const char* dels) {
  /*if (s)*/ {
    int len = strlen(s);

    if (len == 0) {
      return;
    }

    trimStart(s, len, dels);  // ��������
    excludeNull(s, len);      // nil ���l�߂�
  }
}

/*!
* ��������A�z��Ŏw�肳�ꂽ�����Z�b�g�����ׂč폜���܂�
* @param[in,out] s ������
* @param[in] dels ��菜�������Q
*/
void sjis::TrimStringEnd(char* s, const char* dels) {
  /*if (s)*/ {
    int len = strlen(s);

    if (len == 0) {
      return;
    }

    trimEnd(s, len, dels);  // �E������
    excludeNull(s, len);    // nil ���l�߂�
  }
}

/*!
* ���������擾����
* @param[in] s ������
* @return ������
*/
int sjis::GetCharLength(const char* s) {
  int moji = 0;

  while (*s) {
    uint8_t ch = *(uint8_t*)s++;

    if (sjis::IsKanji1(ch)) {
      ++s;
      ++moji;
    } else /*if (!sjis::IsKanji1(ch))*/ {
      // �p�����L��
      //" !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
      if (SJIS_ANK_ALPHA_BEGIN <= ch && ch <= SJIS_ANK_ALPHA_END) {
        ++moji;
      }
      // ���p�J�^�J�i
      //"���������������������������������������������������������������"
      else if (SJIS_ANK_KANA_BEGIN <= ch && ch <= SJIS_ANK_KANA_END) {
        ++moji;

        if (*(uint8_t*)s == SJIS_ANK_KANA_SP1) {  //�
          ++s;
        } else if (*(uint8_t*)s == SJIS_ANK_KANA_SP2) {  //�
          ++s;
        } else {
        }
      } else {
        ++moji;
      }
    }
  }

  return moji;
}

/*!
* ��������������R�s�[
* @param[out] dst �o�͐�
* @param[in] src ���͌�
* @param[in] num ������
* @return �o�͐�
*/
char* sjis::CopyChar(char* dst, const char* src, int num) {
  int pos = sjis::CharToPos(src, num + 1);
  strncpy(dst, src, pos);
  dst[pos] = '\0'; /* '\0' �������t������Ȃ��Ƃ��̑΍� */
  return dst;
}

/*!
* �������ڂ̈ʒu���擾����
* @param[in] s ������
* @param[in] num �擾�������͉̂������ځH�i�P��������w��j
* @return int:�ʒu(byte�P��), -1:���s
*/
int sjis::CharToPos(const char* s, int num) {
  uint8_t* t_s = (uint8_t*)s;
  int ch_size;  // �I�������Ƃ��������T�C�Y(n �����ڂ̈ʒu��߂����߂Ɉ���)

  while (*s) {
    uint8_t ch = *(uint8_t*)s++;

    if (sjis::IsKanji1(ch)) {
      ++s;
      ch_size = 2;
    } else /*if (!sjis::IsKanji1(ch))*/ {
      // �p�����L��
      //" !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
      if (SJIS_ANK_ALPHA_BEGIN <= ch && ch <= SJIS_ANK_ALPHA_END) {
        ch_size = 1;
      }
      // ���p�J�^�J�i
      //"���������������������������������������������������������������"
      else if (SJIS_ANK_KANA_BEGIN <= ch && ch <= SJIS_ANK_KANA_END) {
        if (*(uint8_t*)s == SJIS_ANK_KANA_SP1) {  //�
          ++s;
          ch_size = 2;
        } else if (*(uint8_t*)s == SJIS_ANK_KANA_SP2) {  //�
          ++s;
          ch_size = 2;
        } else {
          ch_size = 1;
        }
      } else {
        ch_size = 1;
      }
    }

    if (--num <= 0) {
      return (uint8_t*)s - t_s - ch_size;  // �������ʒu��Ԃ�
    }
  }

  return -1;
}

/*!
* pos �̈ʒu���������ڂ��擾����
* @param[in] s ������
* @param[in] pos �擾�������ʒu�̕�����(�P�o�C�g����w��)
* @return �ʒu(�������P��), -1:���s
*/
int sjis::PosToChar(const char* s, int pos) {
  int moji = 0;

  while (*s) {
    uint8_t ch = *(uint8_t*)s++;

    if (sjis::IsKanji1(ch)) {
      ++s;
      ++moji;
      --pos;
    } else /*if (!sjis::IsKanji1(ch))*/ {
      // �p�����L��
      //" !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
      if (SJIS_ANK_ALPHA_BEGIN <= ch && ch <= SJIS_ANK_ALPHA_END) {
        ++moji;
      }
      // ���p�J�^�J�i
      //"���������������������������������������������������������������"
      else if (SJIS_ANK_KANA_BEGIN <= ch && ch <= SJIS_ANK_KANA_END) {
        ++moji;

        if (*(uint8_t*)s == SJIS_ANK_KANA_SP1) {  //�
          ++s;
          --pos;
        } else if (*(uint8_t*)s == SJIS_ANK_KANA_SP2) {  //�
          ++s;
          --pos;
        } else {
        }
      } else {
        ++moji;
      }
    }

    if (--pos <= 0) {
      return moji;
    }
  }

  return -1;
}

/*!
* ���p��S�p�ɕϊ�����
* �iAlphabet Numeric Kana -> Double Byte Character Set�j
* @param[out] dst �������ݐ�|�C���^
* @param[in] src �ǂݍ��݌��|�C���^
* @return �������ݐ�|�C���^
* 
* @note
* dst �� src �ɓ������͎̂w��ł��܂���
*/
char* sjis::AnkToDbcs(char* dst, const char* src) {
  char* t_dst = dst;

  while (*src) {
    uint8_t ch = *(uint8_t*)src++;

    if (sjis::IsKanji1(ch)) {
      *dst++ = (char)ch;
      *dst++ = *src++;
    } else /*if (!sjis::IsKanji1(ch))*/ {
      // �p�����L��
      //" !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
      if (SJIS_ANK_ALPHA_BEGIN <= ch && ch <= SJIS_ANK_ALPHA_END) {
        uint8_t* p = (uint8_t*)fc2;
        p += (ch - SJIS_ANK_ALPHA_BEGIN) * 2;
        *dst++ = *(char*)(p + 1);
        *dst++ = *(char*)p;
      }
      // ���p�J�^�J�i
      //"���������������������������������������������������������������"
      else if (SJIS_ANK_KANA_BEGIN <= ch && ch <= SJIS_ANK_KANA_END) {
        uint8_t* p = (uint8_t*)fc1;
        p += (ch - SJIS_ANK_KANA_BEGIN) * 2;
        *dst++ = *(char*)(p + 1);
        *dst++ = *(char*)p;

        if (*(uint8_t*)src == SJIS_ANK_KANA_SP1) {  //�
          if (ch == (uint8_t)'�') {                 // ��
            *(dst - 1) = (char)0x94;
          } else {
            *(dst - 1) += 1;
          }
          ++src;
        } else if (*(uint8_t*)src == SJIS_ANK_KANA_SP2) {  //�
          *(dst - 1) += 2;
          ++src;
        }
      }
      // ���̑�(�����ɂ�����Ă��Ƃ͔�����������Ă��ƁH)
      else {
        *dst++ = (char)ch;
      }
    }
  }

  *dst = '\0';
  return t_dst;
}

/*!
* �S�p�𔼊p�ɕϊ�����
* �iDouble Byte Character Set -> Alphabet Numeric Kana�j
* @param[out] dst �������ݐ�|�C���^
* @param[in] src �ǂݍ��݌��|�C���^
* @return �������ݐ�|�C���^
* 
* @note
* dst �� src �ɓ������̂��w��ł��܂�
*/
char* sjis::DbcsToAnk(char* dst, const char* src) {
  char* t_dst = dst;

  while (*src) {
    uint8_t ch = *(uint8_t*)src++;

    if (sjis::IsKanji1(ch)) {
      if (ch == 0x81) {
        //" ��,.�:;?!��   ^~_         �  /   |  `' "()  []{}    ��    +-    = <>          \$  %#&*@"
        // "�@"0x8140�`"��"0x8197 �܂�
        if (0x40 <= *(uint8_t*)src && *(uint8_t*)src <= 0x97) {
          uint8_t ch2 = fc81[*(uint8_t*)src - 0x40];

          if (ch2 != 0x7f) {
            *dst++ = (char)ch2;
            ++src;
            continue;  //***
          }
        }
      } else if (ch == 0x82) {
        //"0123456789       ABCDEFGHIJKLMNOPQRSTUVWXYZ      abcdefghijklmnopqrstuvwxyz"
        // "�O"0x824f�`"��"0x829a �܂�
        // �O�`�X
        if (0x4f <= *(uint8_t*)src && *(uint8_t*)src <= 0x58) {
          *dst++ = (char)(0x30 + *(uint8_t*)src - 0x4f);
          ++src;
          continue;  //***
        }
        // �`�`�y
        else if (0x60 <= *(uint8_t*)src && *(uint8_t*)src <= 0x79) {
          *dst++ = (char)(0x41 + *(uint8_t*)src - 0x60);
          ++src;
          continue;  //***
        }
        // ���`��
        else if (0x81 <= *(uint8_t*)src && *(uint8_t*)src <= 0x9a) {
          *dst++ = (char)(0x61 + *(uint8_t*)src - 0x81);
          ++src;
          continue;  //***
        }
      } else if (ch == 0x83) {
        //"��������������������������������������������������������������� ��Ӭԭծ������ �  �ݳ"
        // 0000000000010101010101010101010101001010100000012012012012012000000000000000000000001
        // "�@"0x8340�`"��"0x8394 �܂�
        uint8_t ch2 = *(uint8_t*)src - 0x40;
        *dst++ = (char)fc83c[ch2];

        switch ((fc83cf[ch2 / 16] >> ((ch2 % 16) * 2)) & 3) {
        case 1:
          *dst++ = (char)SJIS_ANK_KANA_SP1;
          break;  //�
        case 2:
          *dst++ = (char)SJIS_ANK_KANA_SP2;
          break;  //�
        }

        ++src;
        continue;  //***
      }

      // �����ݒ�
      *dst++ = (char)ch;
      *dst++ = *src++;
    } else /*if (!sjis::IsKanji1(ch))*/ {
      *dst++ = (char)ch;
    }
  }

  *dst = '\0';
  return t_dst;
}

/*!
* �S�p�J�^�J�i��S�p�Ђ炪�Ȃɕϊ�����i�P�����j
* @param[in,out] str ����
* 
* @note
* �S�p�J�^�J�i�łȂ����͕̂ϊ�����܂���
*/
void sjis::KanaToHira1(char* str) {
  uint8_t ch1 = *(uint8_t*)str;
  uint8_t ch2 = *(uint8_t*)(str + 1);

  // "�@"0x8340�`"��"0x8393 �܂�
  // "��"0x829f�`"��"0x82f1 �܂�
  if (ch1 == 0x83 && 0x40 <= ch2 && ch2 <= 0x93) {
    uint8_t ch2_ = (uint8_t)(ch2 - 0x40);
    *(str + 0) = (char)0x82;
    *(str + 1) = (char)(0x9f + ch2_ -
                        ((0x7e < ch2) ? (uint8_t)1 : (uint8_t)0));  //�~�̂��ƂP�o�C�g�̋󔒂��i����
  } else {
    // �����ݒ�
    //		*(str + 0) = (char)ch1;
    //		*(str + 1) = (char)ch2;
  }
}

/*!
* �S�p�J�^�J�i��S�p�Ђ炪�Ȃɕϊ�����
* @param[out] dst �������ݐ�|�C���^
* @param[in] src �ǂݍ��݌��|�C���^
* @return �������ݐ�|�C���^
*/
char* sjis::KanaToHira(char* dst, const char* src) {
  char* t_dst = dst;

  while (*src) {
    uint8_t ch = *(uint8_t*)src++;

    if (sjis::IsKanji1(ch)) {
      *dst++ = (char)ch;
      *dst++ = *src++;

      sjis::KanaToHira1(dst - 2);
    } else /*if (!sjis::IsKanji1(ch))*/ {
      *dst++ = (char)ch;
    }
  }

  *dst = '\0';
  return t_dst;
}

/*!
* �S�p�Ђ炪�Ȃ�S�p�J�^�J�i�ɕϊ�����i�P�����j
* @param[in,out] str ����
* 
* @note
* �S�p�Ђ炪�ȂłȂ����͕̂ϊ�����܂���
*/
void sjis::HiraToKana1(char* str) {
  uint8_t ch1 = *(uint8_t*)str;
  uint8_t ch2 = *(uint8_t*)(str + 1);

  // "�@"0x8340�`"��"0x8393 �܂�
  // "��"0x829f�`"��"0x82f1 �܂�
  if (ch1 == 0x82 && 0x9f <= ch2 && ch2 <= 0xf1) {
    uint8_t ch2_ = (uint8_t)(ch2 - 0x9f);
    *(str + 0) = (char)0x83;
    *(str + 1) =
        (char)(0x40 + ch2_ +
               ((0xdd < ch2) ? (uint8_t)1 : (uint8_t)0));  //�u�~�v�̂��ƂP�o�C�g�̋󔒂��i����
  } else {
    //		*(str + 0) = (char)ch1;
    //		*(str + 1) = (char)ch2;
  }
}

/*!
* �S�p�Ђ炪�Ȃ�S�p�J�^�J�i�ɕϊ�����
* @param[out] dst �������ݐ�|�C���^
* @param[in] src �ǂݍ��݌��|�C���^
* @return �������ݐ�|�C���^
*/
char* sjis::HiraToKana(char* dst, const char* src) {
  char* t_dst = dst;

  while (*src) {
    uint8_t ch = *(uint8_t*)src++;

    if (sjis::IsKanji1(ch)) {
      *dst++ = (char)ch;
      *dst++ = *src++;

      sjis::HiraToKana1(dst - 2);
    } else /*if (!sjis::IsKanji1(ch))*/ {
      *dst++ = (char)ch;
    }
  }

  *dst = '\0';
  return t_dst;
}

/*!
* ������̕ϊ�
* @param[out] dst �ϊ���
* @param[in] src �ϊ���
* @param[in] flag �ϊ��t���O
* @return �ϊ���
*/
char* sjis::StrConv(char* dst, const char* src, int flag) {
  switch (flag) {
  case toHiragana:  // o ��������̃J�^�J�i���Ђ炪�Ȃɕϊ����܂��B���{�ꃍ�P�[���݂̂ɓK�p����܂��B���̃����o�́AVisual Basic �̒萔 vbHiragana �Ɠ����ł��B
    sjis::KanaToHira(dst, src);
    break;

  case toKatakana:  // o ��������̂Ђ炪�Ȃ��J�^�J�i�ɕϊ����܂��B���{�ꃍ�P�[���݂̂ɓK�p����܂��B���̃����o�́AVisual Basic �̒萔 vbKatakana �Ɠ����ł��B
    sjis::HiraToKana(dst, src);
    break;

  case toLinguisticCasing:  // x ������ɓK�p����啶���������̋K�����A�t�@�C�� �V�X�e���̋K�����猾��w�I�ȋK���ɕϊ����܂��B���̃����o�́AVisual Basic �̒萔 vbLinguisticCasing �Ɠ����ł��B
    break;

  case toLowercase:
  case toLowerCase:  // o ��������������ɕϊ����܂��B���̃����o�́AVisual Basic �̒萔 vbLowerCase �Ɠ����ł��B
    strcpy(dst, src);
    strlwr(dst);
    break;

  case toNarrow:  // o ��������̑S�p�����𔼊p�����ɕϊ����܂��B�A�W�A ���P�[���ɓK�p����܂��B���̃����o�́AVisual Basic �̒萔 vbNarrow �Ɠ����ł��B
    sjis::DbcsToAnk(dst, src);
    break;

  case toNone:  // o �ϊ������s���܂���B
    strcpy(dst, src);
    break;

  case toProperCase:  // x ��������̊e��̍ŏ��̕�����啶���ɕϊ����܂��B���̃����o�́AVisual Basic �̒萔 vbProperCase �Ɠ����ł��B
    break;

  case toSimplifiedChinese:  // x ������̔ɑ̎����ȑ̎��ɕϊ����܂��B���̃����o�́AVisual Basic �̒萔 vbSimplifiedChinese �Ɠ����ł��B
    break;

  case toTraditionalChinese:  // x ������̊ȑ̎���ɑ̎��ɕϊ����܂��B���̃����o�́AVisual Basic �̒萔 vbTraditionalChinese �Ɠ����ł��B
    break;

  case toUppercase:
  case toUpperCase:  // o �������啶���ɕϊ����܂��B���̃����o�́AVisual Basic �̒萔 vbUpperCase �Ɠ����ł��B
    strcpy(dst, src);
    strupr(dst);
    break;

  case toWide:  // o ��������̔��p (1 �o�C�g) ������S�p (2 �o�C�g) �����ɕϊ����܂��B�A�W�A ���P�[���ɓK�p����܂��B���̃����o�́AVisual Basic �̒萔 vbWide �Ɠ����ł��B
    sjis::AnkToDbcs(dst, src);
    break;
  }

  return dst;
}

/*!
* ��������(�a�l�@)
* @param[in] s ������
* @param[in] ptn �p�^�[��
* @return ��v�����̃A�h���X, 0:������Ȃ�
* 
* @note
* �P���������_�ŏI�����܂��B
*/
#if 0
char* bm_strstr(const char* s, const char* ptn)
{
	// ������ or �p�^�[��������Ƃ������[
/*	if (!s && !ptn) {
		return -1; // �������Ȃ���[
	}
*/
	
	int s_len	= strlen(s);
	int ptn_len	= strlen(ptn);
	
	// �e�L�X�g�ƃp�^�[���̕s��v�����������Ƃ���
	// �ǂꂾ�����炷���������\\_
	int skip[256];
	
	// �ϐ� i �͒��ڂ��Ă���e�L�X�g�̈ʒu�A
	// �ϐ� j �͒��ڂ��Ă���p�^�[���̈ʒu��\���|�C���^
	int	i, j;
	
	// �e�L�X�g���p�^�[���̂ق��������̂Ō�����Ȃ��ł�
	if (s_len < ptn_len) {
		return 0;
	}
	
	// �\ skip ���쐬����
	for (i = 0; i < 256; ++i) {
		skip[i] = ptn_len;
	}
	for (i = 0; i < ptn_len - 1; ++i) {
		skip[(uint8_t)ptn[i]] = ptn_len - i - 1;
	}
	
	// �|�C���^������������B�p�^�[���̌�납��O�Ɍ������Ĕ�r����̂ŁA
	// �p�^�[���̒��� -1 �ɏ���������B
	i = ptn_len - 1;
	
	// �e�L�X�g�̍Ō���ɍs��������܂ŌJ��Ԃ�
	while (i < s_len) {
		// �|�C���^ j ���p�^�[���̍Ō�̕������w���悤�ɂ���
		j = ptn_len - 1;
		
		// �e�L�X�g�ƃp�^�[������v����ԁA�J��Ԃ�
		while (s[i] == ptn[j]) {
			// �ŏ��̕����܂ň�v������A�T���͐����ł���
			if (j == 0) {
				return s + i;
			}
			
			// �|�C���^ i �� j �����ꂼ��P�������߂�
			i--;
			j--;
		}
		
		// ��v���Ȃ������̂ŁA�p�^�[�������炷
		if (skip[(uint8_t)s[i]] > (ptn_len - j)) {
			i += skip[(uint8_t)s[i]];
		}
		else {
			i += ptn_len - j;
		}
	}
	
	
	// ���ǌ�����Ȃ�����
	return 0;
}
#endif

/*!
* ��������
* @param[in] s ������
* @param[in] ptn �p�^�[��
* @param[in] ptnlen �p�^�[���̒���
* @return ��v�����̃A�h���X, 0:������Ȃ�
*/
char* sjis::SearchNString(const char* s, const char* ptn, int ptnlen) {
  int ch1 = *ptn;                   // �ŏ��̕���
  int ch2 = *(ptn + (ptnlen - 1));  // �Ō�̕���

  int i;

  // s �̒����`�F�b�N
  for (i = 0; i < ptnlen; ++i) {
    if (s[i] == '\0') {
      return 0;  // s �� ptn �����Z��
    }
  }

  s = s + (ptnlen - 1);  // �ʒu�� ptn ���i�߂�

  for (;;) {
    // �Ō�ƍŏ�����v
    if (*s == ch2 && *(s - (ptnlen - 1)) == ch1) {
      // ���ŏ��ƍŌ�͔���ς�
      if (strncmp(s - (ptnlen - 1) + 1, ptn + 1, ptnlen - 2) == 0) {
        return (char*)(s - (ptnlen - 1));
      }
    }
    // null �������̂ŏI��
    else if (*s == '\0') {
      return 0;
    }

    ++s;
  }

  //return 0;
}

/*!
* ��������
* @param[in] s ������
* @param[in] ptn �p�^�[��
* @return ��v�����̃A�h���X, 0:������Ȃ�
*/
char* sjis::SearchString(const char* s, const char* ptn) {
#if 1  //def NCRT
  return SearchNString(s, ptn, strlen(ptn));
#else
  return (char*)::strstr(s, ptn);
#endif
}

/*!
* ���������i�啶�����������ꎋ�j
* @param[in] s ������
* @param[in] ptn �p�^�[��
* @param[in] ptnlen �p�^�[���̒���
* @return ��v�����̃A�h���X, 0:������Ȃ�
*/
char* sjis::SearchNCaseString(const char* s, const char* ptn, int ptnlen) {
  int ch1 = tolower(*ptn);                   // �ŏ��̕���
  int ch2 = tolower(*(ptn + (ptnlen - 1)));  // �Ō�̕���

  // s �̒����`�F�b�N
  for (int i = 0; i < ptnlen; ++i) {
    if (s[i] == '\0') {
      return 0;  // s �� ptn �����Z��
    }
  }

  s = s + (ptnlen - 1);  // �ʒu�� ptn ���i�߂�

  for (;;) {
    // �Ō�ƍŏ�����v
    if (tolower(*s) == ch2 && tolower(*(s - (ptnlen - 1))) == ch1) {
      // ���ŏ��ƍŌ�͔���ς�
      if (strnicmp(s - (ptnlen - 1) + 1, ptn + 1, ptnlen - 2) == 0) {
        return (char*)(s - (ptnlen - 1));
      }
    }
    // null �������̂ŏI��
    else if (*s == '\0') {
      return 0;
    }

    ++s;
  }

  //return 0;
}

/*!
* ���������i�啶�����������ꎋ�j
* @param[in] s ������
* @param[in] ptn �p�^�[��
* @return ��v�����̃A�h���X, 0:������Ȃ�
*/
char* sjis::SearchCaseString(const char* s, const char* ptn) {
  return sjis::SearchNCaseString(s, ptn, strlen(ptn));
}

/*!
* ��������
* @param[in] s ������
* @param[in] ptn �p�^�[��
* @param[in] ptnlen �p�^�[���̒���
* @return ��v�����̃A�h���X, 0:������Ȃ�
*/
char* sjis::LastSearchNString(const char* s, const char* ptn, int ptnlen) {
  int ch1 = *ptn;                   // �ŏ��̕���
  int ch2 = *(ptn + (ptnlen - 1));  // �Ō�̕���

  const char* sbak = s;
  int slen;

  // s �̒����`�F�b�N
  for (int i = 0; i < ptnlen; ++i) {
    if (s[i] == '\0') {
      return 0;  // s �� ptn �����Z��
    }
  }

  slen = strlen(s);
  s = s + slen - (ptnlen - 1);

  for (;;) {
    // �Ō�ƍŏ�����v
    if (*s == ch2 && *(s - (ptnlen - 1)) == ch1) {
      // ���ŏ��ƍŌ�͔���ς�
      if (strncmp(s - (ptnlen - 1) + 1, ptn + 1, ptnlen - 2) == 0) {
        return (char*)(s - (ptnlen - 1));
      }
    }

    // �ŏ��܂ŗ����̂ŏI��
    if (s == sbak) {
      return 0;
    }

    --s;
  }

  //return 0;
}

/*!
* ��������
* @param[in] s ������
* @param[in] ptn �p�^�[��
* @return ��v�����̃A�h���X, 0:������Ȃ�
*/
char* sjis::LastSearchString(const char* s, const char* ptn) {
  return LastSearchNString(s, ptn, strlen(ptn));
}

/*!
* ������̒u���i�o�b�t�@�̐�[���� fromlen �o�C�g�� to �ɒu���j
* @param[in,out] s ���o�͕�����
* @param[in] fromlen �u���Ώە�����̒���
* @param[in] to �u��������
* @param[in] tolen �u���㕶����̒���
* @return �u����̕�����̏I�[
* 
* @note
* �E�e�L�X�g�o�b�t�@�̐�[���� fromlen �̕����������� to �� tolen ���̕�����ɒu�������� <br>
* �Eto �� tolen ���w�肷��K�v�����闝�R�͓��������Œu�����s���ꍇ�� to �̒����𖈉񒲂ׂ鎞�Ԃ��Ȃ���悤�ɂ��邽�� <br>
* �E�u���悪�u���������傫���ꍇ�͒u���������Ƃ̕������̃o�b�t�@�����炩���ߊm�ۂ��Ă����K�v������܂� <br>
*/
char* sjis::ReplaceStringStart(char* s, int fromlen, const char* to, int tolen) {
  int i, j;

  // ������̏I�[���I�[�o�[�����ꍇ�̑Ώ�
  // "abcdefg" �� "efg" �� "hijklmn" �Ȃ�
  // ����K�v���ȁH�i��
  if (s[fromlen] == '\0') {
    s[tolen] = '\0';
  }

  // �u���悪�u���������傫���ꍇ null �ɂȂ�܂� (tolen - fromlen) �������ɂ��炷
  //   [before] abc0123456789
  //   [after]  abcdefg0123456789
  //            (abc -> abcdefg, diff 4)
  // diff �����o�b�t�@�Ƃ��Ďg�����@�ł�
  if (fromlen < tolen) {
    int diff = tolen - fromlen;  // ����

    for (i = fromlen; /*s[i]*/; i += diff) {
      int a = i + diff;  // ����ւ���
      int b = fromlen;   // ����ւ���

      for (j = 0; j < diff; ++j) {
        char ch = s[b + j];  // ����ւ��`
        s[b + j] = s[a + j];
        s[a + j] = ch;

        // null �܂Œu��������I���
        if (ch == '\0') {
          goto _l_next;
        }
      }
    }
  }
  // �u���悪�u���������������ꍇ null �ɂȂ�܂� (tolen - fromlen) �����O�ɂ��炷
  //   [before] abcdefg0123456789
  //   [after]  abc0123456789
  //            (abcdefg -> abc, diff 4)
  else if (fromlen > tolen) {
    int diff = fromlen - tolen;  // ����

    for (i = tolen; s[i + diff]; ++i) {
      s[i] = s[i + diff];
    }
    s[i] = '\0';  // null
  }

_l_next:
  // �u�����̈ʒu�ɒu���敶�����R�s�[
  for (i = 0; i < tolen; ++i) {
    s[i] = to[i];
  }

  // �u�����̈ʒu�{�u���敶����̈ʒu��Ԃ�
  return s + tolen;
}

/*!
* ������̒u���i�o�b�t�@�̒��̍ŏ��� from ��u���j
* @param[in,out] s ���o�͕�����
* @param[in] from �u����
* @param[in] to �u����
* @return �u����̕�����̏I�[, 0:�u��������̂��Ȃ�����1
* 
* @note
* �u���悪�u���������傫���ꍇ�͒u���������Ƃ̕������̃o�b�t�@�����炩���ߊm�ۂ��Ă����K�v������܂� <br>
* 
* @code
* char* p = s;
* sjis::ReplaceString( p, from, to );
* while ( p ) { p = sjis::ReplaceString( p, from, to ); }
* @endcode
*/
char* sjis::ReplaceString(char* s, const char* from, const char* to) {
  int fromlen = strlen(from);
  char* pos = sjis::SearchNString(s, from, fromlen);  // form �����邩�H

  // �����炸
  if (pos == 0) {
    return 0;
  }

  // �u��
  //	return sjis::ReplaceStringStart(s + (pos - s), fromlen, to, strlen(to));
  return sjis::ReplaceStringStart(pos, fromlen, to, strlen(to));
}

/*!
* ������̒u��
* @param[in] s ������
* @param[in] from �u����
* @param[in] to �u����
*/
void sjis::ReplaceStringAll(char* s, const char* from, const char* to) {
#if 1  // �����ł�����
  int fromlen = strlen(from);
  int tolen = strlen(to);

  do {
    s = sjis::SearchNString(s, from, fromlen);  // form �����邩�H

    if (s) {
      s = sjis::ReplaceStringStart(s, fromlen, to, tolen);
    }
  } while (s);
#else
  while (s) {
    s = sjis::ReplaceString(s, from, to);
  }
#endif
}

/*!
* ������̐؂蔲��
* @param[out] dst �o�͐�
* @param[in] src ���͌�
* @param[in] begin �؂蔲���J�n������("" �ł͂��߂���)
* @param[in] end �؂蔲���I��������("" �ōŌ�܂�)
* @return �o�͐�, 0:�؂蔲�����s
* 
* @note
* �^�u�A�X�y�[�X�Ȃǂ�ǉ��������Ȃ��ꍇ�͂��̊֐��̂��Ƃ�
* sjis::RemoveString( str, "\t " );
* ���Ăяo���܂��B
* 
* @code
* char* s = "abcdefg[hijklmn]opqrstu";
* char buff[16];
* sjis::ExtractPairString( buff, s, "[", "]", IGNORE_TABSPACE );
* 
* //***result***
* //hijklmn
* @endcode
*/
char* sjis::ExtractString(char* dst, const char* src, const char* begin, const char* end) {
  int beginlen = strlen(begin);
  int endlen = strlen(end);

  char* t_dst = dst;

  // �X�^�[�g�ʒu����
  if (*begin) {
    char* pos = sjis::SearchString(src, begin);

    if (pos == 0) {
      return 0;  // not find!!
    }

    // �ʒu��i�߂�
    src = pos + beginlen;
  }

  // �؂�o���I���ʒu�܂�
  if (*end) {
    // begin �` end
    while (*src) {
      if (!sjis::IsKanji1(*src)) {
        // �؂�o���I�������񌟍�
        if (strncmp(src, end, endlen) == 0) {
          break;  // �I��
        }

        *dst++ = *src++;
      } else {
        *dst++ = *src++;
        *dst++ = *src++;
      }
    }
  }
  // �Ō�܂�
  else {
    while (*src) {
      if (!sjis::IsKanji1(*src)) {
        *dst++ = *src++;
      } else {
        *dst++ = *src++;
        *dst++ = *src++;
      }
    }
  }

  // �Ō�� null
  *dst = '\0';

  return t_dst;
}

/*!
* ������̐؂蔲���i�y�A�Ȃ̂ŊJ�n�ƏI��������͕K�{�j
* @param[out] dst �o�͐�
* @param[in] src ���͌�
* @param[in] begin �؂蔲���J�n������
* @param[in] end �؂蔲���I��������
* @return �o�͐�, 0:�؂蔲�����s
* 
* @code
* char* s = "abcdefg[hij[k]lmn]opqrstu";
* char buff[16];
* sjis::ExtractPairString( buff, s, "[", "]", IGNORE_TABSPACE );
* 
* //***result***
* //hijklmn
* @endcode
*/
char* sjis::ExtractPairString(char* dst, const char* src, const char* begin, const char* end) {
  int pair_cnt = 0;

  int beginlen = strlen(begin);
  int endlen = strlen(end);

  char* t_dst = dst;

  // �X�^�[�g�ʒu����
  char* pos = sjis::SearchString(src, begin);

  if (pos == 0) {
    return 0;  // not find!!
  }

  // �ʒu��i�߂�
  src = pos + beginlen;

  // begin �̕����������������ꍇ end �̕�������������������܂Ő؂�o��
  while (*src) {
    if (!sjis::IsKanji1(*src)) {
      // �؂�o���I�������񌟍�
      if (strncmp(src, end, endlen) == 0) {
        if (pair_cnt == 0) {
          break;  // �I��
        }

        pair_cnt--;  // pair_cnt �̐������� end �����Ă�������
      }
      // ������ǉ��m�F
      else {
        // begin �Ɠ������������̂Ō��������𑝂₷
        if (strncmp(src, begin, beginlen) == 0) {
          ++pair_cnt;
        }
      }

      *dst++ = *src++;
    } else {
      *dst++ = *src++;
      *dst++ = *src++;
    }
  }

  // �Ō�� null
  *dst = '\0';

  return t_dst;
}

/*!
* ���������؂蕶���ŕ�������
* @param[out] dst �o�̓o�b�t�@
* @param[in] src ���̓o�b�t�@
* @param[in] delimiters �f���~�^���Q
* @param[in] ignores �������Q
* @return ���̓��̓o�b�t�@�J�n�ʒu, 0:�I��
*/
const char* sjis::ExtractTokenStart(char* dst, const char* src, const char* delimiters,
                                    const char* ignores) {
  // ��������
  // ���̕������Ȃ��Ȃ�܂�...
  for (;;) {
    char ch = *src++;

    // nil
    if (ch == '\0') {
      *dst = '\0';  // �I�[
      return 0;
    }
    // �P�o�C�g����
    else if (!sjis::IsKanji1(ch)) {
      // �������Q���猩������
      if (strchr(ignores, ch)) {
        continue;
      }
      // �f���~�^���Q���猩������
      else if (strchr(delimiters, ch)) {
        *dst = '\0';  // �I�[
        return src;
      }
    }
    // �Q�o�C�g����
    else {
      *dst++ = ch;
      ch = *src++;
    }

    *dst++ = ch;
  }

  //	return 0;
}

/*!
* ���������؂蕶���ŕ�������
* @param[out] dst �o�̓o�b�t�@
* @param[in] src ���̓o�b�t�@
* @param[in] delimiters �f���~�^���Q
* @param[in] ignores �������Q
* @return �g�[�N���̐�
* 
* @code
* char* s = "abc,def,,g";
* sjis::ExtractToken( s, s, ",", "" );
* 
* //***result***
* //"abc"
* //"def"
* //""
* //"g"
* @endcode
*/
int sjis::ExtractToken(char* dst, const char* src, const char* delimiters, const char* ignores) {
  int len;
  int tok_cnt = 0;

  // ���͂��o�͂ɂ���
  if (!dst) {
    dst = (char*)src;  // illegal
  }

  // ��̃g�[�N�����ǉ�����
  for (;;) {
    src = sjis::ExtractTokenStart(dst, src, delimiters, ignores);
    len = strlen(dst);  // �g�[�N���̒���

    dst = dst + len + 1;  // �o�̓o�b�t�@�����̈ʒu�֐i�߂�
    ++tok_cnt;            // �g�[�N���J�E���^�𑝂₷

    // �I���
    if (src == 0) {
      break;
    }
  }

  return tok_cnt;
}

/*!
* ���������؂蕶���ŕ�������i��̃g�[�N���͊܂߂Ȃ��j
* @param[out] dst �o�̓o�b�t�@
* @param[in] src ���̓o�b�t�@
* @param[in] delimiters �f���~�^���Q
* @param[in] ignores �������Q
* @return �g�[�N���̐�
*/
int sjis::ExtractTokenWithoutEmpty(char* dst, const char* src, const char* delimiters,
                                   const char* ignores) {
  int len;
  int tok_cnt = 0;

  // ���͂��o�͂ɂ���
  if (!dst) {
    dst = (char*)src;  // illegal
  }

  // ��̃g�[�N���͖�������
  for (;;) {
    src = sjis::ExtractTokenStart(dst, src, delimiters, ignores);
    len = strlen(dst);  // �g�[�N���̒���

    // ��̃g�[�N���͖�������
    if (len == 0) {
      //continue;
    } else {
      dst = dst + len + 1;  // �o�̓o�b�t�@�����̈ʒu�֐i�߂�
      ++tok_cnt;            // �g�[�N���J�E���^�𑝂₷
    }

    // �I���
    if (src == 0) {
      break;
    }
  }

  return tok_cnt;
}

/*!
* \0 �ŋ�؂�ꂽ�g�[�N�������񂩂� n �Ԗڂ̃g�[�N���̃|�C���^���擾����
* @param[in] p ���͌�
* @param[in] n �擾�ʒu
* @return �g�[�N���̈ʒu
*/
char* sjis::TokenOf(char* p, int n) {
  char* t_p = p;

  for (;;) {
    if (*p++ == '\0') {
      if (--n < 0) {
        return t_p;
      }

      t_p = p;
    }
  }

  //	return 0;
}

/*!
* �}�X�N�̃`�F�b�N
* @param[in] s ������
* @param[in] mask �}�X�N
* @return 0:�s��v, 1:��v
* 
* @note
* �C�ӂ� 1 ����			?		�C�ӂ� 1 �����Ɉ�v���܂� <br>
* 1 �܂��͕����̕���	*		�C�ӂ� 1 �܂��͕����̕������������܂��B���Ƃ��΁Anew* �ł́A"new" ���܂ޔC�ӂ̕����� ("newfile.txt" �Ȃ�) ����������܂� <br>
*/
int sjis::CheckMask(const char* s, const char* mask) {
  for (;;) {
    // �I���`�F�b�N
    if (*s == '\0' && *mask == '\0') {
      return 1;
    } else if (*s == '\0' && *mask != '\0') {
      return 0;
    }

    // mask is 1-byte
    if (!sjis::IsKanji1(*mask)) {
      // * ����̒P���T��
      if (*mask == '*') {
        const char* start = mask + 1;
        const char* end;

        //�P��擾
        ++mask;

        if (*mask == '\0') {
          //*�ŏI���Ȃ疳����de
          return 1;
        }

        for (;;) {
          if (*mask == '\0' || *mask == '*' || *mask == '?') {
            end = mask;
            break;
          } else if (sjis::IsKanji1(*mask)) {
            ++mask;
            ++mask;
          } else {
            ++mask;
          }
        }

        //������ɒP�ꂪ�܂܂�Ă���ӏ���T��
        if (end - start > 0) {
          const char* find;

          if (*s == '\0') {
            return 0;
          }

          find = sjis::SearchNCaseString(s, start, end - start);

          if (find) {
            //���������Ƃ���܂Ői�߂�
            s = find + (end - start);
          } else {
            //�݂���Ȃ�
            return 0;
          }
        }
      }
      // �H�͂P�����p�X����
      else if (*mask == '?') {
        ++mask;

        if (!sjis::IsKanji1(*s)) {
          ++s;
        } else {
          ++s;
          ++s;
        }
      } else {
        if (!sjis::IsKanji1(*s)) {
          // �������Ŕ�r
          if (tolower(*mask) != tolower(*s)) {
            return 0;
          }
        } else {
          return 0;
        }

        ++mask;
        ++s;
      }
    }
    // mask is 2-byte
    else {
      if (!sjis::IsKanji1(*s)) {
        return 0;
      } else {
        short chch1 = (*mask << 8) | *(mask + 1);
        short chch2 = (*s << 8) | *(s + 1);

        //�������Ⴄ
        if (chch1 != chch2) {
          return 0;
        }
      }

      ++mask;
      ++mask;

      ++s;
      ++s;
    }
  }

  // �����ɂ͂��Ȃ��͂�
  //	return 0;
}

/*!
* �}�X�N�̃`�F�b�N(�����Ή���)
* @param[in] s ������
* @param[in] masks �}�X�N
* @return 0:�s��v, 1:��v
*/
int sjis::CheckMasks(const char* s, const char* masks) {
  char t_mask[256];

  for (;;) {
    int len;

    masks = sjis::ExtractTokenStart(t_mask, masks, " ,;", "");
    len = strlen(t_mask);

    // ��̃g�[�N���͖�������
    if (len == 0) {
      //continue;
    } else {
      // �}�X�N�̃`�F�b�N
      if (sjis::CheckMask(s, t_mask)) {
        return 1;
      }
    }

    // �I���
    if (masks == 0) {
      break;
    }
  }

  return 0;
}

/*!
* �t�@�C���}�X�N�̃`�F�b�N
* @param[in] fileName �t�@�C����
* @param[in] masks �}�X�N
* @return 0:�s��v, 1:��v
*/
int sjis::CheckFileMasks(const char* fileName, const char* masks) {
  const char* pt = 0;

  {
    const char* s = fileName;

    while (*s) {
      if (sjis::IsKanji1(*s)) {
        s += 2;
      } else /*if (!sjis::IsKanji1(*s))*/ {
        if (*s == 0x5c) {
          pt = s;
        }

        ++s;
      }
    }
  }

  if (pt == 0) {
    pt = strrchr(fileName, '/');
  }

  if (pt == 0) {
    pt = fileName;
  }

  return sjis::CheckMasks(pt, masks);
}

}  //mx

/**
�Q�l http://www.tohoho-web.com/wwwkanji.htm
�� �e�R�[�h�Ԃ̕ϊ��A���S���Y��c1����o�C�g�Ac2����o�C�g�Ƃ��܂��B
   EUC��SJIS��(1)��(3)���ASJIS��EUC��(4)��(2)��g�ݍ��킹�Ă��������B

(1) EUC ���� JIS �ɕϊ�

c1 = c1 - 0x80;
c2 = c2 - 0x80;

(2) JIS ���� EUC �ɕϊ�

c1 = c1 + 0x80;
c2 = c2 + 0x80;

(3) JIS ���� �V�t�gJIS �ɕϊ�

if (c1 % 2) {
    c1 = ((c1 + 1) / 2) + 0x70;
    c2 = c2 + 0x1f;
} else {
    c1 = (c1 / 2) + 0x70;
    c2 = c2 + 0x7d;
}
if (c1 >= 0xa0) { c1 = c1 + 0x40; }
if (c2 >= 0x7f) { c2 = c2 + 1; }

(4) �V�t�gJIS ���� JIS �ɕϊ�

if (c1 >= 0xe0) { c1 = c1 - 0x40; }
if (c2 >= 0x80) { c2 = c2 - 1; }
if (c2 >= 0x9e) {
    c1 = (c1 - 0x70) * 2;
    c2 = c2 - 0x7d;
} else {
    c1 = ((c1 - 0x70) * 2) - 1;
    c2 = c2 - 0x1f;
}
*/
