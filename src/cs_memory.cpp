//-----------------------------------------------------------------------------
/*!
* @file		mx_memory.cpp
* @brief	�������}�l�[�W��
* @author	koma
* @version	$Id$
*
* @note
* �E�X�V����  
*/
//-----------------------------------------------------------------------------
/*
�󂫗̈�Ǝg�p�̈��ʃ��X�g�ɂ���B
�m�ہE����ɑ����̃R�X�g�͂����邪�A�v�f�̌��ɉe�����Ȃ����ϓI�ȑ��x�ɂȂ�B
�����ɁA�󂫗̈惊�X�g�Ɍq����B
���̂Ƃ��A�A�������󂫗̈�ł���Ό������s���B
*/
#include <cstdint>

#ifdef WIN32
#ifdef MEM_DEBUG
#include <stdio.h>   // vsprintf
#include <stdarg.h>  // va_list
#endif
#else
#include <nitro.h>
#endif

#include "mx_types.h"
#include "mx_memory.h"

#ifdef WIN32
#endif
#ifdef BCC32
#else             //Athor
#define __inline  //inline
#endif

#define RoundUp(size, alignment) (((size) + (alignment)-1) & ~((alignment)-1))  // �A���C�����g�␳

#define BLOCK_SIZE (int)sizeof(BLOCK)
#define INFO_SIZE (int)sizeof(INFO)

//#define MX_R_TYPE       int32_t
#define MX_R_TYPE int64_t
#define MX_R_TYPE_SIZE sizeof(MX_R_TYPE)

//! �������u���b�N�Ǘ��\����
typedef struct tagBLOCK {
  struct tagBLOCK* prev;  //!< �O�̗v�f
  struct tagBLOCK* next;  //!< ���̗v�f
  long size;              //!< �T�C�Y  �󂫗̈惊�X�g  �F�󂫃T�C�Y
                          //           �A���P�[�g���X�g�F�g�p�T�C�Y
#ifdef MEM_DEBUG
  long checkLINE;  //!< �擾�����ꏊ�̍s�ԍ�
#else
  long reserve1;
#endif
} BLOCK;

//! �������Ǘ��\����
typedef struct tagINFO {
  BLOCK* freeTop;     //!< �󂫗̈惊�X�g �擪
  BLOCK* freeBottom;  //!< �󂫗̈惊�X�g �I�[
  BLOCK* allocTop;    //!< �A���P�[�g���X�g
  long allocCount;    //!< �A���P�[�g�J�E���^�j

#ifdef MEM_DEBUG
  long debugAllocCount;  //!< �J�E���^�iAlloc �̑����j
  long debugFreeCount;   //!< �J�E���^�iFree �̑����j
  long reserve1;
  long reserve2;
#endif
} INFO;

#ifdef MEM_DEBUG
static int s_CheckLINE = 0;             //!< �s�ԍ�
static uint8_t s_CheckFILE = 0;         //!< �t�@�C�����̒���
static char s_NameCheckFILE[256] = "";  //!< �t�@�C����
#endif

/*!
* �󂫃������u���b�N�̌����i�O���j
* @param[in] block �����J�n�u���b�N
* @param[in] size �擾�T�C�Y
* @return 0:���s, BLOCK*:�擾�\�ȃ������u���b�N
*/
static __inline BLOCK* getFrontBlock(BLOCK* block, int size) {
  for (/* None */; block; block = block->next) {
    // ���X�g�̒��ɔ[�܂�T�C�Y�̗v�f��������
    if (size <= block->size) {
      break;
    }
  }

  return block;
}

/*!
* �󂫃������u���b�N�̌����i����j
* @param[in] block �����J�n�u���b�N
* @param[in] size �擾�T�C�Y
* @return 0:���s, BLOCK* :�擾�\�ȃ������u���b�N
*/
static __inline BLOCK* getRearBlock(BLOCK* block, int size) {
  for (/* None */; block; block = block->prev) {
    // ���X�g�̒��ɔ[�܂�T�C�Y�̗v�f��������
    if (size <= block->size) {
      break;
    }
  }

  return block;
}

