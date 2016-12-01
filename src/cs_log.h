//-----------------------------------------------------------------------------
/*!
* @file		mx_log.h
* @brief	���O�֘A
* @author	koma
* @version	$Id$
*
* @note
* �E�X�V����  
*/
//-----------------------------------------------------------------------------
#ifndef MX_LOG_H
#define MX_LOG_H

#define __SOURCE__ __FILE__ ":" __FUNCTION__ ":" __LINE__

#define LOG(str) mx::logA(str)
#define MBOX(str) mx::messboxA(str)

namespace mx {

/*!
	���O�̏o�́i�}���`�o�C�g�Łj
	@param[in]	str
*/
void logA(const char *str);

/*!
	���O�̏o�́i���C�h�Łj
	@param[in]	str
*/
void logW(const wchar_t *str);

/*!
	���b�Z�[�W�{�b�N�X�i�}���`�o�C�g�Łj
	@param[in]	str
*/
void messboxA(const char *str, const char *caption);

/*!
	���b�Z�[�W�{�b�N�X�i���C�h�Łj
	@param[in]	str
*/
void messboxW(const wchar_t *str, const wchar_t *caption);

}  //mx

#endif  //MX_LOG_H
