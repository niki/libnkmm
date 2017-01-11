// -*- mode:c++; coding:utf-8-ws -*-
//====================================================================
//! @file  rei_lisp.h
//! @brief 簡易LISP処理系
//!
//! @author
//====================================================================
#ifndef REI_LISP_H
#define REI_LISP_H

/*
● 変数

	・Cソース側からの変数設定例

		LISP["self"] = this;

	・LISP側からの変数設定例

		(setq ModelMax 256)
		(setq PI 3.1415926535)
		(setq FileName "test.igb")

● 関数

	・Cソース側からの関数定義例

		static void fn_lisp_disp(Lisp &lisp, LispArgs &args)
		{
			LispVar &ret = lisp.Retval();
			Scene* scene = static_cast<Scene*>(lisp["self"].ptr());
			scene->getModel()->SetVisible(args[1].isTrue());
		}

	・Cソース側からの関数設定例

		LISP.SetFunc("disp", fn_lisp_disp);

	・LISP側からの関数呼び出し例

		(disp #t)

● push, pop

	設定する変数、関数はスタック管理が可能です。

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

#include "rei_vector_map.hpp"

namespace rei {

/*!-----------------------------------------------------------------------
	LISP処理系
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
    bool isEvenQuote_;          //!< ? 偶数番引数をクオートする
    bool isOutputDebugString_;  //!< ? デバッグ出力
    char ifOp_;                 //!< if:  -1:開始,  1:#t,  2:#f
    const char *tagbody_;       //!< ループエントリ
  };

 public:
  typedef unsigned long Hash;  //!< ハッシュ

  typedef Map<Hash, Var> Vars;  //!< 変数リスト
  typedef ArgList<Var> Args;    //!< 引数リスト

  typedef void (*Func)(Lisp &, Args &);  //!< 関数型
  typedef Map<Hash, Func> Funcs;         //!< 関数リスト

 public:
  // コンストラクタ
  Lisp();

  // 変数の参照
  Var &operator[](const Str &var_name);
  const Var &operator[](const Str &var_name) const;

  //! 初期化構造体
  struct Description {
    int varNum;   //!< 変数個数
    int funcNum;  //!< 関数個数
    int argNum;   //!< 変数個数
  };

  /*!
	* 初期化
	* @param[in] desc 初期化情報
	*/
  void initialize(const Description *desc);

  /*!
	* 終了
	*/
  void finalize();

  /*!
	* 評価値のリファレンスを取得
	* @return Var &
	*/
  Var &retval();

  /*!
	* 関数リストに関数を設定
	* @param[in] func_name
	* @param[in] func
	*/
  void registerFunc(const Str &func_name, Func func);

  /*!
	* システム関数リストに関数を追加
	* @param[in] func_name
	* @param[in] func
	*/
  void registerSystemFunc(const Str &func_name, Func func);

  //! 変数・関数・引数リストのクリア（システム関数は対象外）
  void clear();

  //! 関数・変数リストのスタックプッシュ
  void push();

  //! 関数・変数リストのスタックポップ
  void pop();

  /*!
	* パーサー
	* @param[in] exp 式
	* @return 結果
	* 
	* @note
	*   現在、setq if 四則演算(+ - * /)が使用可能です。
	*   (setq T (0.5)) (setq R (0.5))
	*   (if (> TEST 0.0) (1.0) (-1.0))
	*   (if (> Rz2 0.0) (Rz2) (* Rz2 -1.0))
	*/
  Var parse(const Str &exp);

 private:
  /*!
	* 変数リストから変数を取得
	* @param[in] var_hash|var_name
	* @return var &
	*/
  Var &at(Hash var_hash);

  /*!
	* 変数リストから変数を取得
	* @param[in] var_hash|var_name
	* @return const var &
	*/
  const Var &at(Hash var_hash) const;

  /*!
	* 式の評価
	* @param[in] exp
	*/
  void eval(const char *exp);

  void registerVar(Hash hash);

  void registerFunc(Hash hash);

  /*!
	* 文字列を変数に変換
	* @param[out] var
	* @param[in] token
	* @param[in] hashToken
	*/
  void tokenToVar(Var &var, const Str &token, Hash hashToken) const;

  /*!
	* 引数の読み込み（先頭の空白は無視）
	* @param[in] rp
	* @param[out] token
	* @return const char*
	*/
  const char *nextToken(const char *rp, Str &token) const;

  /*!
	* 式のスキップ
	* @param[in,out] rp
	*/
  void skipExpression(const char **rp) const;

 private:
  Var ev_;                    //!< 評価値
  Vars vars_;                 //!< 変数リスト
  Funcs funcs_;               //!< 関数リスト
  Funcs systemFuncs_;         //!< 関数リスト（システム）
  std::vector<Var> vecArgs_;  //!< 引数vector

  // push,pop
  std::stack<int> stackVar_;
  std::stack<int> stackFunc_;
  std::vector<Hash> varList_;   //!< 変数登録リスト（順）
  std::vector<Hash> funcList_;  //!< 関数登録リスト（順）

  // special
  static Hash hash_if;
  static Hash hash_loop;
  static Hash hash_return;
  static Hash hash_setq;
  static Hash hash_defvar;
  static Hash hash_defparameter;

 private:
  // コピー禁止
  Lisp &operator=(const Lisp &rhs);
};

}  //rei

// Alias
typedef rei::Lisp Lisp;

typedef rei::Int LispInt;      //!< Int型
typedef rei::Float LispFloat;  //!< Float型
typedef rei::Bool LispBool;    //!< Bool型
typedef rei::Ptr LispPtr;      //!< ポインター型
typedef rei::Str LispStr;      //!< 文字列型

typedef rei::Var LispVar;
typedef rei::Lisp::Vars LispVars;
typedef rei::Lisp::Funcs LispFuncs;
typedef rei::Lisp::Args LispArgs;

// 代表的なインスタンスを用意
extern Lisp g__example_lisp;
#define LISP g__example_lisp

#endif  //REI_LISP_H
