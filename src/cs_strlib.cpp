//-----------------------------------------------------------------------------
/*!
* @file		mx_strlib.cpp
* @brief	�����񏈗�
* @author	koma
* @version	$Id$
* 
* @note
* �E�X�V����  
*/
//-----------------------------------------------------------------------------

// include
#include <ctype.h>
#include <string.h>
#include <stdio.h>
//#include <extension.h>

#include "mx_strlib.h"

#define NCRT  // No C Runtime

namespace mx {

/*!
	������̒������擾
	@param[in] s ������
	@return ����
*/
int strlen(const char* s) {
#ifdef NCRT
  const char* top = s;
  while (*s++ != '\0') {
  }  // null �����܂Ői�߂�
  return s - top - 1;
#else
  return ::strlen(s);
#endif
}

/*!
	��������R�s�[
	@param[out] dst �o�͐�
	@param[in] src ���͌�
	@return �o�͐�
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
	�������n�R�s�[
	@param[out] dst �o�͐�
	@param[in] drc ���͌�
	@param[in] size ����
	@return �o�͐�
	
	@note
	size �����������ɃR�s�[���܂��B�r���� null(\0) �����ɂȂ��Ă������܂� <br>
	�{���̓���� null �܂łȂ�ł����G <br>
	dst �Ɏ��܂肫��Ȃ� size ���w�肷��ق����ǂ����Ǝv����H <br>
*/
char* strncpy(char* dst, const char* src, int size) {
#ifdef NCRT
  // �Ō�� null �������Ƃ�
  dst[size] = '\0';

  // long(4byte) �P�ʂŏ���
  while (4 <= size) {
    size -= 4;
    *(long*)(dst + size) = *(long*)(src + size);
  }

  // �c�肪����� 1 �o�C�g�P�ʂŏ���
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
	��������啶���ɕϊ�����(a->A)
	@param[in] ch ����
	@return ����
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
	�啶�����������ɕϊ�����(A->a)
	@param[in] ch ����
	@return ����
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
	��������啶���ɕϊ�����(az->AZ)
	@param[in,out] s ������
	@return ������
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
	�啶�����������ɕϊ�����(AZ->az)
	@param[in,out] s ������
	@return ������
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
	�Q�̕������n��r����
	@param[in] s1 ������P
	@param[in] s2 ������Q
	@param[in] len ����
	@return 0:s1 = s2, -1:s1 < s2, 1:s1 > s2
*/
int strncmp(const char* s1, const char* s2, int len) {
#ifdef NCRT
  int ret = 0;

  // ���ʂ��Ⴄ�� len �� 0 �ɂȂ�܂�
  while (0 < len) {
    ret = *s1 - *s2;

    if (ret) {
      break;
    }

    // ��r�T�C�Y�����炷
    --len;

    // ������̃|�C���^��i�߂�
    ++s1;
    ++s2;
  }

  return ret;  // diff ok
#else
  return ::strncmp(s1, s2, len);
#endif
}

/*!
	�Q�̕�������r����
	@param[in] s1 ������P
	@param[in] s2 ������Q
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
	�Q�̕������n�������Ƃ��Ĕ�r����
	@param[in] s1 ������P
	@param[in] s2 ������Q
	@param[in] len ����
	@return 0:s1 = s2, -1:s1 < s2, 1:s1 > s2
*/
int strnicmp(const char* s1, const char* s2, int len) {
// CRT���g�p�����ꍇ�A���܂����삵�Ȃ�
// ������ 05/13/2011 12:47 PM

#if 1  //def NCRT
  int ret = 0;

  // ���ʂ��Ⴄ�� len �� 0 �ɂȂ�܂�
  while (0 < len) {
    ret = tolower(*s1) - tolower(*s2);

    if (ret) {
      break;
    }

    // ��r�T�C�Y�����炷
    --len;

    // ������̃|�C���^��i�߂�
    ++s1;
    ++s2;
  }

  return ret;  // diff ok
#else
  return ::strnicmp(s1, s2, len);
#endif
}

/*!
	�Q�̕�������������Ƃ��Ĕ�r����
	@param[in] s1 ������P
	@param[in] s2 ������Q
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
	��������X�L�������āA�w�肳�ꂽ�������ŏ��ɏo������ʒu�������܂�
	@param[in] s ������
	@param[in] ch �������镶��
	@return ���������ʒu, 0:������Ȃ�����
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
	��������X�L�������āA�w�肳�ꂽ�������Ō�ɏo������ʒu�������܂�
	@param[in] s ������
	@param[in] ch �������镶��
	@return ���������ʒu, 0:������Ȃ�����
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
	��������X�L�������āA�w�肳�ꂽ�����Q���ŏ��ɏo������ʒu�������܂�
	@param[in] s1 ������P
	@param[in] s2 ������Q
	@return ���������ʒu�܂ł̒����B������Ȃ��ꍇ�� s1 �̒���
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
	��������X�L�������āA�w�肳�ꂽ�����Q�ȊO���ŏ��ɏo������ʒu�������܂�
	@param[in] s1 ������P
	@param[in] s2 ������Q
	@return ���������ʒu�܂ł̒����B������Ȃ��ꍇ�� s1 �̒���
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
	��������q����(s1 �� s2 ���q����)
	@param[in,out] s1 ������P
	@param[in] s2 ������Q
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
	��������q����(s1 �� s2 �� len �q����)
	@param[in,out] s1 ������P
	@param[in] s2 ������Q
	@param[in] len ����
	@return char*
*/
char* strncat(char* s1, const char* s2, int len) {
#ifdef NCRT
  return strncpy(s1 + strlen(s1), s2, len);
#else
  return ::strncat(s1, s2, len);
#endif
}

}  //mx
