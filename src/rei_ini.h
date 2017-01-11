// -*- mode:c++; coding:utf-8-ws -*-
//====================================================================
//! @file  rei_ini.h
//! @brief 設定ファイル管理クラス
//!
//! @author
//====================================================================
#ifndef REI_INI_H
#define REI_INI_H

namespace rei {

//class String;
class StringList;

//! IniFile ベースクラス
class IniFile {
 public:
  IniFile();
  explicit IniFile(const rei::string &fileName);
  virtual ~IniFile();

  void open(const rei::string &fileName);
  void save();
  void close();

  void clear();
  bool isEmpty();

  StringList *enumSection(StringList *section);
  StringList *enumKey(StringList *key, const rei::string &section);

  // 削除
  void deleteSection(const rei::string &section);
  void deleteKey(const rei::string &section, const rei::string &key);

  // 読み込み
  rei::string readString(const rei::string &section, const rei::string &key,
                         const rei::string &def);
  int readInteger(const rei::string &section, const rei::string &key, int def);
  bool readBool(const rei::string &section, const rei::string &key, bool def);

  // 書き込み
  void writeString(const rei::string &section, const rei::string &key, const rei::string &write);
  void writeInteger(const rei::string &section, const rei::string &key, int write);
  void writeBool(const rei::string &section, const rei::string &key, bool write,
                 bool string = false);

 private:
  bool isSection(rei::string &s);

  rei::string getKeyData(const rei::string &section, const rei::string &key);
  void setKeyData(const rei::string &section, const rei::string &key, const rei::string &data);

 private:
  StringList *pList_;
  rei::string fileName_;
  bool modified_;
};

}  //rei

#endif  //REI_INI_H
