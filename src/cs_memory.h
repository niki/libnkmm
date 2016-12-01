//-----------------------------------------------------------------------------
/*!
* @file		mx_memory.h
* @brief	メモリマネージャ
* @author	koma
* @version	$Id$
*
* @note
* ・更新履歴  
*/
//-----------------------------------------------------------------------------
#ifndef MX_MEMORY_H
#define MX_MEMORY_H

//#define MEM_DEBUG		// デバッグ機能アリ

#ifdef MEM_DEBUG
#define __MEM_CHECKPOINT__ mi::Memory::SetCheckPoint(__FILE__, __LINE__),
#else
#define __MEM_CHECKPOINT__ /* 指定なし */
#endif

namespace mx {

class Memory {
 public:
  Memory();

  void initialize(void* addr, int size);
  void clear();

  void* allocate(int size, int alignment = sizeof(int));
  void free(void* p);

  int getMaxFreeSize() const;
  int getTotalFreeSize() const;

  void dump();

  /*!
	* チェックポイント設定（ローカル）
	* @param[in] checkFILE
	* @param[in] checkLINE
	*/
  static void SetCheckPoint(char* checkFILE, int checkLINE);

  /*!
	* メモリの内容を比較
	* @param[in] A 要素Ａ
	* @param[in] B 要素Ｂ
	* @param[in] size サイズ
	* @return 0:同じ, int < 0:要素Ｂが大きい, int > 0:要素Ａが大きい
	*/
  static int Compare(void* A, void* B, int size);

  /*!
	* メモリの内容をコピー
	* @param[out] A コピー先バッファ
	* @param[in] B コピー元バッファ
	* @param[in] size サイズ
	* @return コピー先バッファ
	*/
  static void* Copy(void* A, void* B, int size);

  /*!
	* メモリの内容を設定
	* @param[out] A コピー先バッファ
	* @param[in] ch 設定する文字
	* @param[in] size サイズ
	* @return コピー先バッファ
	*/
  static void* Fill(void* A, int ch, int size);

  /*!
	* メモリの内容に０を設定
	* @param[out] A コピー先バッファ
	* @param[in] size サイズ
	* @return コピー先バッファ
	*/
  static void* Zero(void* A, int size);

 private:
  void* addr_;
  long size_;
};

}  //mx

#endif  //MX_MEMORY_H
