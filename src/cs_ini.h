//-----------------------------------------------------------------------------
/*!
* @file		mx_ini.h
* @brief	�ݒ�t�@�C���Ǘ��N���X
* @author	koma
* @version	$Id$
*
* @note
* �E�X�V����  
*/
//-----------------------------------------------------------------------------
#ifndef MX_INI_H
#define MX_INI_H

namespace mx {

//class String;
class StringList;

//! IniFile �x�[�X�N���X
class IniFile {
 public:
  IniFile();
  explicit IniFile(const mx::string &fileName);
  virtual ~IniFile();

  void open(const mx::string &fileName);
  void save();
  void close();

  void clear();
  bool isEmpty();

  StringList *enumSection(StringList *section);
  StringList *enumKey(StringList *key, const mx::string &section);

  // �폜
  void deleteSection(const mx::string &section);
  void deleteKey(const mx::string &section, const mx::string &key);

  // �ǂݍ���
  mx::string readString(const mx::string &section, const mx::string &key, const mx::string &def);
  int readInteger(const mx::string &section, const mx::string &key, int def);
  bool readBool(const mx::string &section, const mx::string &key, bool def);

  // ��������
  void writeString(const mx::string &section, const mx::string &key, const mx::string &write);
  void writeInteger(const mx::string &section, const mx::string &key, int write);
  void writeBool(const mx::string &section, const mx::string &key, bool write, bool string = false);

 private:
  bool isSection(mx::string &s);

  mx::string getKeyData(const mx::string &section, const mx::string &key);
  void setKeyData(const mx::string &section, const mx::string &key, const mx::string &data);

 private:
  StringList *pList_;
  mx::string fileName_;
  bool modified_;
};

}  //mx

#endif  //MX_INI_H
