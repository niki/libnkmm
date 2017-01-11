// -*- mode:c++; coding:utf-8-ws -*-
//====================================================================
//! @file  rei_utils.cpp
//! @brief
//!
//! @author
//====================================================================
#include <stdio.h>   // vsprintf
#include <stdarg.h>  // va_list
#include "rei_memory.h"
#include "rei_basis.h"
#include "rei_strlib.h"  // 文字列処理
#include "rei_math.h"    // 数値処理
#include "rei_string_list.h"
#include "rei_utils.h"

#define MAX_LENGTH 0x2000

namespace rei {
namespace utils {

int AutoCompleteString(const StringList* stringList, const rei::string& s1) {
  for (int i = 0; i < stringList->count(); i++) {
    const rei::string& s2 = stringList->Strings[i];

    if (rei::strnicmp(s2.c_str(), s1.c_str(), s1.length()) == 0) {
      return i;
    }
  }

  return -1;
}

void ExtractTokenToList(StringList* stringList, const rei::string& src,
                        const rei::string& delimiters, const rei::string& ignores) {
  int srclen = src.length();
  char* buff = SAFE_NEW_ARRAY(__SOURCE__, char,
                              srclen);  // バッファ確保(とりあえず src の長さ分あれば保証されるはず)

  // リストのクリア
  stringList->clear();

  const char* p = src.c_str();

  for (;;) {
    p = rei::sjis::ExtractTokenStart(buff, p, delimiters, ignores);

    // リストに追加
    stringList->basic_Add(buff);

    // 終わり
    if (src == 0) {
      break;
    }
  }

  //SAFE_DELETE_ARRAY(buff);
  delete[] buff;
}

struct CommentToSpaceDescription {
  int commentF;  //コメントフラグ
  int stringF;   //文字列フラグ
  bool ignoreString;

  //行コメント
  struct {
    int len;
    const char* ptr;
  } line;

  //ブロックコメント開始
  struct {
    int len;
    const char* ptr;
  } blockBegin;

