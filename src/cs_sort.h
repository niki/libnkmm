//-----------------------------------------------------------------------------
/*!
* @file		mx_sort.h
* @brief	�\�[�g�֐�
* @author	koma
* @version	$Id$
* 
* @note
* �E�X�V����  
*/
//-----------------------------------------------------------------------------

#ifndef MX_SORT_H
#define MX_SORT_H

//#ifdef __cplusplus
//extern "C" {
//#endif /* __cplusplus */

namespace mx {

void bsort(void *array, int n, int w, int (*fcmp)(const void *, const void *));    // �o�u���\�[�g
void inssort(void *array, int n, int w, int (*fcmp)(const void *, const void *));  // �}���\�[�g
void ssort(void *array, int n, int w, int (*fcmp)(const void *, const void *));    // �V�F���\�[�g
void selsort(void *array, int n, int w, int (*fcmp)(const void *, const void *));  // �I���\�[�g
void qsort(void *array, int n, int w, int (*fcmp)(const void *, const void *));    // �N�C�b�N�\�[�g

}  //mx

//#ifdef __cplusplus
//}      /* extern "C" */
//#endif /* __cplusplus */

#endif  //MX_SORT_H
