// -*- mode:c++; coding:utf-8-ws -*-
//====================================================================
//! @file  rei_basic_string.cpp
//! @brief std::string の代替クラス
//!
//! @author
//====================================================================

#include "rei_basic_string.h"

#pragma warning(push)
#pragma warning(disable : 4996)  //C4996: This function or variable may be unsafe.

//------------------------------------------------------------------
// デフォルトコンストラクタ
//------------------------------------------------------------------
basic_string::basic_string()
    : ptr_(str_), length_(0), buffSize_(Local_length_max), isAlloc_(false), isMapping_(false) {
  str_[0] = 0;
}

//------------------------------------------------------------------
// コピーコンストラクタ
//------------------------------------------------------------------
basic_string::basic_string(const basic_string &str)
    : ptr_(str_), length_(0), buffSize_(Local_length_max), isAlloc_(false), isMapping_(false) {
  assign(str.c_str(), str.length());
}

//------------------------------------------------------------------
// コンストラクタ(rp=文字列, count=文字数)
//------------------------------------------------------------------
basic_string::basic_string(const CType *rp, int count)
    : ptr_(str_), length_(0), buffSize_(Local_length_max), isAlloc_(false), isMapping_(false) {
  assign(rp, count);
}

//------------------------------------------------------------------
// コンストラクタ(count=文字数)
//------------------------------------------------------------------
basic_string::basic_string(const basic_string &str, int count)
    : ptr_(str_), length_(0), buffSize_(Local_length_max), isAlloc_(false), isMapping_(false) {
  assign(str, count);
}

//------------------------------------------------------------------
// コンストラクタ(int型を文字列に設定, num=数値, format=printf書式, digits=桁数)
//------------------------------------------------------------------
basic_string::basic_string(int num, const basic_string &format, int digits)
    : ptr_(str_), length_(0), buffSize_(Local_length_max), isAlloc_(false), isMapping_(false) {
  char buf[64];

#if defined(BASIC_STRING_WCS)
  {
    char buf2[64];
    int l = format.length();
    ::wcstombs(buf2, (const wchar_t *)format.c_str(), l);
    buf2[l] = 0;

    if (digits > 0 /*&& format.findNoCase("*") >= 0*/) {
      ::sprintf(buf, buf2, digits, num);
    } else {
      ::sprintf(buf, buf2, num);
    }
  }

  {
    wchar_t buf2[64];
    int l = ::strlen(buf);
    ::mbstowcs(buf2, buf, l);
    buf2[l] = 0;
    assign((CType *)buf2);
  }
#elif defined(BASIC_STRING_MBS)
  if (digits > 0 /*&& format.findNoCase("*") >= 0*/) {
    ::sprintf(buf, (const char *)format.c_str(), digits, num);
  } else {
    ::sprintf(buf, (const char *)format.c_str(), num);
  }

  assign((CType *)buf);
#endif
}

//------------------------------------------------------------------
// コンストラクタ(double型を文字列に設定, num=数値, format=printf書式, digits=桁数)
//------------------------------------------------------------------
basic_string::basic_string(double num, const basic_string &format)
    : ptr_(str_), length_(0), buffSize_(Local_length_max), isAlloc_(false), isMapping_(false) {
  char buf[64];

#if defined(BASIC_STRING_WCS)
  {
    char buf2[64];
    int l = format.length();
    ::wcstombs(buf2, (const wchar_t *)format.c_str(), l);
    buf2[l] = 0;

    ::sprintf(buf, buf2, num);
  }

  {
    wchar_t buf2[64];
    int l = ::strlen(buf);
    ::mbstowcs(buf2, buf, l);
    buf2[l] = 0;
    assign((CType *)buf2);
  }
#elif defined(BASIC_STRING_MBS)
  ::sprintf(buf, (const char *)format.c_str(), num);
  assign((CType *)buf);
#endif
}

//------------------------------------------------------------------
// デストラクタ
//------------------------------------------------------------------
basic_string::~basic_string() {
  // @note スマートポインタを使用することでデストラクタをなくせる
  deleteBuffer();
}

//------------------------------------------------------------------
// 演算子のオーバーロード
//------------------------------------------------------------------
basic_string &basic_string::operator=(const basic_string &rhs) {
  return assign(rhs);
}

//basic_string &basic_string::operator=(const CType *rp)
//{
//	return assign(rp);
//}

//basic_string &basic_string::operator+=(const basic_string &rhs)
//{
//	return append(rhs);
//}

basic_string &basic_string::operator+=(const CType *rp) {
  return append(rp);
}
basic_string &basic_string::operator+=(const CType rch) {
  return append(rch);
}

