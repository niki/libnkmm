//-----------------------------------------------------------------------------
/*!
* @file		mx_var.h
* @brief	ÄpÏ^
* @author	koma
* @note
* EXVð  
*/
//-----------------------------------------------------------------------------
#ifndef MX_VAR_H
#define MX_VAR_H

namespace mx {

typedef int Int;         //!< int^è`
typedef double Float;    //!< float^è`
typedef float Floatf;    //!< 32rbg float^è`
typedef bool Bool;       //!< boolean^è`
typedef void *Ptr;       //!< |C^^è`
typedef mx::string Str;  //!< ¶ñ^è`

/*!-----------------------------------------------------------------------
	ÄpÏ^
*/
class Var {
 public:
  // ÏÌ^
  enum Type {
    kTypeUnknown,  //!<
    kTypeFloat,    //!< ®¬^
    kTypeInt,      //!< ®^
    kTypeBool,     //!< ^U^
    kTypeString,   //!< ¶ñ^
    kTypePointer   //!< |C^^
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

  // ãü
  Var &operator=(const Var &rhs);
  Var &operator=(const Float var);
  Var &operator=(const Int var);
  Var &operator=(const Bool var);
  Var &operator=(const Ptr var);
  Var &operator=(const Str &var);
  Var &operator=(const char *var);
  // Z
  Var &operator+=(const Var &rhs);
  Var &operator-=(const Var &rhs);
  Var &operator*=(const Var &rhs);
  Var &operator/=(const Var &rhs);
  // är
  bool operator==(const Var &rhs) const;
  bool operator!=(const Var &rhs) const;
  bool operator<(const Var &rhs) const;
  bool operator>(const Var &rhs) const;
  bool operator<=(const Var &rhs) const;
  bool operator>=(const Var &rhs) const;
  // LXgZ
  operator Int() const { return asInt(); }
  operator Float() const { return asFloat(); }
  operator Floatf() const { return asFloatf(); }
  operator Bool() const { return asBool(); }
  //	operator const Str&() const  { return asString(); }
  //	operator const char*() const  { return asString().c_str(); }
  // æ¾
  int type() const;
  Int asInt() const;
  Float asFloat() const;
  Floatf asFloatf() const { return static_cast<Floatf>(asFloat()); }
  Bool asBool() const;
  Str asString() const;
  Ptr asPointer() const;
  // æ¾iÈª`j
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

}  //mx

#endif  //MX_VAR_H
