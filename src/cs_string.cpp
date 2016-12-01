/*!
* @file		mx_string.cpp
* @brief	std::string の代替クラス
* @author	koma
* @version	$Id$
*
* @note
* 一定のサイズ内は静的バッファで文字列操作を行う。
* サイズを超える場合はアロケートで拡張する。
*/
#include <wchar.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#pragma warning(push)
#pragma warning(disable : 4996)  //C4996: This function or variable may be unsafe.

namespace mx {

// １バイト文字Ver.
namespace str1 {

typedef char T;
#define BASIC_STRING_MBS
#include "mx_basic_string.cpp"
#undef BASIC_STRING_MBS

}  //str1

// ２バイト文字Ver.
namespace str2 {

typedef wchar_t T;
#define BASIC_STRING_WCS
#include "mx_basic_string.cpp"
#undef BASIC_STRING_WCS

}  //str2
}  //mx

#pragma warning(pop)
