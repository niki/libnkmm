//-----------------------------------------------------------------------------
/*!
* @file		mx_calc.cpp
* @brief	�����񎮂̌v�Z
* @author	koma
* @version	$Id$
*
* @note
* �E�X�V����  
*/
//-----------------------------------------------------------------------------
#include "mx_strlib.h"
#include "mx_math.h"

#define STACK_SIZE 32        // �X�^�b�N�T�C�Y
#define MAX_TOKEN 64         // �g�[�N���̍ő吔
#define TOKEN_SIZE 16        // �g�[�N���̍ő咷
#define EXPRESSION_SIZE 128  // ���̍ő咷

/*!
* �����t�|�[�����h�L�@�ɂ���
* @param[out] porStr �o�̓o�b�t�@
* @param[in] exp ����(�J�b�R() �Ŋ������������w�肵�܂�)
* @return �g�[�N���̐�
*/
static int analyze(char porStr[MAX_TOKEN][TOKEN_SIZE], const char* exp) {
  int sp = 0;      // �X�^�b�N�|�C���^
  int porcnt = 0;  // �g�[�N���J�E���^

  char val[TOKEN_SIZE] = "";  // ���l
  char* val_ptr = val;

  char var[STACK_SIZE];  // ���Z�q�p�̃X�^�b�N

  while (*exp) {
    char ch = *exp++;

    switch (ch) {
    case ',':  // ��؂�
      break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':  // ���l�̏ꍇ
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
      *val_ptr++ = ch;
      break;
    case '+':  // +,- �̏ꍇ�͉��Z�q�X�^�b�N��|���o��
    case '-': {
      int multi = 0;  // * �ɂ���t���O

      if (*val == '\0') {
        if (ch == '+') {
          // ���Z�q�̑O�ɐ��l���Ȃ��Ƃ��� ^(�J���b�g) �ɂ���
          // (+1 * ��ǉ����邱�Ƃŕ���������)
          *val_ptr++ = '^';
          *val_ptr++ = '1';
          multi = 1;
        } else if (ch == '-') {
          // ���Z�q�̑O�ɐ��l���Ȃ��Ƃ��� ~(�`���_) �ɂ���
          // (-1 * ��ǉ����邱�Ƃŕ���������)
          *val_ptr++ = '~';
          *val_ptr++ = '1';
          multi = 1;
        }
      }

      if (*val) {
        *val_ptr = '\0';
        mx::strcpy(porStr[porcnt++], val);

        // val ������
        *val = '\0';
        val_ptr = val;
      }

      while (sp) {
        if (var[sp - 1] == '(') {
          break;
        }

        porStr[porcnt][0] = var[sp - 1];
        porStr[porcnt++][1] = '\0';
        sp--;
      }

      if (multi) {
        if (sp && (var[sp - 1] == '*' || var[sp - 1] == '/')) {
          porStr[porcnt][0] = var[sp - 1];
          porStr[porcnt++][1] = '\0';
          sp--;
        }

        var[sp++] = '*';  // * �������ǉ�
      } else {
        var[sp++] = ch;  // ���Z�q�ǉ�
      }
    } break;
    case '*':  // *./ �̏ꍇ�͉��Z�q�X�^�b�N�ɒǉ�
    case '/':
      if (*val) {
        *val_ptr = '\0';
        mx::strcpy(porStr[porcnt++], val);

        // val ������
        *val = '\0';
        val_ptr = val;
      }
      if (sp && (var[sp - 1] == '*' || var[sp - 1] == '/')) {
        porStr[porcnt][0] = var[sp - 1];
        porStr[porcnt++][1] = '\0';
        sp--;
      }

      var[sp++] = ch;  // ���Z�q�ǉ�
      break;
    case '(':
      var[sp++] = ch;
      break;
    case ')':
      if (*val) {
        *val_ptr = '\0';
        mx::strcpy(porStr[porcnt++], val);

        // val ������
        *val = '\0';
        val_ptr = val;
      }

      while (sp) {
        if (var[sp - 1] == '(') {
          sp--;
          break;
        }

        porStr[porcnt][0] = var[sp - 1];
        porStr[porcnt++][1] = '\0';
        sp--;
      }
      break;
    case ' ':  // ���`����
    case '\t':
      break;
    default:  // ���̑��̕����̓G���[
      return 0;
      //break;
    }
  }

  return porcnt;
}

