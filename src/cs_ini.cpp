//-----------------------------------------------------------------------------
/*!
* @file		mx_ini.cpp
* @brief	�ݒ�t�@�C���Ǘ��N���X
* @author	koma
* @version	$Id$
*
* @note
* �E�X�V����  
*/
//-----------------------------------------------------------------------------
// Include
#ifdef BCC32
#include <windows.h>
#else

#endif

#include <stdio.h>

#include "mx_basis.h"
#include "mx_strlib.h"
#include "mx_math.h"
#include "mx_string_list.h"
#include "mx_utils.h"
#include "mx_ini.h"

#define THIS_SECTION 1  // ����̓Z�N�V����
#define FIND_SECTION 2  // �Ώۂ̃Z�N�V��������������

// �Ώۂ̃Z�N�V�������������Ă���
// item �̓Z�N�V����
#define FIND_SECTION_THIS_SECTION (THIS_SECTION | FIND_SECTION)

// �Ώۂ̃Z�N�V�������������Ă���
// item �̓Z�N�V�����łȂ�
#define FIND_SECTION_THIS_NOSECTION (FIND_SECTION)

// �Ώۂ̃Z�N�V�������������Ă��Ȃ�
// item �̓Z�N�V����
#define NOTFIND_SECTION_THIS_SECTION (THIS_SECTION)

