// -*- mode:c++; coding:utf-8-ws -*-
#ifndef NKMM_SORT_H
#define NKMM_SORT_H

//#ifdef __cplusplus
//extern "C" {
//#endif /* __cplusplus */

namespace nkmm {

void bsort(void *array, int n, int w, int (*fcmp)(const void *, const void *));  // バブルソート
void inssort(void *array, int n, int w, int (*fcmp)(const void *, const void *));  // 挿入ソート
void ssort(void *array, int n, int w, int (*fcmp)(const void *, const void *));  // シェルソート
void selsort(void *array, int n, int w, int (*fcmp)(const void *, const void *));  // 選択ソート
void qsort(void *array, int n, int w, int (*fcmp)(const void *, const void *));  // クイックソート

}  //nkmm

//#ifdef __cplusplus
//}      /* extern "C" */
//#endif /* __cplusplus */

#endif  //NKMM_SORT_H
