// -*- mode:c++; coding:utf-8-ws -*-
//====================================================================
//! @file  rei_log.h
//! @brief ログ関連
//!
//! @author
//====================================================================
#ifndef REI_LOG_H
#define REI_LOG_H

#define __SOURCE__ __FILE__ ":" __FUNCTION__ ":" __LINE__

#define LOG(str) rei::logA(str)
#define MBOX(str) rei::messboxA(str)

namespace rei {

/*!
	ログの出力（マルチバイト版）
	@param[in]	str
*/
void logA(const char *str);

/*!
	ログの出力（ワイド版）
	@param[in]	str
*/
void logW(const wchar_t *str);

/*!
	メッセージボックス（マルチバイト版）
	@param[in]	str
*/
void messboxA(const char *str, const char *caption);

/*!
	メッセージボックス（ワイド版）
	@param[in]	str
*/
void messboxW(const wchar_t *str, const wchar_t *caption);

}  //rei

#endif  //REI_LOG_H
