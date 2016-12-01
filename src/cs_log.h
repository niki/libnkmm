//-----------------------------------------------------------------------------
/*!
* @file		mx_log.h
* @brief	ログ関連
* @author	koma
* @version	$Id$
*
* @note
* ・更新履歴  
*/
//-----------------------------------------------------------------------------
#ifndef MX_LOG_H
#define MX_LOG_H

#define __SOURCE__ __FILE__ ":" __FUNCTION__ ":" __LINE__

#define LOG(str) mx::logA(str)
#define MBOX(str) mx::messboxA(str)

namespace mx {

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

}  //mx

#endif  //MX_LOG_H
