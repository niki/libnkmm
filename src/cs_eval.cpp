//-----------------------------------------------------------------------------
/*!
* @file		mx_eval.cpp
* @brief	���̕]��
* @author	koma
* @version	$Id$
*
* @note
* �E�X�V����  
*/
//-----------------------------------------------------------------------------
#include "mx_strlib.h"
#include "mx_math.h"

#define T_LEFT 0   // ����
#define T_RIGHT 1  // �E��

#define MAX_VALUE 64  // ���l�̍ő咷

// �g�[�N��
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
* ���̕]��(Evaluation) �ċN�p
* @param[in] exp ��
* @param[in] size �]�����鎮�̒���
* @return ���̌���
* 
* @note
* (((100 + 200) <= AAA) && BBB) || (200 <= CCC) �Ƃ������܂� <br>
* �������A�萔�͂��炩���ߐ��l�����Ă����K�v������܂� <br>
*/
static int eval(const char* exp, int size) {
  // 0:���Ӓl 1:�E�Ӓl
  char val[2][MAX_VALUE];
  char* pVal;

  int ope = T_NONE_EXPRESSION;
  int ret = 0;

  // �������ݐ�͍���
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

    // �ȉ��̏����Ŕ�r
    if (size == 0 || ch == '&' || ch == '|') {
      int a, b;  // ���Ӓl�E�E�Ӓl

      switch (ope) {
      // ��r���Z�q�Ȃ�
      case T_NONE_EXPRESSION:
        a = (int)mx::CalcString(val[T_LEFT]);  // �����v�Z
        ret = a;
#ifdef __debugprint
        printf("ope = %d, ret = %d, a = %d(%s)\n", ope, ret, a, val[T_LEFT]);
#endif  // __debugprint
        break;

      // !
      case T_NOT_EXPRESSION:
        b = (int)mx::CalcString(val[T_RIGHT]);  // �����v�Z
        ret = (b == 0);
#ifdef __debugprint
        printf("ope = %d, ret = %d, b = %d(%s)\n", ope, ret, b, val[T_RIGHT]);
#endif  // __debugprint
        break;

      // ��r���Z�q����
      default:
        a = (int)mx::CalcString(val[T_LEFT]);   // �����v�Z
        b = (int)mx::CalcString(val[T_RIGHT]);  // �����v�Z

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

      // ������
      pVal = val[T_LEFT];       // �������ݐ�͍���
      ope = T_NONE_EXPRESSION;  // ���Z�q�͂Ȃ�
    }

    // �I���
    if (size == 0) {
      break;
    }

    switch (ch) {
    // AND(&&)
    case '&':
      if (*exp++ == '&') {
        // �O��̌��ʂ� 0(�U) �ł���Δ��f����Ӗ��͂Ȃ�
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
        }  // �\���G���[
      }
      break;

    // OR(||)
    case '|':
      if (*exp++ == '|') {
        // �O��̌��ʂ� 1(�^) �ł���Δ��f����Ӗ��͂Ȃ�
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
        }  // �\���G���[
      }
      break;

    // �J�b�R������Ƃ��͒�����]��
    case '(': {
      const char* pexp = exp;
      int kakko = 1;  // �J�b�R�̐�

      for (;;) {
        const char ch_ = *exp++;
        if (ch_ == '(') {
          kakko++;
        } else if (ch_ == ')') {
          kakko--;
        }

        // �Ή�����J�b�R�̐��� 0 �ɂȂ����� () �̒��̎���]��
        if (kakko == 0) {
          ret = eval(pexp, exp - pexp - 1);  // �ċN������ () �̒��̎���]������
          size -= exp - pexp;                // ( ���� ) �܂ł̒���������

          // ���ʂ𕶎���ɕϊ�
          if (ope == T_NONE_EXPRESSION) {
            MCR_itoa(ret, val[T_LEFT], 10);
          } else {
            MCR_itoa(ret, val[T_RIGHT], 10);
          }
          break;
        }
      }
    } break;

    // ���l
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

    // ��r���Z�q(=)
    case '=':
      switch (ope) {
      case T_GREATER_EXPRESSION:
        ope = T_GREATER_OR_EQUAL_EXPRESSION;
        break;  // ���O�� > �Ȃ� >= �ɂȂ�
      case T_SMALLER_EXPRESSION:
        ope = T_SMALLER_OR_EQUAL_EXPRESSION;
        break;  // ���O�� < �Ȃ� <= �ɂȂ�
      case T_NOT_EXPRESSION:
        ope = T_NOT_EQUAL_EXPRESSION;
        break;  // ���O�� ! �Ȃ� != �ɂȂ�
      default:
        // ������ = �Ȃ� == �ɂȂ�
        if (*exp == '=') {
          exp++;
          ope = T_EQUAL_EXPRESSION;
        } else {
#ifdef __debugprint
          printf("syntax error\n");
#endif  // __debugprint
          for (;;) {
          }  // �\���G���[
        }
        break;
      }
      pVal = val[T_RIGHT];  // �������ݐ�͉E��
      break;

    // ��r���Z�q(>)
    case '>':
      ope = T_GREATER_EXPRESSION;
      pVal = val[T_RIGHT];  // �������ݐ�͉E��
      break;

    // ��r���Z�q(<)
    case '<':
      ope = T_SMALLER_EXPRESSION;
      pVal = val[T_RIGHT];  // �������ݐ�͉E��
      break;

    // ��r���Z�q(!)
    case '!':
      ope = T_NOT_EXPRESSION;
      pVal = val[T_RIGHT];  // �������ݐ�͉E��
      break;

    // ���`����
    case ' ':
    case '\t':
      break;

    // ���̑��̕����̓G���[
    default:
#ifdef __debugprint
      printf("error, unknown character.\n");
#endif  // __debugprint
      return 0;
      //break;
    }

    // ���������炷
    size--;
  };

#ifdef __debugprint
  printf("eval ret = %d\n", ret);
#endif  // __debugprint
  return ret;
}

namespace mx {

//! ���̕]��(Evaluation)
int Eval(const char* exp) {
  return eval(exp, mx::strlen(exp));
}

}  //mx