basic_string basic_string::operator+(const basic_string &rhs) {
  basic_string s(*this);
  s.append(rhs);
  return s;
}

#if 1  //defined(_MSC_VER)
basic_string basic_string::operator+(const CType *rhs) {
  basic_string s(*this);
  s.append(rhs);
  return s;
}
#endif

//bool basic_string::operator==(const basic_string &rhs) const
//{
//	return compare(rhs) == 0;
//}

bool basic_string::operator==(const CType *rhs) const {
  return compare(rhs) == 0;
}

//bool basic_string::operator!=(const basic_string &rhs) const
//{
//	return compare(rhs) != 0;
//}

bool basic_string::operator!=(const CType *rhs) const {
  return compare(rhs) != 0;
}

// 以下、std::mapなどで使用する場合には実装が必要
//bool basic_string::operator<(const basic_string &rhs) { return compare(rhs) <  0; }
//bool basic_string::operator>(const basic_string &rhs) { return compare(rhs) >  0; }
//bool basic_string::operator<=(const basic_string &rhs) { return compare(rhs) <= 0; }
//bool basic_string::operator>=(const basic_string &rhs) { return compare(rhs) >= 0; }

const CType basic_string::operator[](int index) const {
  return c_str()[index];
}

//------------------------------------------------------------------
// キャスト演算子のオーバーロード
//------------------------------------------------------------------
basic_string::operator const CType *() const {
  return c_str();
}

//------------------------------------------------------------------
// 追加
//------------------------------------------------------------------
basic_string &basic_string::append(const CType rch) {
  int l1 = length();
  //int l2 = 1;

  extendBuffer(l1 + 1, /* copy = */ true);
  ptr_[l1] = rch;
  ptr_[l1 + 1] = 0;
  length_ = l1 + 1;

  return *this;
}

//basic_string &basic_string::append(const basic_string &rstr, int count)
//{
//	return append(rstr.c_str(), count);
//}

basic_string &basic_string::append(const CType *rp, int count) {
  if (!rp) {
    return *this;
  }

  int l1 = length();
  int l2 = (count == 0) ? strlen(rp) : count;

  extendBuffer(l1 + l2, /* copy = */ true);
  strncpy(ptr_ + l1, rp, l2);
  length_ = l1 + l2;

  return *this;
}

//------------------------------------------------------------------
// char型をアサイン
//------------------------------------------------------------------
basic_string &basic_string::assignMBS(const char *str, int count) {
// TODO: mbstowcsを使用する

// MBS to WCS
#if defined(BASIC_STRING_WCS)
  int l = ::strlen(str);
  extendBuffer(l, /* copy = */ false);
  //l_toWCSAscii((wchar_t *)ptr_, (const char *)str, l);
  ::mbstowcs(ptr_, str, l);
  ptr_[l] = 0;

  update();
  return *this;
#elif defined(BASIC_STRING_MBS)
  return assign(str, count);
#endif
}

//------------------------------------------------------------------
// wchar_t型をアサイン
//------------------------------------------------------------------
basic_string &basic_string::assignWCS(const wchar_t *str, int count) {
// TODO: wcstombsを使用する

// WCS to MBS
#if defined(BASIC_STRING_MBS)
  int l = ::wcslen(str);
  extendBuffer(l, /* copy = */ false);
  //l_toMBSAscii((char *)ptr_, (const wchar_t *)str, l);
  ::wcstombs(ptr_, str, l);
  ptr_[l] = 0;

  update();
  return *this;
#elif defined(BASIC_STRING_WCS)
  return assign(str, count);
#endif
}

//------------------------------------------------------------------
// 設定
//------------------------------------------------------------------
basic_string &basic_string::assign(const CType rch) {
  ptr_[0] = rch;
  ptr_[1] = 0;
  length_ = 1;

  return *this;
}

//basic_string &assign(const basic_string &rstr, int count)
//{
//	return assign(rstr.c_str(), count);
//}

basic_string &basic_string::assign(const CType *rp, int count) {
  if (rp) {
    int l = (count == 0) ? strlen(rp) : count;

    extendBuffer(l, /* copy = */ false);
    strncpy(ptr_, rp, l);
    length_ = l;
  } else {
    ptr_[0] = 0;
    length_ = 0;
  }
  return *this;
}

//------------------------------------------------------------------
// 比較
//------------------------------------------------------------------
//int basic_string::compare(const basic_string &rstr) const
//{
//	return strcmp(c_str(), rstr.c_str());
//}

int basic_string::compare(const CType *rp) const {
  return strcmp(c_str(), rp);
}

// 文字取得
const CType &basic_string::at(int index) const {
  return ptr_[index];
}

// C形式の文字列取得
const CType *basic_string::c_str() const {
  return (const CType *)ptr_;
}

