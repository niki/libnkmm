//-----------------------------------------------------------------------------
/*!
* @file		mx_stringlist.cpp
* @brief	mx::string リスト処理
* @author	koma
* @version	$Id$
* 
* @note
* ・更新履歴  
* 2013.08.18	新規に作成した basic_string クラスの管理に変更  
* 2007.04.24	fopen() を mx::File に書き換え  
* 2006.04.29	プリプロセッサ関係のクラスの外に出した  
* 2006.03.24	ファイル書き込みの改良。使用メモリを大幅に減らした  
* 2006.03.10	ファイル読み込みの改良。使用メモリを大幅に減らした  
* 2006.02.21	複数の改行コードの読み込みに対応  
* 2005.07.28	#defineによる置換を単語単位での置換にした  
* 2005.05.20	fopen() で CreateFile() を呼んでいるようなので元に戻す  
* 2005.04.26	Windows のときはファイル処理に API を使うようにした  
*/
//-----------------------------------------------------------------------------
//Include
#ifdef __debugprint
#include <stdio.h>  //f*()
//#  include <stdarg.h>		//va_list()
#endif

#include "mx_string.h"
#include "mx_buffer.h" /* buffer operation */
#include "mx_basis.h"
#include "mx_strlib.h"
#include "mx_file.h"
#include "mx_utils.h"
#include "mx_string_list.h"

#define MESS_FILE_OPEN_ERROR_LOAD "*** ERROR!! Load ﾌｧｲﾙが読み込めない!!(%s)\n"
#define MESS_FILE_OPEN_ERROR_SAVE "*** ERROR!! Save ﾌｧｲﾙが読み込めない!!(%s)\n"
#define MESS_FILE_OPEN_ERROR_SIZE_ZERO "*** ERROR!! ﾌｧｲﾙｻｲｽﾞが0です!!(%s)\n"
#define MESS_FILE_READ "%8d file read!!(%s)\n"
#define MESS_FILE_WRITE "%8d file write!!(%s)\n"
#define MESS_DEFINE_WARNING "\"%s\"はすでに定義されています!!\n"

