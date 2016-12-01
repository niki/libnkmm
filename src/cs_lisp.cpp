//-----------------------------------------------------------------------------
/*!
* @file		mx_lisp.cpp
* @brief	�Ȉ�LISP�����n
* @author	koma
* @note
* �E�X�V����  
*/
//-----------------------------------------------------------------------------
#pragma warning(disable : 4996)

//#if defined(_WIN32) || defined(WIN32)
//# include <windows.h>
//#endif

//#include <wi_platform.h>
#include <stdlib.h>

#include "mx_string.h"
#include "mx_var.h"
#include "mx_lisp.h"

#define USE_VECTOR_CLEAR_BY_SWAP 1      //!< vector �̃N���A�� swap ���g�p����
#define USE_LISP_OUTPUT_DEBUG_STRING 2  //!< �f�o�b�O�o�͂��g�p���� 1:�ꕔ 2:���ׂ�

#if defined(_FINAL)
#undef USE_LISP_OUTPUT_DEBUG_STRING
#define USE_LISP_OUTPUT_DEBUG_STRING 0
#endif

#if USE_LISP_OUTPUT_DEBUG_STRING
#include <windows.h>
#endif

//! + ���Z
static void fn_lisp_plus(Lisp &lisp, LispArgs &args) {
  LispVar &ret = lisp.retval();

  ret = args[1];
  for (int i = 2, size = args.size(); i < size; ++i) {
    ret += args[i];
  }
}

//! - ���Z
static void fn_lisp_minus(Lisp &lisp, LispArgs &args) {
  LispVar &ret = lisp.retval();

  ret = args[1];
  for (int i = 2, size = args.size(); i < size; ++i) {
    ret -= args[i];
  }
}

//! * ��Z
static void fn_lisp_multiply(Lisp &lisp, LispArgs &args) {
  LispVar &ret = lisp.retval();

  ret = args[1];
  for (int i = 2, size = args.size(); i < size; ++i) {
    ret *= args[i];
  }
}

//! / ���Z
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

//! �ϐ��ݒ�
static void fn_lisp_set(Lisp &lisp, LispArgs &args) {
  for (int i = 1, size = args.size(); i < size; i += 2) {
    lisp[args[i].str()] = args[i + 1];
  }
}

//! ����
static void fn_lisp_random(Lisp &lisp, LispArgs &args) {
  LispVar &ret = lisp.retval();

  switch (args[1].type()) {
  case mx::Var::kTypeInt:
    ret = static_cast<LispInt>(rand() % args[1].asInt());
    break;

  case mx::Var::kTypeFloat: {
    LispFloat n = static_cast<LispFloat>(args[1].asFloat() * 1000000.0);
    ret = static_cast<LispFloat>(static_cast<LispFloat>(rand() % (int)n) * 0.0000001);
  } break;

  default:
    ret = static_cast<LispInt>(rand() % args[1].asInt());
    break;
  }
}

namespace mx {

Lisp::Hash Lisp::hash_if;
Lisp::Hash Lisp::hash_loop;
Lisp::Hash Lisp::hash_return;
Lisp::Hash Lisp::hash_setq;
Lisp::Hash Lisp::hash_defvar;
Lisp::Hash Lisp::hash_defparameter;

/*!-----------------------------------------------------------------------
* LISP�����n
*/
Lisp::Lisp() {
  // ���炩���߃n�b�V���l���o���Ă���
  hash_if = Str("if").hash();
  hash_loop = Str("loop").hash();
  hash_return = Str("return").hash();
  hash_setq = Str("setq").hash();
  hash_defvar = Str("defvar").hash();
  hash_defparameter = Str("defparameter").hash();

  clear();

  //
  // �f�t�H���g�ϐ��E�֐��̓o�^
  //

  // �l�����Z
  registerSystemFunc("+", fn_lisp_plus);
  registerSystemFunc("-", fn_lisp_minus);
  registerSystemFunc("*", fn_lisp_multiply);
  registerSystemFunc("/", fn_lisp_divide);
  // ��r
  registerSystemFunc("=", fn_lisp_equals);
  registerSystemFunc("/=", fn_lisp_not_equals);
  registerSystemFunc(">", fn_lisp_greater_than);
  registerSystemFunc("<", fn_lisp_less_than);
  registerSystemFunc(">=", fn_lisp_greater_than_or_equal_to);
  registerSystemFunc("<=", fn_lisp_less_than_or_equal_to);
  // �ݒ�
  registerSystemFunc("set", fn_lisp_set);
  registerSystemFunc("setq", fn_lisp_set);
  registerSystemFunc("defvar", fn_lisp_set);
  registerSystemFunc("defparameter", fn_lisp_set);
  // �W��
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
    ++stackVar_.top();  // ���݂̃X�^�b�N�ʒu�ɒǉ����������L�^
  }
}

