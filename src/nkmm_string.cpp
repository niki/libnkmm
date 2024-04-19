// -*- mode:c++; coding:utf-8-ws -*-
#include <wchar.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#pragma warning(push)
#pragma warning(disable : 4996)  //C4996: This function or variable may be unsafe.

namespace nkmm {

// １バイト文字Ver.
namespace str1 {

typedef char T;
#define BASIC_STRING_MBS
#include "nkmm_basic_string.cpp"
#undef BASIC_STRING_MBS

}  //str1

// ２バイト文字Ver.
namespace str2 {

typedef wchar_t T;
#define BASIC_STRING_WCS
#include "nkmm_basic_string.cpp"
#undef BASIC_STRING_WCS

}  //str2
}  //nkmm

#pragma warning(pop)
