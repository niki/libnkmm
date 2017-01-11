// -*- mode:c++; coding:utf-8-ws -*-
//====================================================================
//! @file  rei_split_string.h
//! @brief 文字列分割
//!
//! @author
//====================================================================
#ifndef REI_SPLITSTRING_H
#define REI_SPLITSTRING_H

namespace rei {

enum eSplitFlag {
  kSplitIncludeTokenDelimiter = (1 << 0),  // デリミタもトークンとして追加する
  kSplitEscapeString = (1 << 1),           // 文字列をエスケープする
  kSplitIgnoreEmpty = (1 << 2)             // 空のトークンを無視する
};

/*!
	分割文字列クラス
	
	@note
	通常、使う時は次のように使用する。

		rei::SplitString split(name, ",");
		split.begin();
		while (split.move_next()) {
			rei::string token = split.at();
		}

	指定のトークンを取得する場合は
	
		rei::string token1 = split.at(0);
		rei::string token2 = split.at(1);
	
	のようにする。
*/
class SplitString {
 public:
  SplitString();
  SplitString(const rei::string &text, const char *delimiters, int flag = kSplitEscapeString);
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
  rei::string at(int index = -1) const;
  rei::string operator[](int index) { return at(index); }

  /*!
		分割
		@param[in]	text テキスト(NULLの場合はテキストを更新しない)
		@param[in]	delimiters デリミタ
		@param[in]	token_delimiter デリミタもトークンにする
		@param[in]	escape_string 文字列をエスケープする
	*/
  void split(const rei::string &text, const rei::string &delimiters, int flag = kSplitEscapeString);

 private:
  void init();

 private:
  rei::string textOrg_;  //!< 文字列のインスタンス（オリジナル）
  rei::string text_;     //!< 文字列のインスタンス

  int tokenCnt_;     //!< トークンの数
  int curTokenPos_;  //!< 現在のトークン位置
  const char *tokenPtr_;
  rei::string curToken_;
  const char *nextTokenPtr_;
};

}  //rei

#endif  //REI_SPLITSTRING_H
