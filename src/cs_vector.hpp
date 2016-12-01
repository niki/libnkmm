//-----------------------------------------------------------------------------
/*!
* @file		mx_vector.hpp
* @brief	���I�z��N���X(�ł��邾���P����)
* @author	koma
* @version	$Id$
* 
* @note
* ���o�I�ɂ� std::vector �Ɠ����悤�Ȃ��̂ł��B�ł��y�΂��B  
* ��ȈႢ�� std::vector �̗̈�m�ۂ� n^2 �ɑ΂��Cn+reserve �ɂȂ�܂��B  
* �z��Ɠ����悤�ɃA�N�Z�X�ł��܂����A�v�f�� push_back() �Ŋg�����܂��B  
* �z��̈�g��̂Ƃ��ɂ́A�z��v�f�̍Ĕz�u���s���Ă��܂��B  
* �Q�Ƃ��������A�폜���x���ł�(�z��Ƃ�������)  
* 
* @note
* �f�Љ����ꂽ�̈�𐮗񂷂鏈������ꂽ���B  
* 
* @note
* �E�X�V����  
* 2006.09.13	�C�e���[�^����  
* 2006.05.31	�}���E�V�F���E�I���E�N�C�b�N�\�[�g��ǉ�  
* 2005.05.02	�A���P�[�g�A�C�e�����`�F�C���łȂ��z��}�b�v��ɒu���悤�ɏC��(�ł��邩����f�Љ���h�����߂̑[�u)  
* 2005.04.30	���܂�[���l���Ȃ��Ŋȑf�ɂ���  
*/
//-----------------------------------------------------------------------------
#ifndef MX_VECTOR_H
#define MX_VECTOR_H

//
#define MX_VECTOR_SMALL

/**
�������m�ێ��� reserve_ �̊Ԋu�Ŕz����g�����Ă����܂��B
�z��g�����Ƀ|�C���^�̍Ĕz�u���s���Ă��邽�߁A
�Ԋu���L���قǍĔz�u�̉񐔂����Ȃ��Ȃ�܂��B
�}�ŕ\���ƁA

�-----------�-----------�
|  alloc 0  |  alloc 1  |
�-----------�-----------�
|         size_         |
�-----------------------�-----------�------------�
|               maxSize_            |  alloc_(2) |
�-----------------------------------�------------�

reserve_ = 2 �̂Ƃ�
�-----------�----------�---------�
|           |          | items_  |
|           | items_[] �---------�
|           |          | items_  |
| items_*[] �-----------�---------�
|           |          | items_  |
|           | items_[] �---------�
|           |          | items_  |
�-----------�----------�---------�

�̂悤�ȍ\���ɂȂ�܂��B
�폜���͗̈���J�������Ɏc���Ă������ƂŁA
�ȍ~�A�ǉ��E�}�������Ƃ��ɐV�����̈���m�ۂ����ɁA
�c���Ă������̈���g�p���邱�ƂŁA�A���P�[�g�̉񐔂����炵�Ă��܂��B
���̂��߁A�g�p���Ă��Ȃ��̈���c���Ă��邽�߂ɁA
�������̎g�p�ʂ�K�v�ȏ�ɏ����\\_��������܂��B
���̂悤�ȏꍇ�͈�x .clean() �����邱�Ƃŉ������Ă��������B
���I�ɔz����g�����Ă����̂Ń������̒f�Љ��̌����ɂȂ�\��������܂��B

    index 1 �� .erase() �����ꍇ
�-�-�-�-�-�-�-�    �-�-�-�-�-�-�-�
|0|1|2|3|?|?|?| �� |0|2|3|1|?|?|?|
�-�-�-�-|-�-�-�    �-�-�-|-�-�-�-�
       size_            size_

    index 1 �� .Insert() �����ꍇ
�-�-�-�-�-�-�-�    �-�-�-�-�-�-�-�
|0|1|2|3|?|?|?| �� |0|4|1|2|3|?|?|
�-�-�-�-|-�-�-�    �-�-�-�-�-|-�-�
       size_                size_

���g�p��
void main()
{
	int i;
	mx::vector<RECT> items;
	
	RECT rect;
	
	rect.left   = 0;
	rect.top    = 0;
	rect.right  = 320;
	rect.bottom = 240;
	items.push_back(rect);
	
	rect.left   = 0;
	rect.top    = 0;
	rect.right  = 640;
	rect.bottom = 480;
	items.insert(0, rect); // index 0 �ɑ}��
	
	rect.left   = 0;
	rect.top    = 0;
	rect.right  = 480;
	rect.bottom = 272;
	items.push_back(rect);
	
	for (i = 0; i < items.size(); ++i) {
		RECT &rc = items[i];
		
		printf("Rectangle:\n");
		printf("  left   %d\n", rc.left);
		printf("  top    %d\n", rc.top);
		printf("  right  %d\n", rc.right);
		printf("  bottom %d\n", rc.bottom);
	}
}
*/

