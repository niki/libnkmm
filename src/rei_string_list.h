// -*- mode:c++; coding:utf-8-ws -*-
//====================================================================
//! @file  rei_string_list.h
//! @brief rei::string リスト処理
//!
//! @author
//====================================================================
#ifndef REI_STRINGLIST_H
#define REI_STRINGLIST_H

//Include
#include "rei_vector.hpp"
#include "rei_string.h"  //文字列処理クラス

//! 改行コードを使用する
//! 使用する場合、セーブ時は考慮されていないので実装が必要
#define USE_STRINGLIST_RETURNCODE 0

namespace rei {

class File;

//改行タイプ
enum {
  kEolEof = 0,  //0x00
  kEolCr,       //CR 0x0d
  kEolLf,       //LF 0x0a
  kEolCrlf      //CRLF 0x0d 0x0a
};

//改行サイズ
enum {
  kSizeEolEof = 0,
  kSizeEolCr = 1,   //CR
  kSizeEolLf = 1,   //LF
  kSizeEolCrlf = 2  //CRLF
};

//! StringList 文字列リストクラス
class StringList {
 public:
  StringList();
  virtual ~StringList();

  rei::string &basic_Add(const rei::string &rp);
  rei::string &basic_Add(const rei::string &rp, int len);
  rei::string &basic_Assign(int index, const rei::string &rp);
  rei::string &basic_Assign(int index, const rei::string &rp, int len);
  rei::string &basic_Concatenate(int index, const rei::string &rp);
  rei::string &basic_Concatenate(int index, const rei::string &rp, int len);
  rei::string &basic_Insert(int index, const rei::string &rp);
  rei::string &basic_Insert(int index, const rei::string &rp, int len);

  // 可変長引数対応版
  //	rei::string		&add(const rei::string &fmt, ...);
  //	rei::string		&assign(int index, const rei::string &fmt, ...);
  //	rei::string		&concatenate(int index, const rei::string &fmt, ...);
  //	rei::string		&insert(int index, const rei::string &fmt, ...);
  rei::string &add(const char *fmt, ...);
  rei::string &assign(int index, const char *fmt, ...);
  rei::string &concatenate(int index, const char *fmt, ...);
  rei::string &insert(int index, const char *fmt, ...);

  void setReturnCode(int type);
  int getReturnCode();

  int count() const;

  void deleteLine(int index);

  // 空白行を削除
  void deleteBlankLines();

  // 行コメント部分を削除する
  void removeLineComment(const rei::string &lineCommentWord);

  void clear();
  void free();

  // ファイルの読み込み
  int loadFromFile(const rei::string &fileName, bool concat = false);

  // ファイルに保存
  int saveToFile(const rei::string &fileName);

  // ソート
  virtual void sort(int (*fnCmp)(const rei::string &, const rei::string &));
  virtual void sort(int sortUp = 1);
  virtual void lengthSort(int sortUp = 1);

  virtual void fromText(const char *data,
                        int size = 0x7FFFFFFF);  // 文字列データからリストデータを作成
  virtual int toString(rei::string &out,
                       const char delimiter = -1);  // リストデータから文字列データを作成

 public:
#ifdef BCC32
  typename
#endif
      rei::vector<rei::string>
          Strings;

 protected:
#if USE_STRINGLIST_RETURNCODE
#ifdef BCC32
  typename
#endif
      rei::vector<int>
          returnCodes_;
#endif  //USE_STRINGLIST_RETURNCODE

 private:
  int returnCode_;
};

}  //rei

#endif  //REI_STRINGLIST_H
