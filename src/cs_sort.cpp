//-----------------------------------------------------------------------------
/*!
* @file		mx_sort.cpp
* @brief	�\�[�g�֐�
* @author	koma
* @version	$Id$
*
* @note
* �E�X�V����  
*/
//-----------------------------------------------------------------------------

// Include
#include "mx_sort.h"
#include "mx_basis.h"
#include "mx_memory.h"

//! �\�[�g�̎��
enum {
  BubbleSort = 0,  //!< �o�u���\�[�g
  InsertSort,      //!< �}���\�[�g
  ShellSort,       //!< �V�F���\�[�g
  SelectSort,      //!< �I���\�[�g
  QuickSort,       //!< �N�C�b�N�\�[�g
};

/*!
	�o�u���\�[�g(��������)
	@param[in,out] data �\�[�g����z��̃|�C���^�̃|�C���^
	@param[in] n �\�[�g����z��̒���
	@param[in] fcmp ��r�֐�
	
	@note
	�|�C���^�̓���ւ���ΏۂƂ����\�[�g
	
	@note
	���~���̔�r�֐��� <br>
	�E���ʂ����ł���΁A����ւ����s�� <br>
	  �`�����a�̕����傫���Ɣ��肷��Ƃ��́A�`�|�a <br>
	  �a�����`�̕����傫���Ɣ��肷��Ƃ��́A�a�|�` <br>
	
	<pre>
	static int	_cmp_func(const void *a, const void *b)
	{
		return( *(int *)a - *(int *)b );
	}
	</pre>
	
	�������̔�r�֐��� <br>
	�E���ʂ����ł���΁A����ւ����s�� <br>
	  �`�����a�̕����������Ɣ��肷��Ƃ��́A�a�|�` <br>
	  �a�����`�̕����������Ɣ��肷��Ƃ��́A�`�|�a <br>
	
	<pre>
	static int	_cmp_func(const void *a, const void *b)
	{
		return( *(int *)b - *(int *)a );
	}
	
	local_BubbleSort( hoge, 8, _cmp_func );
	</pre>
*/
static void local_BubbleSort(uint8_t **data, int n, int (*fcmp)(const void *, const void *)) {
  int i, j;
  uint8_t *temp;  // �e���|����

  // �\�[�g���鐔��ݒ�
  int h = n - 1;

  // �\�[�g����v�f���Ȃ��Ȃ�܂ŌJ��Ԃ�
  do {
    // �c��v�f�����|�P�ŏ�����
    j = -1;

    // �\�[�g����v�f�������J��Ԃ�
    for (i = 1; i <= h; ++i) {
      uint8_t **left = &data[i];       // �����v�f�̃|�C���^
      uint8_t **right = &data[i - 1];  // �E���v�f�̃|�C���^

      // ��r
      //
      if (fcmp(*left, *right) < 0) {
        // �c��v�f�������炷
        j = i - 1;

        // �v�f�̃|�C���^�̓���ւ�
        temp = data[i];
        data[i] = data[j];
        data[j] = temp;
      }
    }

    // �c��v�f���̐ݒ�
    h = j;

  } while (h >= 0);
}

/*!
	�}���\�[�g(��������)
	@param[in,out] data �\�[�g����z��̃|�C���^�̃|�C���^
	@param[in] n �\�[�g����z��̒���
	@param[in] fcmp ��r�֐�
	
	@note
	�|�C���^�̓���ւ���ΏۂƂ����\�[�g
	
	@note
	���~���̔�r�֐��� <br>
	�E���ʂ����ł���΁A����ւ����s�� <br>
	  �`�����a�̕����傫���Ɣ��肷��Ƃ��́A�`�|�a <br>
	  �a�����`�̕����傫���Ɣ��肷��Ƃ��́A�a�|�` <br>
	
	<pre>
	static int	_cmp_func(const void *a, const void *b)
	{
		return( *(int *)a - *(int *)b );
	}
	</pre>
	
	�������̔�r�֐��� <br>
	�E���ʂ����ł���΁A����ւ����s�� <br>
	  �`�����a�̕����������Ɣ��肷��Ƃ��́A�a�|�` <br>
	  �a�����`�̕����������Ɣ��肷��Ƃ��́A�`�|�a <br>
	
	<pre>
	static int	_cmp_func(const void *a, const void *b)
	{
		return( *(int *)b - *(int *)a );
	}
	
	local_InsertSort( hoge, 8, _cmp_func );
	</pre>
*/
static void local_InsertSort(uint8_t **data, int n, int (*fcmp)(const void *, const void *)) {
  int i, j;

  if (n <= 1) {
    return;
  }

  //
  // �}���\�[�g�ł́A���łɃ\�[�g�ς̃��R�[�h�ɐV�������R�[�h�𐳂����ꏊ�ɑ}������B
  // �܂�A�ŏ���2�̃��R�[�h�ɂ��Ă܂����ꊷ���ɂ�萳���������ɂ���B
  // ���ɁA3�ڂ̃��R�[�h��O��2�̃��R�[�h�Ɣ�r���āA�K�v�ɉ����ē��ꊷ�����s���B
  // ����ŁA3�̃��R�[�h�������������ɂȂ�B���̂悤�ɍŌ�̃��R�[�h�܂ŌJ��Ԃ��B
  //

  // 0 ���� n �܂ł��\�[�g�ς݂̎��ɁAdata[i] ��K�؂ȂƂ���ɑ}������
  for (i = 1; i < n; ++i) {
    // �}�����ׂ����R�[�h
    uint8_t *record = data[i];

    // data[j] �� record �����������Ȃ� j �����߂�
    for (j = i - 1; j >= 0 && fcmp(record, data[j]) < 0; --j) {
      // �E��1�v�f�����炷
      data[j + 1] = data[j];
    }

    // record ��}������
    data[j + 1] = record;
  }
}

