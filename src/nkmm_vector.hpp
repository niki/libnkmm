// -*- mode:c++; coding:utf-8-ws -*-
//====================================================================
//! @file  nkmm_vector.hpp
//! @brief 動的配列クラス(できるだけ単純に)
//!
//! @author
//! @note
//!   感覚的には std::vector と同じようなものです。でも及ばず。
//!   主な違いは std::vector の領域確保が n^2 に対し，n+reserve になります。
//!   配列と同じようにアクセスできますが、要素は push_back() で拡張します。
//!   配列領域拡大のときには、配列要素の再配置を行っています。
//!   参照が速い分、削除が遅いです(配列といっしょ)
//! @note
//!   断片化された領域を整列する処理を入れたい。
//! @note
//!   2006.09.13 イテレータ実装
//!   2006.05.31 挿入・シェル・選択・クイックソートを追加
//!   2005.05.02 アロケートアイテムをチェインでなく配列マップ上に置くように修正(できるかぎり断片化を防ぐための措置)
//!   2005.04.30 あまり深く考えないで簡素にした
//====================================================================
#ifndef NKMM_VECTOR_H
#define NKMM_VECTOR_H

//
#define NKMM_VECTOR_SMALL

/**
メモリ確保時に reserve_ の間隔で配列を拡張していきます。
配列拡張時にポインタの再配置を行っているため、
間隔が広いほど再配置の回数が少なくなります。
図で表すと、

･-----------･-----------･
|  alloc 0  |  alloc 1  |
･-----------･-----------･
|         size_         |
･-----------------------･-----------･------------･
|               maxSize_            |  alloc_(2) |
･-----------------------------------･------------･

reserve_ = 2 のとき
･-----------･----------･---------･
|           |          | items_  |
|           | items_[] ･---------･
|           |          | items_  |
| items_*[] ･-----------･---------･
|           |          | items_  |
|           | items_[] ･---------･
|           |          | items_  |
･-----------･----------･---------･

のような構成になります。
削除時は領域を開放せずに残しておくことで、
以降、追加・挿入したときに新しく領域を確保せずに、
残しておいた領域を使用することで、アロケートの回数を減らしています。
そのため、使用していない領域も残っているために、
メモリの使用量を必要以上に消費する可能\_性もあります。
そのような場合は一度 .clean() をすることで解決してください。
動的に配列を拡張していくのでメモリの断片化の原因になる可能性があります。

    index 1 を .erase() した場合
･-･-･-･-･-･-･-･    ･-･-･-･-･-･-･-･
|0|1|2|3|?|?|?| → |0|2|3|1|?|?|?|
･-･-･-･-|-･-･-･    ･-･-･-|-･-･-･-･
       size_            size_

    index 1 に .Insert() した場合
･-･-･-･-･-･-･-･    ･-･-･-･-･-･-･-･
|0|1|2|3|?|?|?| → |0|4|1|2|3|?|?|
･-･-･-･-|-･-･-･    ･-･-･-･-･-|-･-･
       size_                size_

○使用例
void main()
{
	int i;
	nkmm::vector<RECT> items;
	
	RECT rect;
	
	rect.left   = 0;
	rect.top    = 0;
	rect.right  = 320;
	rect.bottom = 240;
	items.push_back(rect);
	
	rect.left   = 0;
	rect.top    = 0;
	rect.right  = 640;
	rect.bottom = 480;
	items.insert(0, rect); // index 0 に挿入
	
	rect.left   = 0;
	rect.top    = 0;
	rect.right  = 480;
	rect.bottom = 272;
	items.push_back(rect);
	
	for (i = 0; i < items.size(); ++i) {
		RECT &rc = items[i];
		
		printf("Rectangle:\n");
		printf("  left   %d\n", rc.left);
		printf("  top    %d\n", rc.top);
		printf("  right  %d\n", rc.right);
		printf("  bottom %d\n", rc.bottom);
	}
}
*/

namespace nkmm {

//!  クラス
template <class T>
class vector {
 public:
  //	typedef T *iterator;
  //	typedef T &reference;
  //	typedef const T *const_iterator;

  //	typedef T *reverse_iterator;
  //	typedef T &reverse_reference;
  //	typedef const T *const_reverse_iterator;