namespace mx {

/*!
	IniFile �N���X�@�R���X�g���N�^
*/
IniFile::IniFile() {}

IniFile::IniFile(const mx::string &fileName) {
  open(fileName);
}

/*!
	IniFile �N���X�@�f�X�g���N�^
*/
IniFile::~IniFile() {
  close();
}

/*!
	�ݒ�t�@�C�����I�[�v������
	@param[in] fileName �I�[�v������t�@�C����
*/
void IniFile::open(const mx::string &fileName) {
  // ���X�g�쐬
  StringList *list = SAFE_NEW(__SOURCE__, StringList);

  // �t�@�C���̓ǂݍ���
  list->loadFromFile(fileName.c_str());
  list->removeLineComment(";");
  list->deleteBlankLines();

  pList_ = list;
  fileName_ = fileName;
}

/*!
	�ݒ�t�@�C����ۑ�����
*/
void IniFile::save() {
  if (modified_) {
    StringList *list = pList_;

    // ���X�g���`
    //		list->Insert(0, ";%s", fileName_.c_str());

    // �Z�N�V�����̊Ԃɋ�s������
    for (int i = list->count() - 1; i >= 1; i--) {
      if (isSection(list->Strings[i])) {
        list->insert(i, "");
      }
    }

    // �Ō�ɋ�s
    list->add("");

    // �ۑ�
    list->saveToFile(fileName_.c_str());

    modified_ = false;
  }
}

/*!
	�ݒ�t�@�C�����N���[�Y����

	@note
	Open() �Ŏw�肵���t�@�C�����ŕۑ����܂� <br>
*/
void IniFile::close() {
  // �I�����łɃt�@�C������������
  save();

  SAFE_DELETE(pList_);
}

/*!
	�N���A
*/
void IniFile::clear() {
  pList_->clear();
}

/*!
	�󂩂ǂ���
	@return true:��, false:��łȂ�
*/
bool IniFile::isEmpty() {
  return (pList_->count() <= 0);
}

/*!
	�Z�N�V�������H
	@param[in] s String
	@return true:�Z�N�V������, �Z�N�V��������Ȃ�
*/
bool IniFile::isSection(mx::string &s) {
  int index = s.find("[");  // "[" ����������Z�N�V�����Ƃ���

  // ��������������
  if (index >= 0) {
    index = s.find("]", index + 1);  // ���I

    if (index >= 0) {
      return true;
    }
  }

  return false;
}

/*!
	�L�[�̃f�[�^���擾����
	@param[in] section �Z�N�V����
	@param[in] key �L�[
	@return �f�[�^
*/
mx::string IniFile::getKeyData(const mx::string &section, const mx::string &key) {
  StringList *list = pList_;
  int state = 0;

  for (int i = 0; i < list->count(); i++) {
    mx::string &item = list->Strings[i];

    // empty!!
    if (item.empty()) {
      continue;
    }

    // �Z�N�V�������`�F�b�N
    if (isSection(item)) {
      state |= THIS_SECTION;
    } else {
      // �Z�N�V�����t���O����菜��
      state &= ~THIS_SECTION;
    }

    switch (state) {
    // �Ώۂ̃Z�N�V�������������Ă���
    // item �̓Z�N�V����
    case FIND_SECTION_THIS_SECTION:
      // ���łɈ�v���Ă����ꍇ�͏I��
      return mx::string("");
    //break;

    // �Ώۂ̃Z�N�V�������������Ă���
    // item �̓Z�N�V�����łȂ�
    case FIND_SECTION_THIS_NOSECTION: {
      int index = item.find("=");

      if (index >= 0) {
        mx::string t_key = item.subString(0, index).trim(" \t");

        // �L�[����v���Ă��邩�H
        if (key.upCase() == t_key.upCase()) {
          return item.subString(index + 1).trim(" \t");
        }
      } else {
        // failed
      }
    } break;

    // �Ώۂ̃Z�N�V�������������Ă��Ȃ�
    // item �̓Z�N�V����
    case NOTFIND_SECTION_THIS_SECTION: {
      int index1 = item.find("[");
      int index2 = item.find("]", index1);

      if (index1 >= 0 && index2 >= 0) {
        mx::string t_section = item.subString(index1 + 1, index2 - index1 - 1).trim(" \t");

        // �Z�N�V��������v���Ă��邩�H
        if (section.upCase() == t_section.upCase()) {
          state |= FIND_SECTION;
        }
      } else {
        // failed
      }
    } break;
    }
  }

  return mx::string("");
}

/*!
	�L�[�̃f�[�^��ݒ肷��
	@param[in] section �Z�N�V����
	@param[in] key �L�[
	@param[in] data �f�[�^
*/
void IniFile::setKeyData(const mx::string &section, const mx::string &key, const mx::string &data) {
  StringList *list = pList_;
  int state = 0;

  for (int i = 0; i < list->count(); i++) {
    mx::string &item = list->Strings[i];

    // empty!!
    if (item.empty()) {
      continue;
    }

    // �Z�N�V�������`�F�b�N
    if (isSection(item)) {
      state |= THIS_SECTION;
    } else {
      // �Z�N�V�����t���O����菜��
      state &= ~THIS_SECTION;
    }

    switch (state) {
    // �Ώۂ̃Z�N�V�������������Ă���
    // item �̓Z�N�V����
    case FIND_SECTION_THIS_SECTION:
      // ���łɈ�v���Ă����ꍇ�͒ǉ�
      list->insert(i, "%s = %s", key.c_str(), data.c_str());  // �ǉ�
      modified_ = true;
      return;
    //break;

    // �Ώۂ̃Z�N�V�������������Ă���
    // item �̓Z�N�V�����łȂ�
    case FIND_SECTION_THIS_NOSECTION: {
      int index = item.find("=");

      if (index >= 0) {
        mx::string t_key = item.subString(0, index).trim(" \t");

        // �L�[����v���Ă��邩�H
        if (key.upCase() == t_key.upCase()) {
          item = key + " = " + data;  // �X�V
          modified_ = true;
          return;
        }
      } else {
        // failed
      }
    } break;

    // �Ώۂ̃Z�N�V�������������Ă��Ȃ�
    // item �̓Z�N�V����
    case NOTFIND_SECTION_THIS_SECTION: {
      int index1 = item.find("[");
      int index2 = item.find("]", index1);

      if (index1 >= 0 && index2 >= 0) {
        mx::string t_section = item.subString(index1 + 1, index2 - index1 - 1).trim(" \t");

        // �Z�N�V��������v���Ă��邩�H
        if (section.upCase() == t_section.upCase()) {
          state |= FIND_SECTION;
        }
      } else {
        // failed
      }
    } break;
    }
  }

  // �V�K
  if (!(state & FIND_SECTION)) {
    list->add("[%s]", section.c_str());
  }

  list->add("%s = %s", key.c_str(), data.c_str());  // �ǉ�
  modified_ = true;
}

/*!
	�Z�N�V������񋓂���
	@param[in] section StringList�Bnil�̂Ƃ��͍쐬����
	@return �Z�N�V�������X�g
*/
StringList *IniFile::enumSection(StringList *section) {
  StringList *list = pList_;
  int state = 0;

  if (section == 0) {
    section = SAFE_NEW(__SOURCE__, StringList);
  }
  section->clear();

  for (int i = 0; i < list->count(); i++) {
    mx::string &item = list->Strings[i];

    // empty!!
    if (item.empty()) {
      continue;
    }

    // �Z�N�V�������`�F�b�N
    if (isSection(item)) {
      state |= THIS_SECTION;
    } else {
      // �Z�N�V�����t���O����菜��
      state &= ~THIS_SECTION;
    }

    switch (state) {
    // item �̓Z�N�V����
    case THIS_SECTION: {
      int index1 = item.find("[");
      int index2 = item.find("]", index1);

      if (index1 >= 0 && index2 >= 0) {
        mx::string t_section = item.subString(index1 + 1, index2 - index1 - 1).trim(" \t");

        // �Z�N�V�����ǉ�
        if (t_section.length() > 0) {
          section->add(t_section.c_str());
        }
      } else {
        // failed
      }
    } break;
    }
  }

  return section;
}

/*!
	�L�[��񋓂���
	@param[in] key StringList�Bnil�̂Ƃ��͍쐬����
	@param[in] section �Z�N�V������
	@return �L�[���X�g
*/
StringList *IniFile::enumKey(StringList *key, const mx::string &section) {
  StringList *list = pList_;
  int state = 0;

  if (key == 0) {
    key = SAFE_NEW(__SOURCE__, StringList);
  }
  key->clear();

  for (int i = 0; i < list->count(); i++) {
    mx::string &item = list->Strings[i];

    // empty!!
    if (item.empty()) {
      continue;
    }

    // �Z�N�V�������`�F�b�N
    if (isSection(item)) {
      state |= THIS_SECTION;
    } else {
      // �Z�N�V�����t���O����菜��
      state &= ~THIS_SECTION;
    }

    switch (state) {
    // �Ώۂ̃Z�N�V�������������Ă���
    // item �̓Z�N�V����
    case FIND_SECTION_THIS_SECTION:
      // ���łɈ�v���Ă����ꍇ�͏I��
      return key;
    //break;

    // �Ώۂ̃Z�N�V�������������Ă���
    // item �̓Z�N�V�����łȂ�
    case FIND_SECTION_THIS_NOSECTION: {
      int index = item.find("=");

      if (index >= 0) {
        mx::string t_key = item.subString(0, index).trim(" \t");

        // �L�[�ǉ�
        if (t_key.length() > 0) {
          key->add(t_key.c_str());
        }
      } else {
        // failed
      }
    } break;

    // �Ώۂ̃Z�N�V�������������Ă��Ȃ�
    // item �̓Z�N�V����
    case NOTFIND_SECTION_THIS_SECTION: {
      int index1 = item.find("[");
      int index2 = item.find("]", index1);

      if (index1 >= 0 && index2 >= 0) {
        mx::string t_section = item.subString(index1 + 1, index2 - index1 - 1).trim(" \t");

        // �Z�N�V��������v���Ă��邩�H
        if (section.upCase() == t_section.upCase()) {
          state |= FIND_SECTION;
        }
      } else {
        // failed
      }
    } break;
    }
  }

  return key;
}

/*!
	�ݒ�t�@�C������Z�N�V�������ƍ폜
	@param[in] section �Z�N�V������
*/
void IniFile::deleteSection(const mx::string &section) {
  StringList *list = pList_;
  int state = 0;

  for (int i = 0; i < list->count(); i++) {
    mx::string &item = list->Strings[i];

    // empty!!
    if (item.empty()) {
      continue;
    }

    // �Z�N�V�������`�F�b�N
    if (isSection(item)) {
      state |= THIS_SECTION;
    } else {
      // �Z�N�V�����t���O����菜��
      state &= ~THIS_SECTION;
    }

    switch (state) {
    // �Ώۂ̃Z�N�V�������������Ă���
    // item �̓Z�N�V����
    case FIND_SECTION_THIS_SECTION:
      // ���łɈ�v���Ă����ꍇ�͏I��
      return;
    //break;

    // �Ώۂ̃Z�N�V�������������Ă���
    // item �̓Z�N�V�����łȂ�
    case FIND_SECTION_THIS_NOSECTION:
      list->deleteLine(i);  // �L�[�͖ⓚ���p�ō폜
      i--;                  //�P�O��
      modified_ = true;
      break;

    // �Ώۂ̃Z�N�V�������������Ă��Ȃ�
    // item �̓Z�N�V����
    case NOTFIND_SECTION_THIS_SECTION: {
      int index1 = item.find("[");
      int index2 = item.find("]", index1);

      if (index1 >= 0 && index2 >= 0) {
        mx::string t_section = item.subString(index1 + 1, index2 - index1 - 1).trim(" \t");

        // �Z�N�V��������v���Ă��邩�H
        if (section.upCase() == t_section.upCase()) {
          state |= FIND_SECTION;
          list->deleteLine(i);

          i--;  //�P�O��

          modified_ = true;
        }
      } else {
        // failed
      }
    } break;
    }
  }
}

/*!
	�ݒ�t�@�C������L�[���폜
	@param[in] section �Z�N�V������
	@param[in] key �L�[��
*/
void IniFile::deleteKey(const mx::string &section, const mx::string &key) {
  StringList *list = pList_;
  int state = 0;

  for (int i = 0; i < list->count(); i++) {
    mx::string &item = list->Strings[i];

    // empty!!
    if (item.empty()) {
      continue;
    }

    // �Z�N�V�������`�F�b�N
    if (isSection(item)) {
      state |= THIS_SECTION;
    } else {
      // �Z�N�V�����t���O����菜��
      state &= ~THIS_SECTION;
    }

    switch (state) {
    // �Ώۂ̃Z�N�V�������������Ă���
    // item �̓Z�N�V����
    case FIND_SECTION_THIS_SECTION:
      // ���łɈ�v���Ă����ꍇ�͏I��
      return;
    //break;

    // �Ώۂ̃Z�N�V�������������Ă���
    // item �̓Z�N�V�����łȂ�
    case FIND_SECTION_THIS_NOSECTION: {
      int index = item.find("=");

      if (index >= 0) {
        mx::string t_key = item.subString(0, index).trim(" \t");

        // �L�[����v���Ă��邩�H
        if (key.upCase() == t_key.upCase()) {
          list->deleteLine(i);  // �폜
          modified_ = true;
          return;
        }
      } else {
        // failed
      }
    } break;

    // �Ώۂ̃Z�N�V�������������Ă��Ȃ�
    // item �̓Z�N�V����
    case NOTFIND_SECTION_THIS_SECTION: {
      int index1 = item.find("[");
      int index2 = item.find("]", index1);

      if (index1 >= 0 && index2 >= 0) {
        mx::string t_section = item.subString(index1 + 1, index2 - index1 - 1).trim(" \t");

        // �Z�N�V��������v���Ă��邩�H
        if (section.upCase() == t_section.upCase()) {
          state |= FIND_SECTION;
        }
      } else {
        // failed
      }
    } break;
    }
  }
}

/*!
	�ݒ�t�@�C������L�[�̒l���擾����(������)
	@param[in] section �Z�N�V������
	@param[in] key �L�[��
	@param[in] def ���ݒ菉�����L�[
	@return �L�[�̒l
*/
mx::string IniFile::readString(const mx::string &section, const mx::string &key,
                               const mx::string &def) {
  mx::string data = getKeyData(section, key);

  // ���ݒ�̂��߃f�t�H���g
  if (data.empty()) {
    data = def;
  }

  return data;
}

/*!
	�ݒ�t�@�C������L�[�̒l���擾����(���l)
	@param[in] section �Z�N�V������
	@param[in] key �L�[��
	@param[in] def ���ݒ菉�����L�[
	@return ���l
*/
int IniFile::readInteger(const mx::string &section, const mx::string &key, int def) {
  mx::string data = getKeyData(section, key);

  // ���ݒ�̂��߃f�t�H���g
  if (data.empty()) {
    return def;
  }

  return data.asInt();
}

/*!
	�ݒ�t�@�C������L�[�̒l���擾����(���l)
	@param[in] section �Z�N�V������
	@param[in] key �L�[��
	@param[in] def ���ݒ菉�����L�[
	@return ����
*/
bool IniFile::readBool(const mx::string &section, const mx::string &key, bool def) {
  mx::string data = getKeyData(section, key);

  // ���ݒ�̂��߃f�t�H���g
  if (data.empty()) {
    return def;
  }

  if (data.upCase() == "TRUE") {
    return true;
  } else if (data.upCase() == "FALSE") {
    return false;
  }

  return (data.asInt() != 0);
}

/*!
	�ݒ�t�@�C���ɃL�[�̒l����������(������)
	@param[in] section �Z�N�V������
	@param[in] key �L�[��
	@param[in] write �������ރf�[�^
*/
void IniFile::writeString(const mx::string &section, const mx::string &key,
                          const mx::string &write) {
  setKeyData(section, key, write);
}

/*!
	�ݒ�t�@�C���ɃL�[�̒l����������(���l)
	@param[in] section �Z�N�V������
	@param[in] key �L�[��
	@param[in] write �������ރf�[�^
*/
void IniFile::writeInteger(const mx::string &section, const mx::string &key, int write) {
  mx::string data(write, "%d");
  setKeyData(section, key, data);
}

/*!
	�ݒ�t�@�C���ɃL�[�̒l����������(bool)
	@param[in] section �Z�N�V������
	@param[in] key �L�[��
	@param[in] write �������ރf�[�^
	@param[in] string true: true or false, false: 1 or 0
*/
void IniFile::writeBool(const mx::string &section, const mx::string &key, bool write, bool string) {
  if (string) {
    if (write) {
      setKeyData(section, key, "true");
    } else {
      setKeyData(section, key, "false");
    }
  } else {
    mx::string data(static_cast<int>(write), "%d");
    setKeyData(section, key, data);
  }
}

}  //mx
