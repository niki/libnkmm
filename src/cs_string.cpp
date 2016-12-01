/*!
* @file		mx_string.cpp
* @brief	std::string �̑�փN���X
* @author	koma
* @version	$Id$
*
* @note
* ���̃T�C�Y���͐ÓI�o�b�t�@�ŕ����񑀍���s���B
* �T�C�Y�𒴂���ꍇ�̓A���P�[�g�Ŋg������B
*/
#include <wchar.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#pragma warning(push)
#pragma warning(disable : 4996)  //C4996: This function or variable may be unsafe.

namespace mx {

// �P�o�C�g����Ver.
namespace str1 {

typedef char T;
#define BASIC_STRING_MBS
#include "mx_basic_string.cpp"
#undef BASIC_STRING_MBS

}  //str1

// �Q�o�C�g����Ver.
namespace str2 {

typedef wchar_t T;
#define BASIC_STRING_WCS
#include "mx_basic_string.cpp"
#undef BASIC_STRING_WCS

}  //str2
}  //mx

#pragma warning(pop)
