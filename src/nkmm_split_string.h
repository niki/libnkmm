// -*- mode:c++; coding:utf-8-ws -*-
#ifndef NKMM_SPLITSTRING_H
#define NKMM_SPLITSTRING_H

namespace nkmm {

enum eSplitFlag {
  kSplitIncludeTokenDelimiter = (1 << 0),  // デリミタもトークンとして追加する
  kSplitEscapeString = (1 << 1),           // 文字列をエスケープする
  kSplitIgnoreEmpty = (1 << 2)             // 空のトークンを無視する
};

/*!
	分割文字列クラス
	
	@note
	通常、使う時は次のように使用する。

		nkmm::SplitString split(name, ",");
		split.begin();
		while (split.move_next()) {
			nkmm::string token = split.at();
		}

	指定のトークンを取得する場合は
	
		nkmm::string token1 = split.at(0);
		nkmm::string token2 = split.at(1);
	
	のようにする。
*/
class SplitString {
 public:
  SplitString();
  SplitString(const nkmm::string &text, const char *delimiters, int flag = kSplitEscapeString);
  ~SplitString();

  /*!
		トークンの個数を取得
		@return トークンの個数
	*/
  int size();

  /*!
		最初のトークンを取得
		@return	トークン
	*/
  void begin();

  /*!
		次のトークンへ移動
		@return	true 成功
				false トークンがない
	*/
  bool move_next();
  void operator++(int) { move_next(); }

  /*!
		トークンを取得
		@param[in]	インデックス
		@return	トークン
	*/
  nkmm::string at(int index = -1) const;
  nkmm::string operator[](int index) { return at(index); }

  /*!
		分割
		@param[in]	text テキスト(NULLの場合はテキストを更新しない)
		@param[in]	delimiters デリミタ
		@param[in]	token_delimiter デリミタもトークンにする
		@param[in]	escape_string 文字列をエスケープする
	*/
  void split(const nkmm::string &text, const nkmm::string &delimiters, int flag = kSplitEscapeString);

 private:
  void init();

 private:
  nkmm::string textOrg_;  //!< 文字列のインスタンス（オリジナル）
  nkmm::string text_;     //!< 文字列のインスタンス

  int tokenCnt_;     //!< トークンの数
  int curTokenPos_;  //!< 現在のトークン位置
  const char *tokenPtr_;
  nkmm::string curToken_;
  const char *nextTokenPtr_;
};

}  //nkmm

#endif  //NKMM_SPLITSTRING_H
