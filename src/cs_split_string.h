//-----------------------------------------------------------------------------
/*!
* @file		mx_split_string.h
* @brief	�����񕪊�
* @author	koma
* @note
* �E�X�V����  
*/
//-----------------------------------------------------------------------------
#ifndef MX_SPLITSTRING_H
#define MX_SPLITSTRING_H

namespace mx {

enum eSplitFlag {
  kSplitIncludeTokenDelimiter = (1 << 0),  // �f���~�^���g�[�N���Ƃ��Ēǉ�����
  kSplitEscapeString = (1 << 1),           // ��������G�X�P�[�v����
  kSplitIgnoreEmpty = (1 << 2)             // ��̃g�[�N���𖳎�����
};

/*!
	����������N���X
	
	@note
	�ʏ�A�g�����͎��̂悤�Ɏg�p����B

		mx::SplitString split(name, ",");
		split.begin();
		while (split.move_next()) {
			mx::string token = split.at();
		}

	�w��̃g�[�N�����擾����ꍇ��
	
		mx::string token1 = split.at(0);
		mx::string token2 = split.at(1);
	
	�̂悤�ɂ���B
*/
class SplitString {
 public:
  SplitString();
  SplitString(const mx::string &text, const char *delimiters, int flag = kSplitEscapeString);
  ~SplitString();

  /*!
		�g�[�N���̌����擾
		@return �g�[�N���̌�
	*/
  int size();

  /*!
		�ŏ��̃g�[�N�����擾
		@return	�g�[�N��
	*/
  void begin();

  /*!
		���̃g�[�N���ֈړ�
		@return	true ����
				false �g�[�N�����Ȃ�
	*/
  bool move_next();
  void operator++(int) { move_next(); }

  /*!
		�g�[�N�����擾
		@param[in]	�C���f�b�N�X
		@return	�g�[�N��
	*/
  mx::string at(int index = -1) const;
  mx::string operator[](int index) { return at(index); }

  /*!
		����
		@param[in]	text �e�L�X�g(NULL�̏ꍇ�̓e�L�X�g���X�V���Ȃ�)
		@param[in]	delimiters �f���~�^
		@param[in]	token_delimiter �f���~�^���g�[�N���ɂ���
		@param[in]	escape_string ��������G�X�P�[�v����
	*/
  void split(const mx::string &text, const mx::string &delimiters, int flag = kSplitEscapeString);

 private:
  void init();

 private:
  mx::string textOrg_;  //!< ������̃C���X�^���X�i�I���W�i���j
  mx::string text_;     //!< ������̃C���X�^���X

  int tokenCnt_;     //!< �g�[�N���̐�
  int curTokenPos_;  //!< ���݂̃g�[�N���ʒu
  const char *tokenPtr_;
  mx::string curToken_;
  const char *nextTokenPtr_;
};

}  //mx

#endif  //MX_SPLITSTRING_H
