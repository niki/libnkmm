// -*- mode:c++; coding:utf-8-ws -*-
//====================================================================
//! @file  rei_log.cpp
//! @brief ログ関連
//!
//! @author
//====================================================================
#pragma warning(disable : 4996)

#include <windows.h>
#include "rei_log.h"

namespace rei {

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

}  //rei
