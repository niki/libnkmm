// -*- mode:c++; coding:utf-8-ws -*-
//====================================================================
//! @file  rei_sort.cpp
//! @brief ソート関数
//!
//! @author
//====================================================================
#include "rei_sort.h"
#include "rei_basis.h"
#include "rei_memory.h"

//! ソートの種類
enum {
  BubbleSort = 0,  //!< バブルソート
  InsertSort,      //!< 挿入ソート
  ShellSort,       //!< シェルソート
  SelectSort,      //!< 選択ソート
  QuickSort,       //!< クイックソート
};

/*!
	バブルソート(内部処理)
	@param[in,out] data ソートする配列のポインタのポインタ
	@param[in] n ソートする配列の長さ
	@param[in] fcmp 比較関数
	
	@note
	ポインタの入れ替えを対象としたソート
	
	@note
	＜降順の比較関数＞ <br>
	・結果が負であれば、入れ替えを行う <br>
	  ＡよりもＢの方が大きいと判定するときは、Ａ－Ｂ <br>
	  ＢよりもＡの方が大きいと判定するときは、Ｂ－Ａ <br>
	
	<pre>
	static int	_cmp_func(const void *a, const void *b)
	{
		return( *(int *)a - *(int *)b );
	}
	</pre>
	
	＜昇順の比較関数＞ <br>
	・結果が負であれば、入れ替えを行う <br>
	  ＡよりもＢの方が小さいと判定するときは、Ｂ－Ａ <br>
	  ＢよりもＡの方が小さいと判定するときは、Ａ－Ｂ <br>
	
	<pre>
	static int	_cmp_func(const void *a, const void *b)
	{
		return( *(int *)b - *(int *)a );
	}
	
	local_BubbleSort( hoge, 8, _cmp_func );
	</pre>
*/
static void local_BubbleSort(uint8_t **data, int n, int (*fcmp)(const void *, const void *)) {
  int i, j;
  uint8_t *temp;  // テンポラリ

  // ソートする数を設定
  int h = n - 1;

  // ソートする要素がなくなるまで繰り返す
  do {
    // 残り要素数を－１で初期化
    j = -1;

    // ソートする要素分だけ繰り返す
    for (i = 1; i <= h; ++i) {
      uint8_t **left = &data[i];       // 左側要素のポインタ
      uint8_t **right = &data[i - 1];  // 右側要素のポインタ

      // 比較
      //
      if (fcmp(*left, *right) < 0) {
        // 残り要素数を減らす
        j = i - 1;

        // 要素のポインタの入れ替え
        temp = data[i];
        data[i] = data[j];
        data[j] = temp;
      }
    }

    // 残り要素数の設定
    h = j;

  } while (h >= 0);
}

/*!
	挿入ソート(内部処理)
	@param[in,out] data ソートする配列のポインタのポインタ
	@param[in] n ソートする配列の長さ
	@param[in] fcmp 比較関数
	
	@note
	ポインタの入れ替えを対象としたソート
	
	@note
	＜降順の比較関数＞ <br>
	・結果が負であれば、入れ替えを行う <br>
	  ＡよりもＢの方が大きいと判定するときは、Ａ－Ｂ <br>
	  ＢよりもＡの方が大きいと判定するときは、Ｂ－Ａ <br>
	
	<pre>
	static int	_cmp_func(const void *a, const void *b)
	{
		return( *(int *)a - *(int *)b );
	}
	</pre>
	
	＜昇順の比較関数＞ <br>
	・結果が負であれば、入れ替えを行う <br>
	  ＡよりもＢの方が小さいと判定するときは、Ｂ－Ａ <br>
	  ＢよりもＡの方が小さいと判定するときは、Ａ－Ｂ <br>
	
	<pre>
	static int	_cmp_func(const void *a, const void *b)
	{
		return( *(int *)b - *(int *)a );
	}
	
	local_InsertSort( hoge, 8, _cmp_func );
	</pre>
*/
static void local_InsertSort(uint8_t **data, int n, int (*fcmp)(const void *, const void *)) {
  int i, j;

  if (n <= 1) {
    return;
  }

  //
  // 挿入ソートでは、すでにソート済のレコードに新しいレコードを正しい場所に挿入する。
  // つまり、最初の2つのレコードについてまず入れ換えにより正しい順序にする。
  // つぎに、3つ目のレコードを前の2つのレコードと比較して、必要に応じて入れ換えを行う。
  // これで、3つのレコードが正しい順序になる。このように最後のレコードまで繰り返す。
  //

  // 0 から n までがソート済みの時に、data[i] を適切なところに挿入する
  for (i = 1; i < n; ++i) {
    // 挿入すべきレコード
    uint8_t *record = data[i];

    // data[j] が record よりも小さくなる j を求める
    for (j = i - 1; j >= 0 && fcmp(record, data[j]) < 0; --j) {
      // 右に1つ要素をずらす
      data[j + 1] = data[j];
    }

    // record を挿入する
    data[j + 1] = record;
  }
}

