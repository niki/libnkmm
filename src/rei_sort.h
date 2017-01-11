// -*- mode:c++; coding:utf-8-ws -*-
//====================================================================
//! @file  rei_sort.h
//! @brief ソート関数
//!
//! @author
//====================================================================
#ifndef REI_SORT_H
#define REI_SORT_H

//#ifdef __cplusplus
//extern "C" {
//#endif /* __cplusplus */

namespace rei {

void bsort(void *array, int n, int w, int (*fcmp)(const void *, const void *));  // バブルソート
void inssort(void *array, int n, int w, int (*fcmp)(const void *, const void *));  // 挿入ソート
void ssort(void *array, int n, int w, int (*fcmp)(const void *, const void *));  // シェルソート
void selsort(void *array, int n, int w, int (*fcmp)(const void *, const void *));  // 選択ソート
void qsort(void *array, int n, int w, int (*fcmp)(const void *, const void *));  // クイックソート

}  //rei

//#ifdef __cplusplus
//}      /* extern "C" */
//#endif /* __cplusplus */

#endif  //REI_SORT_H
