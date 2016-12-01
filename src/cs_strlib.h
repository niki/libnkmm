//-----------------------------------------------------------------------------
/*!
* @file		mx_strlib.h
* @brief	�����񏈗�
* @author	koma
* @version	$Id$
* 
* @note
* �E�X�V����  
*/
//-----------------------------------------------------------------------------

#ifndef MX_STRLIB_H
#define MX_STRLIB_H

// ����
#define IGNORE_TABSPACE (1 << 30)  // �^�u�X�y�[�X�𖳎�����

// ���p�p�����L��
#define ANK_ALPHA_BEGIN (0x20)
#define ANK_ALPHA_END (0x7e)

// VbStrConv �񋓑� ���Q�l�� StrConv ������
enum {
  toHiragana =
      0,               // o ��������̃J�^�J�i���Ђ炪�Ȃɕϊ����܂��B���{�ꃍ�P�[���݂̂ɓK�p����܂��B���̃����o�́AVisual Basic �̒萔 vbHiragana �Ɠ����ł��B
  toKatakana,          // o ��������̂Ђ炪�Ȃ��J�^�J�i�ɕϊ����܂��B���{�ꃍ�P�[���݂̂ɓK�p����܂��B���̃����o�́AVisual Basic �̒萔 vbKatakana �Ɠ����ł��B
  toLinguisticCasing,  // x ������ɓK�p����啶���������̋K�����A�t�@�C�� �V�X�e���̋K�����猾��w�I�ȋK���ɕϊ����܂��B���̃����o�́AVisual Basic �̒萔 vbLinguisticCasing �Ɠ����ł��B
  toLowercase,
  toLowerCase,  // o ��������������ɕϊ����܂��B���̃����o�́AVisual Basic �̒萔 vbLowerCase �Ɠ����ł��B
  toNarrow,              // o ��������̑S�p�����𔼊p�����ɕϊ����܂��B�A�W�A ���P�[���ɓK�p����܂��B���̃����o�́AVisual Basic �̒萔 vbNarrow �Ɠ����ł��B
  toNone,                // o �ϊ������s���܂���B
  toProperCase,          // x ��������̊e��̍ŏ��̕�����啶���ɕϊ����܂��B���̃����o�́AVisual Basic �̒萔 vbProperCase �Ɠ����ł��B
  toSimplifiedChinese,   // x ������̔ɑ̎����ȑ̎��ɕϊ����܂��B���̃����o�́AVisual Basic �̒萔 vbSimplifiedChinese �Ɠ����ł��B
  toTraditionalChinese,  // x ������̊ȑ̎���ɑ̎��ɕϊ����܂��B���̃����o�́AVisual Basic �̒萔 vbTraditionalChinese �Ɠ����ł��B
  toUppercase,
  toUpperCase,  // o �������啶���ɕϊ����܂��B���̃����o�́AVisual Basic �̒萔 vbUpperCase �Ɠ����ł��B
  toWide,  // o ��������̔��p (1 �o�C�g) ������S�p (2 �o�C�g) �����ɕϊ����܂��B�A�W�A ���P�[���ɓK�p����܂��B���̃����o�́AVisual Basic �̒萔 vbWide �Ɠ����ł��B
};

namespace mx {

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

}  //mx

#include "mx_strlib_sjis.h"
//#include "strlib_unicode.h"

#endif  //MX_STRLIB_H
