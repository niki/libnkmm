// -*- mode:c++; coding:utf-8-ws -*-
//====================================================================
//! @file  rei_basic_string.h
//! @brief std::string の代替クラス
//!
//! @author
//! @note
//! 一定のサイズ内は静的バッファで文字列操作を行う。
//! サイズを超える場合はアロケートで拡張する。
//====================================================================
#if defined(BASIC_STRING_MBS)
typedef char CType;
#endif
#if defined(BASIC_STRING_WCS)
typedef wchar_t CType;
#endif

//template <int N>
class basic_string {
 private:
  //! 静的配列で持つ文字列バッファ最大長
  //! このサイズを超えるデータの場合はアロケートが発生するので注意
  static const int Local_length_max = 256;
  // vector<>.push_back(basic_string(string...)) としたとき
  // 文字列の長さが N を超えた場合、内部でアロケートが発生するので問題が起こる
  static const int Tag_max = 4;

 public:
  //------------------------------------------------------------------
  //! デフォルトコンストラクタ
  //------------------------------------------------------------------
  basic_string();

  //------------------------------------------------------------------
  //! コピーコンストラクタ
  //------------------------------------------------------------------
  explicit basic_string(const basic_string &str);

  //------------------------------------------------------------------
  //! コンストラクタ
  //! @param[in] rp 文字列
  //! @param[in] count 文字数
  //------------------------------------------------------------------
  basic_string(const CType *rp, int count = 0);

  //------------------------------------------------------------------
  //! コンストラクタ(count=文字数)
  //! @param[in] str 文字列
  //! @param[in] count 文字数
  //------------------------------------------------------------------
  basic_string(const basic_string &str, int count);

  //------------------------------------------------------------------
  //! コンストラクタ(int型を文字列に設定)
  //! @param[in] num 数値
  //! @param[in] format printf書式
  //! @param[in] digits 桁数
  //------------------------------------------------------------------
  basic_string(int num, const basic_string &format, int digits = 0);

  //------------------------------------------------------------------
  //! コンストラクタ(double型を文字列に設定)
  //! @param[in] num 数値
  //! @param[in] format printf書式
  //------------------------------------------------------------------
  basic_string(double num, const basic_string &format);

  //------------------------------------------------------------------
  //! デストラクタ
  //------------------------------------------------------------------
  ~basic_string();

  //------------------------------------------------------------------
  // 演算子のオーバーロード
  //------------------------------------------------------------------
  basic_string &operator=(const basic_string &rhs);
  //basic_string &operator=(const CType *rp);

  //basic_string &operator+=(const basic_string &rhs);
  basic_string &operator+=(const CType *rp);
  basic_string &operator+=(const CType rch);

  basic_string operator+(const basic_string &rhs);
#if 1  //defined(_MSC_VER)
  basic_string operator+(const CType *rhs);
#endif

  //bool operator==(const basic_string &rhs) const;
  bool operator==(const CType *rhs) const;
  //bool operator!=(const basic_string &rhs) const;
  bool operator!=(const CType *rhs) const;
  // 以下、std::mapなどで使用する場合には実装が必要
  //bool operator<(const basic_string &rhs);
  //bool operator>(const basic_string &rhs);
  //bool operator<=(const basic_string &rhs);
  //bool operator>=(const basic_string &rhs);

  const CType operator[](int index) const;

  //------------------------------------------------------------------
  // キャスト演算子のオーバーロード
  //------------------------------------------------------------------
  operator const CType *() const;

  //------------------------------------------------------------------
  //! 追加
  //! @param[in] rch 追加する文字
  //! @return 文字列
  //------------------------------------------------------------------
  basic_string &append(const CType rch);

  //basic_string &append(const basic_string &rstr, int count = 0);

  //------------------------------------------------------------------
  //! 追加
  //! @param[in] rp 追加する文字列
  //! @param[in] count 文字数
  //------------------------------------------------------------------
  basic_string &append(const CType *rp, int count = 0);

  //------------------------------------------------------------------
  //! 設定(char型をアサイン)
  //! @param[in] str
  //! @param[in] count
  //! @return 文字列
  //------------------------------------------------------------------
  basic_string &assignMBS(const char *str, int count = 0);

  //------------------------------------------------------------------
  //! 設定(wchar_t型をアサイン)
  //! @param[in] str
  //! @param[in] count
  //! @return 文字列
  //------------------------------------------------------------------
  basic_string &assignWCS(const wchar_t *str, int count = 0);

