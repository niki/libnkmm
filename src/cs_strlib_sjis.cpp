//-----------------------------------------------------------------------------
/*!
* @file		mx_str_sjis.cpp
* @brief	文字列処理
* @author	koma
* @version	$Id$
* 
* @note
* ・更新履歴  
* 2005.05.20	ポインタのチェックをはずしてみる  
*/
//-----------------------------------------------------------------------------

// include
#include <ctype.h>
#include <string.h>
#include <stdio.h>
//#include <extension.h>

#include "mx_types.h"
#include "mx_strlib.h"
#include "mx_strlib_sjis.h"
#include "font_code.ci"

// 半角英数字記号
#define SJIS_ANK_ALPHA_BEGIN ANK_ALPHA_BEGIN
#define SJIS_ANK_ALPHA_END ANK_ALPHA_END

// 半角カタカナ
#define SJIS_ANK_KANA_BEGIN 0xa1
#define SJIS_ANK_KANA_END 0xdf

// 半角カタカナ(特殊)
#define SJIS_ANK_KANA_SP1 0xde  //ﾞ
#define SJIS_ANK_KANA_SP2 0xdf  //ﾟ

// 漢字１バイト目
#define SJIS_KANJI11_BEGIN 0x81
#define SJIS_KANJI11_END 0x9f
#define SJIS_KANJI12_BEGIN 0xe0
#define SJIS_KANJI12_END 0xfc

// 漢字２バイト目
#define SJIS_KANJI21_BEGIN 0x40
#define SJIS_KANJI21_END 0x7e
#define SJIS_KANJI22_BEGIN 0x80
#define SJIS_KANJI22_END 0xfc

