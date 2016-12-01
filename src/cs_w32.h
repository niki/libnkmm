//-----------------------------------------------------------------------------
/*!
* @file		mx_w32.h
* @brief	Windows �֘A
* @author	koma
* @version	$Id$
* 
* @note
* �E�X�V����  
*/
//-----------------------------------------------------------------------------

#ifndef MX_W32_H
#define MX_W32_H

/*
SetDwrod �ɐݒ肳��郂�m

�t�H���_
0:�T�u�t�H���_�̗L��
1:����

�t�@�C��
0:�t�@�C���T�C�Y
1:����
*/

namespace mx {

//class String;
class StringList;

namespace win {

enum {
  kShortcutDummy = 0,
  kShortcutPath,
  kShortcutWorkingDirectory,
  kShortcutArguments,
  kShortcutIcon,
  kShortcutDescription,
};

struct ShortcutCommandQue {
  int cmd;
  char* buf;
  int len;
};

// EnumFolders
// EnumFiles
// SearchFiles
// �Ńt�H���_�E�t�@�C���P�ʂŌĂ΂��R�[���o�b�N�֐���ݒ�
void SetCallback(int (*fnCallback)(const mx::string&, void*), void* data);

/*!
* �T�u�t�H���_�̑��݃`�F�b�N(��������)
* @param[in] dir �����t�H���_
* @return 0:�Ȃ�, 1:����
*/
int SubFolderExists(const mx::string& dir);

/*!
* �t�H���_�̗�
* @param[in] dir �����t�H���_
* @return �o�^���� StringList �N���X�̃|�C���^
*/
StringList* EnumFolders(const mx::string& dir);

/*!
* �t�@�C���̗�
* @param[in] dir �����t�H���_
* @param[in] mask �����p�^�[��
* @return �o�^���� StringList �N���X�̃|�C���^
*/
StringList* EnumFiles(const mx::string& dir, const mx::string& mask);

/*!
* �t�@�C���̌���
* @param[in] path �����p�X
* @param[in] patIn �����p�^�[��
* @param[in] patEx �������O�p�^�[��
* @param[in] dirIn �����t�H���_�inil �̂Ƃ��̓T�u�t�H���_���������܂���j
* @param[in] dirEx �������O�t�H���_
* @return �q�b�g������
*/
int SearchFiles(const mx::string& path, const mx::string& patIn, const mx::string& patEx,
                const mx::string& dirIn, const mx::string& dirEx);

/*!
* �\������Ă���E�B���h�E�̃N���X�ꗗ�쐬
* @param[in] visibleOnly
* @return StringList*
*/
StringList* EnumWindow(bool visibleOnly);

/*!
* �v���Z�XID����E�B���h�E�n���h�����擾����
* @param[in] targetID �v���Z�XID
* @param[in] caption �܂܂��^�C�g��
* @return HWND
*/
HWND GetWindowHandleFromProcess(const DWORD targetID, const char* caption);

/*!
* �v���Z�X���X�g�̎擾
* @param[out] processID �v���Z�XID���i�[����z��
* @param[in] size �z��T�C�Y
* @return int �v���Z�X��
*/
int EnumProcess(DWORD* processID, int size);

/*!
* �v���Z�X�n���h�����烂�W���[�������擾
* @param[in] hProcess
* @param[out] moduleName
*/
void GetModuleName(HANDLE hProcess, char* moduleName);

/*!
* �ȈՃt�@�C������
* @param[out] fileList �ǉ�����t�@�C�����X�g
* @param[in] dir �Ώۃf�B���N�g��
* @param[in] mask �}�X�N
* @param[in] excludeDirMask �ΏۊO�f�B���N�g���}�X�N
* @param[in] excludeFileMask �ΏۊO�t�@�C���}�X�N
*/
void DoFind(StringList* fileList, const mx::string& dir, const mx::string& mask,
            const mx::string& excludeDirMask, const mx::string& excludeFileMask);

/*!
* �t�@�C������
* @param[in] fileList �ǉ�����t�@�C�����X�g
* @param[in] dir �Ώۃf�B���N�g��
* @param[in] prefix �v���t�B�N�X
* @param[in] ext �g���q
* @param[in] subLevel �f�B���N�g���[�x
*/
void DoFind2(StringList* fileList, const mx::string& dir, const mx::string& prefix,
             const mx::string& ext, int subLevel);

/*!
* ����t�H���_�̃p�X�擾
* @param[in] hWnd �ΏۃE�B���h�E�n���h��
* @param[in] nFolder �t�H���_ID
* @param[out] path �t�H���_
* @return BOOL
*/
BOOL GetSpecialPath(HWND hWnd, int nFolder, char* Path);
void SpecialPathToPath(mx::string& str);
void PathToSpecialPath(mx::string& str);

/*!
* �V���[�g�J�b�g�̏��擾
* @param[in] link �����N�p�X
* @param[out] path �p�X
* @param[out] work ��ƃf�B���N�g��
* @param[out] param ����
* @param[out] icon �Q�Ƃ���A�C�R���̃p�X
* @return HRESULT
*/
HRESULT GetShortcutInfo(LPCSTR link, LPSTR path, LPSTR work, LPSTR param, LPSTR icon);
void GetShortcutInfoArray(LPCSTR link, ShortcutCommandQue* a, int max);

/*!
* �t�H���_�̑I���_�C�A���O���J��
* @param[in] Handle �n���h��
* @param[out] dirPath �I�������f�B���N�g���p�X
* @param[in] title �^�C�g��
* @param[in] defaultPath �f�t�H���g�p�X
* @return char*
*/
char* GetFolder(HWND Handle, char* dirPath, const char* title, const char* defaultPath = 0);

/*!
* �V���[�g�J�b�g�̍쐬
* @param[in] name �����N��
* @param[in] dir �����N�쐬�f�B���N�g��
* @param[in] fileName �����N��t�@�C���p�X
* @param[in] param �p�����[�^
* @return bool
*/
bool CreateShortcut(const char* name, const char* dir, const char* fileName, const char* param);

/*!
* �t�@�C���̑��݊m�F
* @param[in] fileName �t�@�C���p�X
* @return bool
*/
bool FileExists(const char* fileName);

/*!
* �g���q�Ɋ֘A�t����ꂽ���s�t�@�C���擾
* @param[in] strExtension �g���q
* @return strCommand ���s�t�@�C����
*/
mx::string GetExecutable(const mx::string& strExtension);

}  //win
}  //mx

#endif  //MX_W32_H
