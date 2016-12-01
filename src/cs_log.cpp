//-----------------------------------------------------------------------------
/*!
* @file		mx_log.cpp
* @brief	���O�֘A
* @author	koma
* @note
* �E�X�V����  
*/
//-----------------------------------------------------------------------------
#pragma warning(disable : 4996)

#include <windows.h>
#include "mx_log.h"

namespace mx {

/*!
	���O�̏o�́i�}���`�o�C�g�Łj
	@param[in]	str
*/
void logA(const char *str) {
  OutputDebugStringA(str);
}

/*!
	���O�̏o�́i���C�h�Łj
	@param[in]	str
*/
void logW(const wchar_t *str) {
  OutputDebugStringW(str);
}

/*!
	���b�Z�[�W�{�b�N�X�i�}���`�o�C�g�Łj
*/
void messboxA(const char *str, const char *caption);

/*!
	���b�Z�[�W�{�b�N�X�i���C�h�Łj
	@param[in]	str
*/
void messboxW(const wchar_t *str, const wchar_t *caption);

}  //mx
