// -*- mode:c++; coding:utf-8-ws -*-
//====================================================================
//! @file  rei_lisp.cpp
//! @brief 簡易LISP処理系
//!
//! @author
//====================================================================
#pragma warning(disable : 4996)

//#if defined(_WIN32) || defined(WIN32)
//# include <windows.h>
//#endif

//#include <wi_platform.h>
#include <stdlib.h>

#include "rei_string.h"
#include "rei_var.h"
#include "rei_lisp.h"

#define USE_VECTOR_CLEAR_BY_SWAP 1      //!< vector のクリアに swap を使用する
#define USE_LISP_OUTPUT_DEBUG_STRING 2  //!< デバッグ出力を使用する 1:一部 2:すべて

#if defined(_FINAL)
#undef USE_LISP_OUTPUT_DEBUG_STRING
#define USE_LISP_OUTPUT_DEBUG_STRING 0
#endif

#if USE_LISP_OUTPUT_DEBUG_STRING
#include <windows.h>
#endif

//! + 加算
static void fn_lisp_plus(Lisp &lisp, LispArgs &args) {
  LispVar &ret = lisp.retval();

  ret = args[1];
  for (int i = 2, size = args.size(); i < size; ++i) {
    ret += args[i];
  }
}

//! - 減算
static void fn_lisp_minus(Lisp &lisp, LispArgs &args) {
  LispVar &ret = lisp.retval();

  ret = args[1];
  for (int i = 2, size = args.size(); i < size; ++i) {
    ret -= args[i];
  }
}

//! * 乗算
static void fn_lisp_multiply(Lisp &lisp, LispArgs &args) {
  LispVar &ret = lisp.retval();

  ret = args[1];
  for (int i = 2, size = args.size(); i < size; ++i) {
    ret *= args[i];
  }
}

//! / 除算
static void fn_lisp_divide(Lisp &lisp, LispArgs &args) {
  LispVar &ret = lisp.retval();

  ret = args[1];
  for (int i = 2, size = args.size(); i < size; ++i) {
    ret /= args[i];
  }
}

//! = (equals, 1 + 1 = 2)
static void fn_lisp_equals(Lisp &lisp, LispArgs &args) {
  LispVar &ret = lisp.retval();
  ret = (args[1] == args[2]);
}

//! /= (not equals, 1 + 1 != 2)
static void fn_lisp_not_equals(Lisp &lisp, LispArgs &args) {
  LispVar &ret = lisp.retval();
  ret = (args[1] != args[2]);
}

//! > (Greater than, 5 > 2)
static void fn_lisp_greater_than(Lisp &lisp, LispArgs &args) {
  LispVar &ret = lisp.retval();
  ret = (args[1] > args[2]);
}

//! < (Less than, 7 < 9)
static void fn_lisp_less_than(Lisp &lisp, LispArgs &args) {
  LispVar &ret = lisp.retval();
  ret = (args[1] < args[2]);
}

//! >= (Greater than or equal to, marbles >= 1)
static void fn_lisp_greater_than_or_equal_to(Lisp &lisp, LispArgs &args) {
  LispVar &ret = lisp.retval();
  ret = (args[1] >= args[2]);
}

//! <= (Less than or equal to, dogs <= 3)
static void fn_lisp_less_than_or_equal_to(Lisp &lisp, LispArgs &args) {
  LispVar &ret = lisp.retval();
  ret = (args[1] <= args[2]);
}

//! 変数設定
static void fn_lisp_set(Lisp &lisp, LispArgs &args) {
  for (int i = 1, size = args.size(); i < size; i += 2) {
    lisp[args[i].str()] = args[i + 1];
  }
}

