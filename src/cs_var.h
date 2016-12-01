//-----------------------------------------------------------------------------
/*!
* @file		mx_var.h
* @brief	”Ä—p•Ï”Œ^
* @author	koma
* @note
* EXV—š—ğ  
*/
//-----------------------------------------------------------------------------
#ifndef MX_VAR_H
#define MX_VAR_H

namespace mx {

typedef int Int;         //!< intŒ^’è‹`
typedef double Float;    //!< floatŒ^’è‹`
typedef float Floatf;    //!< 32ƒrƒbƒg floatŒ^’è‹`
typedef bool Bool;       //!< booleanŒ^’è‹`
typedef void *Ptr;       //!< ƒ|ƒCƒ“ƒ^Œ^’è‹`
typedef mx::string Str;  //!< •¶š—ñŒ^’è‹`

/*!-----------------------------------------------------------------------
	”Ä—p•Ï”Œ^
*/
class Var {
 public:
  // •Ï”‚ÌŒ^
  enum Type {
    kTypeUnknown,  //!<
    kTypeFloat,    //!< •‚“®¬”Œ^
    kTypeInt,      //!< ®”Œ^
    kTypeBool,     //!< ^‹UŒ^
    kTypeString,   //!< •¶š—ñŒ^
    kTypePointer   //!< ƒ|ƒCƒ“ƒ^Œ^
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

  // ‘ã“ü
  Var &operator=(const Var &rhs);
  Var &operator=(const Float var);
  Var &operator=(const Int var);
  Var &operator=(const Bool var);
  Var &operator=(const Ptr var);
  Var &operator=(const Str &var);
  Var &operator=(const char *var);
  // ‰‰Z
  Var &operator+=(const Var &rhs);
  Var &operator-=(const Var &rhs);
  Var &operator*=(const Var &rhs);
  Var &operator/=(const Var &rhs);
  // ”äŠr
  bool operator==(const Var &rhs) const;
  bool operator!=(const Var &rhs) const;
  bool operator<(const Var &rhs) const;
  bool operator>(const Var &rhs) const;
  bool operator<=(const Var &rhs) const;
  bool operator>=(const Var &rhs) const;
  // ƒLƒƒƒXƒg‰‰Z
  operator Int() const { return asInt(); }
  operator Float() const { return asFloat(); }
  operator Floatf() const { return asFloatf(); }
  operator Bool() const { return asBool(); }
  //	operator const Str&() const  { return asString(); }
  //	operator const char*() const  { return asString().c_str(); }
  // æ“¾
  int type() const;
  Int asInt() const;
  Float asFloat() const;
  Floatf asFloatf() const { return static_cast<Floatf>(asFloat()); }
  Bool asBool() const;
  Str asString() const;
  Ptr asPointer() const;
  // æ“¾iÈ—ªŒ`j
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
