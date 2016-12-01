//-----------------------------------------------------------------------------
/*!
* @file		mx_eval.cpp
* @brief	式の評価
* @author	koma
* @version	$Id$
*
* @note
* ・更新履歴  
*/
//-----------------------------------------------------------------------------
#include "mx_strlib.h"
#include "mx_math.h"

#define T_LEFT 0   // 左辺
#define T_RIGHT 1  // 右辺

#define MAX_VALUE 64  // 数値の最大長

// トークン
enum {
  T_NONE_EXPRESSION = 0,          //
  T_EQUAL_EXPRESSION,             // ==
  T_GREATER_OR_EQUAL_EXPRESSION,  // >=
  T_GREATER_EXPRESSION,           // >
  T_NOT_EQUAL_EXPRESSION,         // != or <>
  T_SMALLER_EXPRESSION,           // <
  T_SMALLER_OR_EQUAL_EXPRESSION,  // <=
  T_NOT_EXPRESSION,               // !
  //	T_BOOLEAN_AND_EXPRESSION,      // &&
  //	T_BOOLEAN_OR_EXPRESSION,       // ||
};

namespace mx {

double CalcString(const char* exp);

}  //mx

/*!
* 式の評価(Evaluation) 再起用
* @param[in] exp 式
* @param[in] size 評価する式の長さ
* @return 式の結果
* 
* @note
* (((100 + 200) <= AAA) && BBB) || (200 <= CCC) とか書けます <br>
* ただし、定数はあらかじめ数値化しておく必要があります <br>
*/
static int eval(const char* exp, int size) {
  // 0:左辺値 1:右辺値
  char val[2][MAX_VALUE];
  char* pVal;

  int ope = T_NONE_EXPRESSION;
  int ret = 0;

  // 書き込み先は左辺
  pVal = val[T_LEFT];

#ifdef __debugprint
  {
    char buff[256];
    mx::strncpy(buff, exp, size);
    printf("exp = %s\n", buff);
  }
#endif  // __debugprint

  for (;;) {
    const char ch = *exp++;

    // 以下の条件で比較
    if (size == 0 || ch == '&' || ch == '|') {
      int a, b;  // 左辺値・右辺値

      switch (ope) {
      // 比較演算子なし
      case T_NONE_EXPRESSION:
        a = (int)mx::CalcString(val[T_LEFT]);  // 数式計算
        ret = a;
#ifdef __debugprint
        printf("ope = %d, ret = %d, a = %d(%s)\n", ope, ret, a, val[T_LEFT]);
#endif  // __debugprint
        break;

      // !
      case T_NOT_EXPRESSION:
        b = (int)mx::CalcString(val[T_RIGHT]);  // 数式計算
        ret = (b == 0);
#ifdef __debugprint
        printf("ope = %d, ret = %d, b = %d(%s)\n", ope, ret, b, val[T_RIGHT]);
#endif  // __debugprint
        break;

      // 比較演算子あり
      default:
        a = (int)mx::CalcString(val[T_LEFT]);   // 数式計算
        b = (int)mx::CalcString(val[T_RIGHT]);  // 数式計算

        switch (ope) {
        case T_EQUAL_EXPRESSION:
          ret = (a == b);
          break;
        case T_GREATER_OR_EQUAL_EXPRESSION:
          ret = (a >= b);
          break;
        case T_GREATER_EXPRESSION:
          ret = (a > b);
          break;
        case T_NOT_EQUAL_EXPRESSION:
          ret = (a != b);
          break;
        case T_SMALLER_EXPRESSION:
          ret = (a < b);
          break;
        case T_SMALLER_OR_EQUAL_EXPRESSION:
          ret = (a <= b);
          break;
        }
#ifdef __debugprint
        printf("ope = %d, ret = %d, a = %d(%s), b = %d(%s)\n", ope, ret, a, val[T_LEFT], b,
               val[T_RIGHT]);
#endif  // __debugprint
        break;
      }

      // 初期化
      pVal = val[T_LEFT];       // 書き込み先は左辺
      ope = T_NONE_EXPRESSION;  // 演算子はなし
    }

    // 終わり
    if (size == 0) {
      break;
    }

    switch (ch) {
    // AND(&&)
    case '&':
      if (*exp++ == '&') {
        // 前回の結果が 0(偽) であれば判断する意味はない
        if (!ret) {
          return 0;
        } else {
          //ope = T_BOOLEAN_AND_EXPRESSION;
        }
      } else {
#ifdef __debugprint
        printf("syntax error\n");
#endif  // __debugprint
        for (;;) {
        }  // 構文エラー
      }
      break;

    // OR(||)
    case '|':
      if (*exp++ == '|') {
        // 前回の結果が 1(真) であれば判断する意味はない
        if (ret) {
          return 1;
        } else {
          //ope = T_BOOLEAN_OR_EXPRESSION;
        }
      } else {
#ifdef __debugprint
        printf("syntax error\n");
#endif  // __debugprint
        for (;;) {
        }  // 構文エラー
      }
      break;

    // カッコがあるときは中から評価
    case '(': {
      const char* pexp = exp;
      int kakko = 1;  // カッコの数

      for (;;) {
        const char ch_ = *exp++;
        if (ch_ == '(') {
          kakko++;
        } else if (ch_ == ')') {
          kakko--;
        }

        // 対応するカッコの数が 0 になったら () の中の式を評価
        if (kakko == 0) {
          ret = eval(pexp, exp - pexp - 1);  // 再起処理で () の中の式を評価する
          size -= exp - pexp;                // ( から ) までの長さを引く

          // 結果を文字列に変換
          if (ope == T_NONE_EXPRESSION) {
            MCR_itoa(ret, val[T_LEFT], 10);
          } else {
            MCR_itoa(ret, val[T_RIGHT], 10);
          }
          break;
        }
      }
    } break;

    // 数値
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '.':
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'x':
    case '+':
    case '-':
    case '*':
    case '/':
      *pVal++ = ch;
      *pVal = '\0';
      break;

    // 比較演算子(=)
    case '=':
      switch (ope) {
      case T_GREATER_EXPRESSION:
        ope = T_GREATER_OR_EQUAL_EXPRESSION;
        break;  // 直前が > なら >= になる
      case T_SMALLER_EXPRESSION:
        ope = T_SMALLER_OR_EQUAL_EXPRESSION;
        break;  // 直前が < なら <= になる
      case T_NOT_EXPRESSION:
        ope = T_NOT_EQUAL_EXPRESSION;
        break;  // 直前が ! なら != になる
      default:
        // 続けて = なら == になる
        if (*exp == '=') {
          exp++;
          ope = T_EQUAL_EXPRESSION;
        } else {
#ifdef __debugprint
          printf("syntax error\n");
#endif  // __debugprint
          for (;;) {
          }  // 構文エラー
        }
        break;
      }
      pVal = val[T_RIGHT];  // 書き込み先は右辺
      break;

    // 比較演算子(>)
    case '>':
      ope = T_GREATER_EXPRESSION;
      pVal = val[T_RIGHT];  // 書き込み先は右辺
      break;

    // 比較演算子(<)
    case '<':
      ope = T_SMALLER_EXPRESSION;
      pVal = val[T_RIGHT];  // 書き込み先は右辺
      break;

    // 比較演算子(!)
    case '!':
      ope = T_NOT_EXPRESSION;
      pVal = val[T_RIGHT];  // 書き込み先は右辺
      break;

    // 整形文字
    case ' ':
    case '\t':
      break;

    // その他の文字はエラー
    default:
#ifdef __debugprint
      printf("error, unknown character.\n");
#endif  // __debugprint
      return 0;
      //break;
    }

    // 長さを減らす
    size--;
  };

#ifdef __debugprint
  printf("eval ret = %d\n", ret);
#endif  // __debugprint
  return ret;
}

namespace mx {

//! 式の評価(Evaluation)
int Eval(const char* exp) {
  return eval(exp, mx::strlen(exp));
}

}  //mx
