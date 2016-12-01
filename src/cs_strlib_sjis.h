//-----------------------------------------------------------------------------
/*!
* @file		mx_str_sjis.h
* @brief	文字列処理
* @author	koma
* @version	$Id$
* 
* @note
* 扱っている文字はすべて Shift-JIS です  
* 
* @note
* ・更新履歴  
* 2013.5.31	SJISと Unicodeの区別のため，namespaceを sjisに設定  
*/
//-----------------------------------------------------------------------------

#ifndef MX_STR_SJIS_H
#define MX_STR_SJIS_H

//2008.08.29 mx::Search*String*()で 1文字検索がうまくいかない模様

//#ifdef __cplusplus
//extern "C" {
//#endif /* __cplusplus */

namespace mx {
namespace sjis {

//@name CRT SJIS版
//@{
int toupper(int cc);
int tolower(int cc);

char* strupr(char* s);
char* strlwr(char* s);

int strncmp(const char* s1, const char* s2, int len);
int strcmp(const char* s1, const char* s2);
int strnicmp(const char* s1, const char* s2, int len);
int stricmp(const char* s1, const char* s2);
//@}

char* strsub(char* dst, const char* src, int index, int len);
char* strrev(char* s);

int IsTabSpace(char ch);

int IsKanji1(char ch);
int IsKanji2(char ch);
int IsKana(char ch);

void RemoveNString(char* s, const char* dels, int len);
void RemoveString(char* s, const char* dels);

void TrimString(char* s, const char* dels);
void TrimStringStart(char* s, const char* dels);
void TrimStringEnd(char* s, const char* dels);

int GetCharLength(const char* s);                     // 文字数を取得
char* CopyChar(char* dst, const char* src, int num);  // mx::strcpy（文字単位）
int CharToPos(const char* S, int num);                // ｎ文字目の位置を取得
int PosToChar(const char* s, int pos);                // pos の位置が何文字目か取得

char* AnkToDbcs(char* dst, const char* src);  // Alphabet Numeric Kana -> Double Byte Character Set
char* DbcsToAnk(char* dst, const char* src);  // Double Byte Character Set -> Alphabet Numeric Kana
void KanaToHira1(char* str);
char* KanaToHira(char* dst, const char* src);
void HiraToKana1(char* str);
char* HiraToKana(char* dst, const char* src);
char* StrConv(char* dst, const char* src, int flag);

//char* bm_strstr(const char* s, const char* ptn);

char* SearchNString(const char* d, const char* ptn, int ptnlen);      //strnstr
char* SearchString(const char* s, const char* ptn);                   //strstr string.h
char* SearchNCaseString(const char* s, const char* ptn, int ptnlen);  //strnistr
char* SearchCaseString(const char* s, const char* ptn);               //stristr: PHP

char* LastSearchNString(const char* d, const char* ptn, int ptnlen);
char* LastSearchString(const char* s, const char* ptn);

char* ReplaceStringStart(char* s, int fromlen, const char* to, int tolen);
char* ReplaceString(char* s, const char* from, const char* to);
void ReplaceStringAll(char* s, const char* from, const char* to);

char* ExtractString(char* dst, const char* src, const char* begin, const char* end);
char* ExtractPairString(char* dst, const char* src, const char* begin, const char* end);

const char* ExtractTokenStart(char* dst, const char* src, const char* delimiters,
                              const char* ignores);
int ExtractToken(char* dst, const char* src, const char* delimiters, const char* ignores);
int ExtractTokenWithoutEmpty(char* dst, const char* src, const char* delimiters,
                             const char* ignores);
char* TokenOf(char* p, int n);

int CheckMask(const char* s, const char* mask);
int CheckMasks(const char* s, const char* masks);
int CheckFileMasks(const char* fileName, const char* masks);

//#ifdef __cplusplus
//}      /* extern "C" */
//#endif /* __cplusplus */

}  //sjis
}  //mx

#endif  //MX_STR_SJIS_H
