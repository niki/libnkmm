// -*- mode:c++; coding:utf-8-ws -*-
#ifndef NKMM_STRING_HPP
#define NKMM_STRING_HPP

namespace nkmm {

// １バイト文字Ver.
namespace str1 {

#define BASIC_STRING_MBS
#include "nkmm_basic_string.h"
#undef BASIC_STRING_MBS

}  //str1

// ２バイト文字Ver.
namespace str2 {

#define BASIC_STRING_WCS
#include "nkmm_basic_string.h"
#undef BASIC_STRING_WCS

}  //str2
}  //nkmm

namespace nkmm {

typedef str1::basic_string string;
typedef str2::basic_string wstring;

}  //nkmm

#endif  //NKMM_STRING_HPP