namespace mx {

/*!
* 全角小文字を全角大文字に変換する(ａ->Ａ)
* @param[in] cc 文字
* @return 文字
*/
int sjis::toupper(int cc) {
  if ((uint16_t)(cc & 0xff00) == 0x8200 &&
      (0x81 <= (uint16_t)(cc & 0xff) && (uint16_t)(cc & 0xff) <= 0x9a)) {
    return cc - 0x21;
  }

  return cc;
}

/*!
* 全角大文字を全角小文字に変換する(Ａ->ａ)
* @param[in] cc 文字
* @return 文字
*/
int sjis::tolower(int cc) {
  if ((uint16_t)(cc & 0xff00) == 0x8200 &&
      (0x60 <= (uint16_t)(cc & 0xff) && (uint16_t)(cc & 0xff) <= 0x79)) {
    return cc - 0x21;
  }

  return cc;
}

/*!
* 小文字を大文字に変換する(az->AZ)
* @param[in,out] s 文字列
* @return 文字列
*/
char* sjis::strupr(char* s) {
  char* t_s = s;

  while (*s) {
    if (sjis::IsKanji1(*s)) {
      *(short*)s = sjis::toupper(*(short*)s);
      ++s;
      ++s;
    } else /*if (!sjis::IsKanji1(*s))*/ {
      *s = toupper(*s);
      ++s;
    }
  }

  return t_s;
}

/*!
* 大文字を小文字に変換する(AZ->az)
* @param[in,out] s 文字列
* @return 文字列
*/
char* sjis::strlwr(char* s) {
  char* t_s = s;

  while (*s) {
    if (sjis::IsKanji1(*s)) {
      *(short*)s = sjis::tolower(*(short*)s);
      ++s;
      ++s;
    } else /*if (!sjis::IsKanji1(*s))*/ {
      *s = tolower(*s);
      ++s;
    }
  }

  return t_s;
}

/*!
* ２つの文字列をn個比較する(日本語チェックあり)
* @param[in] s1 文字列１
* @param[in] s2 文字列２
* @param[in] len 長さ
* @return 0  s1 = s2, -1  s1 < s2, 1  s1 > s2
*/
int sjis::strncmp(const char* s1, const char* s2, int len) {
  int ret = 0;

  // 結果が違うか len が 0 になるまで
  while (0 < len) {
    if (sjis::IsKanji1(*s1)) {
      if (sjis::IsKanji1(*s2)) {
        ret = (int)(*(uint16_t*)s1 - (int)*(uint16_t*)s2);

        if (ret) {
          break;
        }

        // 比較サイズを減らす
        len -= 2;

        // 文字列のポインタを進める
        s1 += 2;
        s2 += 2;
      } else /*if (!sjis::IsKanji1(*s2))*/ {
        return 1;  // s1 > s2
      }
    } else /*if (!sjis::IsKanji1(*s1))*/ {
      if (!sjis::IsKanji1(*s2)) {
        ret = *s1 - *s2;

        if (ret) {
          break;
        }

        // 比較サイズを減らす
        --len;

        // 文字列のポインタを進める
        ++s1;
        ++s2;
      } else /*if (sjis::IsKanji1(*s2))*/ {
        return -1;  // s1 < s2
      }
    }
  }

  return ret;  // diff ok
}

/*!
* ２つの文字列を比較する(日本語チェックあり)
* @param[in] s1 文字列１
* @param[in] s2 文字列２
* @return 0  s1 = s2, -1  s1 < s2, 1  s1 > s2
*/
int sjis::strcmp(const char* s1, const char* s2) {
  int len1 = strlen(s1);
  int len2 = strlen(s2);

  return sjis::strncmp(s1, s2, (len1 < len2) ? len2 : len1);
}

/*!
* ２つの文字列をn個小文字として比較する(日本語チェックあり)
* @param[in] s1 文字列１
* @param[in] s2 文字列２
* @param[in] len 長さ
* @return 0  s1 = s2, -1  s1 < s2, 1  s1 > s2
*/
int sjis::strnicmp(const char* s1, const char* s2, int len) {
  int ret = 0;

  // 結果が違うか len が 0 になるまで
  while (0 < len) {
    if (sjis::IsKanji1(*s1)) {
      if (sjis::IsKanji1(*s2)) {
        ret = (int)(*(uint16_t*)s1 - (int)*(uint16_t*)s2);

        if (ret) {
          break;
        }

        // 比較サイズを減らす
        len -= 2;

        // 文字列のポインタを進める
        s1 += 2;
        s2 += 2;
      } else /*if (!sjis::IsKanji1(*s2))*/ {
        return 1;  // s1 > s2
      }
    } else /*if (!sjis::IsKanji1(*s1))*/ {
      if (!sjis::IsKanji1(*s2)) {
        ret = tolower(*s1) - tolower(*s2);

        if (ret) {
          break;
        }

        // 比較サイズを減らす
        --len;

        // 文字列のポインタを進める
        ++s1;
        ++s2;
      } else /*if (sjis::IsKanji1(*s2))*/ {
        return -1;  // s1 < s2
      }
    }
  }

  return ret;  // diff ok
}

/*!
* ２つの文字列を小文字として比較する(日本語チェックあり)
* @param[in] s1 文字列１
* @param[in] s2 文字列２
* @return 0  s1 = s2, -1  s1 < s2, 1  s1 > s2
*/
int sjis::stricmp(const char* s1, const char* s2) {
  int len1 = strlen(s1);
  int len2 = strlen(s2);

  return sjis::strnicmp(s1, s2, (s1 < s2) ? len2 : len1);
}

/*!
* 文字列を切り取る
* @param[out] dst 出力先
* @param[in] src 入力元
* @param[in] index 切り取り位置
* @param[in] len 切り取り長さ
* @return char*
*/
char* sjis::strsub(char* dst, const char* src, int index, int len) {
  return strncpy(dst, src + index, len);
}

/*!
* バッファを反転
* @param[in,out] s 文字列
* @return 文字列
*/
char* sjis::strrev(char* s) {
  int i, j;

  // バッファを反転
  for (i = 0, j = strlen(s) - 1; i < j; ++i, --j) {
    char ch = s[i];
    s[i] = s[j];
    s[j] = ch;
  }

  return s;
}

/*!
* タブかスペースかチェック
* @param[in] ch キャラクタ
* @return 0:違う, 1:タブかスペース
*/
int sjis::IsTabSpace(char ch) {
  return (ch == '\t' || ch == ' ');
}

/*!
* 日本語１バイト目の文字か調べる
* @param[in] ch 文字
* @return 1:日本語１バイト目, 0:違う
* 
* @note
* http://www.st.rim.or.jp/~phinloda/cqa/cqa15.html#Q4
*/
int sjis::IsKanji1(char ch) {
  //return ((uint8_t)ch ^ 0x20) - 0xa1 < 0x3c;
  if ((uint8_t)ch >= SJIS_KANJI11_BEGIN && (uint8_t)ch <= SJIS_KANJI11_END) return true;
  if ((uint8_t)ch >= SJIS_KANJI12_BEGIN && (uint8_t)ch <= SJIS_KANJI12_END) return true;
  return false;
}

/*!
* 日本語２バイト目の文字か調べる
* @param[in] ch 文字
* @return 1:日本語２バイト目, 0:違う
*/
int sjis::IsKanji2(char ch) {
  if ((uint8_t)ch >= SJIS_KANJI21_BEGIN && (uint8_t)ch <= SJIS_KANJI21_END) return true;
  if ((uint8_t)ch >= SJIS_KANJI22_BEGIN && (uint8_t)ch <= SJIS_KANJI22_END) return true;
  return false;
}

/*!
* 半角カナか調べる
* @param[in] ch 文字
* @return 1:半角カナ, 0:違う
*/
int sjis::IsKana(char ch) {
  if (SJIS_ANK_KANA_BEGIN <= (uint8_t)ch && (uint8_t)ch <= SJIS_ANK_KANA_END) return true;
  return false;
}

/*!
* 文字列から指定の文字を取り除く
* @param[out] s 文字列
* @param[in] dels 削除文字群
* @param[in] len 長さ
*/
void sjis::RemoveNString(char* s, const char* dels, int len) {
  char* rp = s;

  while (len > 0) {
    if (sjis::IsKanji1(*rp)) {
      int rm = 0;
      const char* p = dels;

      while (*p) {
        if (sjis::IsKanji1(*p)) {
          if (*p == *rp && *(p + 1) == *(rp + 1)) {
            //削除対象
            rm = 1;
            break;
          }
        }

        p += 2;
      }

      if (!rm) {
        *s++ = *(rp + 0);
        *s++ = *(rp + 1);
      }

      ++rp;
      ++rp;

      --len;
      --len;
    } else /*if (!sjis::IsKanji1(*rp))*/ {
      int rm = 0;
      const char* p = dels;

      while (*p) {
        if (!sjis::IsKanji1(*p)) {
          if (*p == *rp) {
            //削除対象
            rm = 1;
            break;
          }
        }

        ++p;
      }

      if (!rm) {
        *s++ = *rp;
      }

      ++rp;

      --len;
    }
  }

  *s = '\0';
}

/*!
* 文字列から指定の１バイト文字を取り除く
* @param[out] s 文字列
* @param[in] dels 削除文字群
*/
void sjis::RemoveString(char* s, const char* dels) {
  sjis::RemoveNString(s, dels, strlen(s));
}

/*!
* len の長さ分だけチェックし nil の分をずらす
* @param[in,out] s 文字列
* @param[in] len 文字列の長さ
* @return 文字列
*/
static __inline char* excludeNull(char* s, int len) {
  int i;
  char* t_s = s;

  // nil を詰める
  for (i = 0; i < len; ++i) {
    // nilじゃないときだけ
    if (t_s[i] != '\0') {
      *s++ = t_s[i];
    }
  }

  *s = '\0';  // 最後に null 文字
  return t_s;
}

/*!
* 先頭から、配列で指定された文字セットをすべて削除します
* @param[in,out] s 文字列
* @param[in] len 文字列の長さ
* @param[in] dels 取り除く対象文字のならび
* 
* @note
* １バイト文字列のみ対応。違う文字がきたらそこで終わり<br>
*/
static __inline void trimStart(char* s, int len, const char* dels) {
  (void)len;

  while (*s) {
    const char* p = strchr(dels, *s);

    if (p == 0) {
      break;
    }

    *s++ = '\0';  // 対象文字は '\0' で埋める
  }
}

/*!
* 末尾から、配列で指定された文字セットをすべて削除します
* @param[in,out] s 文字列
* @param[in] len 文字列の長さ
* @param[in] dels 取り除く対象文字のならび
* 
* @note
* １バイト文字列のみ対応。違う文字がきたらそこで終わり<br>
*/
static __inline void trimEnd(char* s, int len, const char* dels) {
  s += (len - 1);

  while (len--) {
    const char* p = strchr(dels, *s);

    if (p == 0) {
      break;
    }

    *s-- = '\0';  // 対象文字は '\0' で埋める
  }
}

/*!
* 先頭と末尾で、配列で指定された文字セットをすべて削除します
* @param[in,out] s 文字列
* @param[in] dels 取り除く文字群
*/
void sjis::TrimString(char* s, const char* dels) {
  /*if (s)*/ {
    int len = strlen(s);

    if (len == 0) {
      return;
    }

    trimStart(s, len, dels);  // 左側から
    trimEnd(s, len, dels);    // 右側から
    excludeNull(s, len);      // nil を詰める
  }
}

/*!
* 先頭から、配列で指定された文字セットをすべて削除します
* @param[in,out] s 文字列
* @param[in] dels 取り除く文字群
*/
void sjis::TrimStringStart(char* s, const char* dels) {
  /*if (s)*/ {
    int len = strlen(s);

    if (len == 0) {
      return;
    }

    trimStart(s, len, dels);  // 左側から
    excludeNull(s, len);      // nil を詰める
  }
}

/*!
* 末尾から、配列で指定された文字セットをすべて削除します
* @param[in,out] s 文字列
* @param[in] dels 取り除く文字群
*/
void sjis::TrimStringEnd(char* s, const char* dels) {
  /*if (s)*/ {
    int len = strlen(s);

    if (len == 0) {
      return;
    }

    trimEnd(s, len, dels);  // 右側から
    excludeNull(s, len);    // nil を詰める
  }
}

/*!
* 文字数を取得する
* @param[in] s 文字列
* @return 文字数
*/
int sjis::GetCharLength(const char* s) {
  int moji = 0;

  while (*s) {
    uint8_t ch = *(uint8_t*)s++;

    if (sjis::IsKanji1(ch)) {
      ++s;
      ++moji;
    } else /*if (!sjis::IsKanji1(ch))*/ {
      // 英数字記号
      //" !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
      if (SJIS_ANK_ALPHA_BEGIN <= ch && ch <= SJIS_ANK_ALPHA_END) {
        ++moji;
      }
      // 半角カタカナ
      //"｡｢｣､･ｦｧｨｩｪｫｬｭｮｯｰｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃﾄﾅﾆﾇﾈﾉﾊﾋﾌﾍﾎﾏﾐﾑﾒﾓﾔﾕﾖﾗﾘﾙﾚﾛﾜﾝﾞﾟ"
      else if (SJIS_ANK_KANA_BEGIN <= ch && ch <= SJIS_ANK_KANA_END) {
        ++moji;

        if (*(uint8_t*)s == SJIS_ANK_KANA_SP1) {  //ﾞ
          ++s;
        } else if (*(uint8_t*)s == SJIS_ANK_KANA_SP2) {  //ﾟ
          ++s;
        } else {
        }
      } else {
        ++moji;
      }
    }
  }

  return moji;
}

/*!
* 文字列をｎ文字コピー
* @param[out] dst 出力先
* @param[in] src 入力元
* @param[in] num 文字数
* @return 出力先
*/
char* sjis::CopyChar(char* dst, const char* src, int num) {
  int pos = sjis::CharToPos(src, num + 1);
  strncpy(dst, src, pos);
  dst[pos] = '\0'; /* '\0' が自動付加されないときの対策 */
  return dst;
}

/*!
* ｎ文字目の位置を取得する
* @param[in] s 文字列
* @param[in] num 取得したいのは何文字目？（１文字から指定）
* @return int:位置(byte単位), -1:失敗
*/
int sjis::CharToPos(const char* s, int num) {
  uint8_t* t_s = (uint8_t*)s;
  int ch_size;  // 終了したときも文字サイズ(n 文字目の位置を戻すために引く)

  while (*s) {
    uint8_t ch = *(uint8_t*)s++;

    if (sjis::IsKanji1(ch)) {
      ++s;
      ch_size = 2;
    } else /*if (!sjis::IsKanji1(ch))*/ {
      // 英数字記号
      //" !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
      if (SJIS_ANK_ALPHA_BEGIN <= ch && ch <= SJIS_ANK_ALPHA_END) {
        ch_size = 1;
      }
      // 半角カタカナ
      //"｡｢｣､･ｦｧｨｩｪｫｬｭｮｯｰｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃﾄﾅﾆﾇﾈﾉﾊﾋﾌﾍﾎﾏﾐﾑﾒﾓﾔﾕﾖﾗﾘﾙﾚﾛﾜﾝﾞﾟ"
      else if (SJIS_ANK_KANA_BEGIN <= ch && ch <= SJIS_ANK_KANA_END) {
        if (*(uint8_t*)s == SJIS_ANK_KANA_SP1) {  //ﾞ
          ++s;
          ch_size = 2;
        } else if (*(uint8_t*)s == SJIS_ANK_KANA_SP2) {  //ﾟ
          ++s;
          ch_size = 2;
        } else {
          ch_size = 1;
        }
      } else {
        ch_size = 1;
      }
    }

    if (--num <= 0) {
      return (uint8_t*)s - t_s - ch_size;  // 見つけた位置を返す
    }
  }

  return -1;
}

/*!
* pos の位置が何文字目か取得する
* @param[in] s 文字列
* @param[in] pos 取得したい位置の文字数(１バイトから指定)
* @return 位置(文字数単位), -1:失敗
*/
int sjis::PosToChar(const char* s, int pos) {
  int moji = 0;

  while (*s) {
    uint8_t ch = *(uint8_t*)s++;

    if (sjis::IsKanji1(ch)) {
      ++s;
      ++moji;
      --pos;
    } else /*if (!sjis::IsKanji1(ch))*/ {
      // 英数字記号
      //" !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
      if (SJIS_ANK_ALPHA_BEGIN <= ch && ch <= SJIS_ANK_ALPHA_END) {
        ++moji;
      }
      // 半角カタカナ
      //"｡｢｣､･ｦｧｨｩｪｫｬｭｮｯｰｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃﾄﾅﾆﾇﾈﾉﾊﾋﾌﾍﾎﾏﾐﾑﾒﾓﾔﾕﾖﾗﾘﾙﾚﾛﾜﾝﾞﾟ"
      else if (SJIS_ANK_KANA_BEGIN <= ch && ch <= SJIS_ANK_KANA_END) {
        ++moji;

        if (*(uint8_t*)s == SJIS_ANK_KANA_SP1) {  //ﾞ
          ++s;
          --pos;
        } else if (*(uint8_t*)s == SJIS_ANK_KANA_SP2) {  //ﾟ
          ++s;
          --pos;
        } else {
        }
      } else {
        ++moji;
      }
    }

    if (--pos <= 0) {
      return moji;
    }
  }

  return -1;
}

/*!
* 半角を全角に変換する
* （Alphabet Numeric Kana -> Double Byte Character Set）
* @param[out] dst 書き込み先ポインタ
* @param[in] src 読み込み元ポインタ
* @return 書き込み先ポインタ
* 
* @note
* dst と src に同じものは指定できません
*/
char* sjis::AnkToDbcs(char* dst, const char* src) {
  char* t_dst = dst;

  while (*src) {
    uint8_t ch = *(uint8_t*)src++;

    if (sjis::IsKanji1(ch)) {
      *dst++ = (char)ch;
      *dst++ = *src++;
    } else /*if (!sjis::IsKanji1(ch))*/ {
      // 英数字記号
      //" !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
      if (SJIS_ANK_ALPHA_BEGIN <= ch && ch <= SJIS_ANK_ALPHA_END) {
        uint8_t* p = (uint8_t*)fc2;
        p += (ch - SJIS_ANK_ALPHA_BEGIN) * 2;
        *dst++ = *(char*)(p + 1);
        *dst++ = *(char*)p;
      }
      // 半角カタカナ
      //"｡｢｣､･ｦｧｨｩｪｫｬｭｮｯｰｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃﾄﾅﾆﾇﾈﾉﾊﾋﾌﾍﾎﾏﾐﾑﾒﾓﾔﾕﾖﾗﾘﾙﾚﾛﾜﾝﾞﾟ"
      else if (SJIS_ANK_KANA_BEGIN <= ch && ch <= SJIS_ANK_KANA_END) {
        uint8_t* p = (uint8_t*)fc1;
        p += (ch - SJIS_ANK_KANA_BEGIN) * 2;
        *dst++ = *(char*)(p + 1);
        *dst++ = *(char*)p;

        if (*(uint8_t*)src == SJIS_ANK_KANA_SP1) {  //ﾞ
          if (ch == (uint8_t)'ｳ') {                 // ヴ
            *(dst - 1) = (char)0x94;
          } else {
            *(dst - 1) += 1;
          }
          ++src;
        } else if (*(uint8_t*)src == SJIS_ANK_KANA_SP2) {  //ﾟ
          *(dst - 1) += 2;
          ++src;
        }
      }
      // その他(ここにくるってことは抜けがあるってこと？)
      else {
        *dst++ = (char)ch;
      }
    }
  }

  *dst = '\0';
  return t_dst;
}

/*!
* 全角を半角に変換する
* （Double Byte Character Set -> Alphabet Numeric Kana）
* @param[out] dst 書き込み先ポインタ
* @param[in] src 読み込み元ポインタ
* @return 書き込み先ポインタ
* 
* @note
* dst と src に同じものが指定できます
*/
char* sjis::DbcsToAnk(char* dst, const char* src) {
  char* t_dst = dst;

  while (*src) {
    uint8_t ch = *(uint8_t*)src++;

    if (sjis::IsKanji1(ch)) {
      if (ch == 0x81) {
        //" ､｡,.･:;?!ﾞﾟ   ^~_         ｰ  /   |  `' "()  []{}    ｢｣    +-    = <>          \$  %#&*@"
        // "　"0x8140～"＠"0x8197 まで
        if (0x40 <= *(uint8_t*)src && *(uint8_t*)src <= 0x97) {
          uint8_t ch2 = fc81[*(uint8_t*)src - 0x40];

          if (ch2 != 0x7f) {
            *dst++ = (char)ch2;
            ++src;
            continue;  //***
          }
        }
      } else if (ch == 0x82) {
        //"0123456789       ABCDEFGHIJKLMNOPQRSTUVWXYZ      abcdefghijklmnopqrstuvwxyz"
        // "０"0x824f～"ｚ"0x829a まで
        // ０～９
        if (0x4f <= *(uint8_t*)src && *(uint8_t*)src <= 0x58) {
          *dst++ = (char)(0x30 + *(uint8_t*)src - 0x4f);
          ++src;
          continue;  //***
        }
        // Ａ～Ｚ
        else if (0x60 <= *(uint8_t*)src && *(uint8_t*)src <= 0x79) {
          *dst++ = (char)(0x41 + *(uint8_t*)src - 0x60);
          ++src;
          continue;  //***
        }
        // ａ～ｚ
        else if (0x81 <= *(uint8_t*)src && *(uint8_t*)src <= 0x9a) {
          *dst++ = (char)(0x61 + *(uint8_t*)src - 0x81);
          ++src;
          continue;  //***
        }
      } else if (ch == 0x83) {
        //"ｧｱｨｲｩｳｪｴｫｵｶｶｷｷｸｸｹｹｺｺｻｻｼｼｽｽｾｾｿｿﾀﾀﾁﾁｯﾂﾂﾃﾃﾄﾄﾅﾆﾇﾈﾉﾊﾊﾊﾋﾋﾋﾌﾌﾌﾍﾍﾍﾎﾎﾎﾏﾐ ﾑﾒﾓｬﾔｭﾕｮﾖﾗﾘﾙﾚﾛ ﾜ  ｦﾝｳ"
        // 0000000000010101010101010101010101001010100000012012012012012000000000000000000000001
        // "ァ"0x8340～"ヴ"0x8394 まで
        uint8_t ch2 = *(uint8_t*)src - 0x40;
        *dst++ = (char)fc83c[ch2];

        switch ((fc83cf[ch2 / 16] >> ((ch2 % 16) * 2)) & 3) {
        case 1:
          *dst++ = (char)SJIS_ANK_KANA_SP1;
          break;  //ﾞ
        case 2:
          *dst++ = (char)SJIS_ANK_KANA_SP2;
          break;  //ﾟ
        }

        ++src;
        continue;  //***
      }

      // 文字設定
      *dst++ = (char)ch;
      *dst++ = *src++;
    } else /*if (!sjis::IsKanji1(ch))*/ {
      *dst++ = (char)ch;
    }
  }

  *dst = '\0';
  return t_dst;
}

/*!
* 全角カタカナを全角ひらがなに変換する（１文字）
* @param[in,out] str 文字
* 
* @note
* 全角カタカナでないものは変換されません
*/
void sjis::KanaToHira1(char* str) {
  uint8_t ch1 = *(uint8_t*)str;
  uint8_t ch2 = *(uint8_t*)(str + 1);

  // "ァ"0x8340～"ン"0x8393 まで
  // "ぁ"0x829f～"ん"0x82f1 まで
  if (ch1 == 0x83 && 0x40 <= ch2 && ch2 <= 0x93) {
    uint8_t ch2_ = (uint8_t)(ch2 - 0x40);
    *(str + 0) = (char)0x82;
    *(str + 1) = (char)(0x9f + ch2_ -
                        ((0x7e < ch2) ? (uint8_t)1 : (uint8_t)0));  //ミのあと１バイトの空白が（＝＝
  } else {
    // 文字設定
    //		*(str + 0) = (char)ch1;
    //		*(str + 1) = (char)ch2;
  }
}

/*!
* 全角カタカナを全角ひらがなに変換する
* @param[out] dst 書き込み先ポインタ
* @param[in] src 読み込み元ポインタ
* @return 書き込み先ポインタ
*/
char* sjis::KanaToHira(char* dst, const char* src) {
  char* t_dst = dst;

  while (*src) {
    uint8_t ch = *(uint8_t*)src++;

    if (sjis::IsKanji1(ch)) {
      *dst++ = (char)ch;
      *dst++ = *src++;

      sjis::KanaToHira1(dst - 2);
    } else /*if (!sjis::IsKanji1(ch))*/ {
      *dst++ = (char)ch;
    }
  }

  *dst = '\0';
  return t_dst;
}

/*!
* 全角ひらがなを全角カタカナに変換する（１文字）
* @param[in,out] str 文字
* 
* @note
* 全角ひらがなでないものは変換されません
*/
void sjis::HiraToKana1(char* str) {
  uint8_t ch1 = *(uint8_t*)str;
  uint8_t ch2 = *(uint8_t*)(str + 1);

  // "ァ"0x8340～"ン"0x8393 まで
  // "ぁ"0x829f～"ん"0x82f1 まで
  if (ch1 == 0x82 && 0x9f <= ch2 && ch2 <= 0xf1) {
    uint8_t ch2_ = (uint8_t)(ch2 - 0x9f);
    *(str + 0) = (char)0x83;
    *(str + 1) =
        (char)(0x40 + ch2_ +
               ((0xdd < ch2) ? (uint8_t)1 : (uint8_t)0));  //「ミ」のあと１バイトの空白が（＝＝
  } else {
    //		*(str + 0) = (char)ch1;
    //		*(str + 1) = (char)ch2;
  }
}

/*!
* 全角ひらがなを全角カタカナに変換する
* @param[out] dst 書き込み先ポインタ
* @param[in] src 読み込み元ポインタ
* @return 書き込み先ポインタ
*/
char* sjis::HiraToKana(char* dst, const char* src) {
  char* t_dst = dst;

  while (*src) {
    uint8_t ch = *(uint8_t*)src++;

    if (sjis::IsKanji1(ch)) {
      *dst++ = (char)ch;
      *dst++ = *src++;

      sjis::HiraToKana1(dst - 2);
    } else /*if (!sjis::IsKanji1(ch))*/ {
      *dst++ = (char)ch;
    }
  }

  *dst = '\0';
  return t_dst;
}

/*!
* 文字列の変換
* @param[out] dst 変換先
* @param[in] src 変換元
* @param[in] flag 変換フラグ
* @return 変換先
*/
char* sjis::StrConv(char* dst, const char* src, int flag) {
  switch (flag) {
  case toHiragana:  // o 文字列内のカタカナをひらがなに変換します。日本語ロケールのみに適用されます。このメンバは、Visual Basic の定数 vbHiragana と等価です。
    sjis::KanaToHira(dst, src);
    break;

  case toKatakana:  // o 文字列内のひらがなをカタカナに変換します。日本語ロケールのみに適用されます。このメンバは、Visual Basic の定数 vbKatakana と等価です。
    sjis::HiraToKana(dst, src);
    break;

  case toLinguisticCasing:  // x 文字列に適用する大文字小文字の規則を、ファイル システムの規則から言語学的な規則に変換します。このメンバは、Visual Basic の定数 vbLinguisticCasing と等価です。
    break;

  case toLowercase:
  case toLowerCase:  // o 文字列を小文字に変換します。このメンバは、Visual Basic の定数 vbLowerCase と等価です。
    strcpy(dst, src);
    strlwr(dst);
    break;

  case toNarrow:  // o 文字列内の全角文字を半角文字に変換します。アジア ロケールに適用されます。このメンバは、Visual Basic の定数 vbNarrow と等価です。
    sjis::DbcsToAnk(dst, src);
    break;

  case toNone:  // o 変換を実行しません。
    strcpy(dst, src);
    break;

  case toProperCase:  // x 文字列内の各語の最初の文字を大文字に変換します。このメンバは、Visual Basic の定数 vbProperCase と等価です。
    break;

  case toSimplifiedChinese:  // x 中国語の繁体字を簡体字に変換します。このメンバは、Visual Basic の定数 vbSimplifiedChinese と等価です。
    break;

  case toTraditionalChinese:  // x 中国語の簡体字を繁体字に変換します。このメンバは、Visual Basic の定数 vbTraditionalChinese と等価です。
    break;

  case toUppercase:
  case toUpperCase:  // o 文字列を大文字に変換します。このメンバは、Visual Basic の定数 vbUpperCase と等価です。
    strcpy(dst, src);
    strupr(dst);
    break;

  case toWide:  // o 文字列内の半角 (1 バイト) 文字を全角 (2 バイト) 文字に変換します。アジア ロケールに適用されます。このメンバは、Visual Basic の定数 vbWide と等価です。
    sjis::AnkToDbcs(dst, src);
    break;
  }

  return dst;
}

/*!
* 文字検索(ＢＭ法)
* @param[in] s 文字列
* @param[in] ptn パターン
* @return 一致文字のアドレス, 0:見つからない
* 
* @note
* １つ見つけた時点で終了します。
*/
#if 0
char* bm_strstr(const char* s, const char* ptn)
{
	// 文字列 or パターンがあるときだけー
/*	if (!s && !ptn) {
		return -1; // 文字がないよー
	}
*/
	
	int s_len	= strlen(s);
	int ptn_len	= strlen(ptn);
	
	// テキストとパターンの不一致が見つかったときに
	// どれだけずらすかを示す表\_
	int skip[256];
	
	// 変数 i は注目しているテキストの位置、
	// 変数 j は注目しているパターンの位置を表すポインタ
	int	i, j;
	
	// テキストよりパターンのほうが長いので見つからないです
	if (s_len < ptn_len) {
		return 0;
	}
	
	// 表 skip を作成する
	for (i = 0; i < 256; ++i) {
		skip[i] = ptn_len;
	}
	for (i = 0; i < ptn_len - 1; ++i) {
		skip[(uint8_t)ptn[i]] = ptn_len - i - 1;
	}
	
	// ポインタを初期化する。パターンの後ろから前に向かって比較するので、
	// パターンの長さ -1 に初期化する。
	i = ptn_len - 1;
	
	// テキストの最後尾に行き当たるまで繰り返す
	while (i < s_len) {
		// ポインタ j をパターンの最後の文字を指すようにする
		j = ptn_len - 1;
		
		// テキストとパターンが一致する間、繰り返す
		while (s[i] == ptn[j]) {
			// 最初の文字まで一致したら、探索は成功である
			if (j == 0) {
				return s + i;
			}
			
			// ポインタ i と j をそれぞれ１文字分戻す
			i--;
			j--;
		}
		
		// 一致しなかったので、パターンをずらす
		if (skip[(uint8_t)s[i]] > (ptn_len - j)) {
			i += skip[(uint8_t)s[i]];
		}
		else {
			i += ptn_len - j;
		}
	}
	
	
	// 結局見つからなかった
	return 0;
}
#endif

/*!
* 文字検索
* @param[in] s 文字列
* @param[in] ptn パターン
* @param[in] ptnlen パターンの長さ
* @return 一致文字のアドレス, 0:見つからない
*/
char* sjis::SearchNString(const char* s, const char* ptn, int ptnlen) {
  int ch1 = *ptn;                   // 最初の文字
  int ch2 = *(ptn + (ptnlen - 1));  // 最後の文字

  int i;

  // s の長さチェック
  for (i = 0; i < ptnlen; ++i) {
    if (s[i] == '\0') {
      return 0;  // s が ptn よりも短い
    }
  }

  s = s + (ptnlen - 1);  // 位置を ptn 分進める

  for (;;) {
    // 最後と最初が一致
    if (*s == ch2 && *(s - (ptnlen - 1)) == ch1) {
      // ※最初と最後は判定済み
      if (strncmp(s - (ptnlen - 1) + 1, ptn + 1, ptnlen - 2) == 0) {
        return (char*)(s - (ptnlen - 1));
      }
    }
    // null がきたので終了
    else if (*s == '\0') {
      return 0;
    }

    ++s;
  }

  //return 0;
}

/*!
* 文字検索
* @param[in] s 文字列
* @param[in] ptn パターン
* @return 一致文字のアドレス, 0:見つからない
*/
char* sjis::SearchString(const char* s, const char* ptn) {
#if 1  //def NCRT
  return SearchNString(s, ptn, strlen(ptn));
#else
  return (char*)::strstr(s, ptn);
#endif
}

/*!
* 文字検索（大文字小文字同一視）
* @param[in] s 文字列
* @param[in] ptn パターン
* @param[in] ptnlen パターンの長さ
* @return 一致文字のアドレス, 0:見つからない
*/
char* sjis::SearchNCaseString(const char* s, const char* ptn, int ptnlen) {
  int ch1 = tolower(*ptn);                   // 最初の文字
  int ch2 = tolower(*(ptn + (ptnlen - 1)));  // 最後の文字

  // s の長さチェック
  for (int i = 0; i < ptnlen; ++i) {
    if (s[i] == '\0') {
      return 0;  // s が ptn よりも短い
    }
  }

  s = s + (ptnlen - 1);  // 位置を ptn 分進める

  for (;;) {
    // 最後と最初が一致
    if (tolower(*s) == ch2 && tolower(*(s - (ptnlen - 1))) == ch1) {
      // ※最初と最後は判定済み
      if (strnicmp(s - (ptnlen - 1) + 1, ptn + 1, ptnlen - 2) == 0) {
        return (char*)(s - (ptnlen - 1));
      }
    }
    // null がきたので終了
    else if (*s == '\0') {
      return 0;
    }

    ++s;
  }

  //return 0;
}

/*!
* 文字検索（大文字小文字同一視）
* @param[in] s 文字列
* @param[in] ptn パターン
* @return 一致文字のアドレス, 0:見つからない
*/
char* sjis::SearchCaseString(const char* s, const char* ptn) {
  return sjis::SearchNCaseString(s, ptn, strlen(ptn));
}

/*!
* 文字検索
* @param[in] s 文字列
* @param[in] ptn パターン
* @param[in] ptnlen パターンの長さ
* @return 一致文字のアドレス, 0:見つからない
*/
char* sjis::LastSearchNString(const char* s, const char* ptn, int ptnlen) {
  int ch1 = *ptn;                   // 最初の文字
  int ch2 = *(ptn + (ptnlen - 1));  // 最後の文字

  const char* sbak = s;
  int slen;

  // s の長さチェック
  for (int i = 0; i < ptnlen; ++i) {
    if (s[i] == '\0') {
      return 0;  // s が ptn よりも短い
    }
  }

  slen = strlen(s);
  s = s + slen - (ptnlen - 1);

  for (;;) {
    // 最後と最初が一致
    if (*s == ch2 && *(s - (ptnlen - 1)) == ch1) {
      // ※最初と最後は判定済み
      if (strncmp(s - (ptnlen - 1) + 1, ptn + 1, ptnlen - 2) == 0) {
        return (char*)(s - (ptnlen - 1));
      }
    }

    // 最初まで来たので終了
    if (s == sbak) {
      return 0;
    }

    --s;
  }

  //return 0;
}

/*!
* 文字検索
* @param[in] s 文字列
* @param[in] ptn パターン
* @return 一致文字のアドレス, 0:見つからない
*/
char* sjis::LastSearchString(const char* s, const char* ptn) {
  return LastSearchNString(s, ptn, strlen(ptn));
}

/*!
* 文字列の置換（バッファの先端から fromlen バイトを to に置換）
* @param[in,out] s 入出力文字列
* @param[in] fromlen 置換対象文字列の長さ
* @param[in] to 置換文字列
* @param[in] tolen 置換後文字列の長さ
* @return 置換後の文字列の終端
* 
* @note
* ・テキストバッファの先端から fromlen の分だけ文字列 to の tolen 分の文字列に置き換える <br>
* ・to と tolen を指定する必要がある理由は同じ条件で置換を行う場合に to の長さを毎回調べる時間を省けるようにするため <br>
* ・置換先が置換元よりも大きい場合は置換したあとの分だけのバッファをあらかじめ確保しておく必要があります <br>
*/
char* sjis::ReplaceStringStart(char* s, int fromlen, const char* to, int tolen) {
  int i, j;

  // 文字列の終端をオーバーした場合の対処
  // "abcdefg" の "efg" を "hijklmn" など
  // これ必要かな？（汗
  if (s[fromlen] == '\0') {
    s[tolen] = '\0';
  }

  // 置換先が置換元よりも大きい場合 null になるまで (tolen - fromlen) だけ後ろにずらす
  //   [before] abc0123456789
  //   [after]  abcdefg0123456789
  //            (abc -> abcdefg, diff 4)
  // diff 分をバッファとして使う方法です
  if (fromlen < tolen) {
    int diff = tolen - fromlen;  // 差分

    for (i = fromlen; /*s[i]*/; i += diff) {
      int a = i + diff;  // 入れ替え先
      int b = fromlen;   // 入れ替え元

      for (j = 0; j < diff; ++j) {
        char ch = s[b + j];  // 入れ替え～
        s[b + j] = s[a + j];
        s[a + j] = ch;

        // null まで置換したら終わり
        if (ch == '\0') {
          goto _l_next;
        }
      }
    }
  }
  // 置換先が置換元よりも小さい場合 null になるまで (tolen - fromlen) だけ前にずらす
  //   [before] abcdefg0123456789
  //   [after]  abc0123456789
  //            (abcdefg -> abc, diff 4)
  else if (fromlen > tolen) {
    int diff = fromlen - tolen;  // 差分

    for (i = tolen; s[i + diff]; ++i) {
      s[i] = s[i + diff];
    }
    s[i] = '\0';  // null
  }

_l_next:
  // 置換元の位置に置換先文字をコピー
  for (i = 0; i < tolen; ++i) {
    s[i] = to[i];
  }

  // 置換元の位置＋置換先文字列の位置を返す
  return s + tolen;
}

/*!
* 文字列の置換（バッファの中の最初の from を置換）
* @param[in,out] s 入出力文字列
* @param[in] from 置換元
* @param[in] to 置換先
* @return 置換後の文字列の終端, 0:置換するものがなかった1
* 
* @note
* 置換先が置換元よりも大きい場合は置換したあとの分だけのバッファをあらかじめ確保しておく必要があります <br>
* 
* @code
* char* p = s;
* sjis::ReplaceString( p, from, to );
* while ( p ) { p = sjis::ReplaceString( p, from, to ); }
* @endcode
*/
char* sjis::ReplaceString(char* s, const char* from, const char* to) {
  int fromlen = strlen(from);
  char* pos = sjis::SearchNString(s, from, fromlen);  // form があるか？

  // 見つからず
  if (pos == 0) {
    return 0;
  }

  // 置換
  //	return sjis::ReplaceStringStart(s + (pos - s), fromlen, to, strlen(to));
  return sjis::ReplaceStringStart(pos, fromlen, to, strlen(to));
}

/*!
* 文字列の置換
* @param[in] s 文字列
* @param[in] from 置換元
* @param[in] to 置換先
*/
void sjis::ReplaceStringAll(char* s, const char* from, const char* to) {
#if 1  // 少しでも速く
  int fromlen = strlen(from);
  int tolen = strlen(to);

  do {
    s = sjis::SearchNString(s, from, fromlen);  // form があるか？

    if (s) {
      s = sjis::ReplaceStringStart(s, fromlen, to, tolen);
    }
  } while (s);
#else
  while (s) {
    s = sjis::ReplaceString(s, from, to);
  }
#endif
}

/*!
* 文字列の切り抜き
* @param[out] dst 出力先
* @param[in] src 入力元
* @param[in] begin 切り抜き開始文字列("" ではじめから)
* @param[in] end 切り抜き終了文字列("" で最後まで)
* @return 出力先, 0:切り抜き失敗
* 
* @note
* タブ、スペースなどを追加したくない場合はこの関数のあとに
* sjis::RemoveString( str, "\t " );
* を呼び出します。
* 
* @code
* char* s = "abcdefg[hijklmn]opqrstu";
* char buff[16];
* sjis::ExtractPairString( buff, s, "[", "]", IGNORE_TABSPACE );
* 
* //***result***
* //hijklmn
* @endcode
*/
char* sjis::ExtractString(char* dst, const char* src, const char* begin, const char* end) {
  int beginlen = strlen(begin);
  int endlen = strlen(end);

  char* t_dst = dst;

  // スタート位置検索
  if (*begin) {
    char* pos = sjis::SearchString(src, begin);

    if (pos == 0) {
      return 0;  // not find!!
    }

    // 位置を進める
    src = pos + beginlen;
  }

  // 切り出し終了位置まで
  if (*end) {
    // begin ～ end
    while (*src) {
      if (!sjis::IsKanji1(*src)) {
        // 切り出し終了文字列検索
        if (strncmp(src, end, endlen) == 0) {
          break;  // 終了
        }

        *dst++ = *src++;
      } else {
        *dst++ = *src++;
        *dst++ = *src++;
      }
    }
  }
  // 最後まで
  else {
    while (*src) {
      if (!sjis::IsKanji1(*src)) {
        *dst++ = *src++;
      } else {
        *dst++ = *src++;
        *dst++ = *src++;
      }
    }
  }

  // 最後に null
  *dst = '\0';

  return t_dst;
}

/*!
* 文字列の切り抜き（ペアなので開始と終了文字列は必須）
* @param[out] dst 出力先
* @param[in] src 入力元
* @param[in] begin 切り抜き開始文字列
* @param[in] end 切り抜き終了文字列
* @return 出力先, 0:切り抜き失敗
* 
* @code
* char* s = "abcdefg[hij[k]lmn]opqrstu";
* char buff[16];
* sjis::ExtractPairString( buff, s, "[", "]", IGNORE_TABSPACE );
* 
* //***result***
* //hijklmn
* @endcode
*/
char* sjis::ExtractPairString(char* dst, const char* src, const char* begin, const char* end) {
  int pair_cnt = 0;

  int beginlen = strlen(begin);
  int endlen = strlen(end);

  char* t_dst = dst;

  // スタート位置検索
  char* pos = sjis::SearchString(src, begin);

  if (pos == 0) {
    return 0;  // not find!!
  }

  // 位置を進める
  src = pos + beginlen;

  // begin の文字が複数あった場合 end の文字が同じ数だけくるまで切り出す
  while (*src) {
    if (!sjis::IsKanji1(*src)) {
      // 切り出し終了文字列検索
      if (strncmp(src, end, endlen) == 0) {
        if (pair_cnt == 0) {
          break;  // 終了
        }

        pair_cnt--;  // pair_cnt の数だけは end がきても見逃す
      }
      // 文字列追加確認
      else {
        // begin と同じ物がきたので見逃す数を増やす
        if (strncmp(src, begin, beginlen) == 0) {
          ++pair_cnt;
        }
      }

      *dst++ = *src++;
    } else {
      *dst++ = *src++;
      *dst++ = *src++;
    }
  }

  // 最後に null
  *dst = '\0';

  return t_dst;
}

/*!
* 文字列を区切り文字で分解する
* @param[out] dst 出力バッファ
* @param[in] src 入力バッファ
* @param[in] delimiters デリミタ字群
* @param[in] ignores 無視字群
* @return 次の入力バッファ開始位置, 0:終了
*/
const char* sjis::ExtractTokenStart(char* dst, const char* src, const char* delimiters,
                                    const char* ignores) {
  // 検索する
  // 次の文字がなくなるまで...
  for (;;) {
    char ch = *src++;

    // nil
    if (ch == '\0') {
      *dst = '\0';  // 終端
      return 0;
    }
    // １バイト文字
    else if (!sjis::IsKanji1(ch)) {
      // 無視字群から見つかった
      if (strchr(ignores, ch)) {
        continue;
      }
      // デリミタ字群から見つかった
      else if (strchr(delimiters, ch)) {
        *dst = '\0';  // 終端
        return src;
      }
    }
    // ２バイト文字
    else {
      *dst++ = ch;
      ch = *src++;
    }

    *dst++ = ch;
  }

  //	return 0;
}

/*!
* 文字列を区切り文字で分解する
* @param[out] dst 出力バッファ
* @param[in] src 入力バッファ
* @param[in] delimiters デリミタ字群
* @param[in] ignores 無視字群
* @return トークンの数
* 
* @code
* char* s = "abc,def,,g";
* sjis::ExtractToken( s, s, ",", "" );
* 
* //***result***
* //"abc"
* //"def"
* //""
* //"g"
* @endcode
*/
int sjis::ExtractToken(char* dst, const char* src, const char* delimiters, const char* ignores) {
  int len;
  int tok_cnt = 0;

  // 入力を出力にする
  if (!dst) {
    dst = (char*)src;  // illegal
  }

  // 空のトークンも追加する
  for (;;) {
    src = sjis::ExtractTokenStart(dst, src, delimiters, ignores);
    len = strlen(dst);  // トークンの長さ

    dst = dst + len + 1;  // 出力バッファを次の位置へ進める
    ++tok_cnt;            // トークンカウンタを増やす

    // 終わり
    if (src == 0) {
      break;
    }
  }

  return tok_cnt;
}

/*!
* 文字列を区切り文字で分解する（空のトークンは含めない）
* @param[out] dst 出力バッファ
* @param[in] src 入力バッファ
* @param[in] delimiters デリミタ字群
* @param[in] ignores 無視字群
* @return トークンの数
*/
int sjis::ExtractTokenWithoutEmpty(char* dst, const char* src, const char* delimiters,
                                   const char* ignores) {
  int len;
  int tok_cnt = 0;

  // 入力を出力にする
  if (!dst) {
    dst = (char*)src;  // illegal
  }

  // 空のトークンは無視する
  for (;;) {
    src = sjis::ExtractTokenStart(dst, src, delimiters, ignores);
    len = strlen(dst);  // トークンの長さ

    // 空のトークンは無視する
    if (len == 0) {
      //continue;
    } else {
      dst = dst + len + 1;  // 出力バッファを次の位置へ進める
      ++tok_cnt;            // トークンカウンタを増やす
    }

    // 終わり
    if (src == 0) {
      break;
    }
  }

  return tok_cnt;
}

/*!
* \0 で区切られたトークン文字列から n 番目のトークンのポインタを取得する
* @param[in] p 入力元
* @param[in] n 取得位置
* @return トークンの位置
*/
char* sjis::TokenOf(char* p, int n) {
  char* t_p = p;

  for (;;) {
    if (*p++ == '\0') {
      if (--n < 0) {
        return t_p;
      }

      t_p = p;
    }
  }

  //	return 0;
}

/*!
* マスクのチェック
* @param[in] s 文字列
* @param[in] mask マスク
* @return 0:不一致, 1:一致
* 
* @note
* 任意の 1 文字			?		任意の 1 文字に一致します <br>
* 1 つまたは複数の文字	*		任意の 1 つまたは複数の文字を検索します。たとえば、new* では、"new" を含む任意の文字列 ("newfile.txt" など) が検索されます <br>
*/
int sjis::CheckMask(const char* s, const char* mask) {
  for (;;) {
    // 終了チェック
    if (*s == '\0' && *mask == '\0') {
      return 1;
    } else if (*s == '\0' && *mask != '\0') {
      return 0;
    }

    // mask is 1-byte
    if (!sjis::IsKanji1(*mask)) {
      // * 直後の単語を探す
      if (*mask == '*') {
        const char* start = mask + 1;
        const char* end;

        //単語取得
        ++mask;

        if (*mask == '\0') {
          //*で終わりなら無条件de
          return 1;
        }

        for (;;) {
          if (*mask == '\0' || *mask == '*' || *mask == '?') {
            end = mask;
            break;
          } else if (sjis::IsKanji1(*mask)) {
            ++mask;
            ++mask;
          } else {
            ++mask;
          }
        }

        //文字列に単語が含まれている箇所を探す
        if (end - start > 0) {
          const char* find;

          if (*s == '\0') {
            return 0;
          }

          find = sjis::SearchNCaseString(s, start, end - start);

          if (find) {
            //見つかったところまで進める
            s = find + (end - start);
          } else {
            //みつからない
            return 0;
          }
        }
      }
      // ？は１文字パスする
      else if (*mask == '?') {
        ++mask;

        if (!sjis::IsKanji1(*s)) {
          ++s;
        } else {
          ++s;
          ++s;
        }
      } else {
        if (!sjis::IsKanji1(*s)) {
          // 小文字で比較
          if (tolower(*mask) != tolower(*s)) {
            return 0;
          }
        } else {
          return 0;
        }

        ++mask;
        ++s;
      }
    }
    // mask is 2-byte
    else {
      if (!sjis::IsKanji1(*s)) {
        return 0;
      } else {
        short chch1 = (*mask << 8) | *(mask + 1);
        short chch2 = (*s << 8) | *(s + 1);

        //文字が違う
        if (chch1 != chch2) {
          return 0;
        }
      }

      ++mask;
      ++mask;

      ++s;
      ++s;
    }
  }

  // ここにはこないはず
  //	return 0;
}

/*!
* マスクのチェック(複数個対応版)
* @param[in] s 文字列
* @param[in] masks マスク
* @return 0:不一致, 1:一致
*/
int sjis::CheckMasks(const char* s, const char* masks) {
  char t_mask[256];

  for (;;) {
    int len;

    masks = sjis::ExtractTokenStart(t_mask, masks, " ,;", "");
    len = strlen(t_mask);

    // 空のトークンは無視する
    if (len == 0) {
      //continue;
    } else {
      // マスクのチェック
      if (sjis::CheckMask(s, t_mask)) {
        return 1;
      }
    }

    // 終わり
    if (masks == 0) {
      break;
    }
  }

  return 0;
}

/*!
* ファイルマスクのチェック
* @param[in] fileName ファイル名
* @param[in] masks マスク
* @return 0:不一致, 1:一致
*/
int sjis::CheckFileMasks(const char* fileName, const char* masks) {
  const char* pt = 0;

  {
    const char* s = fileName;

    while (*s) {
      if (sjis::IsKanji1(*s)) {
        s += 2;
      } else /*if (!sjis::IsKanji1(*s))*/ {
        if (*s == 0x5c) {
          pt = s;
        }

        ++s;
      }
    }
  }

  if (pt == 0) {
    pt = strrchr(fileName, '/');
  }

  if (pt == 0) {
    pt = fileName;
  }

  return sjis::CheckMasks(pt, masks);
}

}  //mx

