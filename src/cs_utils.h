//-----------------------------------------------------------------------------
/*!
* @file		mx_utils.h
* @brief	
* @author	koma
* @version	$Id$
* 
* @note
* �E�X�V����  
*/
//-----------------------------------------------------------------------------
#ifndef MX_UTILS_H
#define MX_UTILS_H

namespace mx {

//class String;
class StringList;

namespace utils {

/*!
* ���X�g����T���ĕ⊮����
* @param[in] stringList ��⃊�X�g
* @param[in] s1 �⊮����
* @return �⊮��Ώۂ̃C���f�b�N�X
*/
int AutoCompleteString(const StringList* stringList, const mx::string& s1);

/*!
* �g�[�N���؂�o��(�����̋�ؕ����Ή�)
* @param[out] StringList �N���X
* @param[in] src �؂�o���Ώە�����
* @param[in] delimiters �f���~�^���Q
* @param[in] ignores �������Q
* 
* @note
* �ȉ��̏ꍇ�A���p�X�y�[�X�ƃ^�u���Ώ� <br>
* 
* @code
* ExtractTokenToList( stringList, src, " \t", "" );
* @endcode
*/
void ExtractTokenToList(StringList* stringList, const mx::string& src, const mx::string& delimiters,
                        const mx::string& ignores);

/*!
* ���X�g���Ɋ܂܂��R�����g���X�y�[�X�ɂ���
* @param[out] stringList ��菜�� StringList �N���X
* @param[in] lines �s�R�����g
* @param[in] blockBegin �u���b�N�R�����g�J�n
* @param[in] blockEnd �u���b�N�R�����g�I��
* @param[in] ignoreString ������͖�������("", '' �𕶎���Ƃ���)
* 
* @note
* �R�����g�̑ΏۂƂȂ镶���͂��ׂĔ��p�X�y�[�X�ɒu���������܂��B<br>
* ignoreString �� true ���w�肵���ꍇ�͕�������̃R�����g�͑ΏۊO�ł��B<br>
* ������� '' "" �ň͂܂ꂽ�����ł��B<br>
* <br>
* �u���b�N�R�����g�͊J�n���ݒ肳��Ă���Ƃ��̂ݔ��� <br>
* �J�n���w�肳��Ă��ďI�����w�肳��Ă��Ȃ��Ƃ��͒m��܂��� <br>
* <br>
* �R�����g�̃l�X�g�ɂ��Ή� <br>
*/
void CommentToSpace(StringList* stringList, const mx::string& lines,
                    const mx::string& blockBegin = 0, const mx::string& blockEnd = 0,
                    bool ignoreString = false);

void CommentToSpace(char* wp, const char* lines, const char* blockBegin = 0,
                    const char* blockEnd = 0, bool ignoreString = false);

/*!
* �o�b�t�@���g�p���������t������쐬
* @param[in,out] buf �o�b�t�@
* @param[in] fmt... ����
* @return char*
*/
char* sprintf(char* buf, const char* fmt, ...);

/*!
* �o�b�t�@���g�p���������t������쐬
* @param[out] buf �o�b�t�@
* @param[in] fmt �����t������E�ϒ��������܂ރ|�C���^�̃|�C���^
* @return char*
*/
char* svprintf(char* buf, const char** fmt);

}  //utils
}  //mx

#endif  //MX_UTILS_H
