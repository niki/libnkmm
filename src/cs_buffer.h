//-----------------------------------------------------------------------------
/*!
* @file		mx_buffer.h
* @brief	�o�b�t�@����
* @author	koma
* @version	$Id$
*
* @note
* �E�X�V����  
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
* �o�b�t�@����܂�
* @param[in] bp BUFF �\���̂̃|�C���^
* @return 0 ����
*/
int bclose(BUFF* bp);

/*!
* �o�b�t�@���t���b�V�����܂�
* @param[in] bp BUFF �\���̂̃|�C���^
* @return 0 ����
*/
int bflush(BUFF* bp);

/*!
* �o�b�t�@���� 1 �����擾�A�o�b�t�@�|�C���^��i�߂�
* @param[in,out] bp BUFF �\���̂̃|�C���^
* @return �擾���������� int �^�ɕϊ������l
* 
* @note
* �擾���������� unsigned char ���� int �ɕϊ�����܂� <br>
*/
int bgetc(BUFF* bp);

/*!
* �o�b�t�@���當������擾���܂�
* @param[out] dest �o�͐�
* @param[in] n �ǂݎ��ő啶����
* @param[in,out] bp BUFF �\���̂̃|�C���^
* @return �擾����������, nil  �G���[
*/
char* bgets(char* dest, int n, BUFF* bp);

/*!
* �o�b�t�@�̃I�[�v��
* @param[in] path �t�@�C���p�X
* @param[in] mode ���[�h
* @return BUFF �\���̂̃|�C���^, nil �G���[
* 
* @note
* ���݂� "rb" �݂̂̑Ή��ł� <br>
*/
BUFF* bopen(const char* path, const char* mode);

/*!
* �o�b�t�@����P�����o�́A�o�b�t�@�|�C���^��i�߂�
* @param[in] ch �o�͂��镶��
* @param[in,out] bp BUFF �\���̂̃|�C���^
* @return �o�͂�������
* 
* @note
* �擾���������� int ���� unsigned char �ɕϊ�����܂� <br>
*/
int bputc(int ch, BUFF* bp);

// �o�b�t�@�ɕ�������������݂܂�
int bputs(char* str, BUFF* bp);

/*!
* �o�b�t�@����w��T�C�Y�̃f�[�^���w�肵�����������擾����
* @param[out] ptr �ǂݍ��݃f�[�^�i�[��|�C���^
* @param[in] size �ǂݍ��݃f�[�^�̃o�C�g����
* @param[in] n �ǂݍ��݃f�[�^�̐�
* @param[in,out] bp BUFF �\���̂̃|�C���^
* @return �ǂݍ��񂾃T�C�Y
*/
int bread(void* ptr, int size, int n, BUFF* bp);

/*!
* �o�b�t�@�̃V�[�N
* @param[in,out] bp BUFF �\���̂̃|�C���^
* @param[in] offset �I�t�Z�b�g
* @param[in] whence �J�n�ʒu
* @return ��0:����, ��0:�ُ�
*/
int bseek(BUFF* bp, long offset, int whence);

/*!
* �o�b�t�@����w��T�C�Y�̃f�[�^���w�肵�����������o�͂���
* @param[in] ptr �������݃f�[�^�̃|�C���^
* @param[in] size �������݃f�[�^�̃o�C�g����
* @param[in] n �������݃f�[�^�̐�
* @param[in,out] bp BUFF �\���̂̃|�C���^
* @return �������񂾃f�[�^��
*/
int bwrite(const void* ptr, int size, int n, BUFF* bp);

/*!
* �o�b�t�@�|�C���^����I�t�Z�b�g���擾����
* @param[in] bp BUFF �\���̂̃|�C���^
* @return �I�t�Z�b�g�ʒu
*/
int btell(BUFF* bp);

//#ifdef __cplusplus
//}      /* extern "C" */
//#endif /* __cplusplus */

#endif  //MX_BUFFER_H