//------------------------------------------------------------------
// C形式の文字列取得（非const）
//------------------------------------------------------------------
CType *basic_string::data() {
  length_ = 0;
  return ptr_;
}

//------------------------------------------------------------------
// 文字列が空かどうか
//------------------------------------------------------------------
bool basic_string::empty() const {
  return (length() == 0);
}

//------------------------------------------------------------------
// 文字の削除
//------------------------------------------------------------------
basic_string &basic_string::erase(int index, int count) {
  if (index == 0 && count == 0) {  // すべて消去
    ptr_[0] = 0;
  } else if (index > 0 && count == 0) {  // 指定位置から消去
    ptr_[index] = 0;
  } else {  // 指定位置から count だけ消去
    // \note 1文字から1文字を削除する場合の例外処理
    if (count == 1 && length() == 1) {
      ptr_[0] = 0;
    } else {
      // \note 1文字から1文字を削除する場合に問題あり
      basic_string s1 = subString(0, index);
      basic_string s2 = subString(index + count, length() - (index + count));
      *this = s1 + s2;
    }
  }

  update();
  return *this;
}

//------------------------------------------------------------------
// 文字列の挿入
//------------------------------------------------------------------
basic_string &basic_string::insert(int index, const CType *rp /* , int size */) {
  // @TODO 挿入後のサイズチェックを行う

  int len = strlen(rp);
  strrcpy(ptr_ + index + len, ptr_ + index, /* appendTerm = */ true);  // 後半を後ろからコピー
  strncpy(ptr_ + index, rp, len, /* appendTerm = */ false);            // 中間に rp をコピー

  update();
  return *this;
}

//------------------------------------------------------------------
// 長さ取得
//------------------------------------------------------------------
int basic_string::length() const {
  return (length_ <= 0) ? strlen(c_str()) : length_;
}

//------------------------------------------------------------------
// 設定できる文字列の最大長を取得
//------------------------------------------------------------------
int basic_string::max_size() const {
  return buffSize_ - 1;
}

//==================================================================
// 機能拡張
//==================================================================

//------------------------------------------------------------------
// バッファを削除
//------------------------------------------------------------------
void basic_string::deleteBuffer() {
  if (isAlloc_) {
    delete[] ptr_;
  }
}

//------------------------------------------------------------------
// バッファを拡張する
//------------------------------------------------------------------
void basic_string::extendBuffer(int size, bool copy) {
  int capa = max_size();

  // 負の値のときは 1 段階拡張
  if (size == -1) {
    size = capa + 1;
  }

  if (size > capa) {
    while (size > capa) {
      capa <<= 1;
      capa = ((capa + 1 /*NULL*/ + 15) & ~15);  // Round up
    }

    CType *new_ptr = new CType[capa];

    int l;

    if (copy) {
      l = length();
      strncpy(new_ptr, c_str(), l);
    } else {
      l = 0;
      new_ptr[0] = 0;
    }

    deleteBuffer();

    length_ = l;
    buffSize_ = capa;
    ptr_ = new_ptr;
    isAlloc_ = true;
    isMapping_ = false;
    //WI_LOG("*** %s: Size extendBuffer to %d, '%s'\n", __FUNCTION__, capa, new_ptr);
  }
}

//------------------------------------------------------------------
// 外部バッファをマッピングする
//------------------------------------------------------------------
void basic_string::mapBuffer(CType *p) {
  deleteBuffer();

  length_ = 0;
  buffSize_ = 0;
  ptr_ = p;
  isAlloc_ = false;
  isMapping_ = true;

  //高速化のため更新はしない update();
}

//------------------------------------------------------------------
// null または空であるか
//------------------------------------------------------------------
bool basic_string::isNullOrEmpty() const {
  return empty();
}

//------------------------------------------------------------------
// null または空であるか、空白文字だけで構成されているか
//------------------------------------------------------------------
bool basic_string::isNullOrWhiteSpace() const {
  if (isNullOrEmpty()) {
    return true;
  }

  int max_i = length();
  for (int i = 0; i < max_i; i++) {
    const CType ch = at(i);

    if (ch != static_cast<CType>(' ') && ch != static_cast<CType>('\t')) {
      // 空白文字が含まれていない時点で終了
      return false;
    }
  }

  return true;
}

//------------------------------------------------------------------
// 文字列のハッシュ値を計算
//------------------------------------------------------------------
unsigned long basic_string::hash(int index) const {
  unsigned long sum = 0;
  const CType *data = c_str();

  data += index;

#if defined(BASIC_STRING_WCS)
  while (*data) {
    sum = sum * 41 + ((*data) >> 8);
    sum = sum * 41 + ((*data) & 0xff);
    data++;
  }
#elif defined(BASIC_STRING_MBS)
  while (*data) {
    sum = sum * 41 + (*data);
    data++;
  }
#endif

  return sum;
}