  //------------------------------------------------------------------
  //! 設定
  //! @param[in] rch 文字
  //! @return 文字列
  //------------------------------------------------------------------
  basic_string &assign(const CType rch);

  //basic_string &assign(const basic_string &rstr, int count = 0);

  //------------------------------------------------------------------
  //! 設定
  //! @param[in] rp 文字列
  //! @param[in] count 文字数
  //! @return 文字列
  //------------------------------------------------------------------
  basic_string &assign(const CType *rp, int count = 0);

  //------------------------------------------------------------------
  //! 比較
  //! @param[in] str 文字列
  //! @return strcmp関数を参照
  //------------------------------------------------------------------
  //int compare(const basic_string &rstr) const;

  //------------------------------------------------------------------
  //! 比較
  //! @param[in] rp 文字列
  //! @return strcmp関数を参照
  //------------------------------------------------------------------
  int compare(const CType *rp) const;

  //------------------------------------------------------------------
  //! 文字取得
  //! @param[in] index 取得する文字のインデックス
  //! @return 文字
  //------------------------------------------------------------------
  const CType &at(int index) const;

  //------------------------------------------------------------------
  //! C形式の文字列取得
  //! @return C形式の文字列
  //------------------------------------------------------------------
  const CType *c_str() const;

  //------------------------------------------------------------------
  //! C形式の文字列取得（非const）
  //! @return C形式の文字列
  //!
  //! @note
  //! この関数を呼び出すと、length()は再計算されます。
  //------------------------------------------------------------------
  CType *data();

  //------------------------------------------------------------------
  //! 文字列が空かどうか
  //! @return true 空,
  //!         false Not空
  //------------------------------------------------------------------
  bool empty() const;

  //------------------------------------------------------------------
  //! 文字の削除
  //! @param[in] index 位置
  //! @param[in] count 長さ
  //! @return 文字列
  //------------------------------------------------------------------
  basic_string &erase(int index = 0, int count = 0);

  //------------------------------------------------------------------
  //! 文字列の挿入
  //! @param[in] index
  //! @param[in] rp
  //! @return 文字列
  //------------------------------------------------------------------
  basic_string &insert(int index, const CType *rp /* , int size */);

  //------------------------------------------------------------------
  //! 長さ取得
  //! @return 長さ
  //------------------------------------------------------------------
  int length() const;

  //------------------------------------------------------------------
  //! 設定できる文字列の最大長を取得
  //! @return 最大長
  //------------------------------------------------------------------
  int max_size() const;

  // std::string互換
  int find(const basic_string &str, int index = 0) const {
    return findNoCase(str, index, (CType)'\0');
  }
  int find_ci(const basic_string &str, int index = 0) const {
    return findCase(str, index, (CType)'\0');
  }
  int find_first_of(const basic_string &str, int index = 0) const {
    return findNoCase(str, index, (CType)'\0');
  }
  int find_ci_first_of(const basic_string &str, int index = 0) const {
    return findCase(str, index, (CType)'\0');
  }

  //==================================================================
  // 機能拡張
  //==================================================================

  //------------------------------------------------------------------
  //! バッファを削除する
  //------------------------------------------------------------------
  void deleteBuffer();

  //------------------------------------------------------------------
  //! バッファを拡張する
  //! @param[in] size 変更するサイズ(負の値で 1 段階アップ)
  //! @param[in] copy 内容をコピーする
  //! @note
  //! 変更するサイズは 現在のサイズ * 2 の値
  //------------------------------------------------------------------
  void extendBuffer(int size = -1, bool copy = false);

  //------------------------------------------------------------------
  //! 外部バッファをマッピングする
  //! @param[in,out] P 文字列ポインタ
  //------------------------------------------------------------------
  void mapBuffer(CType *p);

  //------------------------------------------------------------------
  //! null または空であるか
  //------------------------------------------------------------------
  bool isNullOrEmpty() const;

  //------------------------------------------------------------------
  //! null または空であるか、空白文字だけで構成されているか
  //------------------------------------------------------------------
  bool isNullOrWhiteSpace() const;

  //------------------------------------------------------------------
  //! 文字列のハッシュ値を計算
  //! @param[in] 文字列
  //! @return 32bitハッシュ値
  //------------------------------------------------------------------
  unsigned long hash(int index = 0) const;

  //------------------------------------------------------------------
  //! マルチ文字を取得（ASCII以外は考慮しない）
  //! @param[out] dest 出力バッファ
  //! @warning 現在、ASCII以外の文字は切り捨てられてしまうので注意
  //------------------------------------------------------------------
  void asMBSAscii(char *dest, int size = -1) const;

