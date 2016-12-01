//-----------------------------------------------------------------------------
/*!
* @file		mx_utils.cpp
* @brief	
* @author	koma
* @version	$Id$
* 
* @note
* �E�X�V����  
*/
//-----------------------------------------------------------------------------
// Include
#include <stdio.h>   // vsprintf
#include <stdarg.h>  // va_list
#include "mx_memory.h"
#include "mx_basis.h"
#include "mx_strlib.h"  // �����񏈗�
#include "mx_math.h"    // ���l����
#include "mx_string_list.h"
#include "mx_utils.h"

#define MAX_LENGTH 0x2000

namespace mx {
namespace utils {

int AutoCompleteString(const StringList* stringList, const mx::string& s1) {
  for (int i = 0; i < stringList->count(); i++) {
    const mx::string& s2 = stringList->Strings[i];

    if (mx::strnicmp(s2.c_str(), s1.c_str(), s1.length()) == 0) {
      return i;
    }
  }

  return -1;
}

void ExtractTokenToList(StringList* stringList, const mx::string& src, const mx::string& delimiters,
                        const mx::string& ignores) {
  int srclen = src.length();
  char* buff = SAFE_NEW_ARRAY(__SOURCE__, char,
                              srclen);  // �o�b�t�@�m��(�Ƃ肠���� src �̒���������Εۏ؂����͂�)

  // ���X�g�̃N���A
  stringList->clear();

  const char* p = src.c_str();

  for (;;) {
    p = mx::sjis::ExtractTokenStart(buff, p, delimiters, ignores);

    // ���X�g�ɒǉ�
    stringList->basic_Add(buff);

    // �I���
    if (src == 0) {
      break;
    }
  }

  //SAFE_DELETE_ARRAY(buff);
  delete[] buff;
}

struct CommentToSpaceDescription {
  int commentF;  //�R�����g�t���O
  int stringF;   //������t���O
  bool ignoreString;

  //�s�R�����g
  struct {
    int len;
    const char* ptr;
  } line;

  //�u���b�N�R�����g�J�n
  struct {
    int len;
    const char* ptr;
  } blockBegin;