//------------------------------------------------------------------
// マルチ文字を取得（ASCII以外は考慮しない）
//------------------------------------------------------------------
void basic_string::asMBSAscii(char *dest, int size) const {
  const CType *src = c_str();

  while (*src && size != 0) {
    *dest++ = (*src++) & 0xff;
    size--;
  }
  *dest = 0;
}

//------------------------------------------------------------------
// マルチ文字列を取得
//------------------------------------------------------------------
void basic_string::asMBS(char *dest) const {
//setlocale(LC_CTYPE, "jpn");

#if defined(BASIC_STRING_WCS)
  ::wcstombs(dest, c_str(), length());
  dest[length()] = 0;
#elif defined(BASIC_STRING_MBS)
  strncpy(dest, c_str(), length());
#endif  //BASIC_STRING_MBS
}
///basic_string basic_string::asMBS() const
///{
///	//setlocale(LC_CTYPE, "jpn");
///
///	basic_string s;
///	s.extendBuffer(length());
///
///#if defined(BASIC_STRING_WCS)
///	::wcstombs(s.data(), c_str(), length());
///	s.data()[length()] = 0;
///#elif defined(BASIC_STRING_MBS)
///	asMBSAscii(s.data());
///#endif
///
///	s.update();
///	return s;
///}

//------------------------------------------------------------------
// ワイド文字列を取得（ASCII以外は考慮しない）
//------------------------------------------------------------------
void basic_string::asWCSAscii(wchar_t *dest, int size) const {
  const CType *src = c_str();

  while (*src && size != 0) {
    *dest++ = (*src++) & 0xff;
    size--;
  }
  *dest = 0;
}

//------------------------------------------------------------------
// ワイド文字列を取得
//------------------------------------------------------------------
void basic_string::asWCS(wchar_t *dest) const {
//setlocale(LC_CTYPE, "jpn");

#if defined(BASIC_STRING_MBS)
  ::mbstowcs(dest, c_str(), length());
  dest[length()] = 0;
#elif defined(BASIC_STRING_WCS)
  strncpy(dest, c_str(), length());
#endif
}
///basic_string basic_string::asWCS() const
///{
///	//setlocale(LC_CTYPE, "jpn");
///
///	basic_string s;
///	s.extendBuffer(length());
///
///#if defined(BASIC_STRING_MBS)
///	::mbstowcs(s.data(), c_str(), length());
///	s.data()[length()] = 0;
///#elif defined(BASIC_STRING_WCS)
///	asWCSAscii(s.data());
///#endif
///
///	s.update();
///	return s;
///}

//------------------------------------------------------------------
// 整数へ変換
//------------------------------------------------------------------
int basic_string::asInt() const {
#if defined(BASIC_STRING_WCS)
  //-char buf[Local_length_max];
  //-asMBSAscii(buf);
  //return ::atoi(buf);
  //return (int)::strtol(buf, 0, 10);
  //-return atoi(buf);
  return atoi(c_str());
#elif defined(BASIC_STRING_MBS)
  //return ::atoi(c_str());
  //return (int)::strtol(c_str(), 0, 10);
  return atoi(c_str());
#endif
}

//------------------------------------------------------------------
// 浮動小数へ変換
//------------------------------------------------------------------
double basic_string::asFloat() const {
#if defined(BASIC_STRING_WCS)
  //-char buf[Local_length_max];
  //-asMBSAscii(buf);
  //return ::atof(buf);
  //return (double)::strtod(buf, 0);
  //-return atof(buf);
  return atof(c_str());
#elif defined(BASIC_STRING_MBS)
  //return ::atof(c_str());
  //return (double)::strtod(c_str(), 0);
  return atof(c_str());
#endif
}

//------------------------------------------------------------------
// ワイド文字かどうか
//------------------------------------------------------------------
bool basic_string::isWide() {
  return (sizeof(CType) != sizeof(char));
}

//------------------------------------------------------------------
// 大文字にする
//------------------------------------------------------------------
void basic_string::upCaseSelf() {
  //azAZ
  int max_i = length();

  for (int i = 0; i < max_i; i++) {
    const CType ch = at(i);

    if (ch >= static_cast<CType>('a') && ch <= static_cast<CType>('z')) {
      ptr_[i] = ch - 0x20 /*a - A*/;
    }
  }
}
basic_string basic_string::upCase() const {
  basic_string s = *this;
  s.upCaseSelf();
  return s;
}

