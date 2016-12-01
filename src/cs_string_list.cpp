//-----------------------------------------------------------------------------
/*!
* @file		mx_stringlist.cpp
* @brief	mx::string ���X�g����
* @author	koma
* @version	$Id$
* 
* @note
* �E�X�V����  
* 2013.08.18	�V�K�ɍ쐬���� basic_string �N���X�̊Ǘ��ɕύX  
* 2007.04.24	fopen() �� mx::File �ɏ�������  
* 2006.04.29	�v���v���Z�b�T�֌W�̃N���X�̊O�ɏo����  
* 2006.03.24	�t�@�C���������݂̉��ǁB�g�p��������啝�Ɍ��炵��  
* 2006.03.10	�t�@�C���ǂݍ��݂̉��ǁB�g�p��������啝�Ɍ��炵��  
* 2006.02.21	�����̉��s�R�[�h�̓ǂݍ��݂ɑΉ�  
* 2005.07.28	#define�ɂ��u����P��P�ʂł̒u���ɂ���  
* 2005.05.20	fopen() �� CreateFile() ���Ă�ł���悤�Ȃ̂Ō��ɖ߂�  
* 2005.04.26	Windows �̂Ƃ��̓t�@�C�������� API ���g���悤�ɂ���  
*/
//-----------------------------------------------------------------------------
//Include
#ifdef __debugprint
#include <stdio.h>  //f*()
//#  include <stdarg.h>		//va_list()
#endif

#include "mx_string.h"
#include "mx_buffer.h" /* buffer operation */
#include "mx_basis.h"
#include "mx_strlib.h"
#include "mx_file.h"
#include "mx_utils.h"
#include "mx_string_list.h"

#define MESS_FILE_OPEN_ERROR_LOAD "*** ERROR!! Load ̧�ق��ǂݍ��߂Ȃ�!!(%s)\n"
#define MESS_FILE_OPEN_ERROR_SAVE "*** ERROR!! Save ̧�ق��ǂݍ��߂Ȃ�!!(%s)\n"
#define MESS_FILE_OPEN_ERROR_SIZE_ZERO "*** ERROR!! ̧�ٻ��ނ�0�ł�!!(%s)\n"
#define MESS_FILE_READ "%8d file read!!(%s)\n"
#define MESS_FILE_WRITE "%8d file write!!(%s)\n"
#define MESS_DEFINE_WARNING "\"%s\"�͂��łɒ�`����Ă��܂�!!\n"

