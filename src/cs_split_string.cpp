//-----------------------------------------------------------------------------
/*!
* @file		mx_split_string.cpp
* @brief	分割文字列クラス
* @author	koma
* @note
* ・更新履歴  
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
	トークンの個数を取得
	@return トークンの個数
*/
int SplitString::size() {
  return tokenCnt_;
}

/*!
	最初のトークンを取得
	@return	トークン
*/
void SplitString::begin() {
  curTokenPos_ = 0;
  curToken_ = tokenPtr_;
  nextTokenPtr_ = tokenPtr_;
}

/*!
	次のトークンへ移動
	@return	true 成功
			false トークンがない
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
	指定のトークンを取得
	@param[in]	インデックス
	@return	トークン
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
	分割
	@param[in]	text テキスト(NULLの場合はテキストを更新しない)
	@param[in]	delimiters デリミタ
	@param[in]	token_delimiter デリミタもトークンにする
	@param[in]	escape_string 文字列をエスケープする
*/
void SplitString::split(const mx::string &text, const mx::string &delimiters, int flag) {
  if (!text.empty()) {
    textOrg_ = text;
  }

  int tok_cnt = 1;
  bool is_escape = false;

  // トークン挿入分を見越した拡張
  // デリミタをトークンに加えない場合は余分な拡張不要
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
      //src++;			// " を含めない
      *dst++ = *src++;  // " を含める
      continue;
    }

    if (is_escape) {
      *dst++ = *src++;
    } else {
      // デリミタ字群から見つかった?
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
            // 空なので追加しない
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
