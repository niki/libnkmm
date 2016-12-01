//-----------------------------------------------------------------------------
/*!
* @file		mx_stringlist.h
* @brief	mx::string リスト処理
* @author	koma
* @version	$Id$
* 
* @note
* ・更新履歴  
*/
//-----------------------------------------------------------------------------
#ifndef MX_STRINGLIST_H
#define MX_STRINGLIST_H

//Include
#include "mx_vector.hpp"
#include "mx_string.h"  //文字列処理クラス

//! 改行コードを使用する
//! 使用する場合、セーブ時は考慮されていないので実装が必要
#define USE_STRINGLIST_RETURNCODE 0

namespace mx {

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

  mx::string &basic_Add(const mx::string &rp);
  mx::string &basic_Add(const mx::string &rp, int len);
  mx::string &basic_Assign(int index, const mx::string &rp);
  mx::string &basic_Assign(int index, const mx::string &rp, int len);
  mx::string &basic_Concatenate(int index, const mx::string &rp);
  mx::string &basic_Concatenate(int index, const mx::string &rp, int len);
  mx::string &basic_Insert(int index, const mx::string &rp);
  mx::string &basic_Insert(int index, const mx::string &rp, int len);

  // 可変長引数対応版
  //	mx::string		&add(const mx::string &fmt, ...);
  //	mx::string		&assign(int index, const mx::string &fmt, ...);
  //	mx::string		&concatenate(int index, const mx::string &fmt, ...);
  //	mx::string		&insert(int index, const mx::string &fmt, ...);
  mx::string &add(const char *fmt, ...);
  mx::string &assign(int index, const char *fmt, ...);
  mx::string &concatenate(int index, const char *fmt, ...);
  mx::string &insert(int index, const char *fmt, ...);

  void setReturnCode(int type);
  int getReturnCode();

  int count() const;

  void deleteLine(int index);

  // 空白行を削除
  void deleteBlankLines();

  // 行コメント部分を削除する
  void removeLineComment(const mx::string &lineCommentWord);

  void clear();
  void free();

  // ファイルの読み込み
  int loadFromFile(const mx::string &fileName, bool concat = false);

  // ファイルに保存
  int saveToFile(const mx::string &fileName);

  // ソート
  virtual void sort(int (*fnCmp)(const mx::string &, const mx::string &));
  virtual void sort(int sortUp = 1);
  virtual void lengthSort(int sortUp = 1);

  virtual void fromText(const char *data,
                        int size = 0x7FFFFFFF);  // 文字列データからリストデータを作成
  virtual int toString(mx::string &out,
                       const char delimiter = -1);  // リストデータから文字列データを作成

 public:
#ifdef BCC32
  typename
#endif
      mx::vector<mx::string>
          Strings;

 protected:
#if USE_STRINGLIST_RETURNCODE
#ifdef BCC32
  typename
#endif
      mx::vector<int>
          returnCodes_;
#endif  //USE_STRINGLIST_RETURNCODE

 private:
  int returnCode_;
};

}  //mx

#endif  //MX_STRINGLIST_H