  //------------------------------------------------------------------
  //! マルチ文字列を取得
  //! @param[in] dest 出力バッファ
  //------------------------------------------------------------------
  void asMBS(char *dest) const;
  //basic_string asMBS() const;

  //------------------------------------------------------------------
  //! ワイド文字列を取得（ASCII以外は考慮しない）
  //! @warning ASCII以外は想定している変換にはならない
  //------------------------------------------------------------------
  void asWCSAscii(wchar_t *dest, int size = -1) const;

  //------------------------------------------------------------------
  //! ワイド文字列を取得
  //! @param[in] dest 出力バッファ
  //------------------------------------------------------------------
  void asWCS(wchar_t *dest) const;
  //basic_string asWCS() const;

  //------------------------------------------------------------------
  //! 整数へ変換
  //! @return 整数値
  //! @warning 不正値の考慮しない
  //------------------------------------------------------------------
  int asInt() const;

  //------------------------------------------------------------------
  //! 浮動小数へ変換
  //! @return 浮動小数値
  //! @warning 不正値の考慮しない
  //------------------------------------------------------------------
  double asFloat() const;

  //------------------------------------------------------------------
  //! ワイド文字かどうか
  //------------------------------------------------------------------
  static bool isWide();

  //------------------------------------------------------------------
  // 大文字にする
  //------------------------------------------------------------------
  void upCaseSelf();
  basic_string upCase() const;

  //------------------------------------------------------------------
  // 小文字にする
  //------------------------------------------------------------------
  void downCaseSelf() const;
  basic_string downCase() const;

  //------------------------------------------------------------------
  //! 文字列から文字列群に含まれる文字を取り除く
  //! @param[in] str 文字列群
  //------------------------------------------------------------------
  void remove(const basic_string &str);

  //------------------------------------------------------------------
  //! 文字列の先頭から文字列群に含まれる文字を取り除く
  //! @param[in] str 文字列群
  //------------------------------------------------------------------
  void trimStartSelf(const basic_string &str);
  basic_string trimStart(const basic_string &str) const;

  //------------------------------------------------------------------
  //! 文字列の末尾から文字列群に含まれる文字を取り除く
  //! @param[in] str 文字列群
  //------------------------------------------------------------------
  void trimEndSelf(const basic_string &str);
  basic_string trimEnd(const basic_string &str) const;

  //------------------------------------------------------------------
  //! 文字列の先頭・末尾から文字列群に含まれる文字を取り除く
  //! @param[in] str 文字列群
  //------------------------------------------------------------------
  void trimSelf(const basic_string &str);
  basic_string trim(const basic_string &str) const;

  //------------------------------------------------------------------
  //! 文字列の抽出
  //! @param[in] index
  //! @param[in] count 文字数。0指定の場合は最後まで
  //! @return 文字列
  //------------------------------------------------------------------
  basic_string subString(int index, int count = 0) const;

  //------------------------------------------------------------------
  //! 指定した文字で埋める
  //------------------------------------------------------------------
  void fill(CType ch);

  //------------------------------------------------------------------
  //! 文字列の先頭から指定した文字列を探す
  //! @param[in] str 探す文字列
  //! @param[in] index
  //! @param[in] termChar 終端文字
  //! @return 見つかった位置（インデックス）
  //------------------------------------------------------------------
  int findNoCase(const basic_string &str, int index = 0, CType termChar = 0) const;

  //------------------------------------------------------------------
  //! 文字列の先頭から指定した文字列を探す（大文字小文字の区別なし）
  //! @param[in] str 探す文字列
  //! @param[in] index
  //! @param[in] termChar 終端文字
  //! @return 見つかった位置（インデックス）
  //------------------------------------------------------------------
  int findCase(const basic_string &str, int index = 0, CType termChar = 0) const;

  //------------------------------------------------------------------
  //! 文字列の先頭から文字を探す
  //! @param[in] ch 文字
  //! @param[in] index
  //! @return 位置
  //------------------------------------------------------------------
  int chr(const CType ch, int index = 0) const;

  //------------------------------------------------------------------
  //! 文字列の末尾から文字を探す
  //! @param[in] ch 文字
  //! @param[in] index
  //! @return 位置
  //------------------------------------------------------------------
  int rchr(const CType ch, int index = 0) const;