/**
参考 http://www.tohoho-web.com/wwwkanji.htm
■ 各コード間の変換アルゴリズムc1を第一バイト、c2を第二バイトとします。
   EUC→SJISは(1)と(3)を、SJIS→EUCは(4)と(2)を組み合わせてください。

(1) EUC から JIS に変換

c1 = c1 - 0x80;
c2 = c2 - 0x80;

(2) JIS から EUC に変換

c1 = c1 + 0x80;
c2 = c2 + 0x80;

(3) JIS から シフトJIS に変換

if (c1 % 2) {
    c1 = ((c1 + 1) / 2) + 0x70;
    c2 = c2 + 0x1f;
} else {
    c1 = (c1 / 2) + 0x70;
    c2 = c2 + 0x7d;
}
if (c1 >= 0xa0) { c1 = c1 + 0x40; }
if (c2 >= 0x7f) { c2 = c2 + 1; }

(4) シフトJIS から JIS に変換

if (c1 >= 0xe0) { c1 = c1 - 0x40; }
if (c2 >= 0x80) { c2 = c2 - 1; }
if (c2 >= 0x9e) {
    c1 = (c1 - 0x70) * 2;
    c2 = c2 - 0x7d;
} else {
    c1 = ((c1 - 0x70) * 2) - 1;
    c2 = c2 - 0x1f;
}
*/
