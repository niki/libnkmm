//-----------------------------------------------------------------------------
/*!
* @file		mx_memory.h
* @brief	�������}�l�[�W��
* @author	koma
* @version	$Id$
*
* @note
* �E�X�V����  
*/
//-----------------------------------------------------------------------------
#ifndef MX_MEMORY_H
#define MX_MEMORY_H

//#define MEM_DEBUG		// �f�o�b�O�@�\�A��

#ifdef MEM_DEBUG
#define __MEM_CHECKPOINT__ mi::Memory::SetCheckPoint(__FILE__, __LINE__),
#else
#define __MEM_CHECKPOINT__ /* �w��Ȃ� */
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
	* �`�F�b�N�|�C���g�ݒ�i���[�J���j
	* @param[in] checkFILE
	* @param[in] checkLINE
	*/
  static void SetCheckPoint(char* checkFILE, int checkLINE);

  /*!
	* �������̓��e���r
	* @param[in] A �v�f�`
	* @param[in] B �v�f�a
	* @param[in] size �T�C�Y
	* @return 0:����, int < 0:�v�f�a���傫��, int > 0:�v�f�`���傫��
	*/
  static int Compare(void* A, void* B, int size);

  /*!
	* �������̓��e���R�s�[
	* @param[out] A �R�s�[��o�b�t�@
	* @param[in] B �R�s�[���o�b�t�@
	* @param[in] size �T�C�Y
	* @return �R�s�[��o�b�t�@
	*/
  static void* Copy(void* A, void* B, int size);

  /*!
	* �������̓��e��ݒ�
	* @param[out] A �R�s�[��o�b�t�@
	* @param[in] ch �ݒ肷�镶��
	* @param[in] size �T�C�Y
	* @return �R�s�[��o�b�t�@
	*/
  static void* Fill(void* A, int ch, int size);

  /*!
	* �������̓��e�ɂO��ݒ�
	* @param[out] A �R�s�[��o�b�t�@
	* @param[in] size �T�C�Y
	* @return �R�s�[��o�b�t�@
	*/
  static void* Zero(void* A, int size);

 private:
  void* addr_;
  long size_;
};

}  //mx

#endif  //MX_MEMORY_H