/*!
	�V�F���\�[�g(��������)
	@param[in,out] data �\�[�g����z��̃|�C���^�̃|�C���^
	@param[in] n �\�[�g����z��̒���
	@param[in] fcmp ��r�֐�
	
	@note
	�|�C���^�̓���ւ���ΏۂƂ����\�[�g
	
	@note
	���~���̔�r�֐��� <br>
	�E���ʂ����ł���΁A����ւ����s�� <br>
	  �`�����a�̕����傫���Ɣ��肷��Ƃ��́A�`�|�a <br>
	  �a�����`�̕����傫���Ɣ��肷��Ƃ��́A�a�|�` <br>
	
	<pre>
	static int	_cmp_func(const void *a, const void *b)
	{
		return( *(int *)a - *(int *)b );
	}
	</pre>
	
	�������̔�r�֐��� <br>
	�E���ʂ����ł���΁A����ւ����s�� <br>
	  �`�����a�̕����������Ɣ��肷��Ƃ��́A�a�|�` <br>
	  �a�����`�̕����������Ɣ��肷��Ƃ��́A�`�|�a <br>
	
	<pre>
	static int	_cmp_func(const void *a, const void *b)
	{
		return( *(int *)b - *(int *)a );
	}
	
	local_ShellSort( hoge, 8, _cmp_func );
	</pre>
*/
static void local_ShellSort(uint8_t **data, int n, int (*fcmp)(const void *, const void *)) {
  int i, j, h;

  if (n <= 1) {
    return;
  }

  // �����l�̌v�Z
  for (h = 1; h <= n; h = 3 * h + 1) {
  }

  while ((h = h / 3) >= 1) {
    // h ���� n �܂ł��\�[�g�ς݂̎��ɁAdata[ i ] ��K�؂ȂƂ���ɑ}������
    for (i = h; i < n; ++i) {
      // �}�����ׂ����R�[�h
      uint8_t *record = data[i];

      // data[j] �� record �����������Ȃ� j �����߂�
      for (j = i - h; j >= 0 && fcmp(record, data[j]) < 0; j -= h) {
        // �E�� h �v�f�����炷
        data[j + h] = data[j];
      }

      // record ��}������
      data[j + h] = record;
    }
  }
}

/*!
	�I���\�[�g(��������)
	@param[in,out] data �\�[�g����z��̃|�C���^�̃|�C���^
	@param[in] n �\�[�g����z��̒���
	@param[in] fcmp ��r�֐�
	
	@note
	�|�C���^�̓���ւ���ΏۂƂ����\�[�g
	
	@note
	���~���̔�r�֐��� <br>
	�E���ʂ����ł���΁A����ւ����s�� <br>
	  �`�����a�̕����傫���Ɣ��肷��Ƃ��́A�`�|�a <br>
	  �a�����`�̕����傫���Ɣ��肷��Ƃ��́A�a�|�` <br>
	
	<pre>
	static int	_cmp_func(const void *a, const void *b)
	{
		return( *(int *)a - *(int *)b );
	}
	</pre>
	
	�������̔�r�֐��� <br>
	�E���ʂ����ł���΁A����ւ����s�� <br>
	  �`�����a�̕����������Ɣ��肷��Ƃ��́A�a�|�` <br>
	  �a�����`�̕����������Ɣ��肷��Ƃ��́A�`�|�a <br>
	
	<pre>
	static int	_cmp_func(const void *a, const void *b)
	{
		return( *(int *)b - *(int *)a );
	}
	
	local_SelectSort( hoge, 8, _cmp_func );
	</pre>
*/
static void local_SelectSort(uint8_t **data, int n, int (*fcmp)(const void *, const void *)) {
  int i, j, k;
  uint8_t *temp;  // �e���|����

  for (i = 0; i < n - 1; ++i) {
    k = i;

    for (j = i + 1; j < n; ++j) {
      if (fcmp(data[j], data[k]) < 0) {
        k = j;
      }
    }

    if (k != i) {
      // �v�f�̃|�C���^�̓���ւ�
      temp = data[i];
      data[i] = data[k];
      data[k] = temp;
    }
  }
}