/*!
* �󂫗̈惊�X�g�̃����N
* @param[in] info �Ǘ��|�C���^
* @param[in] block �ڑ��Ώۃu���b�N
*/
static __inline void linkFreeList(INFO* info, BLOCK* block) {
  BLOCK* pos;

  // block �Ɉ�ԋ߂��v�f��T��
  for (pos = info->freeTop; pos; pos = pos->next) {
    if (block < pos) {
      break;
    }
  }

  // block ���擪
  if (pos == 0) {
    pos = info->freeTop;  // ���ݒn�� freeTop �ɂ���

    // �󂫃��X�g�� block �����ɂȂ�
    if (pos == 0) {
      // �󂫗̈惊�X�g�쐬
      block->prev = 0;
      block->next = 0;

      // ���X�g�ݒ�
      info->freeTop = block;
      info->freeBottom = block;

    } else {
      // block �� pos �ׂ͗荇�킹
      if ((char*)block + block->size == (char*)pos) {
        // block �� pos ������
        BLOCK* next = pos->next;

        block->prev = 0;
        block->next = next;

        if (next) {
          next->prev = block;

        } else /*if (next == 0)*/ {
          // ���X�g�ݒ�
          info->freeBottom = block;
        }

      } else {  // block �� pos �ׂ͗荇���Ă��Ȃ�
        // block �� pos ���Ȃ�
        block->prev = 0;
        block->next = pos;

        pos->prev = block;

        // ���X�g�ݒ�
        info->freeTop = block;
      }
    }

  } else {  // pos <-> block <-> pos->next �łȂ�
    if (pos->next) {
      // pos �� block �ׂ͗荇�킹
      if ((char*)pos + pos->size == (char*)block) {
        // pos �� block �� pos->next �ׂ͗荇�킹�B
        if ((char*)block + block->size == (char*)pos->next) {
          // pos �� block �� pos->next ������
          BLOCK* next = pos->next->next;

          pos->size = pos->size + block->size + pos->next->size;
          pos->next = next;

          if (next) {
            next->prev = pos;

          } else /*if (next == 0)*/ {
            // ���X�g�ݒ�
            info->freeBottom = pos;
          }

        } else {  // pos �� block �����ׂ荇�킹
          // pos �� block ������
          pos->size = pos->size + block->size;
        }

      } else if ((char*)block + block->size ==
                 (char*)pos->next) {  // block �� pos->next �ׂ͗荇�킹
        // block �� pos->next ������
        BLOCK* next = pos->next->next;

        block->size = block->size + pos->next->size;
        block->next = next;

        if (next) {
          next->prev = block;

        } else /*if (next == 0)*/ {
          // ���X�g�ݒ�
          info->freeBottom = block;
        }
      }

    } else /*if (pos->next == 0)*/ {
      // pos �� block �ׂ͗荇�킹
      if ((char*)pos + pos->size == (char*)block) {
        // pos �� block ������
        pos->size = pos->size + block->size;

      } else {  // pos �� block �ׂ͗荇���Ă��Ȃ�
        // pos �� block �ƂȂ���
        pos->next = block;

        block->prev = pos;
        block->next = 0;

        // ���X�g�ݒ�
        info->freeBottom = block;
      }
    }
  }
}

/*!
* �󂫗̈惊�X�g�̃A�������N
* @param[in] info �Ǘ��|�C���^
* @param[in] block �ؒf�Ώۃu���b�N
*/
static __inline void unlinkFreeList(INFO* info, BLOCK* block) {
  BLOCK* prev = block->prev;
  BLOCK* next = block->next;

  if (prev) {
    // �� prev �� next
    if (next) {
      prev->next = next;
      next->prev = prev;

    } else /*if (next == 0)*/ {  // �� prev �~ next
      prev->next = 0;
      info->freeBottom = prev;
    }

  } else /*if (prev == 0)*/ {
    // �~ prev �� next
    if (next) {
      next->prev = 0;
      info->freeTop = next;

    } else /*if (next == 0)*/ {  // �~ prev �~ next
      // �����ɂ���Ƃ�����
      // �󂫂����傤�ǂȂ��Ȃ����Ƃ�
      info->freeTop = 0;
      info->freeBottom = 0;
    }
  }
}

