// -*- mode:c++; coding:utf-8-ws -*-
//====================================================================
//! @file  rei_string.h
//! @brief std::string の代替クラス
//!
//! @author
//====================================================================
#ifndef REI_STRING_HPP
#define REI_STRING_HPP

namespace rei {

// １バイト文字Ver.
namespace str1 {

#define BASIC_STRING_MBS
#include "rei_basic_string.h"
#undef BASIC_STRING_MBS

}  //str1

// ２バイト文字Ver.
namespace str2 {

#define BASIC_STRING_WCS
#include "rei_basic_string.h"
#undef BASIC_STRING_WCS

}  //str2
}  //rei

namespace rei {

typedef str1::basic_string string;
typedef str2::basic_string wstring;

}  //rei

#endif  //REI_STRING_HPP