/*!
	シェルソート(内部処理)
	@param[in,out] data ソートする配列のポインタのポインタ
	@param[in] n ソートする配列の長さ
	@param[in] fcmp 比較関数
	
	@note
	ポインタの入れ替えを対象としたソート
	
	@note
	＜降順の比較関数＞ <br>
	・結果が負であれば、入れ替えを行う <br>
	  ＡよりもＢの方が大きいと判定するときは、Ａ－Ｂ <br>
	  ＢよりもＡの方が大きいと判定するときは、Ｂ－Ａ <br>
	
	<pre>
	static int	_cmp_func(const void *a, const void *b)
	{
		return( *(int *)a - *(int *)b );
	}
	</pre>
	
	＜昇順の比較関数＞ <br>
	・結果が負であれば、入れ替えを行う <br>
	  ＡよりもＢの方が小さいと判定するときは、Ｂ－Ａ <br>
	  ＢよりもＡの方が小さいと判定するときは、Ａ－Ｂ <br>
	
	<pre>
	static int	_cmp_func(const void *a, const void *b)
	{
		return( *(int *)b - *(int *)a );
	}
	
	local_ShellSort( hoge, 8, _cmp_func );
	</pre>
*/
static void local_ShellSort(uint8_t **data, int n, int (*fcmp)(const void *, const void *)) {
  int i, j, h;

  if (n <= 1) {
    return;
  }

  // 初期値の計算
  for (h = 1; h <= n; h = 3 * h + 1) {
  }

  while ((h = h / 3) >= 1) {
    // h から n までがソート済みの時に、data[ i ] を適切なところに挿入する
    for (i = h; i < n; ++i) {
      // 挿入すべきレコード
      uint8_t *record = data[i];

      // data[j] が record よりも小さくなる j を求める
      for (j = i - h; j >= 0 && fcmp(record, data[j]) < 0; j -= h) {
        // 右に h 要素をずらす
        data[j + h] = data[j];
      }

      // record を挿入する
      data[j + h] = record;
    }
  }
}

/*!
	選択ソート(内部処理)
	@param[in,out] data ソートする配列のポインタのポインタ
	@param[in] n ソートする配列の長さ
	@param[in] fcmp 比較関数
	
	@note
	ポインタの入れ替えを対象としたソート
	
	@note
	＜降順の比較関数＞ <br>
	・結果が負であれば、入れ替えを行う <br>
	  ＡよりもＢの方が大きいと判定するときは、Ａ－Ｂ <br>
	  ＢよりもＡの方が大きいと判定するときは、Ｂ－Ａ <br>
	
	<pre>
	static int	_cmp_func(const void *a, const void *b)
	{
		return( *(int *)a - *(int *)b );
	}
	</pre>
	
	＜昇順の比較関数＞ <br>
	・結果が負であれば、入れ替えを行う <br>
	  ＡよりもＢの方が小さいと判定するときは、Ｂ－Ａ <br>
	  ＢよりもＡの方が小さいと判定するときは、Ａ－Ｂ <br>
	
	<pre>
	static int	_cmp_func(const void *a, const void *b)
	{
		return( *(int *)b - *(int *)a );
	}
	
	local_SelectSort( hoge, 8, _cmp_func );
	</pre>
*/
static void local_SelectSort(uint8_t **data, int n, int (*fcmp)(const void *, const void *)) {
  int i, j, k;
  uint8_t *temp;  // テンポラリ

  for (i = 0; i < n - 1; ++i) {
    k = i;

    for (j = i + 1; j < n; ++j) {
      if (fcmp(data[j], data[k]) < 0) {
        k = j;
      }
    }

    if (k != i) {
      // 要素のポインタの入れ替え
      temp = data[i];
      data[i] = data[k];
      data[k] = temp;
    }
  }
}

