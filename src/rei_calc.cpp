// -*- mode:c++; coding:utf-8-ws -*-
//====================================================================
//! @file  rei_calc.cpp
//! @brief 文字列式の計算
//!
//! @author
//====================================================================
#include "rei_strlib.h"
#include "rei_math.h"

#define STACK_SIZE 32        // スタックサイズ
#define MAX_TOKEN 64         // トークンの最大数
#define TOKEN_SIZE 16        // トークンの最大長
#define EXPRESSION_SIZE 128  // 式の最大長

/*!
* 式を逆ポーランド記法にする
* @param[out] porStr 出力バッファ
* @param[in] exp 数式(カッコ() で括った数式を指定します)
* @return トークンの数
*/
static int analyze(char porStr[MAX_TOKEN][TOKEN_SIZE], const char* exp) {
  int sp = 0;      // スタックポインタ
  int porcnt = 0;  // トークンカウンタ

  char val[TOKEN_SIZE] = "";  // 数値
  char* val_ptr = val;

  char var[STACK_SIZE];  // 演算子用のスタック

  while (*exp) {
    char ch = *exp++;

    switch (ch) {
    case ',':  // 区切り
      break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':  // 数値の場合
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
    case '+':  // +,- の場合は演算子スタックを掃き出す
    case '-': {
      int multi = 0;  // * にするフラグ

      if (*val == '\0') {
        if (ch == '+') {
          // 演算子の前に数値がないときは ^(カレット) にする
          // (+1 * を追加することで符号を処理)
          *val_ptr++ = '^';
          *val_ptr++ = '1';
          multi = 1;
        } else if (ch == '-') {
          // 演算子の前に数値がないときは ~(チルダ) にする
          // (-1 * を追加することで符号を処理)
          *val_ptr++ = '~';
          *val_ptr++ = '1';
          multi = 1;
        }
      }

      if (*val) {
        *val_ptr = '\0';
        rei::strcpy(porStr[porcnt++], val);

        // val 初期化
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

        var[sp++] = '*';  // * を強制追加
      } else {
        var[sp++] = ch;  // 演算子追加
      }
    } break;
    case '*':  // *./ の場合は演算子スタックに追加
    case '/':
      if (*val) {
        *val_ptr = '\0';
        rei::strcpy(porStr[porcnt++], val);

        // val 初期化
        *val = '\0';
        val_ptr = val;
      }
      if (sp && (var[sp - 1] == '*' || var[sp - 1] == '/')) {
        porStr[porcnt][0] = var[sp - 1];
        porStr[porcnt++][1] = '\0';
        sp--;
      }

      var[sp++] = ch;  // 演算子追加
      break;
    case '(':
      var[sp++] = ch;
      break;
    case ')':
      if (*val) {
        *val_ptr = '\0';
        rei::strcpy(porStr[porcnt++], val);

        // val 初期化
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
    case ' ':  // 整形文字
    case '\t':
      break;
    default:  // その他の文字はエラー
      return 0;
      //break;
    }
  }

  return porcnt;
}

/*!
	逆ポーランド記法の計算
	@param[in] porCnt トークンの数
	@param[in] porStr トークン文字列
	@return 結果
*/
static double calc(int porCnt, char porStr[MAX_TOKEN][TOKEN_SIZE]) {
  int i;
  int sp = 0;  // スタックポインタ

  double values[STACK_SIZE];  // 逆ポーランド記法計算用のスタック
  double ope1, ope2;

#ifdef __debugprint
  // 式の表示
  printf("式: ");
  for (i = 0; i < porCnt; ++i) {
    printf("%s ", porStr[i]);
  }

  printf("\n");
#endif  // __debugprint

  for (i = 0; i < porCnt; ++i) {
    int sign = 1;
    const char* ch = porStr[i];

    switch (*ch) {
    // 頭に ^(カレット) がついていたら正の数
    case '^':
      sign = 1;
      ch++;
      break;
    // 頭に ~(チルダ) がついていたら負の数
    case '~':
      sign = -1;
      ch++;
      break;
    }

    switch (*ch) {
    // 頭が 0 ~ 9 だったら数値とする
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
        // 16 進数
        values[sp++] = (double)MCR_atoi(ch) * sign;
      } else {
        // 10 進数
        values[sp++] = MCR_atoi(ch) * sign;
      }

      break;
    // ここにくるのは四則演算子だけのハズ
    default:
#ifdef __debugprint
      if (sp < 2) {
        printf("エラーです？\n");
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

  // 結果
  return values[sp - 1];
}

namespace rei {

/*!
	逆ポーランド記法による数式の計算
	@param[in] exp 数式
	@return 解

	@note
	・16 進数の計算も可能です <br>
	・(1 - 2) * (4 + 5) は、 1 2 - 4 5 + * <br>
*/
double CalcString(const char* exp) {
  int porcnt;                          // トークンカウンタ
  char porStr[MAX_TOKEN][TOKEN_SIZE];  // 逆ポーランド記法
  char t_exp[EXPRESSION_SIZE];         // 数式(カッコで括る)

  // 数式を カッコ() で括る
  rei::strcpy(t_exp, "(");
  rei::strcat(t_exp, exp);
  rei::strcat(t_exp, ")");

  porcnt = analyze(porStr, t_exp);  // 式の解析
  return calc(porcnt, porStr);      // 逆ポーランド記法の計算
}

// SI接頭辞の式を計算する
#if 0
double CalcStringSI(const char* exp)
{
	ulString str = exp;
	double degit;
	
	if (str.IsEmpty()) {
		return 0;
	}
	
	// x X を * にする
	str.Replace("x", "*", REPLACEALL);
	str.Replace("X", "*", REPLACEALL);
	
	/** エンジニア数字の変換
		10k+20k を (10*1000)+(20*1000) と変換する
	*/
	
	int pos = 0; // 位置
	for (;;) {
		char ch = str.str()[pos]; // 文字取得
		// k(1000)
		if (ch == 'k' || ch == 'K' || ch == 'm' || ch == 'M') {
			str.str()[pos] = '*'; // ×に置き換える
			
			// かけるだけの値を挿入
			if (ch == 'k' /*|| ch == 'K'*/) {
				// キロ(k) 1 000
				str.Insert( pos + 1, "1000)" );
			} else if (/*ch == 'm' ||*/ ch == 'M') {
				// メガ(M) 1 000 000
				str.Insert( pos + 1, "1000000)" );
			} else if (/*ch == 'g' ||*/ ch == 'G') {
				// ギガ(G) 1 000 000 000
				str.Insert( pos + 1, "1000000000)" );
			}
			
			// 前の演算子まで戻る(初回 pos は * になっているはず)
			for (;;) {
				pos--;
				ch = str.str()[pos];
				
				if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
					str.Insert( pos + 1, "(" );
					break;
				} else if (!pos) {
					// 最初まで戻ったので終わり
					str.Insert( 0, "(" );
					break;
				}
			}
		} else if (ch == '\0') {
			break;
		}
		
		pos++;
	}
	
	// 計算
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

}  //rei