/*!
* �A���P�[�g���X�g�̐ڑ��i�擪�ɒǉ����Ă����j
* @param[in] info �Ǘ��|�C���^
* @param[in] block �ڑ��Ώۃu���b�N
*/
static __inline void linkAllocList(INFO* info, BLOCK* block) {
  // �A���P�[�g���X�g�ǉ�
  block->prev = 0;
  block->next = info->allocTop;

  // allocTop ������ΐݒ�
  if (info->allocTop) {
    info->allocTop->prev = block;
  }

  info->allocTop = block;

  // �A���P�[�g�J�E���^�𑝂₷
  info->allocCount++;
}

/*!
* �A���P�[�g���X�g�̐ؒf
* @param[in] info �Ǘ��|�C���^
* @param[in] block �ؒf�Ώۃu���b�N
*/
static __inline void unlinkAllocList(INFO* info, BLOCK* block) {
  BLOCK* prev = block->prev;
  BLOCK* next = block->next;

  if (prev) {
    if (next) {
      prev->next = next;
      next->prev = prev;

    } else /*if (next == 0)*/ {
      prev->next = 0;
    }

  } else /*if (prev == 0)*/ {
    if (next) {
      next->prev = 0;
      info->allocTop = next;

    } else {
      info->allocTop = 0;
    }
  }

  // �A���P�[�g�J�E���^�����炷
  info->allocCount--;
}