  /*!
	* コンストラクタ
	* @param[in] reserve 拡張のたびに確保する数
	*/
  vector(int reserve = 32) : items_(0), reserve_(reserve), size_(0), maxSize_(0), alloc_(0) {}

  /*!
	* デストラクタ
	*/
  virtual ~vector() { this->clean(); }

  /*!
	* 空かどうか
	* @return true:空, false:空でない
	*/
  bool empty() const { return (size_ == 0); }

  /*!
	* アイテムの確保数設定
	* @param[in] reserve 拡張のたびに確保する数
	*/
  void reserve(int reserve) { reserve_ = reserve; }

  /*!
	* 配列サイズ取得
	* @return int
	*/
  int size() const { return size_; }

  /*!
	* 最大配列サイズ取得
	* @return int
	*/
  int max_size() const { return maxSize_; }

  /*!
	* アイテムのポインタ取得
	* @param[in] index 
	* @return T&
	*/
  T& at(int index) const { return *items_[index]; }

  /*!
	* 最初の要素を取得
	* @return T&
	*/
  T& front() const { return at(0); }

  /*!
	* 最後の要素を取得
	* @return T&
	*/
  T& back() const { return at(size() - 1); }

  /*!
	* [] override
	* @param[in] index
	* @return T&
	*/
  T& operator[](int index) const { return at(index); }

  // @todo
  // イテレータは実装途中。
  // 要素のアドレスは連続していないため
  //     for ( i = begin(); i != end(); i++ )
  // のような使い方ができません。
  // prev() next() を作成し、リストのような形で利用できるようにする必要があります。
  //	iterator	begin() { return at( 0 ); }		// 先頭の要素を指すイテレータを返す
  //	iterator	end() { return at( size_ - 1 ) + 1; }	// 最後の要素の次をを指すイテレータを返す

  //	iterator	rbegin() { return at( size_ - 1 ); }		// 最後の要素を指すイテレータを返す
  //	iterator	rend() { return at( 0 ) - 1; }	// 先頭の要素の前をを指すイテレータを返す

  /*!
	* 末尾の要素を削除
	*/
  void pop_back() { erase(size() - 1); }

  /*!
	* 要素の追加
	* @return T&
	*/
  T& push_back() {
#if 1
    return insert(size());
#else
    int size = size_;        // アイテムカウンタ
    int maxSize = maxSize_;  // 配列サイズ(items_* の要素の総数)

    // アイテムポインタ
    T* item;

    // カウンタの設定
    size_++;

    if (maxSize < size + 1) {
      /**	有効アイテム数が実体アイテム数を上回った場合、
				実体アイテム領域を新しく作成します。
			*/

      // アイテム領域の新規作成
      item = extendCapacity();

      // アイテムの登録:
      //   再配置が行われている場合があるので items = items_ は必ずここでやる
      T** items = items_;  // アイテムポインタ

      // 作成した配列にアイテムを割り当てる
      // 追加の場合は size_ の位置に最初のアイテムが登録されます。
      for (int i = reserve_ - 1; 0 <= i; i--) {
        items[maxSize + i] = &item[i];
      }

    } else {
      /**	配列のカウンタよりもカウンタが小さい場合は、
				既に領域があると判断して、
				既存の配列にアイテムを追加します。
			*/

      // size_ の位置のアイテムを使用するため、やることなし
      item = items_[size];
    }

    // アイテムポインタを返す
    return *item;
#endif
  }

  /*!
	* 要素の追加
	* @param[in] a
	* @return T&
	*/
  T& push_back(const T& a) {
    T& b = push_back();
    b = a;
    return b;
  }