/*!
	�N�C�b�N�\�[�g(��������)
	@param[in,out] data �\�[�g����z��̃|�C���^�̃|�C���^
	@param[in] left �\�[�g���鍶���̈ʒu
	@param[in] right �\�[�g����E���̈ʒu
	@param[in] fcmp ��r�֐�
	
	@note
	�|�C���^�̓���ւ���ΏۂƂ����\�[�g
	
	@note
	���~���̔�r�֐��� <br>
	�E��(�a)��菬�����l��T���������߁A�`�|�a���s�� <br>
	�E���ʂ����ł���΁A�������������l�ƂȂ�B <br>
	�E���ʂ����ł���΁A�������傫���l�ƂȂ�B <br>
	
	<pre>
	static int	_cmp_func(const void *a, const void *b)
	{
		return( *(int *)a - *(int *)b );
	}
	</pre>
	
	�������̔�r�֐��� <br>
	�E��(�a)���傫���l��T���������߁A�a�|�`���s�� <br>
	�E���ʂ����ł���΁A�������傫���l�ƂȂ�B <br>
	�E���ʂ����ł���΁A�������������l�ƂȂ�B <br>
	
	<pre>
	static int	_cmp_func(const void *a, const void *b)
	{
		return( *(int *)b - *(int *)a );
	}
	
	local_QuickSort( hoge, 0, 8, _cmp_func );
	</pre>
*/
static void local_QuickSort(uint8_t **data, int left, int right,
                            int (*fcmp)(const void *, const void *)) {
  int i, j;
  uint8_t *temp;   // �e���|����
  uint8_t *pivot;  // ���Ƃ���v�f�̃|�C���^

  const int THREASHOLD = 11;  // �N�C�b�N�\�[�g��ʂ̃\�[�g�ɐ؂�ւ���臒l

  // �I������
  if (left > right) {
    return;
  }

  // ������x�̗v�f���܂Ō�������A
  // ���ʂȃI�[�o�[�w�b�h��h�����߂ɁA
  // �\�[�g�֐���؂�ւ���B
  if (right - left < THREASHOLD) {
    //		local_BubbleSort(&data[left], right - left + 1, fcmp );		// �o�u���\�[�g
    //		local_InsertSort(&data[left], right - left + 1, fcmp );		// �}���\�[�g
    //		local_ShellSort(&data[left], right - left + 1, fcmp );		// �V�F���\�[�g
    local_SelectSort(&data[left], right - left + 1, fcmp);  // �I���\�[�g
    return;
  }

  // ���S�����ɐݒ�
  pivot = data[(left + right) / 2];

  // i �̏����ʒu
  i = left - 1;

  // j �̏����ʒu
  j = right + 1;

  for (;;) {
    // ��r�֐�:fcmp
    //   �E�������Ƃ��A
    //     �߂�l���O
    //       �~���F�����玲��菬�����l��T��
    //       �����F�����玲���傫���l��T��
    //     �߂�l���O
    //       �~���F�E���玲���傫���l��T��
    //       �����F�E���玲��菬�����l��T��
    while (fcmp(data[++i], pivot) < 0) {
    }
    while (fcmp(data[--j], pivot) > 0) {
    }

    // i �� j ������ւ������I��
    if (i >= j) {
      break;
    }

    // i �� j ���w���v�f�̃|�C���^����ꊷ����
    temp = data[i];
    data[i] = data[j];
    data[j] = temp;
  }

  // ����菬�����O���[�v�ɃN�C�b�N�\�[�g
  local_QuickSort(data, left, i - 1, fcmp);

  // �����傫���O���[�v�ɃN�C�b�N�\�[�g
  local_QuickSort(data, j + 1, right, fcmp);
}