//------------------------------------------------------------------
// 小文字にする
//------------------------------------------------------------------
void basic_string::downCaseSelf() const {
  //azAZ
  int max_i = length();

  for (int i = 0; i < max_i; i++) {
    const CType ch = at(i);

    if (ch >= static_cast<CType>('A') && ch <= static_cast<CType>('Z')) {
      ptr_[i] = ch + 0x20 /*a - A*/;
    }
  }
}
basic_string basic_string::downCase() const {
  basic_string s = *this;
  s.downCaseSelf();
  return s;
}

//------------------------------------------------------------------
// 文字列から文字列群に含まれる文字を取り除く
//------------------------------------------------------------------
void basic_string::remove(const basic_string &str) {
  int max_i = length();
  int cnt = 0;

  for (int i = 0; i < max_i; i++) {
    const CType ch1 = at(i);

    if (str.chr(ch1) < 0) {
      ptr_[cnt++] = ch1;
    }
  }

  ptr_[cnt] = 0;

  update();
}

//------------------------------------------------------------------
// 文字列の先頭から文字列群に含まれる文字を取り除く
//------------------------------------------------------------------
void basic_string::trimStartSelf(const basic_string &str) {
  int max_i = length();

  int i;

  for (i = 0; i < max_i; i++) {
    const CType ch1 = at(i);

    if (str.chr(ch1) < 0) {
      break;
    }
  }

  int cnt = 0;

  for (/*i = 0*/; i < max_i; i++) {
    const CType ch1 = at(i);
    ptr_[cnt++] = ch1;
  }

  ptr_[cnt] = 0;

  update();
}
basic_string basic_string::trimStart(const basic_string &str) const {
  basic_string s = *this;
  s.trimStartSelf(str);
  return s;
}

//------------------------------------------------------------------
// 文字列の末尾から文字列群に含まれる文字を取り除く
//------------------------------------------------------------------
void basic_string::trimEndSelf(const basic_string &str) {
  int max_i = length();

  for (int i = max_i - 1; i >= 0; i--) {
    const CType ch1 = at(i);

    if (str.chr(ch1) < 0) {
      break;
    } else {
      ptr_[i] = 0;
    }
  }

  update();
}
basic_string basic_string::trimEnd(const basic_string &str) const {
  basic_string s = *this;
  s.trimEndSelf(str);
  return s;
}

//------------------------------------------------------------------
// 文字列の先頭・末尾から文字列群に含まれる文字を取り除く
//------------------------------------------------------------------
void basic_string::trimSelf(const basic_string &str) {
  trimEndSelf(str);
  trimStartSelf(str);
}
basic_string basic_string::trim(const basic_string &str) const {
  basic_string s = *this;
  s.trimEndSelf(str);
  s.trimStartSelf(str);
  return s;
}

//------------------------------------------------------------------
// 文字列の抽出
//------------------------------------------------------------------
basic_string basic_string::subString(int index, int count) const {
  return basic_string(c_str() + index, count);
}

//------------------------------------------------------------------
// 指定した文字で埋める
//------------------------------------------------------------------
void basic_string::fill(CType ch) {
#if defined(BASIC_STRING_WCS)
  ::wmemset(ptr_, ch, max_size());
#elif defined(BASIC_STRING_MBS)
  ::memset(ptr_, ch, max_size());
#endif

  ptr_[max_size()] = 0;  // Just in case.

  update();
}

//------------------------------------------------------------------
// 文字列の先頭から指定した文字列を探す
//------------------------------------------------------------------
int basic_string::findNoCase(const basic_string &str, int index, CType termChar) const {
  const CType *start = c_str() + index;
  int pos = reinterpret_cast<int>(strstr(start, str.c_str()));
  pos = pos - reinterpret_cast<int>(c_str());

  if (pos < 0) {
    return -1;
  }

  //
  pos = pos / sizeof(CType);

  // ?終端文字が設定されている
  if (termChar != static_cast<CType>('\0')) {
    int termPos = chr(termChar, index);

    // 見つかった位置が終端文字よりも後だったら見つかっていない
    if (termPos < pos) {
      return -1;
    }
  }

  return pos;
}

//------------------------------------------------------------------
// 文字列の先頭から指定した文字列を探す（大文字小文字の区別なし）
//------------------------------------------------------------------
int basic_string::findCase(const basic_string &str, int index, CType termChar) const {
  basic_string a(*this);
  basic_string b(str);
  a.upCase();
  b.upCase();
  return a.findNoCase(b, index, termChar);
}

//------------------------------------------------------------------
// 文字列の先頭から文字を探す
//------------------------------------------------------------------
int basic_string::chr(const CType ch, int index) const {
  const CType *start = c_str() + index;
  int pos = reinterpret_cast<int>(strchr(start, ch));
  pos = pos - reinterpret_cast<int>(c_str());

  if (pos < 0) {
    return -1;
  }

  //
  pos = pos / sizeof(CType);

  return pos;
}