  //------------------------------------------------------------------
  //! 置換
  //! @param[in] from 置換元文字列
  //! @param[in] to 置換元先字列
  //! @param[in] num 置換回数
  //------------------------------------------------------------------
  void replaceSelf(const basic_string &from, const basic_string &to, int num = 1);
  basic_string replace(const basic_string &from, const basic_string &to, int num = 1) const;

  //------------------------------------------------------------------
  //! 文字列から start と end で囲まれた文字列を抽出
  //! @param[in] start 始端
  //! @param[in] end 終端（nullのときは最後）
  //! @return 抽出された文字列
  //------------------------------------------------------------------
  basic_string extract(const basic_string &start, const basic_string &end) const;

  //------------------------------------------------------------------
  //! パスかどうか判定
  //! @return true:パス, false:パスじゃない
  //------------------------------------------------------------------
  bool isFilePath() const;

  //------------------------------------------------------------------
  //! ファイル名の取得
  //------------------------------------------------------------------
  basic_string asFileName(bool ext = true) const;

  //------------------------------------------------------------------
  //! ファイル名の拡張子取得（.を含む）
  //------------------------------------------------------------------
  basic_string asFileExt() const;

  //------------------------------------------------------------------
  //! ファイルディレクトリの取得
  //! @param[in] dir 変更後のディレクトリ
  //------------------------------------------------------------------
  basic_string asFileDir() const;

  //------------------------------------------------------------------
  //! ファイル拡張子の変更
  //! @param[in] ext 変更後の拡張子
  //------------------------------------------------------------------
  void changeFileExt(const basic_string &ext);

  //------------------------------------------------------------------
  //! ファイル名の変更
  //! @param[in] fname 変更後のファイル名
  //------------------------------------------------------------------
  void changeFileName(const basic_string &fname);

  //------------------------------------------------------------------
  //! タグを設定する
  //! @param[in] index
  //! @param[in] value
  //------------------------------------------------------------------
  void tag(int index, unsigned int value);

  //------------------------------------------------------------------
  //! タグを取得する
  //! @param[in] index
  //------------------------------------------------------------------
  unsigned int tag(int index);

  //------------------------------------------------------------------
  //! 更新
  //------------------------------------------------------------------
  void update();

 private:
  CType *ptr_;
  int length_;                   // 長さ
  int buffSize_;                 // バッファサイズ
  CType str_[Local_length_max];  // ローカルバッファ
  bool isAlloc_;                 // アロケートフラグ
  bool isMapping_;
  unsigned int tag_[Tag_max];

 public:
  //! マルチバイト文字列へ強制変換（ASCII以外は考慮しない, 同一バッファ可）
  //void l_toMBSAscii(char *dest, const wchar_t *src, int count) const;

  //! ワイド文字列へ強制変換（ASCII以外は考慮しない, 同一バッファ可）
  //void l_toWCSAscii(wchar_t *dest, const char *src, int count) const;

  /// 標準関数の char, wchar_t 対応版
  static int strlen(const CType *s);
  static void strcat(CType *dest, const CType *src);
  static void strcpy(CType *dest, const CType *src, bool appendTerm = true);
  static void strncpy(CType *dest, const CType *src, int count, bool appendTerm = true);
  static void strrcpy(CType *dest, const CType *src, bool appendTerm = true);
  static void strnrcpy(CType *dest, const CType *src, int count, bool appendTerm = true);
  static int strcmp(const CType *s1, const CType *s2);
  static int strncmp(const CType *s1, const CType *s2, int count);
  static const CType *strstr(const CType *s, const CType *ptn);
  static const CType *strchr(const CType *s, const CType ch);
  static const CType *strrchr(const CType *s, const CType ch);
  static int atoi(const CType *s);
  static double atof(const CType *s);
};

// 型定義
//typedef basic_string<1000> string;
//typedef basic_string string;

// 演算子のオーバーロード
#if !defined(_MSC_VER)
inline basic_string operator+(const basic_string &lhs, const char *rhs) {
  basic_string s(lhs);
  s.append(reinterpret_cast<CType *>(rhs));
  return s;
}
//inline wstring operator+(const wstring &lhs, const wchar_t *rhs) {
//  wstring s(lhs);
//  s.append(rhs);
//  return s;
//}
#endif
inline basic_string operator+(const CType *lhs, const basic_string &rhs) {
  basic_string s(lhs);
  s.append(rhs.c_str());
  return s;
}
//inline wstring operator+(const wchar_t *lhs, const wstring &rhs) {
//  wstring s(lhs);
//  s.append(rhs.c_str());
//  return s;
//}

//typedef string String;
//typedef wstring WString;