  /*!
	* 要素の挿入
	* @param[in] index
	* @return T&
	*/
  T& insert(int index) {
    int size = size_;        // アイテムカウンタ
    int maxSize = maxSize_;  // 配列サイズ(items_* の要素の総数)

    // アイテムポインタ
    T* item;

    // カウンタの設定
    size_++;

    if (maxSize < size + 1) {
      /**	有効アイテム数が実体アイテム数を上回った場合、
				実体アイテム領域を新しく作成します。
			*/

      // アイテム領域の新規作成
      item = extendCapacity();

      // アイテムの登録:
      //   再配置が行われている場合があるので items = items_ は必ずここでやる
      //   index の位置にアイテムを登録するために index から size_ までを１つずらします。
      T** items = items_;  // アイテムポインタ

      // index から size_ までをずらす
      for (int i = size; index < i; i--) {
        items[i] = items[i - 1];
      }

      // 作成した配列にアイテムを割り当てる
      // 追加の場合は size_ の位置に最初のアイテムが登録されます。
      for (int i = reserve_ - 1; 1 <= i; i--) {
        items[maxSize + i] = &item[i];
      }

      // index の位置にアイテムを登録
      items[index] = item;

    } else {
      /**	配列のカウンタよりもカウンタが小さい場合は、
				既に領域があると判断して、
				既存の配列にアイテムを追加します。
			*/

      T** items = items_;  // アイテムポインタ

      // 既にアイテムが存在しているはずなので
      // カウントの次のアイテムを使用する
      item = items[size];

      // index の位置のアイテム領域を使用するため index から size_ までをずらす
      for (int i = size; index < i; i--) {
        items[i] = items[i - 1];
      }

      // index の位置にアイテムを登録
      items[index] = item;
    }

    // アイテムポインタを返す
    return *item;
  }

  /*!
	* 要素の挿入
	* @param[in] index
	* @param[in] a
	* @return T&
	*/
  T& insert(int index, const T& a) {
    T& b = insert(index);
    b = a;
    return b;
  }

  /*!
	* 要素の削除
	* @param[in] index
	*/
  void erase(int index) {
    /**	index + 1 ～ size_ - 1 までのアイテムを前方に向かって１つずらすことで削除扱いにします。
			このとき index 番目のアイテムを size_ - 1 の位置に再配置することで、再利用を可能とします。
		*/

    int size = size_;  // アイテムカウンタ

    T** items = items_;          // アイテムポインタ
    T* pTarget = items_[index];  // 削除対象アイテム

    // カウンタの設定
    size_--;

    // index から size_ - 1 までをずらす
    for (int i = index; i < size - 1; i++) {
      items[i] = items[i + 1];
    }

    // 削除対象のアイテムを size_ - 1 の位置に再配置
    items[size - 1] = pTarget;
  }

  /*!
	* クリア(カウンタが０になるだけ)
	*/
  void clear() { size_ = 0; }

  /*!
	* 領域のクリア
	*/
  void clean() {
    // アロケートしたアイテムを解放します。
    if (0 < alloc_) {
      int maxSize = maxSize_;  // 配列サイズ(items_* の要素の総数)
      int alloc = alloc_;      // アロケートカウンタ

      for (int i = maxSize + alloc - 1; maxSize <= i; i--) {
        SAFE_DELETE_ARRAY(items_[i]);  // delete!!
      }

      alloc_ = 0;
    }

    // items_ を解放します
    SAFE_DELETE_ARRAY(items_);
    maxSize_ = 0;

    // クリアします
    clear();
  }

//! 配列リストのソート
#ifndef NKMM_VECTOR_SMALL
  void bsort(int (*fnCmp)(const T&, const T&), int offset, int n);    // バブルソート
  void ssort(int (*fnCmp)(const T&, const T&), int offset, int n);    // シェルソート
  void selsort(int (*fnCmp)(const T&, const T&), int offset, int n);  // 選択ソート
#endif                                                                // NKMM_VECTOR_SMALL
  void inssort(int (*fnCmp)(const T&, const T&), int offset, int n);  // 挿入ソート
  void qsort(int (*fnCmp)(const T&, const T&), int left, int right);  // クイックソート

