// -*- mode:c++; coding:utf-8-ws -*-
#ifndef NKMM_INI_H
#define NKMM_INI_H

namespace nkmm {

//class String;
class StringList;

//! IniFile ベースクラス
class IniFile {
 public:
  IniFile();
  explicit IniFile(const nkmm::string &fileName);
  virtual ~IniFile();

  void open(const nkmm::string &fileName);
  void save();
  void close();

  void clear();
  bool isEmpty();

  StringList *enumSection(StringList *section);
  StringList *enumKey(StringList *key, const nkmm::string &section);

  // 削除
  void deleteSection(const nkmm::string &section);
  void deleteKey(const nkmm::string &section, const nkmm::string &key);

  // 読み込み
  nkmm::string readString(const nkmm::string &section, const nkmm::string &key,
                         const nkmm::string &def);
  int readInteger(const nkmm::string &section, const nkmm::string &key, int def);
  bool readBool(const nkmm::string &section, const nkmm::string &key, bool def);

  // 書き込み
  void writeString(const nkmm::string &section, const nkmm::string &key, const nkmm::string &write);
  void writeInteger(const nkmm::string &section, const nkmm::string &key, int write);
  void writeBool(const nkmm::string &section, const nkmm::string &key, bool write,
                 bool string = false);

 private:
  bool isSection(nkmm::string &s);

  nkmm::string getKeyData(const nkmm::string &section, const nkmm::string &key);
  void setKeyData(const nkmm::string &section, const nkmm::string &key, const nkmm::string &data);

 private:
  StringList *pList_;
  nkmm::string fileName_;
  bool modified_;
};

}  //nkmm

#endif  //NKMM_INI_H
