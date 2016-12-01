//-----------------------------------------------------------------------------
/*!
* @file		mx_buffer.cpp
* @brief	�o�b�t�@���� (f*()�̃o�b�t�@��)
* @author	koma
* @version	$Id$
*
* @note
* �E�X�V����  
*/
//-----------------------------------------------------------------------------
#include <stdio.h>
//#include <malloc.h>

#include "mx_buffer.h" /* Buffer operation */
#include "mx_basis.h"

// �o�b�t�@����܂�
int bclose(BUFF* bp) {
  free_(bp->buffer);
  free_(bp);
  return 0;
}

// �o�b�t�@���t���b�V�����܂�
int bflush(BUFF* bp) {
  (void)bp;
  return 0;
}

// �o�b�t�@���� 1 �����擾�A�o�b�t�@�|�C���^��i�߂�
int bgetc(BUFF* bp) {
  unsigned char ch = *bp->curp++;
  return (int)ch;
}

// �o�b�t�@���當������擾���܂�
char* bgets(char* dest, int n, BUFF* bp) {
  char* t_dest = dest;

  while (n--) {
    char ch = *(char*)bp->curp++;

    // �o�b�t�@�I�[�o�[
    if (bp->buffer + bp->bsize < bp->curp) {
      return NULL;
    }

    if (ch == '\r' /*CR(0x0d)*/ || ch == '\n' /*LF(0x0a)*/ || ch == '\0') {
      *dest = '\0';

      // CRLF �Ɣ��f
      if (ch == '\r' && *bp->curp == '\n') {
        bp->curp++;
      }

      return t_dest;
    } else {
      *dest++ = ch;
    }
  }

  return NULL;
}

// �o�b�t�@�̃I�[�v��
BUFF* bopen(const char* path, const char* mode) {
  FILE* fp;
  BUFF* bp;
  int fsize;
  unsigned char* buffer;

  // �t�@�C�����J��
  if ((fp = fopen(path, "rb")) == NULL) {
#ifdef __debugprint
    printf("file open error(%s)\n", path);
#endif            // __debugprint
    return NULL;  //_B_OPEN_ERR;
  }

  // �o�b�t�@�擾�̂��߂Ƀt�@�C���T�C�Y���`�F�b�N����
  fseek(fp, 0L, B_SEEK_END);  // �t�@�C���|�C���^���Ō���ֈړ�
  fsize = ftell(fp);          // �t�@�C���T�C�Y���擾
  fseek(fp, 0L, B_SEEK_SET);  // �t�@�C���|�C���^��擪�ֈړ�

  // �t�@�C���T�C�Y�� 0 �ȉ��Ȃ̂ŃG���[�ŕԂ�
  if (fsize <= 0) {
#ifdef __debugprint
    printf("file size is zero(%s)\n", path);
#endif            // __debugprint
    fclose(fp);   /* file close */
    return NULL;  //_B_OPEN_ERR;
  }

  // �o�b�t�@�擾
  buffer = (unsigned char*)malloc_(sizeof(BUFF) + fsize);
  fread(buffer + sizeof(BUFF), fsize, 1, fp);
  fclose(fp); /* file close */

  // �o�b�t�@�ݒ�
  bp = (BUFF*)buffer;
  bp->curp = buffer + sizeof(BUFF);
  bp->buffer = bp->curp;
  bp->bsize = fsize;

#if _B_USE_NAME
  {
    int i;
    for (i = 0; path[i]; i++) {
      bp->name[i] = path[i];
    }
    bp->name[i] = '\0';
  }
#endif  // _B_USE_NAME

  (void)mode;
  return bp;
}

// �o�b�t�@����P�����o�́A�o�b�t�@�|�C���^��i�߂�
int bputc(int ch, BUFF* bp) {
  *bp->curp++ = (unsigned char)ch;
  return ch;
}

// �o�b�t�@�ɕ�������������݂܂�
int bputs(char* str, BUFF* bp) {
  char* str_ = str;

  while (*str) {
    *bp->curp++ = *str++;
  }

  return str - str_ - 1;
}

// �o�b�t�@����w��T�C�Y�̃f�[�^���w�肵�����������擾����
int bread(void* ptr, int size, int n, BUFF* bp) {
  int i, j;

  for (i = 0; i < n; i++) {
    for (j = 0; j < size; j++) {
      ((unsigned char*)ptr)[i * size + j] = *bp->curp++;
    }
  }

  return size;
}

// �o�b�t�@�̃V�[�N
int bseek(BUFF* bp, long offset, int whence) {
  if (bp == 0) {
    return 1;
  }

  switch (whence) {
  case B_SEEK_CUR:  // ���ݒn����
    bp->curp += offset;
    break;

  case B_SEEK_END:  // �I�[����
    bp->curp = (bp->buffer + bp->bsize) + offset;
    break;

  case B_SEEK_SET:  // �擪����
    bp->curp = bp->buffer + offset;
    break;
  }

  return 0;
}

// �o�b�t�@����w��T�C�Y�̃f�[�^���w�肵�����������o�͂���
int bwrite(const void* ptr, int size, int n, BUFF* bp) {
  int i, j;

  for (i = 0; i < n; i++) {
    for (j = 0; j < size; j++) {
      *bp->curp++ = ((unsigned char*)ptr)[i * size + j];

      if ((bp->buffer + bp->bsize) < bp->curp) {
        return i;
      }
    }
  }

  return n;
}

// �o�b�t�@�|�C���^����I�t�Z�b�g���擾����
int btell(BUFF* bp) {
  return bp->curp - bp->buffer;
}