 private:
  /*!
	* 領域の新規作成
	* @return T*
	*/
  T* extendCapacity() {
    int reserve = reserve_;  // アイテム確保数(一度に確保する数)
    int maxSize = maxSize_;  // 配列サイズ(items_* の要素の総数)
    int alloc = alloc_;      // アロケートカウンタ

    // アイテム領域の作成
    T* item = SAFE_NEW_ARRAY(__SOURCE__, T, reserve);

    // 配列サイズの設定
    maxSize_ = maxSize + reserve;

    // アロケートカウンタの設定
    alloc_++;

    /**	新規アイテムのポインタを割り当てる配列を拡張します。
			アロケートしたアイテムのポインタを保持する領域も拡張します。
			同一メモリ上に置くための仕様です。
			配列マップは以下のようになります。
			
			  000 アイテム１(アロケートアイテム１)
			  001 アイテム２(アロケートアイテム１)
			  002 アイテム３(アロケートアイテム２)
			  003 アイテム４(アロケートアイテム２)
			                 .                    
			                 .                    
			                 .                    
			  050 アロケートアイテム１のポインタ (clean() 時に解放するポインタ)
			  051 アロケートアイテム２のポインタ (clean() 時に解放するポインタ)
			                 .                    
			                 .                    
			                 .                    
		*/
    {
      T** items = items_;  // アイテムポインタ
      T** temp = SAFE_NEW_ARRAY(__SOURCE__, T*, maxSize + reserve + alloc + 1);

      // 既に存在しているアイテムのポインタをコピーします (初めから maxSize 個まで。以降は０である保証なし)
      for (int i = 0; i < maxSize; i++) {
        temp[i] = items[i];
      }

      // アロケートアイテムのポインタをコピーします (アロケートしたアイテムのポインタ保持のため)
      for (int i = maxSize; i < maxSize + alloc; i++) {
        temp[i + reserve] = items[i];
      }

      // 新しく取得したアイテムのポインタを設定
      temp[maxSize + alloc + reserve] = item;

      SAFE_DELETE_ARRAY(items);  // items_ を delete します
      items_ = temp;             // ポインタの再設定
    }

    return item;
  }