/*!
	クイックソート(内部処理)
	@param[in,out] data ソートする配列のポインタのポインタ
	@param[in] left ソートする左側の位置
	@param[in] right ソートする右側の位置
	@param[in] fcmp 比較関数
	
	@note
	ポインタの入れ替えを対象としたソート
	
	@note
	＜降順の比較関数＞ <br>
	・軸(Ｂ)より小さい値を探しだすため、Ａ－Ｂを行う <br>
	・結果が負であれば、軸よりも小さい値となる。 <br>
	・結果が正であれば、軸よりも大きい値となる。 <br>
	
	<pre>
	static int	_cmp_func(const void *a, const void *b)
	{
		return( *(int *)a - *(int *)b );
	}
	</pre>
	
	＜昇順の比較関数＞ <br>
	・軸(Ｂ)より大きい値を探しだすため、Ｂ－Ａを行う <br>
	・結果が負であれば、軸よりも大きい値となる。 <br>
	・結果が正であれば、軸よりも小さい値となる。 <br>
	
	<pre>
	static int	_cmp_func(const void *a, const void *b)
	{
		return( *(int *)b - *(int *)a );
	}
	
	local_QuickSort( hoge, 0, 8, _cmp_func );
	</pre>
*/
static void local_QuickSort(uint8_t **data, int left, int right,
                            int (*fcmp)(const void *, const void *)) {
  int i, j;
  uint8_t *temp;   // テンポラリ
  uint8_t *pivot;  // 軸とする要素のポインタ

  const int THREASHOLD = 11;  // クイックソートを別のソートに切り替える閾値

  // 終了条件
  if (left > right) {
    return;
  }

  // ある程度の要素数まで減ったら、
  // 無駄なオーバーヘッドを防ぐために、
  // ソート関数を切り替える。
  if (right - left < THREASHOLD) {
    //		local_BubbleSort(&data[left], right - left + 1, fcmp );		// バブルソート
    //		local_InsertSort(&data[left], right - left + 1, fcmp );		// 挿入ソート
    //		local_ShellSort(&data[left], right - left + 1, fcmp );		// シェルソート
    local_SelectSort(&data[left], right - left + 1, fcmp);  // 選択ソート
    return;
  }

  // 中心を軸に設定
  pivot = data[(left + right) / 2];

  // i の初期位置
  i = left - 1;

  // j の初期位置
  j = right + 1;

  for (;;) {
    // 比較関数:fcmp
    //   右側を軸とし、
    //     戻り値＜０
    //       降順：左から軸より小さい値を探す
    //       昇順：左から軸より大きい値を探す
    //     戻り値＞０
    //       降順：右から軸より大きい値を探す
    //       昇順：右から軸より小さい値を探す
    while (fcmp(data[++i], pivot) < 0) {
    }
    while (fcmp(data[--j], pivot) > 0) {
    }

    // i と j が入れ替わったら終了
    if (i >= j) {
      break;
    }

    // i と j が指す要素のポインタを入れ換える
    temp = data[i];
    data[i] = data[j];
    data[j] = temp;
  }

  // 軸より小さいグループにクイックソート
  local_QuickSort(data, left, i - 1, fcmp);

  // 軸より大きいグループにクイックソート
  local_QuickSort(data, j + 1, right, fcmp);
}