  //�u���b�N�R�����g�I��
  struct {
    int len;
    const char* ptr;
  } blockEnd;
};

// �R�����g���X�y�[�X�ɕϊ�
static void local_CommentToSpace(char* wp, CommentToSpaceDescription* comm) {
  const int lineLen = comm->line.len;
  const int beginLen = comm->blockBegin.len;
  const int endLen = comm->blockEnd.len;

  const char* pLine = comm->line.ptr;
  const char* pBegin = comm->blockBegin.ptr;
  const char* pEnd = comm->blockEnd.ptr;

  while (*wp) {
    // �R�����g�̂Ƃ��̏���
    if (comm->commentF && comm->stringF == 0) {
      // ���{��`�F�b�N
      if (mx::sjis::IsKanji1(*wp)) {
        *wp++ = ' ';  // to space
        *wp++ = ' ';  // to space

        continue;  // to 'while (*wp)'
      }

      if (0 < beginLen) {
        // �u���b�N�R�����g�̊J�n�`�F�b�N
        if (mx::strncmp(wp, pBegin, beginLen) == 0) {
          // �u���b�N�R�����g��
          mx::Memory::Fill(wp, ' ', beginLen);
          wp += beginLen;

          // �R�����g�t���O ON
          comm->commentF++;

          continue;  // to 'while (*wp)'
        }
        // �u���b�N�R�����g�̏I���`�F�b�N
        else if (mx::strncmp(wp, pEnd, endLen) == 0) {
          // �u���b�N�R�����g��
          mx::Memory::Fill(wp, ' ', endLen);
          wp += endLen;

          // �R�����g�t���O OFF
          comm->commentF--;

          continue;  // to 'while (*wp)'
        }
      }

      // �R�����g�Ȃ̂Œu��������(�^�u�͂��̂܂�)
      if (*wp != '\t') {
        *wp = ' ';  // to space
      }
    } else {
      // ���{��`�F�b�N
      if (mx::sjis::IsKanji1(*wp)) {
        // ������|�C���^��i�߂�
        wp += 2;

        continue;  // to 'while (*wp)'
      }

      // ��������łȂ��Ƃ��̏���
      if (comm->stringF == 0) {
        // �u���b�N�R�����g�̊J�n�`�F�b�N
        if (0 < beginLen && mx::strncmp(wp, pBegin, beginLen) == 0) {
          // �u���b�N�R�����g��
          mx::Memory::Fill(wp, ' ', beginLen);
          wp += beginLen;

          // �R�����g�t���O ON
          comm->commentF++;

          continue;  // to 'while (*wp)'
        }
        // �s�R�����g�̃`�F�b�N
        else if (0 < lineLen && mx::strncmp(wp, pLine, lineLen) == 0) {
          // null �܂ő���
          do {
            // �R�����g�Ȃ̂Œu��������(�^�u�͂��̂܂�)
            if (*wp != '\t') {
              *wp = ' ';  // to space
            }
          } while (*(++wp) != '\0');

          continue;  // to 'while (*wp)'
        }
      }

      // ������`�F�b�N
      if (comm->ignoreString) {
        switch (*wp) {
        case '\'':
          // �G�X�P�[�v�`�F�b�N
          if (*(wp - 1) != '\\') {
            if (comm->stringF == 0) {
              // ������t���O ON
              comm->stringF = 1;
            } else if (comm->stringF == 1) {
              // ������t���O OFF
              comm->stringF = 0;
            }
          }
          break;
        case '"':
          // �G�X�P�[�v�`�F�b�N
          if (*(wp - 1) != '\\') {
            if (comm->stringF == 0) {
              // ������t���O ON
              comm->stringF = 2;
            } else if (comm->stringF == 2) {
              // ������t���O OFF
              comm->stringF = 0;
            }
          }
          break;
        }
      }
    }

    // ������|�C���^��i�߂�
    wp++;
  }
}

void CommentToSpace(StringList* stringList, const mx::string& lines, const mx::string& blockBegin,
                    const mx::string& blockEnd, bool ignoreString) {
  CommentToSpaceDescription comm;
  mx::Memory::Zero(&comm, sizeof(comm));

  comm.ignoreString = ignoreString;

  // �ȒP�ȃ`�F�b�N
  //	if ((blockBegin && !blockEnd) || (!blockBegin && blockEnd)) {
  //		*(char*)0x00000000 = 0;
  //	}

  comm.line.ptr = lines;
  comm.blockBegin.ptr = blockBegin;
  comm.blockEnd.ptr = blockEnd;

  // �s�R�����g�̒���
  if (lines) {
    comm.line.len = lines.length();  // �s�R�����g
  }

  // �u���b�N�R�����g�̒���
  if (blockBegin && blockEnd) {
    comm.blockBegin.len = blockBegin.length();  // �u���b�N�R�����g�J�n
    comm.blockEnd.len = blockEnd.length();      // �u���b�N�R�����g�I��
  }

  for (int i = 0; i < stringList->count(); ++i) {
    local_CommentToSpace(stringList->Strings[i].data(), &comm);  // ���ڏ���������
  }
}

void CommentToSpace(char* wp, const char* lines, const char* blockBegin, const char* blockEnd,
                    bool ignoreString) {
  CommentToSpaceDescription comm;
  mx::Memory::Zero(&comm, sizeof(comm));

  comm.ignoreString = ignoreString;

  // �ȒP�ȃ`�F�b�N
  //	if ((blockBegin && !blockEnd) || (!blockBegin && blockEnd)) {
  //		*(char*)0x00000000 = 0;
  //	}

  comm.line.ptr = lines;
  comm.blockBegin.ptr = blockBegin;
  comm.blockEnd.ptr = blockEnd;

  // �s�R�����g�̒���
  if (lines) {
    comm.line.len = mx::strlen(lines);  // �s�R�����g
  }

  // �u���b�N�R�����g�̒���
  if (blockBegin && blockEnd) {
    comm.blockBegin.len = mx::strlen(blockBegin);  // �u���b�N�R�����g�J�n
    comm.blockEnd.len = mx::strlen(blockEnd);      // �u���b�N�R�����g�I��
  }

  local_CommentToSpace(wp, &comm);
}

char* sprintf(char* buf, const char* fmt, ...) {
  return svprintf(buf, &fmt);
}

char* svprintf(char* buf, const char** fmt) {
  /// �ϒ��������� ///
  if (*fmt) {
    va_list marker;
    va_start(marker, *fmt);
    vsprintf(buf, *fmt, marker);
    va_end(marker);
  } else {
    *buf = '\0';
  }

  return buf;
}

}  //utils
}  //mx
