//-----------------------------------------------------------------------------
/*!
* @file		mx_buffer.h
* @brief	バッファ操作
* @author	koma
* @version	$Id$
*
* @note
* ・更新履歴  
*/
//-----------------------------------------------------------------------------
#ifndef MX_BUFFER_H
#define MX_BUFFER_H

#define _B_USE_NAME 0 /* Use data name */

#define B_SEEK_CUR 1
#define B_SEEK_END 2
#define B_SEEK_SET 0

//#define _B_READY     0x0000     /* No Error */
//#define _B_ERR       0x0010     /* Error */
//#define _B_OVER      0x0020     /* Current pointer over */
//#define _B_WR_ERR    0x0100     /* Buffer write error */

//#define _B_OPEN_ERR  ((BUFF *)0xfefefefe) /* File open error */

typedef struct _BUFF {
  unsigned char* curp;   /* Current active pointer     */
  unsigned char* buffer; /* Data transfer buffer       */
  unsigned char pad0;    /*                            */
  unsigned char pad1;    /*                            */
  int bsize;             /* Buffer size                */
#if _B_USE_NAME
  char name[248]; /* Data name                  */
#endif            // _B_USE_NAME
} BUFF;

//#ifdef __cplusplus
//extern "C" {
//#endif /* __cplusplus */

/*!
* バッファを閉じます
* @param[in] bp BUFF 構造体のポインタ
* @return 0 正常
*/
int bclose(BUFF* bp);

/*!
* バッファをフラッシュします
* @param[in] bp BUFF 構造体のポインタ
* @return 0 正常
*/
int bflush(BUFF* bp);

/*!
* バッファから 1 文字取得、バッファポインタを進める
* @param[in,out] bp BUFF 構造体のポインタ
* @return 取得した文字を int 型に変換した値
* 
* @note
* 取得した文字は unsigned char から int に変換されます <br>
*/
int bgetc(BUFF* bp);

/*!
* バッファから文字列を取得します
* @param[out] dest 出力先
* @param[in] n 読み取る最大文字数
* @param[in,out] bp BUFF 構造体のポインタ
* @return 取得した文字列, nil  エラー
*/
char* bgets(char* dest, int n, BUFF* bp);

/*!
* バッファのオープン
* @param[in] path ファイルパス
* @param[in] mode モード
* @return BUFF 構造体のポインタ, nil エラー
* 
* @note
* 現在は "rb" のみの対応です <br>
*/
BUFF* bopen(const char* path, const char* mode);

/*!
* バッファから１文字出力、バッファポインタを進める
* @param[in] ch 出力する文字
* @param[in,out] bp BUFF 構造体のポインタ
* @return 出力した文字
* 
* @note
* 取得した文字は int から unsigned char に変換されます <br>
*/
int bputc(int ch, BUFF* bp);

// バッファに文字列を書き込みます
int bputs(char* str, BUFF* bp);

/*!
* バッファから指定サイズのデータを指定した個数分だけ取得する
* @param[out] ptr 読み込みデータ格納先ポインタ
* @param[in] size 読み込みデータのバイト長さ
* @param[in] n 読み込みデータの数
* @param[in,out] bp BUFF 構造体のポインタ
* @return 読み込んだサイズ
*/
int bread(void* ptr, int size, int n, BUFF* bp);

/*!
* バッファのシーク
* @param[in,out] bp BUFF 構造体のポインタ
* @param[in] offset オフセット
* @param[in] whence 開始位置
* @return ＝0:正常, ≠0:異常
*/
int bseek(BUFF* bp, long offset, int whence);

/*!
* バッファから指定サイズのデータを指定した個数分だけ出力する
* @param[in] ptr 書き込みデータのポインタ
* @param[in] size 書き込みデータのバイト長さ
* @param[in] n 書き込みデータの数
* @param[in,out] bp BUFF 構造体のポインタ
* @return 書き込んだデータ個数
*/
int bwrite(const void* ptr, int size, int n, BUFF* bp);

/*!
* バッファポインタからオフセットを取得する
* @param[in] bp BUFF 構造体のポインタ
* @return オフセット位置
*/
int btell(BUFF* bp);

//#ifdef __cplusplus
//}      /* extern "C" */
//#endif /* __cplusplus */

#endif  //MX_BUFFER_H