//------------------------------------------------------------------
// 文字列の末尾から文字を探す
//------------------------------------------------------------------
int basic_string::rchr(const CType ch, int index) const {
  const CType *start = c_str() + index;
  int pos = reinterpret_cast<int>(strrchr(start, ch));
  pos = pos - reinterpret_cast<int>(c_str());

  if (pos < 0) {
    return -1;
  }

  //
  pos = pos / sizeof(CType);

  return pos;
}

//------------------------------------------------------------------
// 置換
//------------------------------------------------------------------
void basic_string::replaceSelf(const basic_string &from, const basic_string &to, int num) {
  int fromlen = from.length();
  int tolen = to.length();

  for (int i = 0; i < num; i++) {
    int index = findNoCase(from);
    if (index >= 0) {
      if (fromlen < tolen) {
        // バッファ拡張テスト
        extendBuffer(length() + tolen - fromlen, /* copy = */ true);

        // 後方部分をコピーしてから差し替える
        // from=GHIJ
        // to=123
        // |ABCDEFGHIJKLMN|
        // |ABCDEF|GHI|KLMN| 後半をコピーする
        // |ABCDEF|123|KLMN| 差し替える
        strrcpy(ptr_ + index + tolen, ptr_ + index + fromlen, /* appendTerm = */ true);
        strncpy(ptr_ + index, to.c_str(), tolen, /* appendTerm = */ false);
      } else {
        // 差し替えてから後方部分をコピーする
        // from=GHIJ
        // to=123
        // |ABCDEFGHIJKLMN|
        // |ABCDEF|123|JKLMN| 差し替える
        // |ABCDEF|123|KLMN| 後半をコピーする
        strncpy(ptr_ + index, to.c_str(), tolen, /* appendTerm = */ false);
        strcpy(ptr_ + index + tolen, ptr_ + index + fromlen, /* appendTerm = */ true);
      }
    } else {
      break;
    }
  }

  update();
}
basic_string basic_string::replace(const basic_string &from, const basic_string &to,
                                   int num) const {
  basic_string s = *this;
  s.replaceSelf(from, to, num);
  return s;
}

//------------------------------------------------------------------
// 文字列から start と end で囲まれた文字列を抽出
//------------------------------------------------------------------
basic_string basic_string::extract(const basic_string &start, const basic_string &end) const {
  int pos1;
  if (start.empty()) {
    pos1 = 0;
  } else {
    pos1 = findNoCase(start, 0);
    if (pos1 < 0) {
      // 見つからない
      return basic_string();
    }
  }
  pos1 = pos1 + start.length();

  int pos2;
  if (end.empty()) {
    return subString(pos1);
  } else {
    pos2 = findNoCase(end, pos1);
    if (pos2 < 0) {
      // 見つからない
      return basic_string();
    } else {
      return subString(pos1, pos2 - pos1);
    }
  }
}

