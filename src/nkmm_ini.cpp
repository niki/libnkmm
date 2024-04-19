// -*- mode:c++; coding:utf-8-ws -*-
// Include
#ifdef BCC32
#include <windows.h>
#else

#endif

#include <stdio.h>

#include "nkmm_basis.h"
#include "nkmm_strlib.h"
#include "nkmm_math.h"
#include "nkmm_string_list.h"
#include "nkmm_utils.h"
#include "nkmm_ini.h"

#define THIS_SECTION 1  // これはセクション
#define FIND_SECTION 2  // 対象のセクションが見つかった

// 対象のセクションが見つかっている
// item はセクション
#define FIND_SECTION_THIS_SECTION (THIS_SECTION | FIND_SECTION)

// 対象のセクションが見つかっている
// item はセクションでない
#define FIND_SECTION_THIS_NOSECTION (FIND_SECTION)

// 対象のセクションが見つかっていない
// item はセクション
#define NOTFIND_SECTION_THIS_SECTION (THIS_SECTION)

namespace nkmm {

/*!
	IniFile クラス　コンストラクタ
*/
IniFile::IniFile() {}

IniFile::IniFile(const nkmm::string &fileName) {
  open(fileName);
}

/*!
	IniFile クラス　デストラクタ
*/
IniFile::~IniFile() {
  close();
}

/*!
	設定ファイルをオープンする
	@param[in] fileName オープンするファイル名
*/
void IniFile::open(const nkmm::string &fileName) {
  // リスト作成
  StringList *list = SAFE_NEW(__SOURCE__, StringList);

  // ファイルの読み込み
  list->loadFromFile(fileName.c_str());
  list->removeLineComment(";");
  list->deleteBlankLines();

  pList_ = list;
  fileName_ = fileName;
}

/*!
	設定ファイルを保存する
*/
void IniFile::save() {
  if (modified_) {
    StringList *list = pList_;

    // リスト整形
    //		list->Insert(0, ";%s", fileName_.c_str());

    // セクションの間に空行を入れる
    for (int i = list->count() - 1; i >= 1; i--) {
      if (isSection(list->Strings[i])) {
        list->insert(i, "");
      }
    }

    // 最後に空行
    list->add("");

    // 保存
    list->saveToFile(fileName_.c_str());

    modified_ = false;
  }
}

/*!
	設定ファイルをクローズする

	@note
	Open() で指定したファイル名で保存します <br>
*/
void IniFile::close() {
  // 終了ついでにファイルを書き込む
  save();

  SAFE_DELETE(pList_);
}

/*!
	クリア
*/
void IniFile::clear() {
  pList_->clear();
}

/*!
	空かどうか
	@return true:空, false:空でない
*/
bool IniFile::isEmpty() {
  return (pList_->count() <= 0);
}

/*!
	セクションか？
	@param[in] s String
	@return true:セクションだ, セクションじゃない
*/
bool IniFile::isSection(nkmm::string &s) {
  int index = s.find("[");  // "[" があったらセクションとする

  // もう少し厳密に
  if (index >= 0) {
    index = s.find("]", index + 1);  // 閉じ！

    if (index >= 0) {
      return true;
    }
  }

  return false;
}

/*!
	キーのデータを取得する
	@param[in] section セクション
	@param[in] key キー
	@return データ
*/
nkmm::string IniFile::getKeyData(const nkmm::string &section, const nkmm::string &key) {
  StringList *list = pList_;
  int state = 0;

  for (int i = 0; i < list->count(); i++) {
    nkmm::string &item = list->Strings[i];

    // empty!!
    if (item.empty()) {
      continue;
    }

    // セクションかチェック
    if (isSection(item)) {
      state |= THIS_SECTION;
    } else {
      // セクションフラグを取り除く
      state &= ~THIS_SECTION;
    }

    switch (state) {
    // 対象のセクションが見つかっている
    // item はセクション
    case FIND_SECTION_THIS_SECTION:
      // すでに一致していた場合は終了
      return nkmm::string("");
    //break;

    // 対象のセクションが見つかっている
    // item はセクションでない
    case FIND_SECTION_THIS_NOSECTION: {
      int index = item.find("=");

      if (index >= 0) {
        nkmm::string t_key = item.subString(0, index).trim(" \t");

        // キーが一致しているか？
        if (key.upCase() == t_key.upCase()) {
          return item.subString(index + 1).trim(" \t");
        }
      } else {
        // failed
      }
    } break;

    // 対象のセクションが見つかっていない
    // item はセクション
    case NOTFIND_SECTION_THIS_SECTION: {
      int index1 = item.find("[");
      int index2 = item.find("]", index1);

      if (index1 >= 0 && index2 >= 0) {
        nkmm::string t_section = item.subString(index1 + 1, index2 - index1 - 1).trim(" \t");

        // セクションが一致しているか？
        if (section.upCase() == t_section.upCase()) {
          state |= FIND_SECTION;
        }
      } else {
        // failed
      }
    } break;
    }
  }

  return nkmm::string("");
}

/*!
	キーのデータを設定する
	@param[in] section セクション
	@param[in] key キー
	@param[in] data データ
*/
void IniFile::setKeyData(const nkmm::string &section, const nkmm::string &key,
                         const nkmm::string &data) {
  StringList *list = pList_;
  int state = 0;

  for (int i = 0; i < list->count(); i++) {
    nkmm::string &item = list->Strings[i];

    // empty!!
    if (item.empty()) {
      continue;
    }

    // セクションかチェック
    if (isSection(item)) {
      state |= THIS_SECTION;
    } else {
      // セクションフラグを取り除く
      state &= ~THIS_SECTION;
    }

    switch (state) {
    // 対象のセクションが見つかっている
    // item はセクション
    case FIND_SECTION_THIS_SECTION:
      // すでに一致していた場合は追加
      list->insert(i, "%s = %s", key.c_str(), data.c_str());  // 追加
      modified_ = true;
      return;
    //break;

    // 対象のセクションが見つかっている
    // item はセクションでない
    case FIND_SECTION_THIS_NOSECTION: {
      int index = item.find("=");

      if (index >= 0) {
        nkmm::string t_key = item.subString(0, index).trim(" \t");

        // キーが一致しているか？
        if (key.upCase() == t_key.upCase()) {
          item = key + " = " + data;  // 更新
          modified_ = true;
          return;
        }
      } else {
        // failed
      }
    } break;

    // 対象のセクションが見つかっていない
    // item はセクション
    case NOTFIND_SECTION_THIS_SECTION: {
      int index1 = item.find("[");
      int index2 = item.find("]", index1);

      if (index1 >= 0 && index2 >= 0) {
        nkmm::string t_section = item.subString(index1 + 1, index2 - index1 - 1).trim(" \t");

        // セクションが一致しているか？
        if (section.upCase() == t_section.upCase()) {
          state |= FIND_SECTION;
        }
      } else {
        // failed
      }
    } break;
    }
  }

  // 新規
  if (!(state & FIND_SECTION)) {
    list->add("[%s]", section.c_str());
  }

  list->add("%s = %s", key.c_str(), data.c_str());  // 追加
  modified_ = true;
}

/*!
	セクションを列挙する
	@param[in] section StringList。nilのときは作成する
	@return セクションリスト
*/
StringList *IniFile::enumSection(StringList *section) {
  StringList *list = pList_;
  int state = 0;

  if (section == 0) {
    section = SAFE_NEW(__SOURCE__, StringList);
  }
  section->clear();

  for (int i = 0; i < list->count(); i++) {
    nkmm::string &item = list->Strings[i];

    // empty!!
    if (item.empty()) {
      continue;
    }

    // セクションかチェック
    if (isSection(item)) {
      state |= THIS_SECTION;
    } else {
      // セクションフラグを取り除く
      state &= ~THIS_SECTION;
    }

    switch (state) {
    // item はセクション
    case THIS_SECTION: {
      int index1 = item.find("[");
      int index2 = item.find("]", index1);

      if (index1 >= 0 && index2 >= 0) {
        nkmm::string t_section = item.subString(index1 + 1, index2 - index1 - 1).trim(" \t");

        // セクション追加
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
	キーを列挙する
	@param[in] key StringList。nilのときは作成する
	@param[in] section セクション名
	@return キーリスト
*/
StringList *IniFile::enumKey(StringList *key, const nkmm::string &section) {
  StringList *list = pList_;
  int state = 0;

  if (key == 0) {
    key = SAFE_NEW(__SOURCE__, StringList);
  }
  key->clear();

  for (int i = 0; i < list->count(); i++) {
    nkmm::string &item = list->Strings[i];

    // empty!!
    if (item.empty()) {
      continue;
    }

    // セクションかチェック
    if (isSection(item)) {
      state |= THIS_SECTION;
    } else {
      // セクションフラグを取り除く
      state &= ~THIS_SECTION;
    }

    switch (state) {
    // 対象のセクションが見つかっている
    // item はセクション
    case FIND_SECTION_THIS_SECTION:
      // すでに一致していた場合は終了
      return key;
    //break;

    // 対象のセクションが見つかっている
    // item はセクションでない
    case FIND_SECTION_THIS_NOSECTION: {
      int index = item.find("=");

      if (index >= 0) {
        nkmm::string t_key = item.subString(0, index).trim(" \t");

        // キー追加
        if (t_key.length() > 0) {
          key->add(t_key.c_str());
        }
      } else {
        // failed
      }
    } break;

    // 対象のセクションが見つかっていない
    // item はセクション
    case NOTFIND_SECTION_THIS_SECTION: {
      int index1 = item.find("[");
      int index2 = item.find("]", index1);

      if (index1 >= 0 && index2 >= 0) {
        nkmm::string t_section = item.subString(index1 + 1, index2 - index1 - 1).trim(" \t");

        // セクションが一致しているか？
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
	設定ファイルからセクションごと削除
	@param[in] section セクション名
*/
void IniFile::deleteSection(const nkmm::string &section) {
  StringList *list = pList_;
  int state = 0;

  for (int i = 0; i < list->count(); i++) {
    nkmm::string &item = list->Strings[i];

    // empty!!
    if (item.empty()) {
      continue;
    }

    // セクションかチェック
    if (isSection(item)) {
      state |= THIS_SECTION;
    } else {
      // セクションフラグを取り除く
      state &= ~THIS_SECTION;
    }

    switch (state) {
    // 対象のセクションが見つかっている
    // item はセクション
    case FIND_SECTION_THIS_SECTION:
      // すでに一致していた場合は終了
      return;
    //break;

    // 対象のセクションが見つかっている
    // item はセクションでない
    case FIND_SECTION_THIS_NOSECTION:
      list->deleteLine(i);  // キーは問答無用で削除
      i--;                  //１つ前に
      modified_ = true;
      break;

    // 対象のセクションが見つかっていない
    // item はセクション
    case NOTFIND_SECTION_THIS_SECTION: {
      int index1 = item.find("[");
      int index2 = item.find("]", index1);

      if (index1 >= 0 && index2 >= 0) {
        nkmm::string t_section = item.subString(index1 + 1, index2 - index1 - 1).trim(" \t");

        // セクションが一致しているか？
        if (section.upCase() == t_section.upCase()) {
          state |= FIND_SECTION;
          list->deleteLine(i);

          i--;  //１つ前に

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
	設定ファイルからキーを削除
	@param[in] section セクション名
	@param[in] key キー名
*/
void IniFile::deleteKey(const nkmm::string &section, const nkmm::string &key) {
  StringList *list = pList_;
  int state = 0;

  for (int i = 0; i < list->count(); i++) {
    nkmm::string &item = list->Strings[i];

    // empty!!
    if (item.empty()) {
      continue;
    }

    // セクションかチェック
    if (isSection(item)) {
      state |= THIS_SECTION;
    } else {
      // セクションフラグを取り除く
      state &= ~THIS_SECTION;
    }

    switch (state) {
    // 対象のセクションが見つかっている
    // item はセクション
    case FIND_SECTION_THIS_SECTION:
      // すでに一致していた場合は終了
      return;
    //break;

    // 対象のセクションが見つかっている
    // item はセクションでない
    case FIND_SECTION_THIS_NOSECTION: {
      int index = item.find("=");

      if (index >= 0) {
        nkmm::string t_key = item.subString(0, index).trim(" \t");

        // キーが一致しているか？
        if (key.upCase() == t_key.upCase()) {
          list->deleteLine(i);  // 削除
          modified_ = true;
          return;
        }
      } else {
        // failed
      }
    } break;

    // 対象のセクションが見つかっていない
    // item はセクション
    case NOTFIND_SECTION_THIS_SECTION: {
      int index1 = item.find("[");
      int index2 = item.find("]", index1);

      if (index1 >= 0 && index2 >= 0) {
        nkmm::string t_section = item.subString(index1 + 1, index2 - index1 - 1).trim(" \t");

        // セクションが一致しているか？
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
	設定ファイルからキーの値を取得する(文字列)
	@param[in] section セクション名
	@param[in] key キー名
	@param[in] def 未設定初期化キー
	@return キーの値
*/
nkmm::string IniFile::readString(const nkmm::string &section, const nkmm::string &key,
                                const nkmm::string &def) {
  nkmm::string data = getKeyData(section, key);

  // 未設定のためデフォルト
  if (data.empty()) {
    data = def;
  }

  return data;
}

/*!
	設定ファイルからキーの値を取得する(数値)
	@param[in] section セクション名
	@param[in] key キー名
	@param[in] def 未設定初期化キー
	@return 数値
*/
int IniFile::readInteger(const nkmm::string &section, const nkmm::string &key, int def) {
  nkmm::string data = getKeyData(section, key);

  // 未設定のためデフォルト
  if (data.empty()) {
    return def;
  }

  return data.asInt();
}

/*!
	設定ファイルからキーの値を取得する(数値)
	@param[in] section セクション名
	@param[in] key キー名
	@param[in] def 未設定初期化キー
	@return 結果
*/
bool IniFile::readBool(const nkmm::string &section, const nkmm::string &key, bool def) {
  nkmm::string data = getKeyData(section, key);

  // 未設定のためデフォルト
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
	設定ファイルにキーの値を書き込む(文字列)
	@param[in] section セクション名
	@param[in] key キー名
	@param[in] write 書き込むデータ
*/
void IniFile::writeString(const nkmm::string &section, const nkmm::string &key,
                          const nkmm::string &write) {
  setKeyData(section, key, write);
}

/*!
	設定ファイルにキーの値を書き込む(数値)
	@param[in] section セクション名
	@param[in] key キー名
	@param[in] write 書き込むデータ
*/
void IniFile::writeInteger(const nkmm::string &section, const nkmm::string &key, int write) {
  nkmm::string data(write, "%d");
  setKeyData(section, key, data);
}

/*!
	設定ファイルにキーの値を書き込む(bool)
	@param[in] section セクション名
	@param[in] key キー名
	@param[in] write 書き込むデータ
	@param[in] string true: true or false, false: 1 or 0
*/
void IniFile::writeBool(const nkmm::string &section, const nkmm::string &key, bool write,
                        bool string) {
  if (string) {
    if (write) {
      setKeyData(section, key, "true");
    } else {
      setKeyData(section, key, "false");
    }
  } else {
    nkmm::string data(static_cast<int>(write), "%d");
    setKeyData(section, key, data);
  }
}

}  //nkmm
