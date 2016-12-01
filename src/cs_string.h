/*!
* @file		mx_string.h
* @brief	std::string �̑�փN���X
* @author	koma
* @version	$Id$
*
* @note
* ���̃T�C�Y���͐ÓI�o�b�t�@�ŕ����񑀍���s���B
* �T�C�Y�𒴂���ꍇ�̓A���P�[�g�Ŋg������B
*/
#ifndef MX_STRING_HPP
#define MX_STRING_HPP

namespace mx {

// �P�o�C�g����Ver.
namespace str1 {

#define BASIC_STRING_MBS
#include "mx_basic_string.h"
#undef BASIC_STRING_MBS

}  //str1

// �Q�o�C�g����Ver.
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