//------------------------------------------------------------------
// パスかどうか判定
//------------------------------------------------------------------
bool basic_string::isFilePath() const {
  const CType ch1 = c_str()[0];
  const CType ch2 = c_str()[1];

  if ((('a' <= ch1 && ch1 <= 'z') || ('A' <= ch1 && ch1 <= 'Z')) && ch2 == ':') {
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------
// ファイル名の取得
//------------------------------------------------------------------
basic_string basic_string::asFileName(bool ext) const {
  basic_string s = *this;
  if (!ext) {
    int pos_ext = s.rchr(static_cast<CType>('.'));
    if (pos_ext >= 0) {
      s.data()[pos_ext] = 0;
      s.update();
    }
  }

  int pos = s.rchr(static_cast<CType>('\\'));
  if (pos < 0) {
    pos = s.rchr(static_cast<CType>('/'));
  }
  if (pos >= 0) {
    return s.subString(pos + 1);
  }

  return basic_string(reinterpret_cast<CType *>(""));
}

//------------------------------------------------------------------
// ファイル名の拡張子取得（.を含む）
//------------------------------------------------------------------
basic_string basic_string::asFileExt() const {
  basic_string s = *this;
  int pos = s.rchr(static_cast<CType>('.'));
  if (pos >= 0) {
    return s.subString(pos);
  }

  return basic_string(reinterpret_cast<CType *>(""));
}

//------------------------------------------------------------------
// ファイルディレクトリの取得
//------------------------------------------------------------------
basic_string basic_string::asFileDir() const {
  basic_string s = *this;
  int pos = s.rchr(static_cast<CType>('\\'));
  if (pos < 0) {
    pos = s.rchr(static_cast<CType>('/'));
  }
  if (pos >= 0) {
    s.data()[pos] = 0;
    s.update();
    return s;
  }

  return basic_string(reinterpret_cast<CType *>(""));
}

//------------------------------------------------------------------
// ファイル拡張子の変更
//------------------------------------------------------------------
void basic_string::changeFileExt(const basic_string &ext) {
  int pos = rchr(static_cast<CType>('.'));
  if (pos >= 0) {
    strcpy(ptr_ + pos + 1, ext.c_str());
  }

  update();
}

//------------------------------------------------------------------
// ファイル名の変更
//------------------------------------------------------------------
void basic_string::changeFileName(const basic_string &fname) {
  int pos = rchr(static_cast<CType>('\\'));
  if (pos < 0) {
    pos = rchr(static_cast<CType>('/'));
  }
  if (pos >= 0) {
    strcpy(ptr_ + pos + 1, fname.c_str());
  } else {
    strcpy(ptr_, fname.c_str());
  }

  update();
}

//------------------------------------------------------------------
// タグを設定する
//------------------------------------------------------------------
void basic_string::tag(int index, unsigned int value) {
  // @todo オーバーフローチェック
  tag_[index] = value;
}

//------------------------------------------------------------------
// タグを取得する
//------------------------------------------------------------------
unsigned int basic_string::tag(int index) {
  // @todo オーバーフローチェック
  return tag_[index];
}

// 更新
void basic_string::update() {
  // 文字列の長さを計算
  length_ = strlen(c_str());
}

//------------------------------------------------------------------
// マルチバイト文字列へ強制変換（ASCII以外は考慮しない, 同一バッファ可）
//------------------------------------------------------------------
//void basic_string::l_toMBSAscii(char *dest, const wchar_t *src, int count) const
//{
//	dest[count] = 0;
//	for (int i = count - 1; i >= 0; i--) {
//		dest[i] = (src[i] & 0xff);
//	}
//}

//------------------------------------------------------------------
// ワイド文字列へ強制変換（ASCII以外は考慮しない, 同一バッファ可）
//------------------------------------------------------------------
//void basic_string::l_toWCSAscii(wchar_t *dest, const char *src, int count) const
//{
//	dest[count] = 0;
//	for (int i = count - 1; i >= 0; i--) {
//		dest[i] = src[i];
//	}
//}

//------------------------------------------------------------------
// 文字列の長さを取得
//------------------------------------------------------------------
int basic_string::strlen(const CType *s) {
  if (isWide()) {
    //const char *tail = reinterpret_cast<const char *>(::wmemchr((const wchar_t *)s, 0, max_size() + 1));
    //return (tail - reinterpret_cast<const char *>(s)) / 2;
    return ::wcslen(reinterpret_cast<const wchar_t *>(s));
  } else {
    //const char *tail = reinterpret_cast<const char *>(::memchr((const char *)s, 0, max_size() + 1));
    //return (tail - reinterpret_cast<const char *>(s));
    return ::strlen(reinterpret_cast<const char *>(s));
  }
}

//------------------------------------------------------------------
// 文字列の連結
//------------------------------------------------------------------
void basic_string::strcat(CType *dest, const CType *src) {
  strcpy(dest + strlen(dest), src);
}

//------------------------------------------------------------------
// 文字列のコピー
//------------------------------------------------------------------
void basic_string::strcpy(CType *dest, const CType *src, bool appendTerm) {
  strncpy(dest, src, strlen(src), appendTerm);
}

//------------------------------------------------------------------
// 文字列のコピー(n)
//------------------------------------------------------------------
void basic_string::strncpy(CType *dest, const CType *src, int count, bool appendTerm) {
  if (count > 0) {
    // ダイレクトにコピーするのでバッファオーバーに注意
    ::memcpy(reinterpret_cast<void *>(dest), reinterpret_cast<const void *>(src),
             count * sizeof(CType));
  }

  if (appendTerm) {
    dest[count] = 0;
  }
}

//------------------------------------------------------------------
// 文字列を後ろからコピー
//------------------------------------------------------------------
void basic_string::strrcpy(CType *dest, const CType *src, bool appendTerm) {
  strnrcpy(dest, src, strlen(src), appendTerm);
}

//------------------------------------------------------------------
// 文字列を後ろからコピー(n)
//------------------------------------------------------------------
void basic_string::strnrcpy(CType *dest, const CType *src, int count, bool appendTerm) {
  // ダイレクトにコピーするのでバッファオーバーに注意

  for (int i = count - 1; i >= 0; i--) {
    dest[i] = src[i];
  }

  //::memmove((void *)dest, (const void *)src, count * sizeof(CType));

  if (appendTerm) {
    dest[count] = 0;
  }
}

//------------------------------------------------------------------
// 文字列の比較
//------------------------------------------------------------------
int basic_string::strcmp(const CType *s1, const CType *s2) {
  int ret;

  if (isWide()) {
    ret = ::wcscmp(reinterpret_cast<const wchar_t *>(s1), reinterpret_cast<const wchar_t *>(s2));
  } else {
    ret = ::strcmp(reinterpret_cast<const char *>(s1), reinterpret_cast<const char *>(s2));
  }

  return ret;
}

//------------------------------------------------------------------
// 文字列の比較(n)
//------------------------------------------------------------------
int basic_string::strncmp(const CType *s1, const CType *s2, int count) {
  int ret;

  if (isWide()) {
    ret = ::wcsncmp(reinterpret_cast<const wchar_t *>(s1), reinterpret_cast<const wchar_t *>(s2),
                    count);
  } else {
    ret = ::strncmp(reinterpret_cast<const char *>(s1), reinterpret_cast<const char *>(s2), count);
  }

  return ret;
}

//------------------------------------------------------------------
// 文字列を探す
//------------------------------------------------------------------
const CType *basic_string::strstr(const CType *s, const CType *ptn) {
  const CType *ret;

  if (isWide()) {
    ret = reinterpret_cast<const CType *>(
        ::wcsstr(reinterpret_cast<const wchar_t *>(s), reinterpret_cast<const wchar_t *>(ptn)));
  } else {
    ret = reinterpret_cast<const CType *>(
        ::strstr(reinterpret_cast<const char *>(s), reinterpret_cast<const char *>(ptn)));
  }

  return ret;
}

//------------------------------------------------------------------
// 文字列に文字があるか前から探す
//------------------------------------------------------------------
const CType *basic_string::strchr(const CType *s, const CType ch) {
  const CType *ret;

  if (isWide()) {
    ret = reinterpret_cast<const CType *>(
        ::wcschr(reinterpret_cast<const wchar_t *>(s), static_cast<const wchar_t>(ch)));
  } else {
    ret = reinterpret_cast<const CType *>(
        ::strchr(reinterpret_cast<const char *>(s), static_cast<const char>(ch)));
  }

  return ret;
}

//------------------------------------------------------------------
// 文字列に文字があるか後ろから探す
//------------------------------------------------------------------
const CType *basic_string::strrchr(const CType *s, const CType ch) {
  const CType *ret;

  if (isWide()) {
    ret = reinterpret_cast<const CType *>(
        ::wcsrchr(reinterpret_cast<const wchar_t *>(s), static_cast<const wchar_t>(ch)));
  } else {
    ret = reinterpret_cast<const CType *>(
        ::strrchr(reinterpret_cast<const char *>(s), static_cast<const char>(ch)));
  }

  return ret;
}

//------------------------------------------------------------------
// 文字列を数値に変換
//------------------------------------------------------------------
int basic_string::atoi(const CType *s) {
  while (*s == static_cast<CType>(' ')) {
    s++;
  }  // 先頭の空白を飛ばす

  // 符号チェック
  int sign = 0;
  switch (*s) {
  case static_cast<CType>('-'):
    sign = -1;
  // fall through
  case static_cast<CType>('+'):
    s++;
    break;
  }

  int ret = 0;
  while (*s >= static_cast<CType>('0') && *s <= static_cast<CType>('9')) {
    ret = ret * 10 + *s - static_cast<CType>('0');
    s++;
  }

  return (sign != 0) ? -ret : ret;
}

//------------------------------------------------------------------
// 文字列を数値に変換
//------------------------------------------------------------------
double basic_string::atof(const CType *s) {
  while (*s == static_cast<CType>(' ')) {
    s++;
  }  // 先頭の空白を飛ばす

  // 符号チェック
  int sign = 0;
  switch (*s) {
  case static_cast<CType>('-'):
    sign = -1;
  // fall through
  case static_cast<CType>('+'):
    s++;
    break;
  }

  //
  double ret = 0;
  double decimal = 1;
  // 整数部
  while (*s >= static_cast<CType>('0') && *s <= static_cast<CType>('9')) {
    ret = ret * 10 + *s - static_cast<CType>('0');
    s++;
  }
  // 小数点
  if (*s == static_cast<CType>('.')) {
    s++;
  }
  //小数部
  while (*s >= static_cast<CType>('0') && *s <= static_cast<CType>('9')) {
    ret = ret * 10 + *s - static_cast<CType>('0');
    decimal *= 0.1;
    s++;
  }

  return ((sign != 0) ? -ret : ret) * decimal;
}

#pragma warning(pop)