/*!
	ソート基底
	@param[in] type ソートの種類
	@param[out] dest 出力先バッファ
	@param[in] src 入力元バッファ
	@param[in] n ソートする配列の長さ
	@param[in] w 要素１つのサイズ
	@param[in] swap_w 入れ替えサイズ
	@param[in] fcmp 比較関数
	
	@note
	入力元と出力先が同じ場合はテンポラリバッファを作成します。 <br>
	比較関数の作成方法は、各ソート処理の説明を見てください。 <br>
	
	@note
	iSort( BubbleSort, hoge, hoge, 8, sizeof(int), sizeof(int), _cmp_func );
*/
static void iSort(int type, void *dest, void *src, int n, int w, int swap_w,
                  int (*fcmp)(const void *, const void *)) {
  int i;
  uint8_t **pp;   // ソートする要素のポインタ
  uint8_t *data;  // ソートする要素の実体

  // 要素のポインタの領域を確保
  pp = (uint8_t **)malloc_(sizeof(uint8_t *) * n);

  /**********************/
  /* ソートバッファ設定 */
  /**********************/
  // 入力元と出力先が同じなのでテンポラリバッファを作成
  if (dest == src) {
    // 要素の領域を確保
    data = (uint8_t *)malloc_(sizeof(uint8_t) * n * w);

    // 元データをコピー
    rei::Memory::Copy(data, src, n * w);
  }
  // 入力元バッファをそのまま使用
  else {
    // 要素は入力元
    data = (uint8_t *)src;
  }

  // ソートのためにポインタのマップの作成
  for (i = 0; i < n; ++i) {
    // ソートするデータのポインタを割り当てる
    pp[i] = &data[i * w];
  }

  // ソート処理
  switch (type) {
  case BubbleSort:
    local_BubbleSort(pp, n, fcmp);
    break;  // バブルソート
  case InsertSort:
    local_InsertSort(pp, n, fcmp);
    break;  // 挿入ソート
  case ShellSort:
    local_ShellSort(pp, n, fcmp);
    break;  // シェルソート
  case SelectSort:
    local_SelectSort(pp, n, fcmp);
    break;  // 選択ソート
  case QuickSort:
    local_QuickSort(pp, 0, n - 1, fcmp);
    break;  // クイックソート
  }

  // ソートした順番にデータをコピー
  for (i = 0; i < n; ++i) {
    rei::Memory::Copy(&((uint8_t *)dest)[i * w], pp[i], swap_w);
  }

  // ソートに使った領域を開放
  if (dest == src) {
    free_(data);
  }
  free_(pp);
}

/*!
	ソート
	@param[in,out] array ソートするバッファ
	@param[in] n ソートする配列の長さ
	@param[in] w 要素１つのサイズ
	@param[in] fcmp 比較関数
	
	@note
	入力元と出力先が同じ場合はテンポラリバッファを作成します。 <br>
	比較関数の作成方法は、各ソート処理の説明を見てください。 <br>
	
	@note
	Sort( BubbleSort, hoge, 8, sizeof(int), _cmp_func );
*/

namespace rei {

void bsort(void *array, int n, int w, int (*fcmp)(const void *, const void *)) {
  iSort(BubbleSort, array, array, n, w, w, fcmp);
}
void inssort(void *array, int n, int w, int (*fcmp)(const void *, const void *)) {
  iSort(InsertSort, array, array, n, w, w, fcmp);
}
void ssort(void *array, int n, int w, int (*fcmp)(const void *, const void *)) {
  iSort(ShellSort, array, array, n, w, w, fcmp);
}
void selsort(void *array, int n, int w, int (*fcmp)(const void *, const void *)) {
  iSort(SelectSort, array, array, n, w, w, fcmp);
}
void qsort(void *array, int n, int w, int (*fcmp)(const void *, const void *)) {
  iSort(QuickSort, array, array, n, w, w, fcmp);
}

}  //rei