namespace mx {

//!  �N���X
template <class T>
class vector {
 public:
  //	typedef T *iterator;
  //	typedef T &reference;
  //	typedef const T *const_iterator;

  //	typedef T *reverse_iterator;
  //	typedef T &reverse_reference;
  //	typedef const T *const_reverse_iterator;

  /*!
	* �R���X�g���N�^
	* @param[in] reserve �g���̂��тɊm�ۂ��鐔
	*/
  vector(int reserve = 32) : items_(0), reserve_(reserve), size_(0), maxSize_(0), alloc_(0) {}

  /*!
	* �f�X�g���N�^
	*/
  virtual ~vector() { this->clean(); }

  /*!
	* �󂩂ǂ���
	* @return true:��, false:��łȂ�
	*/
  bool empty() const { return (size_ == 0); }

  /*!
	* �A�C�e���̊m�ې��ݒ�
	* @param[in] reserve �g���̂��тɊm�ۂ��鐔
	*/
  void reserve(int reserve) { reserve_ = reserve; }

  /*!
	* �z��T�C�Y�擾
	* @return int
	*/
  int size() const { return size_; }

  /*!
	* �ő�z��T�C�Y�擾
	* @return int
	*/
  int max_size() const { return maxSize_; }

  /*!
	* �A�C�e���̃|�C���^�擾
	* @param[in] index 
	* @return T&
	*/
  T& at(int index) const { return *items_[index]; }

  /*!
	* �ŏ��̗v�f���擾
	* @return T&
	*/
  T& front() const { return at(0); }

  /*!
	* �Ō�̗v�f���擾
	* @return T&
	*/
  T& back() const { return at(size() - 1); }

  /*!
	* [] override
	* @param[in] index
	* @return T&
	*/
  T& operator[](int index) const { return at(index); }

  // @todo
  // �C�e���[�^�͎����r���B
  // �v�f�̃A�h���X�͘A�����Ă��Ȃ�����
  //     for ( i = begin(); i != end(); i++ )
  // �̂悤�Ȏg�������ł��܂���B
  // prev() next() ���쐬���A���X�g�̂悤�Ȍ`�ŗ��p�ł���悤�ɂ���K�v������܂��B
  //	iterator	begin() { return at( 0 ); }		// �擪�̗v�f���w���C�e���[�^��Ԃ�
  //	iterator	end() { return at( size_ - 1 ) + 1; }	// �Ō�̗v�f�̎������w���C�e���[�^��Ԃ�

  //	iterator	rbegin() { return at( size_ - 1 ); }		// �Ō�̗v�f���w���C�e���[�^��Ԃ�
  //	iterator	rend() { return at( 0 ) - 1; }	// �擪�̗v�f�̑O�����w���C�e���[�^��Ԃ�

  /*!
	* �����̗v�f���폜
	*/
  void pop_back() { erase(size() - 1); }

