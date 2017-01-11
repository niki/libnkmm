// -*- mode:c++; coding:utf-8-ws -*-
//====================================================================
//! @file  rei_var.cpp
//! @brief 汎用変数型
//!
//! @author
//====================================================================
#include "rei_string.h"
#include "rei_var.h"

namespace rei {

Var::Var() : int_(0), str_(), type_(kTypeInt) {}

Var::Var(Float var) : float_(var), str_(), type_(kTypeFloat) {}

Var::Var(Int var) : int_(var), str_(), type_(kTypeInt) {}

Var::Var(Bool var) : int_((Int)var), str_(), type_(kTypeBool) {}

Var::Var(const Ptr var) : int_((Int)var), str_(), type_(kTypePointer) {}

Var::Var(const Str &var) : int_(0), str_(var), type_(kTypeString) {}

Var::Var(const char *var) : int_(0), str_(var), type_(kTypeString) {}

Var::Var(Int i, const char *var) : int_(i), str_(var), type_(kTypeInt) {}

Var &Var::operator=(const Var &rhs) {
  switch (rhs.type()) {
  case kTypeString:
    type_ = kTypeString;
    int_ = 0;
    str_.assign(rhs.str_);
    break;
  default:
    type_ = rhs.type();
    int_ = rhs.int_;
    str_.assign("");
    break;
  }
  return *this;
}

Var &Var::operator=(const Float rhs) {
  type_ = kTypeFloat;
  float_ = rhs;
  return *this;
}

Var &Var::operator=(const Int rhs) {
  type_ = kTypeInt;
  int_ = rhs;
  return *this;
}

Var &Var::operator=(const Bool rhs) {
  type_ = kTypeBool;
  int_ = (Int)rhs;
  return *this;
}

Var &Var::operator=(const Ptr rhs) {
  type_ = kTypePointer;
  int_ = (Int)rhs;
  return *this;
}

Var &Var::operator=(const Str &rhs) {
  type_ = kTypeString;
  str_.assign(rhs);
  return *this;
}

Var &Var::operator=(const char *rhs) {
  type_ = kTypeString;
  str_.assign(rhs);
  return *this;
}

/*!
* 計算（演算子の違いだけなのでマクロで定義する）
*/
#define CALCULATION(OP)                                 \
  \
{                                                  \
    switch (type()) {                                   \
    case kTypeFloat:                                    \
      if (rhs.type() == kTypeFloat) {                   \
        float_##OP = rhs.float_;                        \
      } else {                                          \
        float_##OP = (Float)rhs.int_;                   \
      }                                                 \
      break;                                            \
                                                        \
    case kTypeInt:                                      \
      if (rhs.type() == kTypeFloat) {                   \
        type_ = kTypeFloat; /*Floatに汚染される*/ \
        float_ = (Float)int_##OP rhs.float_;            \
      } else {                                          \
        int_##OP = rhs.int_;                            \
      }                                                 \
      break;                                            \
                                                        \
    case kTypeString:                                   \
      /* not support. */                                \
      break;                                            \
                                                        \
    default:                                            \
      /* not support. */                                \
      break;                                            \
    }                                                   \
  \
}

Var &Var::operator+=(const Var &rhs) {
  CALCULATION(+)
  return *this;
}

Var &Var::operator-=(const Var &rhs) {
  CALCULATION(-)
  return *this;
}

Var &Var::operator*=(const Var &rhs) {
  CALCULATION(*)
  return *this;
}

Var &Var::operator/=(const Var &rhs) {
  CALCULATION(/)
  return *this;
}

bool Var::operator==(const Var &rhs) const {
  switch (type()) {
  case kTypeBool:
    return (asBool() == rhs.asBool());
  case kTypeFloat:
    return (asFloat() == rhs.asFloat());
  case kTypeString:
    return (asString() == rhs.asString());
  default:
    return (asInt() == rhs.asInt());
  }
}

bool Var::operator!=(const Var &rhs) const {
  switch (type()) {
  case kTypeBool:
    return (asBool() != rhs.asBool());
  case kTypeFloat:
    return (asFloat() != rhs.asFloat());
  case kTypeString:
    return (asString() != rhs.asString());
  default:
    return (asInt() != rhs.asInt());
  }
}

/*!
* 比較（演算子の違いだけなのでマクロで定義する）
*/
#define COMPARE(OP)                         \
  \
{                                      \
    switch (type()) {                       \
    case kTypeFloat:                        \
      return (asFloat()##OP rhs.asFloat()); \
    case kTypeString:                       \
      return false;                         \
    default:                                \
      return (asInt()##OP rhs.asInt());     \
    }                                       \
  \
}

bool Var::operator<(const Var &rhs) const {
  COMPARE(<)
}

bool Var::operator>(const Var &rhs) const {
  COMPARE(>)
}

bool Var::operator<=(const Var &rhs) const {
  COMPARE(<=)
}

bool Var::operator>=(const Var &rhs) const {
  COMPARE(>=)
}

int Var::type() const {
  return type_;
}

Float Var::asFloat() const {
  return (type() == kTypeInt) ? (Float)int_ : float_;
}

Int Var::asInt() const {
  return (type() == kTypeFloat) ? (Int)float_ : int_;
}

Bool Var::asBool() const {
  return (asInt() != 0);
}

Str Var::asString() const {
  return Str(str_.c_str());
}

Ptr Var::asPointer() const {
  return (Ptr)int_;
}

const Str &Var::str() const {
  return str_;
}

}  //rei
