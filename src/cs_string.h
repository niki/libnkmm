/*!
* @file		mx_string.h
* @brief	std::string の代替クラス
* @author	koma
* @version	$Id$
*
* @note
* 一定のサイズ内は静的バッファで文字列操作を行う。
* サイズを超える場合はアロケートで拡張する。
*/
#ifndef MX_STRING_HPP
#define MX_STRING_HPP

namespace mx {

// １バイト文字Ver.
namespace str1 {

#define BASIC_STRING_MBS
#include "mx_basic_string.h"
#undef BASIC_STRING_MBS

}  //str1

// ２バイト文字Ver.
namespace str2 {

#define BASIC_STRING_WCS
#include "mx_basic_string.h"
#undef BASIC_STRING_WCS

}  //str2
}  //mx

namespace mx {

typedef str1::basic_string string;
typedef str2::basic_string wstring;

}  //mx

#endif  //MX_STRING_HPP
