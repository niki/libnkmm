//-----------------------------------------------------------------------------
/*!
* @file		mx_lisp.h
* @brief	�Ȉ�LISP�����n
* @author	koma
* @note
* �E�X�V����  
*/
//-----------------------------------------------------------------------------
#ifndef MX_LISP_H
#define MX_LISP_H

/*
�� �ϐ�

	�EC�\�[�X������̕ϐ��ݒ��

		LISP["self"] = this;

	�ELISP������̕ϐ��ݒ��

		(setq ModelMax 256)
		(setq PI 3.1415926535)
		(setq FileName "test.igb")

�� �֐�

	�EC�\�[�X������̊֐���`��

		static void fn_lisp_disp(Lisp &lisp, LispArgs &args)
		{
			LispVar &ret = lisp.Retval();
			Scene* scene = static_cast<Scene*>(lisp["self"].ptr());
			scene->getModel()->SetVisible(args[1].isTrue());
		}

	�EC�\�[�X������̊֐��ݒ��

		LISP.SetFunc("disp", fn_lisp_disp);

	�ELISP������̊֐��Ăяo����

		(disp #t)

�� push, pop

	�ݒ肷��ϐ��A�֐��̓X�^�b�N�Ǘ����\�ł��B

		LISP.Push()
		LISP["a"] = a;
		LISP.Push()
		LISP["b"] = b;
		LISP["c"] = c;

		 => a b c
		LISP.Pop()
		 => a
		LISP.Pop()
		 => none
*/

#include <stack>
#include <vector>

#include <map>
#define Map std::map

#include "mx_vector_map.hpp"

namespace mx {

/*!-----------------------------------------------------------------------
	LISP�����n
*/
class Lisp {
 public:
  template <class T>
  class ArgList : public VectorMap<T> {
    ArgList() {}  //Default constructor ban

   public:
    explicit ArgList(std::vector<T> &args_ref)
        : VectorMap(args_ref)
        , isEvenQuote_(false)
        , isOutputDebugString_(true)
        , ifOp_(0)
        , tagbody_(0) {}

   public:
    bool isEvenQuote_;          //!< ? �����Ԉ������N�I�[�g����
    bool isOutputDebugString_;  //!< ? �f�o�b�O�o��
    char ifOp_;                 //!< if:  -1:�J�n,  1:#t,  2:#f
    const char *tagbody_;       //!< ���[�v�G���g��
  };

 public:
  typedef unsigned long Hash;  //!< �n�b�V��

  typedef Map<Hash, Var> Vars;  //!< �ϐ����X�g
  typedef ArgList<Var> Args;    //!< �������X�g

  typedef void (*Func)(Lisp &, Args &);  //!< �֐��^
  typedef Map<Hash, Func> Funcs;         //!< �֐����X�g

 public:
  // �R���X�g���N�^
  Lisp();

  // �ϐ��̎Q��
  Var &operator[](const Str &var_name);
  const Var &operator[](const Str &var_name) const;

  //! �������\����
  struct Description {
    int varNum;   //!< �ϐ���
    int funcNum;  //!< �֐���
    int argNum;   //!< �ϐ���
  };

  /*!
	* ������
	* @param[in] desc ���������
	*/
  void initialize(const Description *desc);

  /*!
	* �I��
	*/
  void finalize();

  /*!
	* �]���l�̃��t�@�����X���擾
	* @return Var &
	*/
  Var &retval();

  /*!
	* �֐����X�g�Ɋ֐���ݒ�
	* @param[in] func_name
	* @param[in] func
	*/
  void registerFunc(const Str &func_name, Func func);

  /*!
	* �V�X�e���֐����X�g�Ɋ֐���ǉ�
	* @param[in] func_name
	* @param[in] func
	*/
  void registerSystemFunc(const Str &func_name, Func func);

  //! �ϐ��E�֐��E�������X�g�̃N���A�i�V�X�e���֐��͑ΏۊO�j
  void clear();

  //! �֐��E�ϐ����X�g�̃X�^�b�N�v�b�V��
  void push();

  //! �֐��E�ϐ����X�g�̃X�^�b�N�|�b�v
  void pop();

  /*!
	* �p�[�T�[
	* @param[in] exp ��
	* @return ����
	* 
	* @note
	*   ���݁Asetq if �l�����Z(+ - * /)���g�p�\�ł��B
	*   (setq T (0.5)) (setq R (0.5))
	*   (if (> TEST 0.0) (1.0) (-1.0))
	*   (if (> Rz2 0.0) (Rz2) (* Rz2 -1.0))
	*/
  Var parse(const Str &exp);

 private:
  /*!
	* �ϐ����X�g����ϐ����擾
	* @param[in] var_hash|var_name
	* @return var &
	*/
  Var &at(Hash var_hash);

  /*!
	* �ϐ����X�g����ϐ����擾
	* @param[in] var_hash|var_name
	* @return const var &
	*/
  const Var &at(Hash var_hash) const;

  /*!
	* ���̕]��
	* @param[in] exp
	*/
  void eval(const char *exp);

  void registerVar(Hash hash);

  void registerFunc(Hash hash);

  /*!
	* �������ϐ��ɕϊ�
	* @param[out] var
	* @param[in] token
	* @param[in] hashToken
	*/
  void tokenToVar(Var &var, const Str &token, Hash hashToken) const;

  /*!
	* �����̓ǂݍ��݁i�擪�̋󔒂͖����j
	* @param[in] rp
	* @param[out] token
	* @return const char*
	*/
  const char *nextToken(const char *rp, Str &token) const;

  /*!
	* ���̃X�L�b�v
	* @param[in,out] rp
	*/
  void skipExpression(const char **rp) const;

 private:
  Var ev_;                    //!< �]���l
  Vars vars_;                 //!< �ϐ����X�g
  Funcs funcs_;               //!< �֐����X�g
  Funcs systemFuncs_;         //!< �֐����X�g�i�V�X�e���j
  std::vector<Var> vecArgs_;  //!< ����vector

  // push,pop
  std::stack<int> stackVar_;
  std::stack<int> stackFunc_;
  std::vector<Hash> varList_;   //!< �ϐ��o�^���X�g�i���j
  std::vector<Hash> funcList_;  //!< �֐��o�^���X�g�i���j

  // special
  static Hash hash_if;
  static Hash hash_loop;
  static Hash hash_return;
  static Hash hash_setq;
  static Hash hash_defvar;
  static Hash hash_defparameter;

 private:
  // �R�s�[�֎~
  Lisp &operator=(const Lisp &rhs);
};

}  //mx

// Alias
typedef mx::Lisp Lisp;

typedef mx::Int LispInt;      //!< Int�^
typedef mx::Float LispFloat;  //!< Float�^
typedef mx::Bool LispBool;    //!< Bool�^
typedef mx::Ptr LispPtr;      //!< �|�C���^�[�^
typedef mx::Str LispStr;      //!< ������^

typedef mx::Var LispVar;
typedef mx::Lisp::Vars LispVars;
typedef mx::Lisp::Funcs LispFuncs;
typedef mx::Lisp::Args LispArgs;

// ��\�I�ȃC���X�^���X��p��
extern Lisp g__example_lisp;
#define LISP g__example_lisp

#endif  //MX_LISP_H