  /*!
	* �v�f�̒ǉ�
	* @return T&
	*/
  T& push_back() {
#if 1
    return insert(size());
#else
    int size = size_;        // �A�C�e���J�E���^
    int maxSize = maxSize_;  // �z��T�C�Y(items_* �̗v�f�̑���)

    // �A�C�e���|�C���^
    T* item;

    // �J�E���^�̐ݒ�
    size_++;

    if (maxSize < size + 1) {
      /**	�L���A�C�e���������̃A�C�e�������������ꍇ�A
				���̃A�C�e���̈��V�����쐬���܂��B
			*/

      // �A�C�e���̈�̐V�K�쐬
      item = extendCapacity();

      // �A�C�e���̓o�^:
      //   �Ĕz�u���s���Ă���ꍇ������̂� items = items_ �͕K�������ł��
      T** items = items_;  // �A�C�e���|�C���^

      // �쐬�����z��ɃA�C�e�������蓖�Ă�
      // �ǉ��̏ꍇ�� size_ �̈ʒu�ɍŏ��̃A�C�e�����o�^����܂��B
      for (int i = reserve_ - 1; 0 <= i; i--) {
        items[maxSize + i] = &item[i];
      }

    } else {
      /**	�z��̃J�E���^�����J�E���^���������ꍇ�́A
				���ɗ̈悪����Ɣ��f���āA
				�����̔z��ɃA�C�e����ǉ����܂��B
			*/

      // size_ �̈ʒu�̃A�C�e�����g�p���邽�߁A��邱�ƂȂ�
      item = items_[size];
    }

    // �A�C�e���|�C���^��Ԃ�
    return *item;
#endif
  }

  /*!
	* �v�f�̒ǉ�
	* @param[in] a
	* @return T&
	*/
  T& push_back(const T& a) {
    T& b = push_back();
    b = a;
    return b;
  }

  /*!
	* �v�f�̑}��
	* @param[in] index
	* @return T&
	*/
  T& insert(int index) {
    int size = size_;        // �A�C�e���J�E���^
    int maxSize = maxSize_;  // �z��T�C�Y(items_* �̗v�f�̑���)

    // �A�C�e���|�C���^
    T* item;

    // �J�E���^�̐ݒ�
    size_++;

    if (maxSize < size + 1) {
      /**	�L���A�C�e���������̃A�C�e�������������ꍇ�A
				���̃A�C�e���̈��V�����쐬���܂��B
			*/

      // �A�C�e���̈�̐V�K�쐬
      item = extendCapacity();

      // �A�C�e���̓o�^:
      //   �Ĕz�u���s���Ă���ꍇ������̂� items = items_ �͕K�������ł��
      //   index �̈ʒu�ɃA�C�e����o�^���邽�߂� index ���� size_ �܂ł��P���炵�܂��B
      T** items = items_;  // �A�C�e���|�C���^

      // index ���� size_ �܂ł����炷
      for (int i = size; index < i; i--) {
        items[i] = items[i - 1];
      }

      // �쐬�����z��ɃA�C�e�������蓖�Ă�
      // �ǉ��̏ꍇ�� size_ �̈ʒu�ɍŏ��̃A�C�e�����o�^����܂��B
      for (int i = reserve_ - 1; 1 <= i; i--) {
        items[maxSize + i] = &item[i];
      }

      // index �̈ʒu�ɃA�C�e����o�^
      items[index] = item;

    } else {
      /**	�z��̃J�E���^�����J�E���^���������ꍇ�́A
				���ɗ̈悪����Ɣ��f���āA
				�����̔z��ɃA�C�e����ǉ����܂��B
			*/

      T** items = items_;  // �A�C�e���|�C���^

      // ���ɃA�C�e�������݂��Ă���͂��Ȃ̂�
      // �J�E���g�̎��̃A�C�e�����g�p����
      item = items[size];

      // index �̈ʒu�̃A�C�e���̈���g�p���邽�� index ���� size_ �܂ł����炷
      for (int i = size; index < i; i--) {
        items[i] = items[i - 1];
      }

      // index �̈ʒu�ɃA�C�e����o�^
      items[index] = item;
    }

    // �A�C�e���|�C���^��Ԃ�
    return *item;
  }

  /*!
	* �v�f�̑}��
	* @param[in] index
	* @param[in] a
	* @return T&
	*/
  T& insert(int index, const T& a) {
    T& b = insert(index);
    b = a;
    return b;
  }

  /*!
	* �v�f�̍폜
	* @param[in] index
	*/
  void erase(int index) {
    /**	index + 1 �` size_ - 1 �܂ł̃A�C�e����O���Ɍ������ĂP���炷���Ƃō폜�����ɂ��܂��B
			���̂Ƃ� index �Ԗڂ̃A�C�e���� size_ - 1 �̈ʒu�ɍĔz�u���邱�ƂŁA�ė��p���\�Ƃ��܂��B
		*/

    int size = size_;  // �A�C�e���J�E���^

    T** items = items_;          // �A�C�e���|�C���^
    T* pTarget = items_[index];  // �폜�ΏۃA�C�e��

    // �J�E���^�̐ݒ�
    size_--;

    // index ���� size_ - 1 �܂ł����炷
    for (int i = index; i < size - 1; i++) {
      items[i] = items[i + 1];
    }

    // �폜�Ώۂ̃A�C�e���� size_ - 1 �̈ʒu�ɍĔz�u
    items[size - 1] = pTarget;
  }