//! 乱数
static void fn_lisp_random(Lisp &lisp, LispArgs &args) {
  LispVar &ret = lisp.retval();

  switch (args[1].type()) {
  case rei::Var::kTypeInt:
    ret = static_cast<LispInt>(rand() % args[1].asInt());
    break;

  case rei::Var::kTypeFloat: {
    LispFloat n = static_cast<LispFloat>(args[1].asFloat() * 1000000.0);
    ret = static_cast<LispFloat>(static_cast<LispFloat>(rand() % (int)n) * 0.0000001);
  } break;

  default:
    ret = static_cast<LispInt>(rand() % args[1].asInt());
    break;
  }
}

namespace rei {

Lisp::Hash Lisp::hash_if;
Lisp::Hash Lisp::hash_loop;
Lisp::Hash Lisp::hash_return;
Lisp::Hash Lisp::hash_setq;
Lisp::Hash Lisp::hash_defvar;
Lisp::Hash Lisp::hash_defparameter;

/*!-----------------------------------------------------------------------
* LISP処理系
*/
Lisp::Lisp() {
  // あらかじめハッシュ値を出しておく
  hash_if = Str("if").hash();
  hash_loop = Str("loop").hash();
  hash_return = Str("return").hash();
  hash_setq = Str("setq").hash();
  hash_defvar = Str("defvar").hash();
  hash_defparameter = Str("defparameter").hash();

  clear();

  //
  // デフォルト変数・関数の登録
  //

  // 四則演算
  registerSystemFunc("+", fn_lisp_plus);
  registerSystemFunc("-", fn_lisp_minus);
  registerSystemFunc("*", fn_lisp_multiply);
  registerSystemFunc("/", fn_lisp_divide);
  // 比較
  registerSystemFunc("=", fn_lisp_equals);
  registerSystemFunc("/=", fn_lisp_not_equals);
  registerSystemFunc(">", fn_lisp_greater_than);
  registerSystemFunc("<", fn_lisp_less_than);
  registerSystemFunc(">=", fn_lisp_greater_than_or_equal_to);
  registerSystemFunc("<=", fn_lisp_less_than_or_equal_to);
  // 設定
  registerSystemFunc("set", fn_lisp_set);
  registerSystemFunc("setq", fn_lisp_set);
  registerSystemFunc("defvar", fn_lisp_set);
  registerSystemFunc("defparameter", fn_lisp_set);
  // 標準
  //	registerSystemFunc("random", fn_lisp_random);
}

Var &Lisp::operator[](const Str &var_name) {
  Hash var_hash = var_name.hash();
  return at(var_hash);
}
const Var &Lisp::operator[](const Str &var_name) const {
  Hash var_hash = var_name.hash();
  return at(var_hash);
}

void Lisp::registerVar(Hash hash) {
  if (vars_.count(hash) == 0) {
    varList_.push_back(hash);
    ++stackVar_.top();  // 現在のスタック位置に追加した数を記録
  }
}

void Lisp::registerFunc(Hash hash) {
  if (funcs_.count(hash) == 0) {
    funcList_.push_back(hash);
    ++stackFunc_.top();  // 現在のスタック位置に追加した数を記録
  }
}

void Lisp::initialize(const Description *desc) {
  varList_.reserve(desc->varNum);
  funcList_.reserve(desc->funcNum);
  vecArgs_.reserve(desc->argNum);
}

void Lisp::finalize() {}

Var &Lisp::retval() {
  return ev_;
}

void Lisp::registerFunc(const Str &func_name, Func func) {
  Hash hash = func_name.hash();
  registerFunc(hash);
  funcs_[hash] = func;
}

void Lisp::registerSystemFunc(const Str &func_name, Func func) {
  Hash hash = func_name.hash();
  systemFuncs_[hash] = func;
}

Var &Lisp::at(Hash var_hash) {
  Vars::iterator it = vars_.find(var_hash);
  if (it != vars_.end()) {
    return (*it).second;

  } else {
    // 変数が未定義
    Var &var = vars_[var_hash];
    return var;
  }
}

const Var &Lisp::at(Hash var_hash) const {
  Vars::const_iterator it = vars_.find(var_hash);
  if (it == vars_.end()) {
    // 変数が未定義
    *(int *)0 = 0;
  }

  return (*it).second;
}

void Lisp::clear() {
  vars_.clear();
  funcs_.clear();
  vecArgs_.clear();

  // push,pop
  {
    varList_.clear();
    funcList_.clear();

    while (!stackVar_.empty()) {
      stackVar_.pop();
    }
    while (!stackFunc_.empty()) {
      stackFunc_.pop();
    }

    stackVar_.push(0);
    stackFunc_.push(0);
  }

// vectorをswapでクリア
#if USE_VECTOR_CLEAR_BY_SWAP
  Vars().swap(vars_);
  Funcs().swap(funcs_);
  std::vector<Var>().swap(vecArgs_);

  // push,pop
  {
    std::vector<Hash>().swap(varList_);
    std::vector<Hash>().swap(funcList_);
  }
#endif
}

void Lisp::push() {
  stackVar_.push(0);
  stackFunc_.push(0);

  ::OutputDebugStringW(L"*** LISP PUSH: vars size " +
                       rei::wstring(static_cast<int>(vars_.size()), L"%d") + L", \n");
}

void Lisp::pop() {
  // 現在のスタック位置に記録された変数・関数を削除する

  // var
  if (stackVar_.top() > 0) {
    for (int i = 0, size = stackVar_.top(); i < size; i++) {
      Vars::iterator var_it = vars_.find(varList_.back());
      if (var_it != vars_.end()) {
        vars_.erase(var_it);
      }

      varList_.pop_back();
    }
  }
  stackVar_.pop();

  // func
  if (stackFunc_.top() > 0) {
    for (int i = 0, size = stackFunc_.top(); i < size; i++) {
      Funcs::iterator func_it = funcs_.find(funcList_.back());
      if (func_it != funcs_.end()) {
        funcs_.erase(func_it);
      }

      funcList_.pop_back();
    }
  }
  stackFunc_.pop();

  ::OutputDebugStringW(L"*** LISP POP: vars size " +
                       rei::wstring(static_cast<int>(vars_.size()), L"%d") + L", \n");
}

void Lisp::tokenToVar(Var &var, const Str &token, Hash hashToken) const {
  //log("%s\n", token.c_str());

  const char ch1 = token[0];
  const char ch2 = token[1];

  // まずは文字列として設定
  //var = token;

  switch (ch1) {
  // クォート
  case '\'':
    var = token.c_str() + 1;
    break;
  // 文字列
  case '"':
    var = token.trim("\"");
    break;
  // 特殊
  case '#':
    switch (ch2) {
    // true
    case 't':
      var = true;
      break;
    // false
    case 'f':
      var = false;
      break;
    // 16進数
    case 'x':
    case 'X': {
      Str s(token);
      s.data()[0] = '0';
      s.data()[1] = 'x';

      long n = strtol(s.c_str(), 0, 16);
      var = static_cast<Int>(n);
    } break;
    }
    //
    //var = token;	// 文字列としても保持
    //
    break;
  default: {
    // ? 変数参照
    Vars::const_iterator it = vars_.find(hashToken);
    if (it != vars_.end()) {
      // 変数
      var = (*it).second;

    } else {
      // 数値
      if ((ch1 >= '0' && ch1 <= '9') ||
          ((ch1 == '+' || ch1 == '-') && (ch2 >= '0' && ch2 <= '9'))) {
        if (token.chr('.') >= 0) {
          var = static_cast<Float>(token.asFloat());

        } else {
          var = static_cast<Int>(token.asInt());
        }

      } else {
        // 未定義
        var = Var();
      }
    }
  }
  //
  //var = token;	// 文字列としても保持
  //
  break;
  }
}

const char *Lisp::nextToken(const char *rp, Str &token) const {
  char buf[256];
  bool stringFlag = false;

  char *wp = buf;

  char ch = *rp;
  while (ch) {
    if (ch == '"') {
      // 文字列
      stringFlag ^= true;
      *wp++ = ch;

    } else if (stringFlag) {
      // 文字列
      *wp++ = ch;

    } else {
      switch (ch) {
      // ブロック区切り(1)
      case ' ':
      case '\t':
        if (wp == buf) {
          // 先頭に空白は追加しない

        } else {
          *wp = 0;
          token = buf;
          return rp;  //function end
        }
        break;
      // ブロック区切り(2)
      case '(':
      case ')':
        if (wp == buf) {
          *wp++ = ch;
          ++rp;
        }

        *wp = 0;
        token = buf;
        return rp;  //function end
                    //break;
      default:
        *wp++ = ch;
        break;
      }
    }

    ++rp;
    ch = *rp;
  }

  *wp = 0;
  token = buf;
  return rp;
}

void Lisp::skipExpression(const char **rp) const {
  int expLv = 0;

  char ch = *(*rp);
  while (ch) {
    switch (ch) {
    // 文字列
    case '"':
      break;
    // ブロック区切り
    case ' ':
    case '\t':
      if (expLv == 0) {
        //ok
        (*rp)++;
        return;  //function end
      }
      break;
    case '(':
      expLv++;
      break;
    case ')':
      if (expLv == 0) {
        (*rp)--;
        return;  //function end
      }
      expLv--;
      if (expLv == 0) {
        //ok
        (*rp)++;
        return;  //function end
      }
      break;
    }

    (*rp)++;
    ch = *(*rp);
  }
}

void Lisp::eval(const char *exp) {
  std::stack<ArgList<Var> > S;  //!< スタック
  S.push(ArgList<Var>(vecArgs_));

  size_t returnLv = static_cast<size_t>(-1);  //!< 戻りブロックレベル
  Str token;

  while (*exp) {
    const char *expOld = exp;
    exp = nextToken(exp, token);

    //		token.trim0(" \t");

    // 空
    if (token.empty()) {
      continue;
    }

    ArgList<Var> &vmap = S.top();
    Args &args = vmap;

    bool expSkip = false;  // ? 式をスキップする

    // 比較結果によってブロックをスキップする
    //   1: １番目のブロックをスキップ
    //   2: ２番目のブロックをスキップ
    if (vmap.ifOp_ > 0) {
      vmap.ifOp_--;
      if (vmap.ifOp_ == 0) {
        expSkip = true;
      }
    }

    // returnされたときのブロックスキップ処理
    if (S.size() >= returnLv) {
      vmap.tagbody_ = 0;
      expSkip = true;
    }

    const char ch1 = token[0];

    if (ch1 == ')') {  //@ Close
      // 繰り返し
      if (vmap.tagbody_) {
        exp = vmap.tagbody_;

      } else {
        // 関数呼び出し
        if (!args.empty()) {
#if USE_LISP_OUTPUT_DEBUG_STRING
          // 関数名の先頭に @ がついている場合は出力しない
          if (vmap.isOutputDebugString_ && args[0].str()[0] != '@') {
            rei::string s("LISP: ");
            s += args[0].str();
#if USE_LISP_OUTPUT_DEBUG_STRING == 2
            for (int i = 1, n = args.size(); i < n; i++) {
              switch (args[i].type()) {
              case Var::kTypeUnknown:
                s += " [?]" + args[i].str() + "";
                break;
              case Var::kTypeFloat:
                s += " [f]" + rei::string(args[i].ff(), "%f");
                break;
              case Var::kTypeInt:
                s += " [d]" + rei::string(args[i].i(), "%d");
                break;
              case Var::kTypeBool:
                s += " [b]" + rei::string(args[i].asBool() ? "true" : "false");
                break;
              case Var::kTypeString:
                s += " [s] \"" + args[i].str() + "\"";
                break;
              case Var::kTypePointer:
                s += " [p]" + rei::string(reinterpret_cast<Int>(args[i].ptr()), "%08x");
                break;
              }
            }
#endif  //USE_LISP_OUTPUT_DEBUG_STRING == 2
            s += "\n";
            ::OutputDebugStringA(s.c_str());
          }
#endif  //USE_LISP_OUTPUT_DEBUG_STRING

          Func func = reinterpret_cast<Func>(args[0].i());
          func(*this, args);
          // 呼ばれる関数内で ev_ に値を入れるようにし
          // それを戻り値として使用する
        }

        // スタックをポップする（デストラクタで引数マップがクリアされる）
        S.pop();

        ArgList<Var> &latest = S.top();

        // 関数が設定されているので引数を追加
        if (!latest.empty()) {
          // 引数の追加
          latest.push_back(ev_);

        } else if (latest.ifOp_ < 0) {  // if比較直後 cmp
          latest.ifOp_ = ev_.asBool() ? 2 : 1;
          // |   |
          // |   #f: １番目のブロックをスキップ
          // #t: ２番目のブロックをスキップ
        }

        // 戻りインデントレベルよりも浅くなったら通常へ
        if (S.size() < returnLv) {
          returnLv = static_cast<size_t>(-1);
        }
      }

    } else if (expSkip) {  //@ 式のスキップ
      if (ch1 == '(') {  // トークンが '(' のときは正しく飛ばすために '(' を含める
        --exp;
      }
      skipExpression(&exp);
      //LOG("skip, '%s'\n", token.c_str());

    } else if (ch1 == '(') {  //@ Open
      // スタックをプッシュする
      S.push(ArgList<Var>(vecArgs_));

    } else {
      const Hash hashToken = token.hash(token[0] == '@' ? 1 : 0);
      // トークンの先頭が @ のときは OutputDebugString を使用しないようにする
      // LISPにはない機能。バッチを模倣。

      if (hashToken == hash_if) {
        vmap.ifOp_ = -1;
      }  // 分岐
      else if (hashToken == hash_loop) {
        vmap.tagbody_ = expOld;
      }  // ループ
      else if (hashToken == hash_return) {
        returnLv = S.size() - 1;
      }  // ブロックを抜ける
      else {
        // ? 関数呼び出し
        if (args.empty()) {
          Funcs::iterator it = systemFuncs_.find(hashToken);
          bool is_func = (it != systemFuncs_.end());
          if (is_func) {
            /*
						システム定義関数
						*/
            // ちょっと特殊、偶数番引数をクオートする
            //if (token == "setq" || token == "defvar" || token == "defparameter") {
            if (hashToken == hash_setq || hashToken == hash_defvar ||
                hashToken == hash_defparameter) {
              vmap.isEvenQuote_ = true;
            }
            // システム関数はログ出力しない
            vmap.isOutputDebugString_ = false;

            // 関数のアドレスを第一引数として設定する
            Var func_val(reinterpret_cast<Int>((*it).second), token);

            // 引数の追加
            args.push_back(func_val);

          } else {
            /*
						ユーザー定義関数
						*/
            it = funcs_.find(hashToken);
            is_func = (it != funcs_.end());

            if (is_func) {
              // 関数のアドレスを第一引数として設定する
              Var func_val(reinterpret_cast<Int>((*it).second), token);

              // 引数の追加
              args.push_back(func_val);

            } else {
              // 関数に該当しない場合は値とする
              tokenToVar(ev_, token, hashToken);
            }
          }

        } else /*if (! args.empty())*/ {  // 関数が設定されているので引数を追加
          if (vmap.isEvenQuote_ && (args.size() & 1)) {  // 現個数が奇数なら...
            token.insert(0, "'");
          }

          Var val;
          tokenToVar(val, token, hashToken);  // トークンを変数へ

          // 引数の追加
          args.push_back(val);
        }
      }
    }
  }
}

Var Lisp::parse(const Str &exp) {
  ev_ = Var();

  const char *c_exp = exp.c_str();
  eval(c_exp);

  return ev_;
}

}  //rei

// 代表的なインスタンスを用意
Lisp g__example_lisp;
