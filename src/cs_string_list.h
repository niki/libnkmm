//-----------------------------------------------------------------------------
/*!
* @file		mx_stringlist.h
* @brief	mx::string ���X�g����
* @author	koma
* @version	$Id$
* 
* @note
* �E�X�V����  
*/
//-----------------------------------------------------------------------------
#ifndef MX_STRINGLIST_H
#define MX_STRINGLIST_H

//Include
#include "mx_vector.hpp"
#include "mx_string.h"  //�����񏈗��N���X

//! ���s�R�[�h���g�p����
//! �g�p����ꍇ�A�Z�[�u���͍l������Ă��Ȃ��̂Ŏ������K�v
#define USE_STRINGLIST_RETURNCODE 0

namespace mx {

class File;

//���s�^�C�v
enum {
  kEolEof = 0,  //0x00
  kEolCr,       //CR 0x0d
  kEolLf,       //LF 0x0a
  kEolCrlf      //CRLF 0x0d 0x0a
};

//���s�T�C�Y
enum {
  kSizeEolEof = 0,
  kSizeEolCr = 1,   //CR
  kSizeEolLf = 1,   //LF
  kSizeEolCrlf = 2  //CRLF
};

//! StringList �����񃊃X�g�N���X
class StringList {
 public:
  StringList();
  virtual ~StringList();

  mx::string &basic_Add(const mx::string &rp);
  mx::string &basic_Add(const mx::string &rp, int len);
  mx::string &basic_Assign(int index, const mx::string &rp);
  mx::string &basic_Assign(int index, const mx::string &rp, int len);
  mx::string &basic_Concatenate(int index, const mx::string &rp);
  mx::string &basic_Concatenate(int index, const mx::string &rp, int len);
  mx::string &basic_Insert(int index, const mx::string &rp);
  mx::string &basic_Insert(int index, const mx::string &rp, int len);

  // �ϒ������Ή���
  //	mx::string		&add(const mx::string &fmt, ...);
  //	mx::string		&assign(int index, const mx::string &fmt, ...);
  //	mx::string		&concatenate(int index, const mx::string &fmt, ...);
  //	mx::string		&insert(int index, const mx::string &fmt, ...);
  mx::string &add(const char *fmt, ...);
  mx::string &assign(int index, const char *fmt, ...);
  mx::string &concatenate(int index, const char *fmt, ...);
  mx::string &insert(int index, const char *fmt, ...);

  void setReturnCode(int type);
  int getReturnCode();

  int count() const;

  void deleteLine(int index);

  // �󔒍s���폜
  void deleteBlankLines();

  // �s�R�����g�������폜����
  void removeLineComment(const mx::string &lineCommentWord);

  void clear();
  void free();

  // �t�@�C���̓ǂݍ���
  int loadFromFile(const mx::string &fileName, bool concat = false);

  // �t�@�C���ɕۑ�
  int saveToFile(const mx::string &fileName);

  // �\�[�g
  virtual void sort(int (*fnCmp)(const mx::string &, const mx::string &));
  virtual void sort(int sortUp = 1);
  virtual void lengthSort(int sortUp = 1);

  virtual void fromText(const char *data,
                        int size = 0x7FFFFFFF);  // ������f�[�^���烊�X�g�f�[�^���쐬
  virtual int toString(mx::string &out,
                       const char delimiter = -1);  // ���X�g�f�[�^���當����f�[�^���쐬

 public:
#ifdef BCC32
  typename
#endif
      mx::vector<mx::string>
          Strings;

 protected:
#if USE_STRINGLIST_RETURNCODE
#ifdef BCC32
  typename
#endif
      mx::vector<int>
          returnCodes_;
#endif  //USE_STRINGLIST_RETURNCODE

 private:
  int returnCode_;
};

}  //mx

#endif  //MX_STRINGLIST_H
