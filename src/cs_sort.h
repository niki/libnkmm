//-----------------------------------------------------------------------------
/*!
* @file		mx_sort.h
* @brief	ソート関数
* @author	koma
* @version	$Id$
* 
* @note
* ・更新履歴  
*/
//-----------------------------------------------------------------------------

#ifndef MX_SORT_H
#define MX_SORT_H

//#ifdef __cplusplus
//extern "C" {
//#endif /* __cplusplus */

namespace mx {

void bsort(void *array, int n, int w, int (*fcmp)(const void *, const void *));    // バブルソート
void inssort(void *array, int n, int w, int (*fcmp)(const void *, const void *));  // 挿入ソート
void ssort(void *array, int n, int w, int (*fcmp)(const void *, const void *));    // シェルソート
void selsort(void *array, int n, int w, int (*fcmp)(const void *, const void *));  // 選択ソート
void qsort(void *array, int n, int w, int (*fcmp)(const void *, const void *));    // クイックソート

}  //mx

//#ifdef __cplusplus
//}      /* extern "C" */
//#endif /* __cplusplus */

#endif  //MX_SORT_H
