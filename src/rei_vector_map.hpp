// -*- mode:c++; coding:utf-8-ws -*-
//====================================================================
//! @file  rei_vector_map.hpp
//! @brief vectorのマッピング
//!
//! @author
//====================================================================
#ifndef REI_VECTOR_MAP_HPP
#define REI_VECTOR_MAP_HPP

#include <vector>

namespace rei {

//! vectorマッピングクラス（渡された vector.size()を0とした vectorとして扱います）
template <class T>
class VectorMap {
 public:
  /*!
	* コンストラクタ
	* @param[in] args_ref
	*/
  explicit VectorMap(std::vector<T> &args_ref)
      : ref_(args_ref)
      , size_(0)
      , begin_(args_ref.empty() ? 0 : static_cast<int>(args_ref.size())) {}

  /*!
	* デストラクタ
	*/
  ~VectorMap() {
    // 追加した分を popする
    clear();
  }

  /*!
	* 参照
	* @param[in] index
	* @return T&
	*/
  T &operator[](int index) { return ref_[begin_ + index]; }

  /*!
	* 参照
	* @param[in] index
	* @return const T&
	*/
  const T &operator[](int index) const { return ref_[begin_ + index]; }

  /*!
	* 要素を追加
	* @param[in] v
	*/
  void push_back(const T &v) {
    //::OutputDebugStringA((v.str_ + "\n").c_str());
    ref_.push_back(v);
    ++size_;
  }

  /*!
	* サイズ取得
	* @return int
	*/
  int size() const { return size_; }

  /*!
	* 空かどうか
	* @return true/false
	*/
  bool empty() const { return (size() == 0); }

  /*!
	* 要素をクリア
	*/
  void clear() {
    for (int i = 0; i < size_; i++) {
      ref_.pop_back();
    }
    size_ = 0;
  }

 private:
  //! デフォルトコンストラクタ禁止
  VectorMap() {}

 private:
  std::vector<T> &ref_;  //!< vecotr参照
  int size_;             //!< サイズ
  int begin_;            //!< 開始位置
};

}  //rei

#endif  //REI_VECTOR_MAP_HPP
