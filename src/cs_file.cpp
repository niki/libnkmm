//-----------------------------------------------------------------------------
/*!
* @file		mx_file.cpp
* @brief	ファイル管理クラス
* @author	koma
* @version	$Id$
*
* @note
* ・更新履歴  
*/
//-----------------------------------------------------------------------------
// Include
#include <stdio.h>
#include "mx_basis.h"
#include "mx_string.h"
#include "mx_file.h"

namespace mx {

/*!
	File クラス　コンストラクタ
*/
File::File() : fp_(0), size_(0) {}

File::File(const mx::string& fileName, bool writeMode) : fp_(0), size_(0) {
  if (fileName.length() > 0) {
    open(fileName, writeMode);
  }
}

/*!
	File クラス　デストラクタ
*/
File::~File() {
  close();
}

/*!
	ファイルをオープンする
	@param[in] fileName オープンするファイル名
	@param[in] writeMode ファイルオープンモード
	@return 1
*/
int File::open(const mx::string& fileName, bool writeMode) {
  FILE* fp;

  if (writeMode) {
    if ((fp = fopen(fileName.c_str(), "wb")) == NULL) {
      return 0;
    }
  } else {
    if ((fp = fopen(fileName.c_str(), "rb")) == NULL) {
      return 0;
    }
  }

  fp_ = reinterpret_cast<uint32_t>(fp);

  // ファイルサイズ取得
  fseek(fp, 0, SEEK_END);
  size_ = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  return 1;
}

/*!
	ファイルをクローズする
*/
void File::close() {
  if (fp_) {
    fclose(reinterpret_cast<FILE*>(fp_));
    fp_ = 0;
  }
}

/*!
	EOFか判定
	@return false:EOFではない, true:EOF
*/
bool File::isEof() const {
  if (!feof(reinterpret_cast<FILE*>(fp_))) {
    return false;
  }

  return true;
}

/*!
	１文字読み込む
	@return 取得した文字
*/
int File::getChar() {
  return fgetc(reinterpret_cast<FILE*>(fp_));
}

/*!
	１文字書き込む
	@param[in] ch 書き込む文字
	@return 書き込んだ文字
*/
int File::putChar(int ch) {
  return fputc(ch, reinterpret_cast<FILE*>(fp_));
}

/*!
	１文字戻す
	@param[in] ch 戻す文字
*/
void File::unGetChar(int ch) {
  ungetc(ch, reinterpret_cast<FILE*>(fp_));  // ファイルポインタを戻す
}

/*!
	size 分読み込む
	@param[out] buf 読み込むバッファ
	@param[in] size 読み込むサイズ
	@return 0:失敗, 1:成功
*/
int File::read(void* buf, int size) {
  if (size < 0) {
    size = getSize();
  }

  if (fread(buf, size, 1, reinterpret_cast<FILE*>(fp_)) < 1) {
    return 0;
  }

  return 1;
}

/*!
	size 分読み込む (バッファのアロケートをする)
	@param[out] buf 読み込むバッファ
	@return 0:失敗, 1:成功
*/
int File::allocRead(void** buf) {
  int size = getSize();
  *buf = SAFE_NEW_ARRAY(__SOURCE__, uint8_t, size);

  int ret = read(*buf, size);

  return ret;
}

/*!
	size 分書き込む
	@param[in] buf 書き込むバッファ
	@param[in] size 書き込むサイズ
	@return 0:失敗, 1:成功
*/
int File::write(void* buf, int size) {
  if (fwrite(buf, size, 1, reinterpret_cast<FILE*>(fp_)) < 1) {
    return 0;
  }

  return 1;
}

}  //mx