namespace mx {

const int s_SizeOf_EOL[] = {
    kSizeEolEof,
    kSizeEolCr,    //CR
    kSizeEolLf,    //LF
    kSizeEolCrlf,  //CRLF
};

/*!
	StringList クラス　コンストラクタ
*/
StringList::StringList() : returnCode_(kEolCrlf) {
  //アイテムの初期化
  Strings.reserve(256);  //アイテム拡張サイズ
}

/*!
	StringList クラス　デストラクタ
*/
StringList::~StringList() {}

/*!
	文字列領域の作成
	@param[in] rp 文字列
	@return 追加した String クラスの参照
*/
mx::string &StringList::basic_Add(const mx::string &rp) {
  mx::string &item = Strings.push_back(rp);
#if USE_STRINGLIST_RETURNCODE
  returnCodes_.push_back(getReturnCode());
#endif  //USE_STRINGLIST_RETURNCODE
  return item;
}

/*!
	文字列領域の作成
	@param[in] rp 文字列
	@param[in] len 長さ
	@return 追加した String クラスの参照
*/
mx::string &StringList::basic_Add(const mx::string &rp, int len) {
  mx::string &item = Strings.push_back();
  if (len > 0) {
    item.assign(rp, len);
  }
#if USE_STRINGLIST_RETURNCODE
  returnCodes_.push_back(getReturnCode());
#endif  //USE_STRINGLIST_RETURNCODE
  return item;
}

/*!
	文字列挿入
	@param[in] index 追加位置
	@param[in] rp 文字列
	@return 追加した String クラスの参照
*/
mx::string &StringList::basic_Insert(int index, const mx::string &rp) {
  mx::string &item = Strings.insert(index, rp);
#if USE_STRINGLIST_RETURNCODE
  returnCodes_.insert(index, getReturnCode());
#endif  //USE_STRINGLIST_RETURNCODE
  return item;
}

/*!
	文字列挿入
	@param[in] index 追加位置
	@param[in] rp 文字列
	@param[in] len 長さ
	@return 追加した String クラスの参照
*/
mx::string &StringList::basic_Insert(int index, const mx::string &rp, int len) {
  mx::string &item = Strings.insert(index);
  if (len > 0) {
    item.assign(rp, len);
  }
#if USE_STRINGLIST_RETURNCODE
  returnCodes_.insert(index, getReturnCode());
#endif  //USE_STRINGLIST_RETURNCODE
  return item;
}

/*!
	文字列変更
	@param[in] index 変更位置
	@param[in] rp] 文字列
	@return 追加した String クラスの参照
*/
mx::string &StringList::basic_Assign(int index, const mx::string &rp) {
  Strings[index] = rp;
  return Strings[index];
}

/*!
	文字列変更
	@param[in] index 変更位置
	@param[in] rp] 文字列
	@param[in] len 長さ
	@return 追加した String クラスの参照
*/
mx::string &StringList::basic_Assign(int index, const mx::string &rp, int len) {
  if (len == 0) {
    Strings[index] = "";
  } else {
    Strings[index].assign(rp, len);
  }
  return Strings[index];
}

/*!
	文字列をつなげる
	@param[in] index つなげる位置
	@param[in] rp 文字列
	@return 追加した mx::string クラスの参照
*/
mx::string &StringList::basic_Concatenate(int index, const mx::string &rp) {
  Strings[index] += rp;
  return Strings[index];
}

/*!
	文字列をつなげる
	@param[in] index つなげる位置
	@param[in] rp 文字列
	@param[in] len 長さ(-1で自動)
	@return 追加した mx::string クラスの参照
*/
mx::string &StringList::basic_Concatenate(int index, const mx::string &rp, int len) {
  if (len == 0) {
    //
  } else {
    Strings[index].append(rp, len);
  }
  return Strings[index];
}

//mx::string &StringList::add(const mx::string &fmt, ...)
mx::string &StringList::add(const char *fmt, ...) {
  if (*fmt) {
    mx::string s;
    s.extendBuffer(0x1000);
    return basic_Add(utils::svprintf(s.data(), &fmt));
  } else {
    return basic_Add("");
  }
}

//mx::string &StringList::assign(int index, const mx::string &fmt, ...)
mx::string &StringList::assign(int index, const char *fmt, ...) {
  if (*fmt) {
    mx::string s;
    s.extendBuffer(0x1000);
    return basic_Assign(index, utils::svprintf(s.data(), &fmt));
  } else {
    return basic_Assign(index, "");
  }
}

//mx::string &StringList::concatenate(int index, const mx::string &fmt, ...)
mx::string &StringList::concatenate(int index, const char *fmt, ...) {
  if (*fmt) {
    mx::string s;
    s.extendBuffer(0x1000);
    return basic_Concatenate(index, utils::svprintf(s.data(), &fmt));
  } else {
    // 文字列がないのでなにもしなくていい
    return Strings[index];
  }
}

//mx::string &StringList::insert(int index, const mx::string &fmt, ...)
mx::string &StringList::insert(int index, const char *fmt, ...) {
  if (*fmt) {
    mx::string s;
    s.extendBuffer(0x1000);
    return basic_Insert(index, utils::svprintf(s.data(), &fmt));
  } else {
    return basic_Insert(index, "");
  }
}

/*!
	デフォルト改行コード設定
	@param[in] type 改行コードタイプ
*/
void StringList::setReturnCode(int type) {
  returnCode_ = type;
}

/*!
	デフォルト改行コード取得
	@return 改行コードタイプ
*/
int StringList::getReturnCode() {
  return returnCode_;
}

/*!
	行数取得
	@return 行数
*/
int StringList::count() const {
  return Strings.size();
}

/*!
	比較関数(昇順)	文字列
	@param[in] a データＡ
	@param[in] b データＢ
	@return int
*/
///template <class T> static int fnSortStringUp(const T &a, const T &b)
static int fnSortStringUp(const mx::string &a, const mx::string &b) {
  ///	return (*(T*)&a).compare(b);
  return a.compare(b);
}

/*!
	比較関数(降順)	文字列
	@param[in] a データＡ
	@param[in] b データＢ
	@return int
*/
///template <class T> static int fnSortStringDown(const T &a, const T &b)
static int fnSortStringDown(const mx::string &a, const mx::string &b) {
  ///	return (*(T*)&b).compare(a);
  return b.compare(a);
}

/*!
	比較関数(昇順)	長さ
	@param[in] a データＡ
	@param[in] b データＢ
	@return int
*/
///template <class T> static int fnSortLengthUp(const T &a, const T &b)
static int fnSortLengthUp(const mx::string &a, const mx::string &b) {
  ///	return (*(T*)&a).length() - (*(T*)&b).length();
  return a.length() - b.length();
}

/*!
	比較関数(降順)	長さ
	@param[in] a データＡ
	@param[in] b データＢ
	@return int
*/
///template <class T> static int fnSortLengthDown(const T &a, const T &b)
static int fnSortLengthDown(const mx::string &a, const mx::string &b) {
  ///	return (*(T*)&b).length() - (*(T*)&a).length();
  return b.length() - a.length();
}

/*!
	文字列リストのソート
	@param[in] fnCmp 比較関数
*/
void StringList::sort(int (*fnCmp)(const mx::string &, const mx::string &)) {
  //	Strings.bsort(fnCmp, 0, Strings.size());
  Strings.qsort(fnCmp, 0, Strings.size() - 1);
}

/*!
	文字列リストのソート
	@param[in] sortUp 0:降順 1:昇順
*/
void StringList::sort(int sortUp) {
  static int (*s_fnCmp[2])(const mx::string &, const mx::string &) = {
      fnSortStringDown,  //文字列・降順
      fnSortStringUp,    //文字列・昇順
  };

  sort(s_fnCmp[sortUp]);
}

/*!
	文字列リストのソート(長さ)
	@param[in] sortUp 0:降順 1:昇順
*/
void StringList::lengthSort(int sortUp) {
  static int (*s_fnCmp[2])(const mx::string &, const mx::string &) = {
      fnSortLengthDown,  //文字列・降順
      fnSortLengthUp,    //文字列・昇順
  };

  sort(s_fnCmp[sortUp]);
}

/*!
	行の削除
	@param[in] index インデックス
*/
void StringList::deleteLine(int index) {
  Strings.erase(index);
#if USE_STRINGLIST_RETURNCODE
  returnCodes_.erase(index);
#endif  //USE_STRINGLIST_RETURNCODE
}

/*!
	空白行の削除
*/
void StringList::deleteBlankLines() {
  for (int i = count() - 1; i >= 0; --i) {
    mx::string &s = Strings[i];

    //文字列をスキャンして、指定された文字群以外が最初に出現する位置を見つけます
    const int pos = mx::strspn(s.c_str(), " \t");

    //タブとスペース以外はなかったので削除
    if (s.length() == pos) {
      Strings.erase(i);
#if USE_STRINGLIST_RETURNCODE
      returnCodes_.erase(i);
#endif  //USE_STRINGLIST_RETURNCODE
    }
  }
}

/*!
	行コメント処理
*/
void StringList::removeLineComment(const mx::string &lineCommentWord) {
  for (int i = count() - 1; i >= 0; i--) {
    mx::string &s = Strings[i];

    int pos = s.find(lineCommentWord);

    if (pos >= 0) {
      s.erase();
    }
  }
}

/*!
	クリア
*/
void StringList::clear() {
  Strings.clear();
#if USE_STRINGLIST_RETURNCODE
  returnCodes_.clear();
#endif  //USE_STRINGLIST_RETURNCODE
}

/*!
	解放
*/
void StringList::free() {
  Strings.clean();
#if USE_STRINGLIST_RETURNCODE
  returnCodes_.clear_clean();
#endif  //USE_STRINGLIST_RETURNCODE
}

/*!
	テキスト型データからリストを作成
	@param[in] data データ
*/
void StringList::fromText(const char *data, int size) {
  //データからリストを作成していく
  const char *top = data;
  const char *entry = data;
  int count = 0;

  for (;;) {
    int return_code;
    int ch = *data++;  //１文字取得

    // データ終了
    if (data - top > size) {
      return_code = kEolEof;
    } else {
      switch (ch) {
      //CR(0x0d)
      case '\r':
        //LF(0x0a)
        if (*data == '\n') {
          return_code = kEolCrlf;

          //CRLF(0x0d 0x0a)
          data++;
        } else {
          return_code = kEolCr;
        }
        break;

      //LF(0x0a)
      case '\n':
//LFCR は認めません
#if 1
        return_code = kEolLf;
#else
        //CR(0x0d)
        if (*data == '\r') {
          return_code = kEolLfcr;

          //LFCR(0x0a 0x0d)
          data++;
        } else {
          return_code = kEolLf;
        }
#endif
        break;

      //null
      case '\0':
        return_code = kEolEof;
        break;

      //その他
      default:
        count++;
        //break;
        continue;  //※追加しない
      }
    }

    //リスト追加
    if (count == 0) {
      basic_Add("");
    } else {
      basic_Add(entry, count);
    }

#if USE_STRINGLIST_RETURNCODE
    // 改行コード
    returnCodes_.push_back(return_code);
#else
    (void)return_code;
#endif  //USE_STRINGLIST_RETURNCODE

    // データ終了
    if (data - top > size) {
      break;
    }

    //ch が nil になったら終わり
    if (ch == '\0') {
      break;
    }

    //次のエントリポイントの設定
    entry = data;
    count = 0;
  }
}

/*!
	ファイルの読み込み
	@param[in] fileName 読み込むファイル名
	@param[in] concat 連結
	@return 0:読み込み失敗, !0:読み込み成功
*/
int StringList::loadFromFile(const mx::string &fileName, bool concat) {
  File file;

  //リストをクリアしない
  if (concat) {
#if USE_STRINGLIST_RETURNCODE
    if (0 < count()) {
      int &r = returnCodes_.back();
      r = getReturnCode();  //EOF を 改行 にする
    }
#endif  //USE_STRINGLIST_RETURNCODE
  }
  //リストをクリアする
  else {
    clear();
  }

  //ファイルを開く
  if (!file.open(fileName, false)) {
#ifdef __debugprint
    printf(MESS_FILE_OPEN_ERROR_LOAD, fileName.c_str());
#endif  //__debugprint
    return 0;
  }

  //ファイルサイズが 0 以下なのでエラーで返す
  if (file.getSize() <= 0) {
#ifdef __debugprint
    printf(MESS_FILE_OPEN_ERROR_SIZE_ZERO, file.GetSize(), fileName.c_str());
#endif  //__debugprint
    file.close();
    return 0;
  }

  char *buf = 0;
  file.allocRead((void **)&buf);

  fromText(buf, file.getSize());

  SAFE_DELETE_ARRAY(buf);
  file.close();

#ifdef __debugprint
  printf(MESS_FILE_READ, file.GetSize(), fileName.c_str());
#endif  //__debugprint
  return file.getSize();
}

/*!
	リストをテキスト型データに変換
	@param[out] out
	@param[in] delimiter デリミタ
	@return size
*/
int StringList::toString(mx::string &out, const char delimiter) {
  if (Strings.empty()) {
    out = mx::string("");
    return 0;
  }

  int size = 0;
  BUFF b;

  //デフォルトモード(設定されている改行コード)
  if (-1 == delimiter) {
    //サイズ計算
    for (int i = 0; i < count(); i++) {
      mx::string &s = Strings[i];
      size += s.length() + 2;  //改行分も
    }
  }
  //デリミタ指定モード
  else {
    //サイズ計算
    for (int i = 0; i < count(); i++) {
      mx::string &s = Strings[i];
      size += s.length() + 1;  //+ '\0'
    }
  }

  //サイズからバッファ取得
  b.buffer = SAFE_NEW_ARRAY(__SOURCE__, uint8_t, size);
  b.curp = b.buffer;
  b.bsize = size;

  //デフォルトモード(設定されている改行コード)
  if (-1 == delimiter) {
    //連結
    for (int i = 0; i < count(); i++) {
      mx::string &s = Strings[i];

      bwrite(s.c_str(), s.length(), 1, &b);

      if (i < count() - 1) {
        bputc(0x0d, &b);  //改行(CR)
        bputc(0x0a, &b);  //改行(LF)
      } else {
        bputc(0x00, &b);  //EOF
      }
    }
    //bputc(0x00, &b); //EOF
  }
  //デリミタ指定モード
  else {
    //連結
    for (int i = 0; i < count(); i++) {
      mx::string &s = Strings[i];

      bwrite(s.c_str(), s.length(), 1, &b);
      bputc(delimiter, &b);  //'\0'
    }
  }

  out = (char *)b.buffer;
  SAFE_DELETE_ARRAY(b.buffer);

  if (-1 == delimiter) {
    return size - 2;
  } else {
    return size;
  }
}

/*!
	ファイルに保存
	@param[in] fileName 読み込むファイル名
	@return 0:成功, 1:失敗
*/
int StringList::saveToFile(const mx::string &fileName) {
  File file;

  //とりあえずファイルを削除する
  // remove(fileName.c_str());

  //ファイルを開く
  if (!file.open(fileName, true)) {
#ifdef __debugprint
    printf(MESS_FILE_OPEN_ERROR_SAVE, fileName.c_str());
#endif  //__debugprint
    return 1;
  }

  mx::string s;
  int size = toString(s);
  file.write((void *)s.c_str(), size);
  file.close();

#ifdef __debugprint
  printf(MESS_FILE_WRITE, "***", fileName.c_str());
#endif  //__debugprint
  return 0;
}

}  //mx
