//-----------------------------------------------------------------------------
/*!
* @file		mx_file.h
* @brief	�t�@�C���Ǘ��N���X
* @author	koma
* @version	$Id$
* 
* @note
* �E�X�V����  
*/
//-----------------------------------------------------------------------------
#ifndef MX_FILE_H
#define MX_FILE_H

namespace mx {

//class String;

class File {
 public:
  File();
  File(const mx::string& fileName, bool writeMode = false);
  virtual ~File();

  int open(const mx::string& fileName, bool writeMode = false);
  void close();

  uint32_t getLastModified();  // �ŏI�X�V���̎擾
  int getSize() const { return size_; }

  bool isEof() const;

  int getChar();
  int putChar(int ch);

  void unGetChar(int ch);

  int read(void* buf, int size = -1);
  int allocRead(void** buf);

  int write(void* buf, int size);

 private:
  uint32_t fp_;  //!< �t�@�C���|�C���^
  int size_;     //!< �T�C�Y
};

}  //mx

#endif  //MX_FILE_H
