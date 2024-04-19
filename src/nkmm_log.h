// -*- mode:c++; coding:utf-8-ws -*-
#ifndef NKMM_LOG_H
#define NKMM_LOG_H

#define __SOURCE__ __FILE__ ":" __FUNCTION__ ":" __LINE__

#define LOG(str) nkmm::logA(str)
#define MBOX(str) nkmm::messboxA(str)

namespace nkmm {

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

}  //nkmm

#endif  //NKMM_LOG_H