namespace mx {

Memory::Memory() : addr_(nullptr), size_(0) {}

void Memory::initialize(void* addr, int size) {
  INFO* info = (INFO*)addr;
  BLOCK* block;

#ifdef MEM_DEBUG
  // �A�h���X�s��
  if (addr == 0) {
    printf("�A�h���X���s��\n");
    return;
  }
#endif

  // �󂫗̈惊�X�g�쐬�i�󂫂Ƃ��čŏ��ɂP�쐬���Ă����j
  block = (BLOCK*)((char*)addr + INFO_SIZE);
  block->prev = 0;
  block->next = 0;
  block->size = size - INFO_SIZE;

  // �o�b�t�@�̐ݒ�
  info->freeTop = block;
  info->freeBottom = block;
  info->allocTop = 0;
  info->allocCount = 0;  // �A���P�[�g�J�E���^

#ifdef MEM_DEBUG
  info->debugAllocCount = 0;
  info->debugFreeCount = 0;
#endif

  // �z�u
  addr_ = addr;
  size_ = size;

#ifdef MEM_DEBUG
  printf("=======================================================\n");
  printf("MEMOEY CREATE!!\n");
  printf("  Area: 0x%.8x - 0x%.8x\n", addr, (char*)addr + size - 1);
  printf("  size: %d\n", size);
  printf("=======================================================\n");
#endif
}

void Memory::clear() {
  initialize(addr_, size_);
}

void* Memory::allocate(int size, int alignment) {
  INFO* info;
  BLOCK* block;  // ���݂̗v�f
  BLOCK* add;

  // �T�C�Y���s��
  if (size < 0) {
#ifdef MEM_DEBUG
    printf("�T�C�Y�̎w�肪�s���ł�\n");
#endif
    return (void*)0;
  }

  info = (INFO*)addr_;

// �擾�T�C�Y�̐ݒ�
#ifdef MEM_DEBUG
  size = RoundUp(size + BLOCK_SIZE + s_CheckFILE, alignment);
#else
  size = RoundUp(size + BLOCK_SIZE, alignment);
#endif

  // �������u���b�N�̎擾�i�O����j
  if (alignment > 0) {
    block = getFrontBlock(info->freeTop, size);

    // �擾���s
    if (block == 0) {
#ifdef MEM_DEBUG
      printf("FRONT:�L���ȃ������u���b�N��������܂���(%d)\n", size);
#endif
      return (void*)0;
    }

    // �ǉ��u���b�N�ݒ�
    add = block;

    if (block->size == size) {
      /*
			* �擾�����󂫃��X�g���T�C�Y0�ɂȂ������߁C�󂫗̈�Ƃ��Ďg���Ȃ�
			* ���̂��߁C�󂫗̈惊�X�g����ؒf����K�v������
			*/

      // �󂫗̈惊�X�g����ؒf�i�T�C�Y���O�ɂȂ���������g���Ȃ��j
      unlinkFreeList(info, block);

    } else {
      /*
			* �O������̎擾�ɔ����C�擪�A�h���X���ω����Ă��܂�����
			* �擾�����󂫃��X�g�̎c��T�C�Y���̃u���b�N���č\�z��
			* �ēx�C�󂫗̈惊�X�g�ɐڑ����Ȃ���
			*/

      int rem_size = block->size - size;  // �c��T�C�Y

      // �󂫗̈惊�X�g����ؒf
      unlinkFreeList(info, block);

      // �u���b�N�Đݒ�
      block = (BLOCK*)((char*)block + size);
      block->size = rem_size;  // �c��T�C�Y�X�V

      // �󂫗̈惊�X�g�ɐڑ�
      linkFreeList(info, block);
    }

  } else /*if (alignment < 0)*/ {  // �������u���b�N�̎擾�i��납��j
    block = getRearBlock(info->freeBottom, size);

    // �擾���s
    if (block == 0) {
#ifdef MEM_DEBUG
      printf("REAR:�L���ȃ������u���b�N��������܂���(%d)\n", size);
#endif
      return (void*)0;
    }

    // �ǉ��u���b�N�ݒ�
    add = (BLOCK*)((char*)block + block->size - size);

    if (block->size == size) {
      // �󂫗̈惊�X�g����ؒf�i�T�C�Y���O�ɂȂ���������g���Ȃ��j
      unlinkFreeList(info, block);

    } else {
      /*
			* �������̎擾�Ȃ̂ŋ󂫗̈惊�X�g�ɓo�^����Ă���u���b�N�T�C�Y�̍X�V������OK
			* �V���ɐڑ����Ȃ����K�v���Ȃ�
			*/

      // �c��T�C�Y�X�V
      block->size = block->size - size;
    }
  }

  // �A���P�[�g���X�g�X�V
  linkAllocList(info, add);

  // �u���b�N�T�C�Y�ݒ�
  add->size = size;

#ifdef MEM_DEBUG
  // �`�F�b�N�|�C���g�ݒ�
  SetCheckPoint(add);

  // �J�E���^�𑝂₷
  info->debugAllocCount++;
#endif

// �Ԃ��|�C���^�ɏ��͊܂܂�
#ifdef MEM_DEBUG
  return (char*)add + BLOCK_SIZE + s_CheckFILE;
#else
  return (char*)add + BLOCK_SIZE;
#endif
}

void Memory::free(void* p) {
  INFO* info;
  BLOCK* block;

  // null ����̕ۏ�
  if (p == 0) {
    return;
  }

  info = (INFO*)addr_;

// �u���b�N�ݒ�
#ifdef MEM_DEBUG
  block = (BLOCK*)((char*)p - BLOCK_SIZE - *((uint8_t*)P - 1));
#else
  block = (BLOCK*)((char*)p - BLOCK_SIZE);
#endif

  // �A���P�[�g���X�g�X�V
  unlinkAllocList(info, block);

  // �󂫗̈惊�X�g�X�V
  linkFreeList(info, block);

#ifdef MEM_DEBUG
  // �J�E���^�����炷
  info->debugFreeCount++;
#endif
}

int Memory::getMaxFreeSize() const {
  INFO* info = (INFO*)addr_;
  BLOCK* pos;
  int size = 0;

  for (pos = info->freeTop; pos; pos = pos->next) {
    if (size < pos->size) {
      size = pos->size;
    }
  }

  return size;
}

int Memory::getTotalFreeSize() const {
  INFO* info = (INFO*)addr_;
  BLOCK* pos;
  int size = 0;

  for (pos = info->freeTop; pos; pos = pos->next) {
    size += pos->size;
  }

  return size;
}

void Memory::dump() {
#ifdef MEM_DEBUG
  INFO* info = (INFO*)addr_;
  BLOCK* pos;
  int total, count;

  printf("\n---------------------\n");
  printf(".debugAllocCount  : %d\n", info->debugAllocCount);
  printf(".debugFreeCount : %d\n", info->debugFreeCount);

  printf("----- space list -----\n");
  total = 0;
  count = 0;
  for (pos = info->freeTop; pos; pos = pos->next) {
    count++;
    printf("%5d: 0x%08x - 0x%08x %d\n", count, pos, (char*)pos + pos->size - 1, pos->size);
    total += pos->size;
  }
  printf("== Total      : %d(%dKB)\n", total, total / 1024);
  printf("== Count      : %d\n", count);

  printf("----- alloc list ------\n");
  total = 0;
  count = 0;
  for (pos = info->allocTop; pos; pos = pos->next) {
    count++;
    if (0 < pos->checkLINE) {
      printf("%5d: 0x%08x - 0x%08x %d, %s(%d)\n", count, pos, (char*)pos + pos->size - 1, pos->size,
             (char*)pos + BLOCK_SIZE, pos->checkLINE);
    } else {
      printf("%5d: 0x%08x - 0x%08x %d, %s(%d)\n", count, pos, (char*)pos + pos->size - 1, pos->size,
             (char*)pos + BLOCK_SIZE, pos->checkLINE);
    }
    total += pos->size;
  }
  printf("== Total      : %d(%dKB)\n", total, total / 1024);
  printf("== Count      : %d\n", count);
  printf("== .allocCount: %d\n", info->allocCount);

  printf("---------------------\n");
#endif
}

// �`�F�b�N�|�C���g�ݒ�i���[�J���j
void Memory::SetCheckPoint(char* checkFILE, int checkLINE) {
#ifdef MEM_DEBUG
  char* dst = s_NameCheckFILE;

  if (!(*checkFILE)) {
    s_CheckFILE = 0;
    s_CheckLINE = checkLINE;
    return;
  }

  // ��������R�s�[
  while (*checkFILE) {
    *dst++ = *checkFILE++;
  }
  *dst++ = '\0';

  s_CheckFILE = (uint8_t)RoundUp((dst - s_NameCheckFILE) + 1, 4);  // �S�o�C�g���E�ɂȂ�悤��
                                                                   // +1 �o�C�g�͕�����̒����L�^�p
  s_CheckLINE = checkLINE;                                         // �s�ԍ�
#else
  (void)checkFILE;
  (void)checkLINE;
#endif
}

// �������̓��e���r
int Memory::Compare(void* A, void* B, int size) {
  MX_R_TYPE* a1 = (MX_R_TYPE*)A;
  MX_R_TYPE* b1 = (MX_R_TYPE*)B;

  // KX_R_TYPE �P�ʂŏ���
  while (MX_R_TYPE_SIZE <= size) {
    size -= MX_R_TYPE_SIZE;

    if (*a1 != *b1) {
      return int(*a1 - *b1);
    }

    a1++;
    b1++;
  }

  int8_t* a8 = (int8_t*)a1;
  int8_t* b8 = (int8_t*)b1;

  // �c�肪����� 1 �o�C�g�P�ʂŏ���
  while (0 < size) {
    size -= 1;

    if (*a8 != *b8) {
      return int(*a8 - *b8);
    }

    a8++;
    b8++;
  }

  return 0;
}

// �������̓��e���R�s�[
void* Memory::Copy(void* A, void* B, int size) {
  MX_R_TYPE* a1 = (MX_R_TYPE*)A;
  MX_R_TYPE* b1 = (MX_R_TYPE*)B;

  // KX_R_TYPE �P�ʂŏ���
  while (MX_R_TYPE_SIZE <= size) {
    size -= MX_R_TYPE_SIZE;
    *a1++ = *b1++;
  }

  int8_t* a8 = (int8_t*)a1;
  int8_t* b8 = (int8_t*)b1;

  // �c�肪����� 1 �o�C�g�P�ʂŏ���
  while (0 < size) {
    size -= 1;
    *a8++ = *b8++;
  }

  return A;
}

// �������̓��e��ݒ�
void* Memory::Fill(void* A, int ch, int size) {
  const int32_t ch_ = ((ch & 0xff) << 24) | ((ch & 0xff) << 16) | ((ch & 0xff) << 8) | (ch & 0xff);

  int32_t* a1 = (int32_t*)A;

  // int32_t(4byte) �P�ʂŏ���
  while (int32_size <= size) {
    size -= int32_size;
    *a1++ = ch_;
  }

  int8_t* a8 = (int8_t*)a1;

  // �c�肪����� 1 �o�C�g�P�ʂŏ���
  while (0 < size) {
    size -= 1;
    *a8++ = ch_ & 0xff;
  }

  return A;
}

// �������̓��e�ɂO��ݒ�
void* Memory::Zero(void* A, int size) {
  return Fill(A, 0, size);
}

}  //mx
