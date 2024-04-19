// -*- mode:c++; coding:utf-8-ws -*-
#ifndef NKMM_UTILS_H
#define NKMM_UTILS_H

namespace nkmm {

//class String;
class StringList;

namespace utils {

/*!
* リストから探して補完する
* @param[in] stringList 候補リスト
* @param[in] s1 補完文字
* @return 補完先対象のインデックス
*/
int AutoCompleteString(const StringList* stringList, const nkmm::string& s1);

/*!
* トークン切り出し(複数の区切文字対応)
* @param[out] StringList クラス
* @param[in] src 切り出し対象文字列
* @param[in] delimiters デリミタ字群
* @param[in] ignores 無視字群
* 
* @note
* 以下の場合、半角スペースとタブが対象 <br>
* 
* @code
* ExtractTokenToList( stringList, src, " \t", "" );
* @endcode
*/
void ExtractTokenToList(StringList* stringList, const nkmm::string& src,
                        const nkmm::string& delimiters, const nkmm::string& ignores);

/*!
* リスト内に含まれるコメントをスペースにする
* @param[out] stringList 取り除く StringList クラス
* @param[in] lines 行コメント
* @param[in] blockBegin ブロックコメント開始
* @param[in] blockEnd ブロックコメント終了
* @param[in] ignoreString 文字列は無視する("", '' を文字列とする)
* 
* @note
* コメントの対象となる文字はすべて半角スペースに置き換えられます。<br>
* ignoreString に true を指定した場合は文字列内のコメントは対象外です。<br>
* 文字列は '' "" で囲まれた文字です。<br>
* <br>
* ブロックコメントは開始が設定されているときのみ判定 <br>
* 開始が指定されていて終了が指定されていないときは知りません <br>
* <br>
* コメントのネストにも対応 <br>
*/
void CommentToSpace(StringList* stringList, const nkmm::string& lines,
                    const nkmm::string& blockBegin = 0, const nkmm::string& blockEnd = 0,
                    bool ignoreString = false);

void CommentToSpace(char* wp, const char* lines, const char* blockBegin = 0,
                    const char* blockEnd = 0, bool ignoreString = false);

/*!
* バッファを使用した書式付文字列作成
* @param[in,out] buf バッファ
* @param[in] fmt... 書式
* @return char*
*/
char* sprintf(char* buf, const char* fmt, ...);

/*!
* バッファを使用した書式付文字列作成
* @param[out] buf バッファ
* @param[in] fmt 書式付文字列・可変長引数を含むポインタのポインタ
* @return char*
*/
char* svprintf(char* buf, const char** fmt);

}  //utils
}  //nkmm

#endif  //NKMM_UTILS_H