  //ブロックコメント終了
  struct {
    int len;
    const char* ptr;
  } blockEnd;
};

// コメントをスペースに変換
static void local_CommentToSpace(char* wp, CommentToSpaceDescription* comm) {
  const int lineLen = comm->line.len;
  const int beginLen = comm->blockBegin.len;
  const int endLen = comm->blockEnd.len;

  const char* pLine = comm->line.ptr;
  const char* pBegin = comm->blockBegin.ptr;
  const char* pEnd = comm->blockEnd.ptr;

  while (*wp) {
    // コメントのときの処理
    if (comm->commentF && comm->stringF == 0) {
      // 日本語チェック
      if (rei::sjis::IsKanji1(*wp)) {
        *wp++ = ' ';  // to space
        *wp++ = ' ';  // to space

        continue;  // to 'while (*wp)'
      }

      if (0 < beginLen) {
        // ブロックコメントの開始チェック
        if (rei::strncmp(wp, pBegin, beginLen) == 0) {
          // ブロックコメント分
          rei::Memory::Fill(wp, ' ', beginLen);
          wp += beginLen;

          // コメントフラグ ON
          comm->commentF++;

          continue;  // to 'while (*wp)'
        }
        // ブロックコメントの終了チェック
        else if (rei::strncmp(wp, pEnd, endLen) == 0) {
          // ブロックコメント分
          rei::Memory::Fill(wp, ' ', endLen);
          wp += endLen;

          // コメントフラグ OFF
          comm->commentF--;

          continue;  // to 'while (*wp)'
        }
      }

      // コメントなので置き換える(タブはそのまま)
      if (*wp != '\t') {
        *wp = ' ';  // to space
      }
    } else {
      // 日本語チェック
      if (rei::sjis::IsKanji1(*wp)) {
        // 文字列ポインタを進める
        wp += 2;

        continue;  // to 'while (*wp)'
      }

      // 文字列内でないときの処理
      if (comm->stringF == 0) {
        // ブロックコメントの開始チェック
        if (0 < beginLen && rei::strncmp(wp, pBegin, beginLen) == 0) {
          // ブロックコメント分
          rei::Memory::Fill(wp, ' ', beginLen);
          wp += beginLen;

          // コメントフラグ ON
          comm->commentF++;

          continue;  // to 'while (*wp)'
        }
        // 行コメントのチェック
        else if (0 < lineLen && rei::strncmp(wp, pLine, lineLen) == 0) {
          // null まで送る
          do {
            // コメントなので置き換える(タブはそのまま)
            if (*wp != '\t') {
              *wp = ' ';  // to space
            }
          } while (*(++wp) != '\0');

          continue;  // to 'while (*wp)'
        }
      }

      // 文字列チェック
      if (comm->ignoreString) {
        switch (*wp) {
        case '\'':
          // エスケープチェック
          if (*(wp - 1) != '\\') {
            if (comm->stringF == 0) {
              // 文字列フラグ ON
              comm->stringF = 1;
            } else if (comm->stringF == 1) {
              // 文字列フラグ OFF
              comm->stringF = 0;
            }
          }
          break;
        case '"':
          // エスケープチェック
          if (*(wp - 1) != '\\') {
            if (comm->stringF == 0) {
              // 文字列フラグ ON
              comm->stringF = 2;
            } else if (comm->stringF == 2) {
              // 文字列フラグ OFF
              comm->stringF = 0;
            }
          }
          break;
        }
      }
    }

    // 文字列ポインタを進める
    wp++;
  }
}

void CommentToSpace(StringList* stringList, const rei::string& lines, const rei::string& blockBegin,
                    const rei::string& blockEnd, bool ignoreString) {
  CommentToSpaceDescription comm;
  rei::Memory::Zero(&comm, sizeof(comm));

  comm.ignoreString = ignoreString;

  // 簡単なチェック
  //	if ((blockBegin && !blockEnd) || (!blockBegin && blockEnd)) {
  //		*(char*)0x00000000 = 0;
  //	}

  comm.line.ptr = lines;
  comm.blockBegin.ptr = blockBegin;
  comm.blockEnd.ptr = blockEnd;

  // 行コメントの長さ
  if (lines) {
    comm.line.len = lines.length();  // 行コメント
  }

  // ブロックコメントの長さ
  if (blockBegin && blockEnd) {
    comm.blockBegin.len = blockBegin.length();  // ブロックコメント開始
    comm.blockEnd.len = blockEnd.length();      // ブロックコメント終了
  }

  for (int i = 0; i < stringList->count(); ++i) {
    local_CommentToSpace(stringList->Strings[i].data(), &comm);  // 直接書きかえる
  }
}

void CommentToSpace(char* wp, const char* lines, const char* blockBegin, const char* blockEnd,
                    bool ignoreString) {
  CommentToSpaceDescription comm;
  rei::Memory::Zero(&comm, sizeof(comm));

  comm.ignoreString = ignoreString;

  // 簡単なチェック
  //	if ((blockBegin && !blockEnd) || (!blockBegin && blockEnd)) {
  //		*(char*)0x00000000 = 0;
  //	}

  comm.line.ptr = lines;
  comm.blockBegin.ptr = blockBegin;
  comm.blockEnd.ptr = blockEnd;

  // 行コメントの長さ
  if (lines) {
    comm.line.len = rei::strlen(lines);  // 行コメント
  }

  // ブロックコメントの長さ
  if (blockBegin && blockEnd) {
    comm.blockBegin.len = rei::strlen(blockBegin);  // ブロックコメント開始
    comm.blockEnd.len = rei::strlen(blockEnd);      // ブロックコメント終了
  }

  local_CommentToSpace(wp, &comm);
}

char* sprintf(char* buf, const char* fmt, ...) {
  return svprintf(buf, &fmt);
}

char* svprintf(char* buf, const char** fmt) {
  /// 可変長引数処理 ///
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
}  //rei