void Lisp::registerFunc(Hash hash) {
  if (funcs_.count(hash) == 0) {
    funcList_.push_back(hash);
    ++stackFunc_.top();  // ���݂̃X�^�b�N�ʒu�ɒǉ����������L�^
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
    // �ϐ�������`
    Var &var = vars_[var_hash];
    return var;
  }
}

const Var &Lisp::at(Hash var_hash) const {
  Vars::const_iterator it = vars_.find(var_hash);
  if (it == vars_.end()) {
    // �ϐ�������`
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

// vector��swap�ŃN���A
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
                       mx::wstring(static_cast<int>(vars_.size()), L"%d") + L", \n");
}

void Lisp::pop() {
  // ���݂̃X�^�b�N�ʒu�ɋL�^���ꂽ�ϐ��E�֐����폜����

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
                       mx::wstring(static_cast<int>(vars_.size()), L"%d") + L", \n");
}

void Lisp::tokenToVar(Var &var, const Str &token, Hash hashToken) const {
  //log("%s\n", token.c_str());

  const char ch1 = token[0];
  const char ch2 = token[1];

  // �܂��͕�����Ƃ��Đݒ�
  //var = token;

  switch (ch1) {
  // �N�H�[�g
  case '\'':
    var = token.c_str() + 1;
    break;
  // ������
  case '"':
    var = token.trim("\"");
    break;
  // ����
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
    // 16�i��
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
    //var = token;	// ������Ƃ��Ă��ێ�
    //
    break;
  default: {
    // ? �ϐ��Q��
    Vars::const_iterator it = vars_.find(hashToken);
    if (it != vars_.end()) {
      // �ϐ�
      var = (*it).second;

    } else {
      // ���l
      if ((ch1 >= '0' && ch1 <= '9') ||
          ((ch1 == '+' || ch1 == '-') && (ch2 >= '0' && ch2 <= '9'))) {
        if (token.chr('.') >= 0) {
          var = static_cast<Float>(token.asFloat());

        } else {
          var = static_cast<Int>(token.asInt());
        }

      } else {
        // ����`
        var = Var();
      }
    }
  }
  //
  //var = token;	// ������Ƃ��Ă��ێ�
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
      // ������
      stringFlag ^= true;
      *wp++ = ch;

    } else if (stringFlag) {
      // ������
      *wp++ = ch;

    } else {
      switch (ch) {
      // �u���b�N��؂�(1)
      case ' ':
      case '\t':
        if (wp == buf) {
          // �擪�ɋ󔒂͒ǉ����Ȃ�

        } else {
          *wp = 0;
          token = buf;
          return rp;  //function end
        }
        break;
      // �u���b�N��؂�(2)
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
    // ������
    case '"':
      break;
    // �u���b�N��؂�
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
  std::stack<ArgList<Var> > S;  //!< �X�^�b�N
  S.push(ArgList<Var>(vecArgs_));

  size_t returnLv = static_cast<size_t>(-1);  //!< �߂�u���b�N���x��
  Str token;

  while (*exp) {
    const char *expOld = exp;
    exp = nextToken(exp, token);

    //		token.trim0(" \t");

    // ��
    if (token.empty()) {
      continue;
    }

    ArgList<Var> &vmap = S.top();
    Args &args = vmap;

    bool expSkip = false;  // ? �����X�L�b�v����

    // ��r���ʂɂ���ău���b�N���X�L�b�v����
    //   1: �P�Ԗڂ̃u���b�N���X�L�b�v
    //   2: �Q�Ԗڂ̃u���b�N���X�L�b�v
    if (vmap.ifOp_ > 0) {
      vmap.ifOp_--;
      if (vmap.ifOp_ == 0) {
        expSkip = true;
      }
    }

    // return���ꂽ�Ƃ��̃u���b�N�X�L�b�v����
    if (S.size() >= returnLv) {
      vmap.tagbody_ = 0;
      expSkip = true;
    }

    const char ch1 = token[0];

    if (ch1 == ')') {  //@ Close
      // �J��Ԃ�
      if (vmap.tagbody_) {
        exp = vmap.tagbody_;

      } else {
        // �֐��Ăяo��
        if (!args.empty()) {
#if USE_LISP_OUTPUT_DEBUG_STRING
          // �֐����̐擪�� @ �����Ă���ꍇ�͏o�͂��Ȃ�
          if (vmap.isOutputDebugString_ && args[0].str()[0] != '@') {
            mx::string s("LISP: ");
            s += args[0].str();
#if USE_LISP_OUTPUT_DEBUG_STRING == 2
            for (int i = 1, n = args.size(); i < n; i++) {
              switch (args[i].type()) {
              case Var::kTypeUnknown:
                s += " [?]" + args[i].str() + "";
                break;
              case Var::kTypeFloat:
                s += " [f]" + mx::string(args[i].ff(), "%f");
                break;
              case Var::kTypeInt:
                s += " [d]" + mx::string(args[i].i(), "%d");
                break;
              case Var::kTypeBool:
                s += " [b]" + mx::string(args[i].asBool() ? "true" : "false");
                break;
              case Var::kTypeString:
                s += " [s] \"" + args[i].str() + "\"";
                break;
              case Var::kTypePointer:
                s += " [p]" + mx::string(reinterpret_cast<Int>(args[i].ptr()), "%08x");
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
          // �Ă΂��֐����� ev_ �ɒl������悤�ɂ�
          // �����߂�l�Ƃ��Ďg�p����
        }

        // �X�^�b�N���|�b�v����i�f�X�g���N�^�ň����}�b�v���N���A�����j
        S.pop();

        ArgList<Var> &latest = S.top();

        // �֐����ݒ肳��Ă���̂ň�����ǉ�
        if (!latest.empty()) {
          // �����̒ǉ�
          latest.push_back(ev_);

        } else if (latest.ifOp_ < 0) {  // if��r���� cmp
          latest.ifOp_ = ev_.asBool() ? 2 : 1;
          // |   |
          // |   #f: �P�Ԗڂ̃u���b�N���X�L�b�v
          // #t: �Q�Ԗڂ̃u���b�N���X�L�b�v
        }

        // �߂�C���f���g���x�������󂭂Ȃ�����ʏ��
        if (S.size() < returnLv) {
          returnLv = static_cast<size_t>(-1);
        }
      }

    } else if (expSkip) {  //@ ���̃X�L�b�v
      if (ch1 == '(') {    // �g�[�N���� '(' �̂Ƃ��͐�������΂����߂� '(' ���܂߂�
        --exp;
      }
      skipExpression(&exp);
      //LOG("skip, '%s'\n", token.c_str());

    } else if (ch1 == '(') {  //@ Open
      // �X�^�b�N���v�b�V������
      S.push(ArgList<Var>(vecArgs_));

    } else {
      const Hash hashToken = token.hash(token[0] == '@' ? 1 : 0);
      // �g�[�N���̐擪�� @ �̂Ƃ��� OutputDebugString ���g�p���Ȃ��悤�ɂ���
      // LISP�ɂ͂Ȃ��@�\�B�o�b�`��͕�B

      if (hashToken == hash_if) {
        vmap.ifOp_ = -1;
      }  // ����
      else if (hashToken == hash_loop) {
        vmap.tagbody_ = expOld;
      }  // ���[�v
      else if (hashToken == hash_return) {
        returnLv = S.size() - 1;
      }  // �u���b�N�𔲂���
      else {
        // ? �֐��Ăяo��
        if (args.empty()) {
          Funcs::iterator it = systemFuncs_.find(hashToken);
          bool is_func = (it != systemFuncs_.end());
          if (is_func) {
            /*
						�V�X�e����`�֐�
						*/
            // ������Ɠ���A�����Ԉ������N�I�[�g����
            //if (token == "setq" || token == "defvar" || token == "defparameter") {
            if (hashToken == hash_setq || hashToken == hash_defvar ||
                hashToken == hash_defparameter) {
              vmap.isEvenQuote_ = true;
            }
            // �V�X�e���֐��̓��O�o�͂��Ȃ�
            vmap.isOutputDebugString_ = false;

            // �֐��̃A�h���X��������Ƃ��Đݒ肷��
            Var func_val(reinterpret_cast<Int>((*it).second), token);

            // �����̒ǉ�
            args.push_back(func_val);

          } else {
            /*
						���[�U�[��`�֐�
						*/
            it = funcs_.find(hashToken);
            is_func = (it != funcs_.end());

            if (is_func) {
              // �֐��̃A�h���X��������Ƃ��Đݒ肷��
              Var func_val(reinterpret_cast<Int>((*it).second), token);

              // �����̒ǉ�
              args.push_back(func_val);

            } else {
              // �֐��ɊY�����Ȃ��ꍇ�͒l�Ƃ���
              tokenToVar(ev_, token, hashToken);
            }
          }

        } else /*if (! args.empty())*/ {                 // �֐����ݒ肳��Ă���̂ň�����ǉ�
          if (vmap.isEvenQuote_ && (args.size() & 1)) {  // ��������Ȃ�...
            token.insert(0, "'");
          }

          Var val;
          tokenToVar(val, token, hashToken);  // �g�[�N����ϐ���

          // �����̒ǉ�
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

}  //mx

// ��\�I�ȃC���X�^���X��p��
Lisp g__example_lisp;