/*!
	�t�|�[�����h�L�@�̌v�Z
	@param[in] porCnt �g�[�N���̐�
	@param[in] porStr �g�[�N��������
	@return ����
*/
static double calc(int porCnt, char porStr[MAX_TOKEN][TOKEN_SIZE]) {
  int i;
  int sp = 0;  // �X�^�b�N�|�C���^

  double values[STACK_SIZE];  // �t�|�[�����h�L�@�v�Z�p�̃X�^�b�N
  double ope1, ope2;

#ifdef __debugprint
  // ���̕\��
  printf("��: ");
  for (i = 0; i < porCnt; ++i) {
    printf("%s ", porStr[i]);
  }

  printf("\n");
#endif  // __debugprint

  for (i = 0; i < porCnt; ++i) {
    int sign = 1;
    const char* ch = porStr[i];

    switch (*ch) {
    // ���� ^(�J���b�g) �����Ă����琳�̐�
    case '^':
      sign = 1;
      ch++;
      break;
    // ���� ~(�`���_) �����Ă����畉�̐�
    case '~':
      sign = -1;
      ch++;
      break;
    }

    switch (*ch) {
    // ���� 0 ~ 9 �������琔�l�Ƃ���
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
      if (*(ch + 1) == 'x' || *(ch + 1) == 'X') {
        // 16 �i��
        values[sp++] = (double)MCR_atoi(ch) * sign;
      } else {
        // 10 �i��
        values[sp++] = MCR_atoi(ch) * sign;
      }

      break;
    // �����ɂ���͎̂l�����Z�q�����̃n�Y
    default:
#ifdef __debugprint
      if (sp < 2) {
        printf("�G���[�ł��H\n");
      }
#endif  // __debugprint

      ope2 = values[sp - 1];
      sp--;
      ope1 = values[sp - 1];
      sp--;

      switch (*ch) {
      case '+':
        values[sp++] = ope1 + ope2;
        break;
      case '-':
        values[sp++] = ope1 - ope2;
        break;
      case '*':
        values[sp++] = ope1 * ope2;
        break;
      case '/':
        values[sp++] = ope1 / ope2;
        break;
      }

      break;
    }
  }

  // ����
  return values[sp - 1];
}

namespace mx {

/*!
	�t�|�[�����h�L�@�ɂ�鐔���̌v�Z
	@param[in] exp ����
	@return ��

	@note
	�E16 �i���̌v�Z���\�ł� <br>
	�E(1 - 2) * (4 + 5) �́A 1 2 - 4 5 + * <br>
*/
double CalcString(const char* exp) {
  int porcnt;                          // �g�[�N���J�E���^
  char porStr[MAX_TOKEN][TOKEN_SIZE];  // �t�|�[�����h�L�@
  char t_exp[EXPRESSION_SIZE];         // ����(�J�b�R�Ŋ���)

  // ������ �J�b�R() �Ŋ���
  mx::strcpy(t_exp, "(");
  mx::strcat(t_exp, exp);
  mx::strcat(t_exp, ")");

  porcnt = analyze(porStr, t_exp);  // ���̉��
  return calc(porcnt, porStr);      // �t�|�[�����h�L�@�̌v�Z
}

// SI�ړ����̎����v�Z����
#if 0
double CalcStringSI(const char* exp)
{
	ulString str = exp;
	double degit;
	
	if (str.IsEmpty()) {
		return 0;
	}
	
	// x X �� * �ɂ���
	str.Replace("x", "*", REPLACEALL);
	str.Replace("X", "*", REPLACEALL);
	
	/** �G���W�j�A�����̕ϊ�
		10k+20k �� (10*1000)+(20*1000) �ƕϊ�����
	*/
	
	int pos = 0; // �ʒu
	for (;;) {
		char ch = str.str()[pos]; // �����擾
		// k(1000)
		if (ch == 'k' || ch == 'K' || ch == 'm' || ch == 'M') {
			str.str()[pos] = '*'; // �~�ɒu��������
			
			// �����邾���̒l��}��
			if (ch == 'k' /*|| ch == 'K'*/) {
				// �L��(k) 1 000
				str.Insert( pos + 1, "1000)" );
			} else if (/*ch == 'm' ||*/ ch == 'M') {
				// ���K(M) 1 000 000
				str.Insert( pos + 1, "1000000)" );
			} else if (/*ch == 'g' ||*/ ch == 'G') {
				// �M�K(G) 1 000 000 000
				str.Insert( pos + 1, "1000000000)" );
			}
			
			// �O�̉��Z�q�܂Ŗ߂�(���� pos �� * �ɂȂ��Ă���͂�)
			for (;;) {
				pos--;
				ch = str.str()[pos];
				
				if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
					str.Insert( pos + 1, "(" );
					break;
				} else if (!pos) {
					// �ŏ��܂Ŗ߂����̂ŏI���
					str.Insert( 0, "(" );
					break;
				}
			}
		} else if (ch == '\0') {
			break;
		}
		
		pos++;
	}
	
	// �v�Z
	degit = CalcString(str.c_str());
#if 0
	{
		char buff[256];
		sprintf(buff,"%f\n", degit);
		//OutputDebugString(buff);
	}
#endif
	
	return degit;
}
#endif

}  //mx
