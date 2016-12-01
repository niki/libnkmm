//-----------------------------------------------------------------------------
/*!
* @file		mx_ringbuffer.h
* @brief	�����O�o�b�t�@ �e���v���[�g�N���X
* @author	koma
* @version	$Id$
*
* @note
* �E�X�V����  
*/
//-----------------------------------------------------------------------------
#ifndef MX_RINGBUFFER_H
#define MX_RINGBUFFER_H

//! �����O�o�b�t�@ �e���v���[�g�N���X
template <class T>
class ringbuffer {
 public:
  ringbuffer() : maxBuffer_(0), pBuffers_(0) { clear(); }
  virtual ~ringbuffer() { SAFE_DELETE_ARRAY(pBuffers_); }

  void size(int s) {
    if (pBuffers_ != 0) {
      SAFE_DELETE_ARRAY(pBuffers_);
    }

    maxBuffer_ = s;
    pBuffers_ = SAFE_NEW_ARRAY(__SOURCE__, T, s);
  }

  void add(T &data) {
    int max = maxBuffer_;
    int count = count_;
    int start = start_;
    int current = current_;

    // �����ς��ɂȂ��Ă���̂ŃX�^�[�g�ʒu�����炷
    // (��ԌÂ����̂���㏑������Ă����܂�)
    if (0 < count && current == start) {
      start = (start + 1) % max;
    }

    pBuffers_[current] = data;
    current = (current + 1) % max;

    start_ = start;
    current_ = current;

    ++count_;

    if (max < count_) {
      count_ = maxBuffer_;
    }
  }

  T &at(int i) { return pBuffers_[(start_ + i) % maxBuffer_]; }
  int count() { return count_; }

  void clear() {
    start_ = 0;
    current_ = 0;
    count_ = 0;
  }

 private:
  int maxBuffer_;
  T *pBuffers_;

  int start_;    //!< �X�^�[�g�ʒu
  int current_;  //!< ���݈ʒu

  int count_;
};

#endif  //MX_RINGBUFFER_H
