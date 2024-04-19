// -*- mode:c++; coding:utf-8-ws -*-
#ifndef NKMM_VAR_H
#define NKMM_VAR_H

namespace nkmm {

typedef int Int;          //!< int型定義
typedef double Float;     //!< float型定義
typedef float Floatf;     //!< 32ビット float型定義
typedef bool Bool;        //!< boolean型定義
typedef void *Ptr;        //!< ポインタ型定義
typedef nkmm::string Str;  //!< 文字列型定義

/*!-----------------------------------------------------------------------
	汎用変数型
*/
class Var {
 public:
  // 変数の型
  enum Type {
    kTypeUnknown,  //!<
    kTypeFloat,    //!< 浮動小数型
    kTypeInt,      //!< 整数型
    kTypeBool,     //!< 真偽型
    kTypeString,   //!< 文字列型
    kTypePointer   //!< ポインタ型
  };

 public:
  Var();
  explicit Var(Float var);
  explicit Var(Int var);
  explicit Var(Bool var);
  explicit Var(const Ptr var);
  explicit Var(const Str &var);
  explicit Var(const char *var);
  Var(Int i, const char *var);

  // 代入
  Var &operator=(const Var &rhs);
  Var &operator=(const Float var);
  Var &operator=(const Int var);
  Var &operator=(const Bool var);
  Var &operator=(const Ptr var);
  Var &operator=(const Str &var);
  Var &operator=(const char *var);
  // 演算
  Var &operator+=(const Var &rhs);
  Var &operator-=(const Var &rhs);
  Var &operator*=(const Var &rhs);
  Var &operator/=(const Var &rhs);
  // 比較
  bool operator==(const Var &rhs) const;
  bool operator!=(const Var &rhs) const;
  bool operator<(const Var &rhs) const;
  bool operator>(const Var &rhs) const;
  bool operator<=(const Var &rhs) const;
  bool operator>=(const Var &rhs) const;
  // キャスト演算
  operator Int() const { return asInt(); }
  operator Float() const { return asFloat(); }
  operator Floatf() const { return asFloatf(); }
  operator Bool() const { return asBool(); }
  //	operator const Str&() const  { return asString(); }
  //	operator const char*() const  { return asString().c_str(); }
  // 取得
  int type() const;
  Int asInt() const;
  Float asFloat() const;
  Floatf asFloatf() const { return static_cast<Floatf>(asFloat()); }
  Bool asBool() const;
  Str asString() const;
  Ptr asPointer() const;
  // 取得（省略形）
  Int i() const { return asInt(); }
  Float f() const { return asFloat(); }
  Floatf ff() const { return asFloatf(); }
  const Str &str() const;
  Ptr ptr() const { return asPointer(); }

 private:
  union {
    Int int_;
    Float float_;
  };
  Str str_;
  int type_;
};

}  //nkmm

#endif  //NKMM_VAR_H
