//-----------------------------------------------------------------------------
/*!
* @file		mx_var.h
* @brief	�ėp�ϐ��^
* @author	koma
* @note
* �E�X�V����  
*/
//-----------------------------------------------------------------------------
#ifndef MX_VAR_H
#define MX_VAR_H

namespace mx {

typedef int Int;         //!< int�^��`
typedef double Float;    //!< float�^��`
typedef float Floatf;    //!< 32�r�b�g float�^��`
typedef bool Bool;       //!< boolean�^��`
typedef void *Ptr;       //!< �|�C���^�^��`
typedef mx::string Str;  //!< ������^��`

/*!-----------------------------------------------------------------------
	�ėp�ϐ��^
*/
class Var {
 public:
  // �ϐ��̌^
  enum Type {
    kTypeUnknown,  //!<
    kTypeFloat,    //!< ���������^
    kTypeInt,      //!< �����^
    kTypeBool,     //!< �^�U�^
    kTypeString,   //!< ������^
    kTypePointer   //!< �|�C���^�^
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

  // ���
  Var &operator=(const Var &rhs);
  Var &operator=(const Float var);
  Var &operator=(const Int var);
  Var &operator=(const Bool var);
  Var &operator=(const Ptr var);
  Var &operator=(const Str &var);
  Var &operator=(const char *var);
  // ���Z
  Var &operator+=(const Var &rhs);
  Var &operator-=(const Var &rhs);
  Var &operator*=(const Var &rhs);
  Var &operator/=(const Var &rhs);
  // ��r
  bool operator==(const Var &rhs) const;
  bool operator!=(const Var &rhs) const;
  bool operator<(const Var &rhs) const;
  bool operator>(const Var &rhs) const;
  bool operator<=(const Var &rhs) const;
  bool operator>=(const Var &rhs) const;
  // �L���X�g���Z
  operator Int() const { return asInt(); }
  operator Float() const { return asFloat(); }
  operator Floatf() const { return asFloatf(); }
  operator Bool() const { return asBool(); }
  //	operator const Str&() const  { return asString(); }
  //	operator const char*() const  { return asString().c_str(); }
  // �擾
  int type() const;
  Int asInt() const;
  Float asFloat() const;
  Floatf asFloatf() const { return static_cast<Floatf>(asFloat()); }
  Bool asBool() const;
  Str asString() const;
  Ptr asPointer() const;
  // �擾�i�ȗ��`�j
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
