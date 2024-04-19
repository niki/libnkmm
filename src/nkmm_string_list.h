// -*- mode:c++; coding:utf-8-ws -*-
#ifndef NKMM_STRINGLIST_H
#define NKMM_STRINGLIST_H

//Include
#include "nkmm_vector.hpp"
#include "nkmm_string.h"  //文字列処理クラス

//! 改行コードを使用する
//! 使用する場合、セーブ時は考慮されていないので実装が必要
#define USE_STRINGLIST_RETURNCODE 0

namespace nkmm {

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

  nkmm::string &basic_Add(const nkmm::string &rp);
  nkmm::string &basic_Add(const nkmm::string &rp, int len);
  nkmm::string &basic_Assign(int index, const nkmm::string &rp);
  nkmm::string &basic_Assign(int index, const nkmm::string &rp, int len);
  nkmm::string &basic_Concatenate(int index, const nkmm::string &rp);
  nkmm::string &basic_Concatenate(int index, const nkmm::string &rp, int len);
  nkmm::string &basic_Insert(int index, const nkmm::string &rp);
  nkmm::string &basic_Insert(int index, const nkmm::string &rp, int len);

  // 可変長引数対応版
  //	nkmm::string		&add(const nkmm::string &fmt, ...);
  //	nkmm::string		&assign(int index, const nkmm::string &fmt, ...);
  //	nkmm::string		&concatenate(int index, const nkmm::string &fmt, ...);
  //	nkmm::string		&insert(int index, const nkmm::string &fmt, ...);
  nkmm::string &add(const char *fmt, ...);
  nkmm::string &assign(int index, const char *fmt, ...);
  nkmm::string &concatenate(int index, const char *fmt, ...);
  nkmm::string &insert(int index, const char *fmt, ...);

  void setReturnCode(int type);
  int getReturnCode();

  int count() const;

  void deleteLine(int index);

  // 空白行を削除
  void deleteBlankLines();

  // 行コメント部分を削除する
  void removeLineComment(const nkmm::string &lineCommentWord);

  void clear();
  void free();

  // ファイルの読み込み
  int loadFromFile(const nkmm::string &fileName, bool concat = false);

  // ファイルに保存
  int saveToFile(const nkmm::string &fileName);

  // ソート
  virtual void sort(int (*fnCmp)(const nkmm::string &, const nkmm::string &));
  virtual void sort(int sortUp = 1);
  virtual void lengthSort(int sortUp = 1);

  virtual void fromText(const char *data,
                        int size = 0x7FFFFFFF);  // 文字列データからリストデータを作成
  virtual int toString(nkmm::string &out,
                       const char delimiter = -1);  // リストデータから文字列データを作成

 public:
#ifdef BCC32
  typename
#endif
      nkmm::vector<nkmm::string>
          Strings;

 protected:
#if USE_STRINGLIST_RETURNCODE
#ifdef BCC32
  typename
#endif
      nkmm::vector<int>
          returnCodes_;
#endif  //USE_STRINGLIST_RETURNCODE

 private:
  int returnCode_;
};

}  //nkmm

#endif  //NKMM_STRINGLIST_H