/*!
	�\�[�g���
	@param[in] type �\�[�g�̎��
	@param[out] dest �o�͐�o�b�t�@
	@param[in] src ���͌��o�b�t�@
	@param[in] n �\�[�g����z��̒���
	@param[in] w �v�f�P�̃T�C�Y
	@param[in] swap_w ����ւ��T�C�Y
	@param[in] fcmp ��r�֐�
	
	@note
	���͌��Əo�͐悪�����ꍇ�̓e���|�����o�b�t�@���쐬���܂��B <br>
	��r�֐��̍쐬���@�́A�e�\�[�g�����̐��������Ă��������B <br>
	
	@note
	iSort( BubbleSort, hoge, hoge, 8, sizeof(int), sizeof(int), _cmp_func );
*/
static void iSort(int type, void *dest, void *src, int n, int w, int swap_w,
                  int (*fcmp)(const void *, const void *)) {
  int i;
  uint8_t **pp;   // �\�[�g����v�f�̃|�C���^
  uint8_t *data;  // �\�[�g����v�f�̎���

  // �v�f�̃|�C���^�̗̈���m��
  pp = (uint8_t **)malloc_(sizeof(uint8_t *) * n);

  /**********************/
  /* �\�[�g�o�b�t�@�ݒ� */
  /**********************/
  // ���͌��Əo�͐悪�����Ȃ̂Ńe���|�����o�b�t�@���쐬
  if (dest == src) {
    // �v�f�̗̈���m��
    data = (uint8_t *)malloc_(sizeof(uint8_t) * n * w);

    // ���f�[�^���R�s�[
    mx::Memory::Copy(data, src, n * w);
  }
  // ���͌��o�b�t�@�����̂܂܎g�p
  else {
    // �v�f�͓��͌�
    data = (uint8_t *)src;
  }

  // �\�[�g�̂��߂Ƀ|�C���^�̃}�b�v�̍쐬
  for (i = 0; i < n; ++i) {
    // �\�[�g����f�[�^�̃|�C���^�����蓖�Ă�
    pp[i] = &data[i * w];
  }

  // �\�[�g����
  switch (type) {
  case BubbleSort:
    local_BubbleSort(pp, n, fcmp);
    break;  // �o�u���\�[�g
  case InsertSort:
    local_InsertSort(pp, n, fcmp);
    break;  // �}���\�[�g
  case ShellSort:
    local_ShellSort(pp, n, fcmp);
    break;  // �V�F���\�[�g
  case SelectSort:
    local_SelectSort(pp, n, fcmp);
    break;  // �I���\�[�g
  case QuickSort:
    local_QuickSort(pp, 0, n - 1, fcmp);
    break;  // �N�C�b�N�\�[�g
  }

  // �\�[�g�������ԂɃf�[�^���R�s�[
  for (i = 0; i < n; ++i) {
    mx::Memory::Copy(&((uint8_t *)dest)[i * w], pp[i], swap_w);
  }

  // �\�[�g�Ɏg�����̈���J��
  if (dest == src) {
    free_(data);
  }
  free_(pp);
}

/*!
	�\�[�g
	@param[in,out] array �\�[�g����o�b�t�@
	@param[in] n �\�[�g����z��̒���
	@param[in] w �v�f�P�̃T�C�Y
	@param[in] fcmp ��r�֐�
	
	@note
	���͌��Əo�͐悪�����ꍇ�̓e���|�����o�b�t�@���쐬���܂��B <br>
	��r�֐��̍쐬���@�́A�e�\�[�g�����̐��������Ă��������B <br>
	
	@note
	Sort( BubbleSort, hoge, 8, sizeof(int), _cmp_func );
*/

namespace mx {

void bsort(void *array, int n, int w, int (*fcmp)(const void *, const void *)) {
  iSort(BubbleSort, array, array, n, w, w, fcmp);
}
void inssort(void *array, int n, int w, int (*fcmp)(const void *, const void *)) {
  iSort(InsertSort, array, array, n, w, w, fcmp);
}
void ssort(void *array, int n, int w, int (*fcmp)(const void *, const void *)) {
  iSort(ShellSort, array, array, n, w, w, fcmp);
}
void selsort(void *array, int n, int w, int (*fcmp)(const void *, const void *)) {
  iSort(SelectSort, array, array, n, w, w, fcmp);
}
void qsort(void *array, int n, int w, int (*fcmp)(const void *, const void *)) {
  iSort(QuickSort, array, array, n, w, w, fcmp);
}

}  //mx