 private:
  T** items_;    //!< アイテムオブジェクト
  int reserve_;  //!< アイテム確保数(一度に確保する数)
  int size_;     //!< アイテムカウンタ
  int maxSize_;  //!< 配列サイズ(items_* の要素の総数)
  int alloc_;    //!< アロケートカウンタ
};

/*!
* 配列リストのソート(バブルソート)
* @param[in] fnCmp 比較関数
* @param[in] offset 開始オフセット
* @param[in] n ソート個数
* 
* @code
* template <class T> static int _fnCompare(const T &a, const T &b)
* {
* 	return b.ZOrder - a.ZOrder;
* }
* @endcode
*/
#ifndef NKMM_VECTOR_SMALL
template <class T>
void Vector<T>::bsort(int (*fnCmp)(const T&, const T&), int offset, int n) {
  int i, j;
  T** data = &items_[offset];  // アイテムポインタ

  // ソートする数を設定
  int h = n - 1;

  // ソートする要素がなくなるまで繰り返す
  do {
    // 残り要素数を－１で初期化
    j = -1;

    // ソートする要素分だけ繰り返す
    for (i = 1; i <= h; i++) {
      T* l = data[i];      // 左側要素のポインタ
      T* r = data[i - 1];  // 右側要素のポインタ

      // 比較
      if (fnCmp(*l, *r) < 0) {
        // 残り要素数を減らす
        j = i - 1;

        // 要素のポインタの入れ替え
        data[i] = r;
        data[j] = l;
      }
    }

    // 残り要素数の設定
    h = j;

  } while (h >= 0);
}
#endif  // NKMM_VECTOR_SMALL

/*!
* 配列リストのソート(シェルソート)
* @param[in] fnCmp 比較関数
* @param[in] offset 開始オフセット
* @param[in] n ソート個数
* 
* @code
* template <class T> static int _fnCompare(const T &a, const T &b)
* {
* 	return b.ZOrder - a.ZOrder;
* }
* @endcode
*/
#ifndef NKMM_VECTOR_SMALL
template <class T>
void Vector<T>::ssort(int (*fnCmp)(const T&, const T&), int offset, int n) {
  int i, j;
  T** data = &items_[offset];  // アイテムポインタ

  if (n <= 1) {
    return 0;
  }

  // 初期値の計算
  for (h = 1; h <= n; h = 3 * h + 1) {
  }

  while ((h = h / 3) >= 1) {
    // h から n までがソート済みの時に、data[ i ] を適切なところに挿入する
    for (i = h; i < n; i++) {
      // 挿入すべきレコード
      T* record = data[i];

      // data[j] が record よりも小さくなる j を求める
      for (j = i - h; j >= 0 && fnCmp(*record, *data[j]) < 0; j -= h) {
        // 右に h 要素をずらす
        data[j + h] = data[j];
      }

      // record を挿入する
      data[j + h] = record;
    }
  }
}
#endif  // NKMM_VECTOR_SMALL

/*!
* 配列リストのソート(選択ソート)
* @param[in] fnCmp 比較関数
* @param[in] offset 開始オフセット
* @param[in] n ソート個数
* 
* @code
* template <class T> static int _fnCompare(const T &a, const T &b)
* {
* 	return b.ZOrder - a.ZOrder;
* }
* @endcode
*/
#ifndef NKMM_VECTOR_SMALL
template <class T>
void Vector<T>::selsort(int (*fnCmp)(const T&, const T&), int offset, int n) {
  int i, j, k;
  T** data = &items_[offset];  // アイテムポインタ

  T* temp;  // テンポラリ

  for (i = 0; i < n - 1; i++) {
    k = i;

    for (j = i + 1; j < n; j++) {
      if (fnCmp(*data[j], *data[k]) < 0) {
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
#endif  // NKMM_VECTOR_SMALL

/*!
* 配列リストのソート(挿入ソート)
* @param[in] fnCmp 比較関数
* @param[in] offset 開始オフセット
* @param[in] n ソート個数
* 
* @code
* template <class T> static int _fnCompare(const T &a, const T &b)
* {
* 	return b.ZOrder - a.ZOrder;
* }
* @endcode
*/
template <class T>
void vector<T>::inssort(int (*fnCmp)(const T&, const T&), int offset, int n) {
  int i, j;
  T** data = &items_[offset];  // アイテムポインタ

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
  for (i = 1; i < n; i++) {
    // 挿入すべきレコード
    T* record = data[i];

    // data[j] が record よりも小さくなる j を求める
    for (j = i - 1; j >= 0 && fnCmp(*record, *data[j]) < 0; j--) {
      // 右に1つ要素をずらす
      data[j + 1] = data[j];
    }

    // record を挿入する
    data[j + 1] = record;
  }
}

/*!
* 配列リストのソート(クイックソート)
* @param[in] fnCmp 比較関数
* @param[in] left ソートする左側の位置
* @param[in] right ソートする右側の位置
* 
* @code
* template <class T> static int _fnCompare(const T &a, const T &b)
* {
* 	return b.ZOrder - a.ZOrder;
* }
* @endcode
*/
template <class T>
void vector<T>::qsort(int (*fnCmp)(const T&, const T&), int left, int right) {
  int i, j;
  T** data = items_;  // アイテムポインタ

  T* temp;   // テンポラリ
  T* pivot;  // 軸とする要素のポインタ

  const int THREASHOLD = 7;  // クイックソートを別のソートに切り替える閾値

  // 終了条件
  if (left > right) {
    return;
  }

  // ある程度の要素数まで減ったら、
  // 無駄なオーバーヘッドを防ぐために、
  // ソート関数を切り替える。
  if (right - left < THREASHOLD) {
    //		return this->bsort  (fnCmp, left, right - left + 1);	// バブルソート
    return this->inssort(fnCmp, left, right - left + 1);  // 挿入ソート
    //		return this->ssort  (fnCmp, left, right - left + 1);	// シェルソート
    //		return this->selsort(fnCmp, left, right - left + 1);	// 選択ソート
  }

  // 中心を軸に設定
  pivot = data[(left + right) / 2];

  // i の初期位置
  i = left - 1;

  // j の初期位置
  j = right + 1;

  for (;;) {
    // 比較関数:fnCmp
    //   右側を軸とし、
    //     戻り値＜０
    //       降順：左から軸より小さい値を探す
    //       昇順：左から軸より大きい値を探す
    //     戻り値＞０
    //       降順：右から軸より大きい値を探す
    //       昇順：右から軸より小さい値を探す
    while (fnCmp(*data[++i], *pivot) < 0) {
    }
    while (fnCmp(*data[--j], *pivot) > 0) {
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
  this->qsort(fnCmp, left, i - 1);

  // 軸より大きいグループにクイックソート
  this->qsort(fnCmp, j + 1, right);
}

}  //nkmm

#if defined(NKMM_VECTOR_SMALL)
#undef NKMM_VECTOR_SMALL
#endif

#endif  //NKMM_VECTOR_H