  /*!
	* �N���A(�J�E���^���O�ɂȂ邾��)
	*/
  void clear() { size_ = 0; }

  /*!
	* �̈�̃N���A
	*/
  void clean() {
    // �A���P�[�g�����A�C�e����������܂��B
    if (0 < alloc_) {
      int maxSize = maxSize_;  // �z��T�C�Y(items_* �̗v�f�̑���)
      int alloc = alloc_;      // �A���P�[�g�J�E���^

      for (int i = maxSize + alloc - 1; maxSize <= i; i--) {
        SAFE_DELETE_ARRAY(items_[i]);  // delete!!
      }

      alloc_ = 0;
    }

    // items_ ��������܂�
    SAFE_DELETE_ARRAY(items_);
    maxSize_ = 0;

    // �N���A���܂�
    clear();
  }

//! �z�񃊃X�g�̃\�[�g
#ifndef MX_VECTOR_SMALL
  void bsort(int (*fnCmp)(const T&, const T&), int offset, int n);    // �o�u���\�[�g
  void ssort(int (*fnCmp)(const T&, const T&), int offset, int n);    // �V�F���\�[�g
  void selsort(int (*fnCmp)(const T&, const T&), int offset, int n);  // �I���\�[�g
#endif                                                                // MX_VECTOR_SMALL
  void inssort(int (*fnCmp)(const T&, const T&), int offset, int n);  // �}���\�[�g
  void qsort(int (*fnCmp)(const T&, const T&), int left, int right);  // �N�C�b�N�\�[�g

 private:
  /*!
	* �̈�̐V�K�쐬
	* @return T*
	*/
  T* extendCapacity() {
    int reserve = reserve_;  // �A�C�e���m�ې�(��x�Ɋm�ۂ��鐔)
    int maxSize = maxSize_;  // �z��T�C�Y(items_* �̗v�f�̑���)
    int alloc = alloc_;      // �A���P�[�g�J�E���^

    // �A�C�e���̈�̍쐬
    T* item = SAFE_NEW_ARRAY(__SOURCE__, T, reserve);

    // �z��T�C�Y�̐ݒ�
    maxSize_ = maxSize + reserve;

    // �A���P�[�g�J�E���^�̐ݒ�
    alloc_++;

    /**	�V�K�A�C�e���̃|�C���^�����蓖�Ă�z����g�����܂��B
			�A���P�[�g�����A�C�e���̃|�C���^��ێ�����̈���g�����܂��B
			���ꃁ������ɒu�����߂̎d�l�ł��B
			�z��}�b�v�͈ȉ��̂悤�ɂȂ�܂��B
			
			  000 �A�C�e���P(�A���P�[�g�A�C�e���P)
			  001 �A�C�e���Q(�A���P�[�g�A�C�e���P)
			  002 �A�C�e���R(�A���P�[�g�A�C�e���Q)
			  003 �A�C�e���S(�A���P�[�g�A�C�e���Q)
			                 .                    
			                 .                    
			                 .                    
			  050 �A���P�[�g�A�C�e���P�̃|�C���^ (clean() ���ɉ������|�C���^)
			  051 �A���P�[�g�A�C�e���Q�̃|�C���^ (clean() ���ɉ������|�C���^)
			                 .                    
			                 .                    
			                 .                    
		*/
    {
      T** items = items_;  // �A�C�e���|�C���^
      T** temp = SAFE_NEW_ARRAY(__SOURCE__, T*, maxSize + reserve + alloc + 1);

      // ���ɑ��݂��Ă���A�C�e���̃|�C���^���R�s�[���܂� (���߂��� maxSize �܂ŁB�ȍ~�͂O�ł���ۏ؂Ȃ�)
      for (int i = 0; i < maxSize; i++) {
        temp[i] = items[i];
      }

      // �A���P�[�g�A�C�e���̃|�C���^���R�s�[���܂� (�A���P�[�g�����A�C�e���̃|�C���^�ێ��̂���)
      for (int i = maxSize; i < maxSize + alloc; i++) {
        temp[i + reserve] = items[i];
      }

      // �V�����擾�����A�C�e���̃|�C���^��ݒ�
      temp[maxSize + alloc + reserve] = item;

      SAFE_DELETE_ARRAY(items);  // items_ �� delete ���܂�
      items_ = temp;             // �|�C���^�̍Đݒ�
    }

    return item;
  }

