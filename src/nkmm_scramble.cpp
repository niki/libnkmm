// -*- mode:c++; coding:utf-8-ws -*-
#include <string.h>
#include "nkmm_scramble.h"

namespace nkmm {
namespace scramble {

/*!
	データのスクランブル
	@param[in,out] data データ
	@param[in] size サイズ
	@param[in] suger シュガー
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
	データのスクランブル復元
	@param[in,out] data データ
	@param[in] size サイズ
	@param[in] suger シュガー
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
}  //nkmm