namespace mx {

const int s_SizeOf_EOL[] = {
    kSizeEolEof,
    kSizeEolCr,    //CR
    kSizeEolLf,    //LF
    kSizeEolCrlf,  //CRLF
};

/*!
	StringList �N���X�@�R���X�g���N�^
*/
StringList::StringList() : returnCode_(kEolCrlf) {
  //�A�C�e���̏�����
  Strings.reserve(256);  //�A�C�e���g���T�C�Y
}

/*!
	StringList �N���X�@�f�X�g���N�^
*/
StringList::~StringList() {}

/*!
	������̈�̍쐬
	@param[in] rp ������
	@return �ǉ����� String �N���X�̎Q��
*/
mx::string &StringList::basic_Add(const mx::string &rp) {
  mx::string &item = Strings.push_back(rp);
#if USE_STRINGLIST_RETURNCODE
  returnCodes_.push_back(getReturnCode());
#endif  //USE_STRINGLIST_RETURNCODE
  return item;
}

/*!
	������̈�̍쐬
	@param[in] rp ������
	@param[in] len ����
	@return �ǉ����� String �N���X�̎Q��
*/
mx::string &StringList::basic_Add(const mx::string &rp, int len) {
  mx::string &item = Strings.push_back();
  if (len > 0) {
    item.assign(rp, len);
  }
#if USE_STRINGLIST_RETURNCODE
  returnCodes_.push_back(getReturnCode());
#endif  //USE_STRINGLIST_RETURNCODE
  return item;
}

/*!
	������}��
	@param[in] index �ǉ��ʒu
	@param[in] rp ������
	@return �ǉ����� String �N���X�̎Q��
*/
mx::string &StringList::basic_Insert(int index, const mx::string &rp) {
  mx::string &item = Strings.insert(index, rp);
#if USE_STRINGLIST_RETURNCODE
  returnCodes_.insert(index, getReturnCode());
#endif  //USE_STRINGLIST_RETURNCODE
  return item;
}

/*!
	������}��
	@param[in] index �ǉ��ʒu
	@param[in] rp ������
	@param[in] len ����
	@return �ǉ����� String �N���X�̎Q��
*/
mx::string &StringList::basic_Insert(int index, const mx::string &rp, int len) {
  mx::string &item = Strings.insert(index);
  if (len > 0) {
    item.assign(rp, len);
  }
#if USE_STRINGLIST_RETURNCODE
  returnCodes_.insert(index, getReturnCode());
#endif  //USE_STRINGLIST_RETURNCODE
  return item;
}

/*!
	������ύX
	@param[in] index �ύX�ʒu
	@param[in] rp] ������
	@return �ǉ����� String �N���X�̎Q��
*/
mx::string &StringList::basic_Assign(int index, const mx::string &rp) {
  Strings[index] = rp;
  return Strings[index];
}

/*!
	������ύX
	@param[in] index �ύX�ʒu
	@param[in] rp] ������
	@param[in] len ����
	@return �ǉ����� String �N���X�̎Q��
*/
mx::string &StringList::basic_Assign(int index, const mx::string &rp, int len) {
  if (len == 0) {
    Strings[index] = "";
  } else {
    Strings[index].assign(rp, len);
  }
  return Strings[index];
}

/*!
	��������Ȃ���
	@param[in] index �Ȃ���ʒu
	@param[in] rp ������
	@return �ǉ����� mx::string �N���X�̎Q��
*/
mx::string &StringList::basic_Concatenate(int index, const mx::string &rp) {
  Strings[index] += rp;
  return Strings[index];
}

/*!
	��������Ȃ���
	@param[in] index �Ȃ���ʒu
	@param[in] rp ������
	@param[in] len ����(-1�Ŏ���)
	@return �ǉ����� mx::string �N���X�̎Q��
*/
mx::string &StringList::basic_Concatenate(int index, const mx::string &rp, int len) {
  if (len == 0) {
    //
  } else {
    Strings[index].append(rp, len);
  }
  return Strings[index];
}

//mx::string &StringList::add(const mx::string &fmt, ...)
mx::string &StringList::add(const char *fmt, ...) {
  if (*fmt) {
    mx::string s;
    s.extendBuffer(0x1000);
    return basic_Add(utils::svprintf(s.data(), &fmt));
  } else {
    return basic_Add("");
  }
}

//mx::string &StringList::assign(int index, const mx::string &fmt, ...)
mx::string &StringList::assign(int index, const char *fmt, ...) {
  if (*fmt) {
    mx::string s;
    s.extendBuffer(0x1000);
    return basic_Assign(index, utils::svprintf(s.data(), &fmt));
  } else {
    return basic_Assign(index, "");
  }
}

//mx::string &StringList::concatenate(int index, const mx::string &fmt, ...)
mx::string &StringList::concatenate(int index, const char *fmt, ...) {
  if (*fmt) {
    mx::string s;
    s.extendBuffer(0x1000);
    return basic_Concatenate(index, utils::svprintf(s.data(), &fmt));
  } else {
    // �����񂪂Ȃ��̂łȂɂ����Ȃ��Ă���
    return Strings[index];
  }
}

//mx::string &StringList::insert(int index, const mx::string &fmt, ...)
mx::string &StringList::insert(int index, const char *fmt, ...) {
  if (*fmt) {
    mx::string s;
    s.extendBuffer(0x1000);
    return basic_Insert(index, utils::svprintf(s.data(), &fmt));
  } else {
    return basic_Insert(index, "");
  }
}

/*!
	�f�t�H���g���s�R�[�h�ݒ�
	@param[in] type ���s�R�[�h�^�C�v
*/
void StringList::setReturnCode(int type) {
  returnCode_ = type;
}

/*!
	�f�t�H���g���s�R�[�h�擾
	@return ���s�R�[�h�^�C�v
*/
int StringList::getReturnCode() {
  return returnCode_;
}

/*!
	�s���擾
	@return �s��
*/
int StringList::count() const {
  return Strings.size();
}

/*!
	��r�֐�(����)	������
	@param[in] a �f�[�^�`
	@param[in] b �f�[�^�a
	@return int
*/
///template <class T> static int fnSortStringUp(const T &a, const T &b)
static int fnSortStringUp(const mx::string &a, const mx::string &b) {
  ///	return (*(T*)&a).compare(b);
  return a.compare(b);
}

/*!
	��r�֐�(�~��)	������
	@param[in] a �f�[�^�`
	@param[in] b �f�[�^�a
	@return int
*/
///template <class T> static int fnSortStringDown(const T &a, const T &b)
static int fnSortStringDown(const mx::string &a, const mx::string &b) {
  ///	return (*(T*)&b).compare(a);
  return b.compare(a);
}

/*!
	��r�֐�(����)	����
	@param[in] a �f�[�^�`
	@param[in] b �f�[�^�a
	@return int
*/
///template <class T> static int fnSortLengthUp(const T &a, const T &b)
static int fnSortLengthUp(const mx::string &a, const mx::string &b) {
  ///	return (*(T*)&a).length() - (*(T*)&b).length();
  return a.length() - b.length();
}

/*!
	��r�֐�(�~��)	����
	@param[in] a �f�[�^�`
	@param[in] b �f�[�^�a
	@return int
*/
///template <class T> static int fnSortLengthDown(const T &a, const T &b)
static int fnSortLengthDown(const mx::string &a, const mx::string &b) {
  ///	return (*(T*)&b).length() - (*(T*)&a).length();
  return b.length() - a.length();
}

/*!
	�����񃊃X�g�̃\�[�g
	@param[in] fnCmp ��r�֐�
*/
void StringList::sort(int (*fnCmp)(const mx::string &, const mx::string &)) {
  //	Strings.bsort(fnCmp, 0, Strings.size());
  Strings.qsort(fnCmp, 0, Strings.size() - 1);
}

/*!
	�����񃊃X�g�̃\�[�g
	@param[in] sortUp 0:�~�� 1:����
*/
void StringList::sort(int sortUp) {
  static int (*s_fnCmp[2])(const mx::string &, const mx::string &) = {
      fnSortStringDown,  //������E�~��
      fnSortStringUp,    //������E����
  };

  sort(s_fnCmp[sortUp]);
}

/*!
	�����񃊃X�g�̃\�[�g(����)
	@param[in] sortUp 0:�~�� 1:����
*/
void StringList::lengthSort(int sortUp) {
  static int (*s_fnCmp[2])(const mx::string &, const mx::string &) = {
      fnSortLengthDown,  //������E�~��
      fnSortLengthUp,    //������E����
  };

  sort(s_fnCmp[sortUp]);
}

/*!
	�s�̍폜
	@param[in] index �C���f�b�N�X
*/
void StringList::deleteLine(int index) {
  Strings.erase(index);
#if USE_STRINGLIST_RETURNCODE
  returnCodes_.erase(index);
#endif  //USE_STRINGLIST_RETURNCODE
}

/*!
	�󔒍s�̍폜
*/
void StringList::deleteBlankLines() {
  for (int i = count() - 1; i >= 0; --i) {
    mx::string &s = Strings[i];

    //��������X�L�������āA�w�肳�ꂽ�����Q�ȊO���ŏ��ɏo������ʒu�������܂�
    const int pos = mx::strspn(s.c_str(), " \t");

    //�^�u�ƃX�y�[�X�ȊO�͂Ȃ������̂ō폜
    if (s.length() == pos) {
      Strings.erase(i);
#if USE_STRINGLIST_RETURNCODE
      returnCodes_.erase(i);
#endif  //USE_STRINGLIST_RETURNCODE
    }
  }
}

/*!
	�s�R�����g����
*/
void StringList::removeLineComment(const mx::string &lineCommentWord) {
  for (int i = count() - 1; i >= 0; i--) {
    mx::string &s = Strings[i];

    int pos = s.find(lineCommentWord);

    if (pos >= 0) {
      s.erase();
    }
  }
}

/*!
	�N���A
*/
void StringList::clear() {
  Strings.clear();
#if USE_STRINGLIST_RETURNCODE
  returnCodes_.clear();
#endif  //USE_STRINGLIST_RETURNCODE
}

/*!
	���
*/
void StringList::free() {
  Strings.clean();
#if USE_STRINGLIST_RETURNCODE
  returnCodes_.clear_clean();
#endif  //USE_STRINGLIST_RETURNCODE
}

/*!
	�e�L�X�g�^�f�[�^���烊�X�g���쐬
	@param[in] data �f�[�^
*/
void StringList::fromText(const char *data, int size) {
  //�f�[�^���烊�X�g���쐬���Ă���
  const char *top = data;
  const char *entry = data;
  int count = 0;

  for (;;) {
    int return_code;
    int ch = *data++;  //�P�����擾

    // �f�[�^�I��
    if (data - top > size) {
      return_code = kEolEof;
    } else {
      switch (ch) {
      //CR(0x0d)
      case '\r':
        //LF(0x0a)
        if (*data == '\n') {
          return_code = kEolCrlf;

          //CRLF(0x0d 0x0a)
          data++;
        } else {
          return_code = kEolCr;
        }
        break;

      //LF(0x0a)
      case '\n':
//LFCR �͔F�߂܂���
#if 1
        return_code = kEolLf;
#else
        //CR(0x0d)
        if (*data == '\r') {
          return_code = kEolLfcr;

          //LFCR(0x0a 0x0d)
          data++;
        } else {
          return_code = kEolLf;
        }
#endif
        break;

      //null
      case '\0':
        return_code = kEolEof;
        break;

      //���̑�
      default:
        count++;
        //break;
        continue;  //���ǉ����Ȃ�
      }
    }

    //���X�g�ǉ�
    if (count == 0) {
      basic_Add("");
    } else {
      basic_Add(entry, count);
    }

#if USE_STRINGLIST_RETURNCODE
    // ���s�R�[�h
    returnCodes_.push_back(return_code);
#else
    (void)return_code;
#endif  //USE_STRINGLIST_RETURNCODE

    // �f�[�^�I��
    if (data - top > size) {
      break;
    }

    //ch �� nil �ɂȂ�����I���
    if (ch == '\0') {
      break;
    }

    //���̃G���g���|�C���g�̐ݒ�
    entry = data;
    count = 0;
  }
}

/*!
	�t�@�C���̓ǂݍ���
	@param[in] fileName �ǂݍ��ރt�@�C����
	@param[in] concat �A��
	@return 0:�ǂݍ��ݎ��s, !0:�ǂݍ��ݐ���
*/
int StringList::loadFromFile(const mx::string &fileName, bool concat) {
  File file;

  //���X�g���N���A���Ȃ�
  if (concat) {
#if USE_STRINGLIST_RETURNCODE
    if (0 < count()) {
      int &r = returnCodes_.back();
      r = getReturnCode();  //EOF �� ���s �ɂ���
    }
#endif  //USE_STRINGLIST_RETURNCODE
  }
  //���X�g���N���A����
  else {
    clear();
  }

  //�t�@�C�����J��
  if (!file.open(fileName, false)) {
#ifdef __debugprint
    printf(MESS_FILE_OPEN_ERROR_LOAD, fileName.c_str());
#endif  //__debugprint
    return 0;
  }

  //�t�@�C���T�C�Y�� 0 �ȉ��Ȃ̂ŃG���[�ŕԂ�
  if (file.getSize() <= 0) {
#ifdef __debugprint
    printf(MESS_FILE_OPEN_ERROR_SIZE_ZERO, file.GetSize(), fileName.c_str());
#endif  //__debugprint
    file.close();
    return 0;
  }

  char *buf = 0;
  file.allocRead((void **)&buf);

  fromText(buf, file.getSize());

  SAFE_DELETE_ARRAY(buf);
  file.close();

#ifdef __debugprint
  printf(MESS_FILE_READ, file.GetSize(), fileName.c_str());
#endif  //__debugprint
  return file.getSize();
}

/*!
	���X�g���e�L�X�g�^�f�[�^�ɕϊ�
	@param[out] out
	@param[in] delimiter �f���~�^
	@return size
*/
int StringList::toString(mx::string &out, const char delimiter) {
  if (Strings.empty()) {
    out = mx::string("");
    return 0;
  }

  int size = 0;
  BUFF b;

  //�f�t�H���g���[�h(�ݒ肳��Ă�����s�R�[�h)
  if (-1 == delimiter) {
    //�T�C�Y�v�Z
    for (int i = 0; i < count(); i++) {
      mx::string &s = Strings[i];
      size += s.length() + 2;  //���s����
    }
  }
  //�f���~�^�w�胂�[�h
  else {
    //�T�C�Y�v�Z
    for (int i = 0; i < count(); i++) {
      mx::string &s = Strings[i];
      size += s.length() + 1;  //+ '\0'
    }
  }

  //�T�C�Y����o�b�t�@�擾
  b.buffer = SAFE_NEW_ARRAY(__SOURCE__, uint8_t, size);
  b.curp = b.buffer;
  b.bsize = size;

  //�f�t�H���g���[�h(�ݒ肳��Ă�����s�R�[�h)
  if (-1 == delimiter) {
    //�A��
    for (int i = 0; i < count(); i++) {
      mx::string &s = Strings[i];

      bwrite(s.c_str(), s.length(), 1, &b);

      if (i < count() - 1) {
        bputc(0x0d, &b);  //���s(CR)
        bputc(0x0a, &b);  //���s(LF)
      } else {
        bputc(0x00, &b);  //EOF
      }
    }
    //bputc(0x00, &b); //EOF
  }
  //�f���~�^�w�胂�[�h
  else {
    //�A��
    for (int i = 0; i < count(); i++) {
      mx::string &s = Strings[i];

      bwrite(s.c_str(), s.length(), 1, &b);
      bputc(delimiter, &b);  //'\0'
    }
  }

  out = (char *)b.buffer;
  SAFE_DELETE_ARRAY(b.buffer);

  if (-1 == delimiter) {
    return size - 2;
  } else {
    return size;
  }
}

/*!
	�t�@�C���ɕۑ�
	@param[in] fileName �ǂݍ��ރt�@�C����
	@return 0:����, 1:���s
*/
int StringList::saveToFile(const mx::string &fileName) {
  File file;

  //�Ƃ肠�����t�@�C�����폜����
  // remove(fileName.c_str());

  //�t�@�C�����J��
  if (!file.open(fileName, true)) {
#ifdef __debugprint
    printf(MESS_FILE_OPEN_ERROR_SAVE, fileName.c_str());
#endif  //__debugprint
    return 1;
  }

  mx::string s;
  int size = toString(s);
  file.write((void *)s.c_str(), size);
  file.close();

#ifdef __debugprint
  printf(MESS_FILE_WRITE, "***", fileName.c_str());
#endif  //__debugprint
  return 0;
}

}  //mx