 private:
  T** items_;    //!< �A�C�e���I�u�W�F�N�g
  int reserve_;  //!< �A�C�e���m�ې�(��x�Ɋm�ۂ��鐔)
  int size_;     //!< �A�C�e���J�E���^
  int maxSize_;  //!< �z��T�C�Y(items_* �̗v�f�̑���)
  int alloc_;    //!< �A���P�[�g�J�E���^
};

/*!
* �z�񃊃X�g�̃\�[�g(�o�u���\�[�g)
* @param[in] fnCmp ��r�֐�
* @param[in] offset �J�n�I�t�Z�b�g
* @param[in] n �\�[�g��
* 
* @code
* template <class T> static int _fnCompare(const T &a, const T &b)
* {
* 	return b.ZOrder - a.ZOrder;
* }
* @endcode
*/
#ifndef MX_VECTOR_SMALL
template <class T>
void Vector<T>::bsort(int (*fnCmp)(const T&, const T&), int offset, int n) {
  int i, j;
  T** data = &items_[offset];  // �A�C�e���|�C���^

  // �\�[�g���鐔��ݒ�
  int h = n - 1;

  // �\�[�g����v�f���Ȃ��Ȃ�܂ŌJ��Ԃ�
  do {
    // �c��v�f�����|�P�ŏ�����
    j = -1;

    // �\�[�g����v�f�������J��Ԃ�
    for (i = 1; i <= h; i++) {
      T* l = data[i];      // �����v�f�̃|�C���^
      T* r = data[i - 1];  // �E���v�f�̃|�C���^

      // ��r
      if (fnCmp(*l, *r) < 0) {
        // �c��v�f�������炷
        j = i - 1;

        // �v�f�̃|�C���^�̓���ւ�
        data[i] = r;
        data[j] = l;
      }
    }

    // �c��v�f���̐ݒ�
    h = j;

  } while (h >= 0);
}
#endif  // MX_VECTOR_SMALL

/*!
* �z�񃊃X�g�̃\�[�g(�V�F���\�[�g)
* @param[in] fnCmp ��r�֐�
* @param[in] offset �J�n�I�t�Z�b�g
* @param[in] n �\�[�g��
* 
* @code
* template <class T> static int _fnCompare(const T &a, const T &b)
* {
* 	return b.ZOrder - a.ZOrder;
* }
* @endcode
*/
#ifndef MX_VECTOR_SMALL
template <class T>
void Vector<T>::ssort(int (*fnCmp)(const T&, const T&), int offset, int n) {
  int i, j;
  T** data = &items_[offset];  // �A�C�e���|�C���^

  if (n <= 1) {
    return 0;
  }

  // �����l�̌v�Z
  for (h = 1; h <= n; h = 3 * h + 1) {
  }

  while ((h = h / 3) >= 1) {
    // h ���� n �܂ł��\�[�g�ς݂̎��ɁAdata[ i ] ��K�؂ȂƂ���ɑ}������
    for (i = h; i < n; i++) {
      // �}�����ׂ����R�[�h
      T* record = data[i];

      // data[j] �� record �����������Ȃ� j �����߂�
      for (j = i - h; j >= 0 && fnCmp(*record, *data[j]) < 0; j -= h) {
        // �E�� h �v�f�����炷
        data[j + h] = data[j];
      }

      // record ��}������
      data[j + h] = record;
    }
  }
}
#endif  // MX_VECTOR_SMALL

/*!
* �z�񃊃X�g�̃\�[�g(�I���\�[�g)
* @param[in] fnCmp ��r�֐�
* @param[in] offset �J�n�I�t�Z�b�g
* @param[in] n �\�[�g��
* 
* @code
* template <class T> static int _fnCompare(const T &a, const T &b)
* {
* 	return b.ZOrder - a.ZOrder;
* }
* @endcode
*/
#ifndef MX_VECTOR_SMALL
template <class T>
void Vector<T>::selsort(int (*fnCmp)(const T&, const T&), int offset, int n) {
  int i, j, k;
  T** data = &items_[offset];  // �A�C�e���|�C���^

  T* temp;  // �e���|����

  for (i = 0; i < n - 1; i++) {
    k = i;

    for (j = i + 1; j < n; j++) {
      if (fnCmp(*data[j], *data[k]) < 0) {
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
#endif  // MX_VECTOR_SMALL

/*!
* �z�񃊃X�g�̃\�[�g(�}���\�[�g)
* @param[in] fnCmp ��r�֐�
* @param[in] offset �J�n�I�t�Z�b�g
* @param[in] n �\�[�g��
* 
* @code
* template <class T> static int _fnCompare(const T &a, const T &b)
* {
* 	return b.ZOrder - a.ZOrder;
* }
* @endcode
*/
template <class T>
void vector<T>::inssort(int (*fnCmp)(const T&, const T&), int offset, int n) {
  int i, j;
  T** data = &items_[offset];  // �A�C�e���|�C���^

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
  for (i = 1; i < n; i++) {
    // �}�����ׂ����R�[�h
    T* record = data[i];

    // data[j] �� record �����������Ȃ� j �����߂�
    for (j = i - 1; j >= 0 && fnCmp(*record, *data[j]) < 0; j--) {
      // �E��1�v�f�����炷
      data[j + 1] = data[j];
    }

    // record ��}������
    data[j + 1] = record;
  }
}

/*!
* �z�񃊃X�g�̃\�[�g(�N�C�b�N�\�[�g)
* @param[in] fnCmp ��r�֐�
* @param[in] left �\�[�g���鍶���̈ʒu
* @param[in] right �\�[�g����E���̈ʒu
* 
* @code
* template <class T> static int _fnCompare(const T &a, const T &b)
* {
* 	return b.ZOrder - a.ZOrder;
* }
* @endcode
*/
template <class T>
void vector<T>::qsort(int (*fnCmp)(const T&, const T&), int left, int right) {
  int i, j;
  T** data = items_;  // �A�C�e���|�C���^

  T* temp;   // �e���|����
  T* pivot;  // ���Ƃ���v�f�̃|�C���^

  const int THREASHOLD = 7;  // �N�C�b�N�\�[�g��ʂ̃\�[�g�ɐ؂�ւ���臒l

  // �I������
  if (left > right) {
    return;
  }

  // ������x�̗v�f���܂Ō�������A
  // ���ʂȃI�[�o�[�w�b�h��h�����߂ɁA
  // �\�[�g�֐���؂�ւ���B
  if (right - left < THREASHOLD) {
    //		return this->bsort  (fnCmp, left, right - left + 1);	// �o�u���\�[�g
    return this->inssort(fnCmp, left, right - left + 1);  // �}���\�[�g
    //		return this->ssort  (fnCmp, left, right - left + 1);	// �V�F���\�[�g
    //		return this->selsort(fnCmp, left, right - left + 1);	// �I���\�[�g
  }

  // ���S�����ɐݒ�
  pivot = data[(left + right) / 2];

  // i �̏����ʒu
  i = left - 1;

  // j �̏����ʒu
  j = right + 1;

  for (;;) {
    // ��r�֐�:fnCmp
    //   �E�������Ƃ��A
    //     �߂�l���O
    //       �~���F�����玲��菬�����l��T��
    //       �����F�����玲���傫���l��T��
    //     �߂�l���O
    //       �~���F�E���玲���傫���l��T��
    //       �����F�E���玲��菬�����l��T��
    while (fnCmp(*data[++i], *pivot) < 0) {
    }
    while (fnCmp(*data[--j], *pivot) > 0) {
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
  this->qsort(fnCmp, left, i - 1);

  // �����傫���O���[�v�ɃN�C�b�N�\�[�g
  this->qsort(fnCmp, j + 1, right);
}

}  //mx

#if defined(MX_VECTOR_SMALL)
#undef MX_VECTOR_SMALL
#endif

#endif  //MX_VECTOR_H
