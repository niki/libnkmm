// -*- mode:c++; coding:utf-8-ws -*-
#pragma warning(disable : 4996)

#include <windows.h>
#include "nkmm_log.h"

namespace nkmm {

/*!
	ログの出力（マルチバイト版）
	@param[in]	str
*/
void logA(const char *str) {
  OutputDebugStringA(str);
}

/*!
	ログの出力（ワイド版）
	@param[in]	str
*/
void logW(const wchar_t *str) {
  OutputDebugStringW(str);
}

/*!
	メッセージボックス（マルチバイト版）
*/
void messboxA(const char *str, const char *caption);

/*!
	メッセージボックス（ワイド版）
	@param[in]	str
*/
void messboxW(const wchar_t *str, const wchar_t *caption);

}  //nkmm
