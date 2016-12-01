//-----------------------------------------------------------------------------
/*!
* @file		mx_vector_map.hpp
* @brief	vector�̃}�b�s���O
* @author	koma
* @note
* �E�X�V����  
*/
//-----------------------------------------------------------------------------
#ifndef MX_VECTOR_MAP_HPP
#define MX_VECTOR_MAP_HPP

#include <vector>

namespace mx {

//! vector�}�b�s���O�N���X�i�n���ꂽ vector.size()��0�Ƃ��� vector�Ƃ��Ĉ����܂��j
template <class T>
class VectorMap {
 public:
  /*!
	* �R���X�g���N�^
	* @param[in] args_ref
	*/
  explicit VectorMap(std::vector<T> &args_ref)
      : ref_(args_ref)
      , size_(0)
      , begin_(args_ref.empty() ? 0 : static_cast<int>(args_ref.size())) {}

  /*!
	* �f�X�g���N�^
	*/
  ~VectorMap() {
    // �ǉ��������� pop����
    clear();
  }

  /*!
	* �Q��
	* @param[in] index
	* @return T&
	*/
  T &operator[](int index) { return ref_[begin_ + index]; }

  /*!
	* �Q��
	* @param[in] index
	* @return const T&
	*/
  const T &operator[](int index) const { return ref_[begin_ + index]; }

  /*!
	* �v�f��ǉ�
	* @param[in] v
	*/
  void push_back(const T &v) {
    //::OutputDebugStringA((v.str_ + "\n").c_str());
    ref_.push_back(v);
    ++size_;
  }

  /*!
	* �T�C�Y�擾
	* @return int
	*/
  int size() const { return size_; }

  /*!
	* �󂩂ǂ���
	* @return true/false
	*/
  bool empty() const { return (size() == 0); }

  /*!
	* �v�f���N���A
	*/
  void clear() {
    for (int i = 0; i < size_; i++) {
      ref_.pop_back();
    }
    size_ = 0;
  }

 private:
  //! �f�t�H���g�R���X�g���N�^�֎~
  VectorMap() {}

 private:
  std::vector<T> &ref_;  //!< vecotr�Q��
  int size_;             //!< �T�C�Y
  int begin_;            //!< �J�n�ʒu
};

}  //mx

#endif  //MX_VECTOR_MAP_HPP
