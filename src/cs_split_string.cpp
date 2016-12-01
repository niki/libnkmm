//-----------------------------------------------------------------------------
/*!
* @file		mx_split_string.cpp
* @brief	����������N���X
* @author	koma
* @note
* �E�X�V����  
*/
//-----------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS

#if defined(_WIN32)
#include <windows.h>
#endif

#include "mx_string.h"
#include "mx_split_string.h"

namespace mx {

SplitString::SplitString() : textOrg_(""), text_("") {
  init();
}

SplitString::SplitString(const mx::string &text, const char *delimiters, int flag)
    : textOrg_(text), text_("") {
  init();
  split(NULL, delimiters, flag);
}

SplitString::~SplitString() {}

void SplitString::init() {
  //textOrg_ = "";
  //text_ = "";
  tokenCnt_ = 0;
  curTokenPos_ = 0;
  tokenPtr_ = 0;
  curToken_ = "";
  nextTokenPtr_ = 0;
}

/*!
	�g�[�N���̌����擾
	@return �g�[�N���̌�
*/
int SplitString::size() {
  return tokenCnt_;
}

/*!
	�ŏ��̃g�[�N�����擾
	@return	�g�[�N��
*/
void SplitString::begin() {
  curTokenPos_ = 0;
  curToken_ = tokenPtr_;
  nextTokenPtr_ = tokenPtr_;
}

/*!
	���̃g�[�N���ֈړ�
	@return	true ����
			false �g�[�N�����Ȃ�
*/
bool SplitString::move_next() {
  bool ret = (curTokenPos_ < size());
  curToken_ = nextTokenPtr_;
  if (ret) {
    ++curTokenPos_;

    int len = text_.isWide() ? wcslen(reinterpret_cast<const wchar_t *>(nextTokenPtr_))
                             : strlen(reinterpret_cast<const char *>(nextTokenPtr_));
    nextTokenPtr_ = nextTokenPtr_ + len + 1;
  }
  return ret;
}

/*!
	�w��̃g�[�N�����擾
	@param[in]	�C���f�b�N�X
	@return	�g�[�N��
*/
mx::string SplitString::at(int index) const {
#if 1
  const char *token_ptr = tokenPtr_;

  if (index == -1) {
    index = curTokenPos_ - 1;
  }

  for (int i = 0; i < index; ++i) {
    int len = strlen(token_ptr);
    token_ptr = token_ptr + len + 1;
  }

  return mx::string(token_ptr);
#else
  begin();

  for (int i = 0; i <= index; ++i) {
    move_next();
  }

  return at();
#endif
}

/*!
	����
	@param[in]	text �e�L�X�g(NULL�̏ꍇ�̓e�L�X�g���X�V���Ȃ�)
	@param[in]	delimiters �f���~�^
	@param[in]	token_delimiter �f���~�^���g�[�N���ɂ���
	@param[in]	escape_string ��������G�X�P�[�v����
*/
void SplitString::split(const mx::string &text, const mx::string &delimiters, int flag) {
  if (!text.empty()) {
    textOrg_ = text;
  }

  int tok_cnt = 1;
  bool is_escape = false;

  // �g�[�N���}���������z�����g��
  // �f���~�^���g�[�N���ɉ����Ȃ��ꍇ�͗]���Ȋg���s�v
  if (flag & kSplitIncludeTokenDelimiter) {
    if (textOrg_.length() * 3 > text_.max_size()) {
      text_.extendBuffer(textOrg_.length() * 3, false);
    }
  }

  text_.extendBuffer(textOrg_.max_size(), false);

  const char *src = textOrg_.data();
  char *dst = text_.data();
  char *entry = dst;

  *dst = 0;
  while (*src) {
    char ch = *src;

    if (ch == 0x22 /*"*/ && (flag & kSplitEscapeString)) {
      is_escape ^= true;
      //src++;			// " ���܂߂Ȃ�
      *dst++ = *src++;  // " ���܂߂�
      continue;
    }

    if (is_escape) {
      *dst++ = *src++;
    } else {
      // �f���~�^���Q���猩������?
      int pos = delimiters.chr(ch);
      if (pos >= 0) {
        if (flag & kSplitIncludeTokenDelimiter) {
          *dst++ = 0;
          ++tok_cnt;

          *dst++ = *src++;
          *dst++ = 0;
          ++tok_cnt;
        } else {
          if ((flag & kSplitIgnoreEmpty) && (dst - entry) == 0) {
            // ��Ȃ̂Œǉ����Ȃ�
          } else {
            *dst++ = 0;
            ++tok_cnt;
          }

          ++src;
        }

        entry = dst;
      } else {
        *dst++ = *src++;
      }
    }
  }
  *dst = 0;

  tokenCnt_ = tok_cnt;
  tokenPtr_ = text_.data();
  //DEBUGPRINTLOG("*> '%s'\n", tokenPtr_);

  begin();
}

}  //mx
