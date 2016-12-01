//-----------------------------------------------------------------------------
/*!
* @file		mx_file.cpp
* @brief	�t�@�C���Ǘ��N���X
* @author	koma
* @version	$Id$
*
* @note
* �E�X�V����  
*/
//-----------------------------------------------------------------------------
// Include
#include <stdio.h>
#include "mx_basis.h"
#include "mx_string.h"
#include "mx_file.h"

namespace mx {

/*!
	File �N���X�@�R���X�g���N�^
*/
File::File() : fp_(0), size_(0) {}

File::File(const mx::string& fileName, bool writeMode) : fp_(0), size_(0) {
  if (fileName.length() > 0) {
    open(fileName, writeMode);
  }
}

/*!
	File �N���X�@�f�X�g���N�^
*/
File::~File() {
  close();
}

/*!
	�t�@�C�����I�[�v������
	@param[in] fileName �I�[�v������t�@�C����
	@param[in] writeMode �t�@�C���I�[�v�����[�h
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

  // �t�@�C���T�C�Y�擾
  fseek(fp, 0, SEEK_END);
  size_ = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  return 1;
}

/*!
	�t�@�C�����N���[�Y����
*/
void File::close() {
  if (fp_) {
    fclose(reinterpret_cast<FILE*>(fp_));
    fp_ = 0;
  }
}

/*!
	EOF������
	@return false:EOF�ł͂Ȃ�, true:EOF
*/
bool File::isEof() const {
  if (!feof(reinterpret_cast<FILE*>(fp_))) {
    return false;
  }

  return true;
}

/*!
	�P�����ǂݍ���
	@return �擾��������
*/
int File::getChar() {
  return fgetc(reinterpret_cast<FILE*>(fp_));
}

/*!
	�P������������
	@param[in] ch �������ޕ���
	@return �������񂾕���
*/
int File::putChar(int ch) {
  return fputc(ch, reinterpret_cast<FILE*>(fp_));
}

/*!
	�P�����߂�
	@param[in] ch �߂�����
*/
void File::unGetChar(int ch) {
  ungetc(ch, reinterpret_cast<FILE*>(fp_));  // �t�@�C���|�C���^��߂�
}

/*!
	size ���ǂݍ���
	@param[out] buf �ǂݍ��ރo�b�t�@
	@param[in] size �ǂݍ��ރT�C�Y
	@return 0:���s, 1:����
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
	size ���ǂݍ��� (�o�b�t�@�̃A���P�[�g������)
	@param[out] buf �ǂݍ��ރo�b�t�@
	@return 0:���s, 1:����
*/
int File::allocRead(void** buf) {
  int size = getSize();
  *buf = SAFE_NEW_ARRAY(__SOURCE__, uint8_t, size);

  int ret = read(*buf, size);

  return ret;
}

/*!
	size ����������
	@param[in] buf �������ރo�b�t�@
	@param[in] size �������ރT�C�Y
	@return 0:���s, 1:����
*/
int File::write(void* buf, int size) {
  if (fwrite(buf, size, 1, reinterpret_cast<FILE*>(fp_)) < 1) {
    return 0;
  }

  return 1;
}

}  //mx
