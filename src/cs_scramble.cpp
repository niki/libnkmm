//-----------------------------------------------------------------------------
/*!
* @file		mx_scramble.cpp
* @brief	Scramble
* @author	koma
* @version	$Id:$
*
* @note
* �E�X�V����  
*/
//-----------------------------------------------------------------------------
#include <string.h>
#include "mx_scramble.h"

namespace mx {
namespace scramble {

/*!
	�f�[�^�̃X�N�����u��
	@param[in,out] data �f�[�^
	@param[in] size �T�C�Y
	@param[in] suger �V���K�[
*/
void Encode(char *data, int size, const char *sugar) {
  int l = strlen(sugar);
  char x = 0xa5 + size / 1024;
  for (int i = 0; i < size; ++i) {
    char d = data[i];
    data[i] ^= (x + sugar[i % l]);
    x ^= d;
    x += 37;
  }
}

/*!
	�f�[�^�̃X�N�����u������
	@param[in,out] data �f�[�^
	@param[in] size �T�C�Y
	@param[in] suger �V���K�[
*/
void Decode(char *data, int size, const char *sugar) {
  int l = strlen(sugar);
  char x = 0xa5 + size / 1024;
  for (int i = 0; i < size; ++i) {
    data[i] ^= (x + sugar[i % l]);
    x ^= data[i];
    x += 37;
  }
}

}  //scramble
}  //mx
