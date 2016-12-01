//-----------------------------------------------------------------------------
/*!
* @file		mx_w32.cpp
* @brief	Windows �֘A
* @author	koma
* @version	$Id$
* 
* @note
* �E�X�V����  
*/
//-----------------------------------------------------------------------------

/*
�Q�l�F
http://www2u.biglobe.ne.jp/~shinta/cbuilder/_BT_Tips_Ctt.htm#ShortcutwoSakuseisuru
http://www.ne.jp/asahi/protech/hiroaki/programing/cb.html
*/

//#include "stdafx.h"
#include <stdio.h>
//#include <windows.h>
#include <windowsx.h>
#include <shlobj.h>
#include <shlwapi.h>
//#include <tlhelp32.h>
#include <psapi.h>

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "psapi.lib")

// Include
#include "mx_memory.h"
#include "mx_basis.h"
#include "mx_math.h"
#include "mx_strlib.h"
#include "mx_string_list.h"
#include "mx_utils.h"

#include "mx_w32.h"
#include "special_path_name.h"

#define FILE_ATTRIBUTE_NONE 0x00000000
#define FILE_ATTRIBUTE_ALL 0x0000ffff

namespace mx {
namespace win {

//! �t�@�C���񋓏��\����
struct EnumFilesDescription {
  WIN32_FIND_DATA fd;  //!< �񋓏��擾�p
  StringList *files;   //!< �t�@�C�����o�^�p���X�g
  const char *dir;     //!< �����Ώۃt�H���_
  const char *pat;     //!< �����Ώۃp�^�[��
};

#define NEXT_TOKEN(p) (p + mx::strlen(p) + 1)

static int local_SearchFolders(mx::string &path, const mx::string &patIn, const mx::string &patEx,
                               const mx::string &dirIn, const mx::string &dirEx);
static int fnDefaultCallback(const mx::string &fname, void *data);

static int (*s_fnCallback)(const mx::string &, void *) = fnDefaultCallback;
static void *s_data = 0;

/*!
* �f�t�H���g�R�[���o�b�N
* @param[in] fname �t�@�C����
* @param[in] data �f�[�^
* @return 0
*/
static int fnDefaultCallback(const mx::string &fname, void *data) {
  (void)fname;
  (void)data;
  return 0;
}

/*!
* �t�H���_�̗񋓊֐�
* @param[in] fname �t�@�C����
* @param[in] data �f�[�^
* @return 0 �Ƃ肠�����O��Ԃ�
*/
static int fnEnumFoldersCallback(const mx::string &fname, void *data) {
  EnumFilesDescription *ef = (EnumFilesDescription *)data;

  // �t�@�C�������Ȃ��ăt�H���_�����\��
  mx::string folder;

  folder = ef->dir;
  folder += "\\";
  folder += fname;

  // ���X�g�Ƀt�H���_��ǉ�
  mx::string &item = ef->files->basic_Add(fname);
  item.tag(0, SubFolderExists(folder));
  item.tag(1, ef->fd.dwFileAttributes);  // �t�@�C������

  return 0;
}

/*!
* �t�@�C���̗񋓊֐�
* @param[in] fname �t�@�C����
* @param[in] data �f�[�^
* @return 0  �Ƃ肠�����O��Ԃ�
*/
static int fnEnumFilesCallback(const mx::string &fname, void *data) {
  EnumFilesDescription *ef = reinterpret_cast<EnumFilesDescription *>(data);

  // ���X�g�Ƀt�@�C����ǉ�
  mx::string &item = ef->files->basic_Add(fname);
  item.tag(0, ef->fd.nFileSizeLow);      // �t�@�C���T�C�Y
  item.tag(1, ef->fd.dwFileAttributes);  // �t�@�C������

  return 0;
}

/*!
* �t�@�C���̗񋓂��A���X�g���쐬����
* @param[in] func ���s�֐�
* @param[out] files ���X�g���쐬���� StringList �N���X�̃|�C���^
* @param[in] dir �����t�H���_
* @param[in] pat �����p�^�[��
* @param[in] reqattr �v�����ꂽ�擾���
* @param[in] unreqattr �v�����Ȃ��擾���
* @return ��v������
* 
* @note
* FILE_ATTRIBUTE_ARCHIVE		// �A�[�J�C�u�t�@�C��<br>
* FILE_ATTRIBUTE_COMPRESSED	// ���k�t�@�C��<br>
* FILE_ATTRIBUTE_DIRECTORY		// �f�B���N�g��<br>
* FILE_ATTRIBUTE_NORMAL		// ���̑����͐ݒ肳��Ă��܂���<br>
* FILE_ATTRIBUTE_HIDDEN		// �B���t�@�C��<br>
* FILE_ATTRIBUTE_READONLY		// �㏑���֎~<br>
* FILE_ATTRIBUTE_SYSTEM		// �V�X�e���t�@�C��<br>
* FILE_ATTRIBUTE_TEMPORARY		// �ꎞ�L���t�@�C��<br>
*/
static int local_EnumFilesToList(const u_long reqattr, const u_long unreqattr) {
  HANDLE hFind;  // �����p�̃n���h��
  int nFileCount = 0;
  char t_path[256];

  EnumFilesDescription *ef = reinterpret_cast<EnumFilesDescription *>(s_data);

  mx::strcpy(t_path, ef->dir);
  mx::strcat(t_path, "\\");
  mx::strcat(t_path, ef->pat);

  // �ŏ��̃t�@�C��������
  hFind = ::FindFirstFile(t_path, &ef->fd);
  if (hFind == INVALID_HANDLE_VALUE) {
    return 0;  // �t�@�C���͂P�����݂��Ȃ�(���ۂ� "." ".." ������̂Ŏ��s����Ȃ��͂�)
  }

  // �Ō�܂�
  do {
    // "." ".." �͖���
    if (mx::strcmp(ef->fd.cFileName, ".") && mx::strcmp(ef->fd.cFileName, "..")) {
      u_long attr = ef->fd.dwFileAttributes;

      // �v�����ꂽ�����̃t�@�C��������
      // �v�����Ȃ������̃t�@�C���ł͂Ȃ�
      if ((attr & reqattr) && !(attr & unreqattr)) {
        // �v�����ꂽ�����������̂Ńt�@�C���J�E���^�𑝂₷
        nFileCount++;

        // �R�[���o�b�N�֐��i�Ԃ�l�� 1 ��������I���R�[�h�Ȃ̂œr���ł��~�߂�j
        if (s_fnCallback(ef->fd.cFileName, s_data) == 1) {
          break;
        }
      }
    }
  } while (::FindNextFile(hFind, &ef->fd) != NULL);  // �t�@�C���������Ȃ�܂ő�����

  ::FindClose(hFind);  //�n���h�������

  // ��v��������Ԃ�
  return nFileCount;
}

/*!
* �R�[���o�b�N�֐��̐ݒ�
* @param[in] fnCallback �R�[���o�b�N�֐�
* @param[in] data �R�[���o�b�N�֐��ɓn���f�[�^
*/
void SetCallback(int (*fnCallback)(const mx::string &, void *), void *data) {
  if (fnCallback) {
    s_fnCallback = fnCallback;
    s_data = data;

  } else {
    s_fnCallback = fnDefaultCallback;
    s_data = NULL;
  }
}

/*!
* �T�u�t�H���_�̑��݃`�F�b�N(��������)
* @param[in] dir �����t�H���_
* @return 0:�Ȃ�, 1:����
*/
int SubFolderExists(const mx::string &dir) {
  int sub = 0;
  HANDLE hFind;        // �����p�̃n���h��
  WIN32_FIND_DATA fd;  // �񋓏��擾�p
  char t_path[256];

  mx::strcpy(t_path, dir.c_str());
  mx::strcat(t_path, "\\*.*");

  // �ŏ��̃t�@�C��������
  hFind = ::FindFirstFile(t_path, &fd);

  if (hFind == INVALID_HANDLE_VALUE) {
    return 0;  // �t�@�C���͂P�����݂��Ȃ�(���ۂ� "." ".." ������̂Ŏ��s����Ȃ��͂�)
  }

  // �Ō�܂�
  do {
    // "." ".." �͖���
    if (mx::strcmp(fd.cFileName, ".") && mx::strcmp(fd.cFileName, "..")) {
      if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        sub = 1;
        break;
      }
    }
  } while (::FindNextFile(hFind, &fd) != NULL);  // �t�@�C���������Ȃ�܂ő�����

  ::FindClose(hFind);  //�n���h�������

  return sub;
}

/*!
* �t�H���_�̗�
* @param[in] dir �����t�H���_
* @return �o�^���� StringList �N���X�̃|�C���^
* 
* @note
* �g���I�������K�� delete ���Ă��������B<br>
* 
* @code
* StringList *list = EnumFolders( "c:\" );
* 	/// ���� ///
* delete list;
* @endcode
*/
StringList *EnumFolders(const mx::string &dir) {
  EnumFilesDescription ef;

  // �t�@�C���񋓏��\���̐ݒ�
  ef.files = SAFE_NEW(__SOURCE__, StringList);  // �o�^�p StringList �N���X
  ef.dir = dir.c_str();
  ef.pat = "*";

  // �R�[���o�b�N�ݒ�
  SetCallback(fnEnumFoldersCallback, &ef);

  // �t�@�C���̗�
  local_EnumFilesToList(FILE_ATTRIBUTE_DIRECTORY, FILE_ATTRIBUTE_NONE);

  return ef.files;
}

/*!
* �t�@�C���̗�
* @param[in] dir �����t�H���_
* @param[in] mask �����p�^�[��
* @return �o�^���� StringList �N���X�̃|�C���^
* 
* @note
* �g���I�������K�� delete ���Ă��������B<br>
* 
* @code
* StringList *list = EnumFiles( "c:\" );
* 	/// ���� ///
* delete list;
* @endcode
*/
StringList *EnumFiles(const mx::string &dir, const mx::string &mask) {
  int tok_cnt;
  char t_mask[256];
  char *maskP = t_mask;

  EnumFilesDescription ef;

  // �t�@�C���񋓏��\���̐ݒ�
  ef.files = SAFE_NEW(__SOURCE__, StringList);  // �o�^�p StringList �N���X
  ef.dir = dir.c_str();
  ef.pat = "*";

  // �R�[���o�b�N�ݒ�
  SetCallback(fnEnumFilesCallback, &ef);

  // �g�[�N���̔����o��
  tok_cnt = mx::sjis::ExtractTokenWithoutEmpty(t_mask, mask.c_str(), " ,;",
                                               "");  // �g�[�N���� ' ' ',' ';'

  // �t�@�C���̗�
  for (int i = 0; i < tok_cnt; i++) {
    ef.pat = maskP;
    maskP = NEXT_TOKEN(maskP);

    local_EnumFilesToList(FILE_ATTRIBUTE_ALL, FILE_ATTRIBUTE_DIRECTORY);
  }

  return ef.files;
}

/*!
* �t�@�C���̌���
* @param[in] path �����p�X
* @param[in] patIn �����p�^�[��
* @param[in] patEx �������O�p�^�[��
* @param[in] dirIn �����t�H���_�inil �̂Ƃ��̓T�u�t�H���_���������܂���j
* @param[in] dirEx �������O�t�H���_
* @return �q�b�g������
* 
* @note
* dirIn �� nil �̂Ƃ��́A�T�u�t�H���_�̌������s���܂���B<br>
* dirEx ���w�肷��ꍇ�� dirIn �ɂ� "*" �܂��� nil �ȊO�̕���������Ă��������B<br>
*/
static int local_SearchFiles(mx::string &path, const mx::string &patIn, const mx::string &patEx,
                             const mx::string &dirIn, const mx::string &dirEx) {
  int nCount = 0;

  // �͂��߂Ƀt�H���_�`�F�b�N
  if (dirIn.length() > 0) {
    nCount += local_SearchFolders(path, patIn, patEx, dirIn, dirEx);
  }

  WIN32_FIND_DATA fd;  // �񋓏��擾�p
  HANDLE hFind;        // �����p�̃n���h��

  // �p�^�[�����o
  char t_mask[256];
  char *maskP = t_mask;
  int tok_cnt = mx::sjis::ExtractTokenWithoutEmpty(t_mask, patIn.c_str(), " ,;", "");

  for (int i = 0; i < tok_cnt; i++) {
    // �p�^�[�����쐬
    mx::string searchPath = path + "\\" + maskP;

    maskP = NEXT_TOKEN(maskP);

    // �ŏ��̃t�@�C��������
    hFind = ::FindFirstFile(searchPath.c_str(), &fd);

    if (hFind != INVALID_HANDLE_VALUE) {
      // �t�@�C���������Ȃ�܂ő�����
      do {
        // . �� .. �͖�������
        if (mx::strcmp(fd.cFileName, ".") != 0 && mx::strcmp(fd.cFileName, "..") != 0) {
          // �t�H���_���������ׂ�
          if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {

          } else {
            int nCheck = 1;

            // ���O�p�^�[���̃`�F�b�N
            if (patEx.length() > 0) {
              if (mx::sjis::CheckMasks(fd.cFileName, patEx.c_str())) {
                nCheck = 0;
              }
            }

            if (nCheck == 1) {
              // �t�@�C������������
              mx::string s = path + "\\" + fd.cFileName;

              nCount++;

              // �R�[���o�b�N�֐��i�Ԃ�l�� 1 ��������I���R�[�h�Ȃ̂œr���ł��~�߂�j
              if (s_fnCallback(s.c_str(), s_data) == 1) {
                break;
              }
            }
          }
        }
      } while (::FindNextFile(hFind, &fd));

      // �n���h�������
      ::FindClose(hFind);
    }
  }

  return nCount;
}

/*!
* �t�H���_�̌���
* @param[in] path �����p�X
* @param[in] patIn �����p�^�[��
* @param[in] patEx �������O�p�^�[��
* @param[in] dirIn �����t�H���_�inil �̂Ƃ��̓T�u�t�H���_���������܂���j
* @param[in] dirEx �������O�t�H���_
* @return �q�b�g������
*/
static int local_SearchFolders(mx::string &path, const mx::string &patIn, const mx::string &patEx,
                               const mx::string &dirIn, const mx::string &dirEx) {
  int nCount = 0;

  WIN32_FIND_DATA fd;  // �񋓏��擾�p
  HANDLE hFind;        // �����p�̃n���h��

  // �p�^�[�����o
  char t_mask[256];
  char *maskP = t_mask;
  int tok_cnt = mx::sjis::ExtractTokenWithoutEmpty(t_mask, dirIn.c_str(), " ,;", "");

  for (int i = 0; i < tok_cnt; i++) {
    // �p�^�[�����쐬
    mx::string searchPath = path + "\\" + maskP;

    maskP = NEXT_TOKEN(maskP);

    // �ŏ��̃t�@�C��������
    hFind = ::FindFirstFile(searchPath.c_str(), &fd);

    if (hFind != INVALID_HANDLE_VALUE) {
      // �t�@�C���������Ȃ�܂ő�����
      do {
        // . �� .. �͖�������
        if (mx::strcmp(fd.cFileName, ".") != 0 && mx::strcmp(fd.cFileName, "..") != 0) {
          // �t�H���_���������ׂ�
          if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            int nCheck = 1;

            // ���O�p�^�[���̃`�F�b�N
            if (dirEx.length() > 0) {
              if (mx::sjis::CheckMasks(fd.cFileName, dirEx.c_str())) {
                nCheck = 0;
              }
            }

            if (nCheck == 1) {
              // �t�@�C�����쐬
              mx::string folderPath = path + "\\" + fd.cFileName;

              // �T�u�t�H���_����������
              nCount += local_SearchFiles(folderPath, patIn, patEx, dirIn, dirEx);
            }
          }
        }
      } while (::FindNextFile(hFind, &fd));

      // �n���h�������
      ::FindClose(hFind);
    }
  }

  return nCount;
}

/*!
* �t�@�C���̌���
* @param[in] path �����p�X
* @param[in] patIn �����p�^�[��
* @param[in] patEx �������O�p�^�[��
* @param[in] dirIn �����t�H���_�inil �̂Ƃ��̓T�u�t�H���_���������܂���j
* @param[in] dirEx �������O�t�H���_
* @return �q�b�g������
*/
int SearchFiles(const mx::string &path, const mx::string &patIn, const mx::string &patEx,
                const mx::string &dirIn, const mx::string &dirEx) {
  mx::string path_ = path;
  return local_SearchFiles(path_, patIn, patEx, dirIn, dirEx);
}

/*!
* �\������Ă���E�B���h�E�̃N���X�ꗗ�쐬
* @param[in] visibleOnly
* @return StringList*
* 
* @note
* WS_OVERLAPPED           0x00000000      �I�[�o�[���b�v�E�B���h�E���쐬���܂��B�I�[�o�[���b�v�E�B���h�E�̓^�C�g���Ƙg�������܂��B
* WS_TILED
* WS_POPUP                0x80000000      �|�b�v�A�b�v�E�B���h�E���쐬���܂��B���̃X�^�C���́AWS_CHILD �X�^�C���ƈꏏ�ɂ͎g���܂���B
* WS_CHILD                0x40000000      �q�E�B���h�E���쐬���܂��B���̃X�^�C���́AWS_POPUP �X�^�C���ƈꏏ�ɂ͎g���܂���B
* WS_CHILDWINDOW
* WS_MINIMIZE             0x20000000      �E�B���h�E���ŏ����̏�Ԃō쐬���܂��B
* WS_ICONIC
* WS_VISIBLE              0x10000000      ����Ԃ̃E�B���h�E���쐬���܂��B
* WS_DISABLED             0x08000000      ���� (�g�p�s�\) �ȃE�B���h�E���쐬���܂��B�����ȃE�B���h�E�́A�L���ɂ���܂ŁA���[�U�[����̓��͂��󂯎��܂���B
* WS_CLIPSIBLINGS         0x04000000      �Z��֌W�ɂ���q�E�B���h�E���N���b�v���܂��B
* WS_CLIPCHILDREN         0x02000000      �e�E�B���h�E������`�悷��Ƃ��ɁA�q�E�B���h�E����߂�̈�����O���܂��B���̃X�^�C���́A�e�E�B���h�E���쐬����Ƃ��Ɏg���܂��B
* WS_CAPTION              0x00C00000      �^�C�g���o�[�����E�B���h�E���쐬���܂��B�iWS_BORDER | WS_DLGFRAME�j
* WS_BORDER               0x00800000      ���E�������E�B���h�E���쐬���܂��B
* WS_DLGFRAME             0x00400000      �_�C�A���O�{�b�N�X�ň�ʓI�Ɏg����X�^�C���̋��E�����E�B���h�E���쐬���܂��B
* WS_VSCROLL              0x00200000      �����X�N���[���o�[�����E�B���h�E���쐬���܂��B
* WS_HSCROLL              0x00100000      �����X�N���[���o�[�����E�B���h�E���쐬���܂��B
* WS_SYSMENU              0x00080000      �^�C�g���o�[��ɃE�B���h�E���j���[�{�b�N�X�����E�B���h�E���쐬���܂��B
* WS_THICKFRAME           0x00040000      �T�C�Y�ύX���E�����E�B���h�E���쐬���܂��B
* WS_SIZEBOX
* WS_GROUP                0x00020000      �R���g���[���O���[�v�̍ŏ��̃R���g���[�����w�肵�܂��B���̃R���g���[������A���� WS_GROUP �X�^�C���̃R���g���[��������܂łɒ�`���ꂽ�R���g���[�����A�R���g���[���O���[�v�ɂȂ�܂��B
* WS_TABSTOP              0x00010000      ���[�U�[���mTab�n�L�[�������Ɠ��̓t�H�[�J�X���󂯎��R���g���[�����w�肵�܂��B�mTab�n�L�[�������ƁAWS_TABSTOP �X�^�C���������̃R���g���[���ɁA���̓t�H�[�J�X���ړ����܂��B
* WS_MINIMIZEBOX          0x00020000      �ŏ����{�^�������E�B���h�E���쐬���܂��B WS_SYSMENU �X�^�C�����w�肷��K�v������܂��B�g���X�^�C���� WS_EX_CONTEXTHELP ���w�肷�邱�Ƃ͂ł��܂���B
* WS_MAXIMIZEBOX          0x00010000      �ő剻�{�^�������E�B���h�E���쐬���܂��B WS_SYSMENU �X�^�C�����w�肷��K�v������܂��B�g���X�^�C���� WS_EX_CONTEXTHELP ���w�肷�邱�Ƃ͂ł��܂���B
* WS_OVERLAPPEDWINDOW     0x00CF0000      WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX �ɓ������B
* WS_POPUPWINDOW          0x80880000      WS_POPUP | WS_BORDER | WS_SYSMENU �ɓ������B
*/
StringList *EnumWindow(bool visibleOnly) {
  u_long nStyle;

  char szCaption[256];
  char szClassName[256];

  StringList *list = SAFE_NEW(__SOURCE__, StringList);  // new List

  HWND hWnd = ::GetTopWindow(::GetDesktopWindow());

  for (0; hWnd; hWnd = ::GetNextWindow(hWnd, GW_HWNDNEXT)) {
    nStyle = ::GetWindowLong(hWnd, GWL_STYLE);

    // �|�b�v�A�b�v�E�B���h�E�E�q�E�B���h�E�̓_��
    if (nStyle & (WS_POPUP | WS_CHILD)) {
      continue;
    }

    // �s���`�F�b�N
    if (visibleOnly && !::IsWindowVisible(hWnd)) {
      continue;
    }

    ::GetWindowText(hWnd, szCaption, 256);
    ::GetClassName(hWnd, szClassName, 256);

    // ���X�g�ɓ������̂��o�^����Ă��邩�`�F�b�N
    for (int i = list->count() - 1; i >= 0; i--) {
      if (list->Strings[i].tag(0) == (u_long)hWnd) {
        goto _Lnext;
      }
    }

    {
      //mx::string &s = list->Add( "%s@%s,%.8x", szCaption, szClassName, ::GetWindowLong( hWnd, GWL_STYLE ));
      mx::string &s = list->add("%s@%s", szCaption, szClassName);
      s.tag(0, (u_long)hWnd);
      s.tag(1, nStyle);
    }

  _Lnext:;
  }

  return list;
}

/*!
* �v���Z�XID����E�B���h�E�n���h�����擾����
* @param[in] targetID �v���Z�XID
* @param[in] caption �܂܂��^�C�g��
* @return HWND
*/
HWND GetWindowHandleFromProcess(const DWORD targetID, const char *caption) {
  HWND hWnd = ::GetTopWindow(NULL);

  do {
    //if (GetWindowLong(hWnd, GWL_HWNDPARENT) != 0 || !IsWindowVisible(hWnd))
    if (!::IsWindowVisible(hWnd)) {
      continue;
    }

    DWORD processID;
    ::GetWindowThreadProcessId(hWnd, &processID);

    if (targetID == processID) {
      char szCaption[1000];
      ::GetWindowText(hWnd, szCaption, 1000);

      if (mx::sjis::SearchString(szCaption, caption)) {
        return hWnd;
      }
    }
  } while ((hWnd = ::GetNextWindow(hWnd, GW_HWNDNEXT)) != NULL);

  return NULL;
}

/*!
* �v���Z�X���X�g�̎擾
* @param[out] processID �v���Z�XID���i�[����z��
* @param[in] size �z��T�C�Y
* @return int �v���Z�X��
*/
int EnumProcess(DWORD *processID, int size) {
  DWORD ProcessNum;
  DWORD ReturnSize;
  //	PROCESS_MEMORY_COUNTERS MemInfo;

  // �v���Z�XID�̈ꗗ�Ɛ����擾
  ::EnumProcesses(processID, size * sizeof(DWORD), &ReturnSize);
  ProcessNum = ReturnSize / sizeof(DWORD);

  return ProcessNum;
}

/*!
* �v���Z�X�n���h�����烂�W���[�������擾
* @param[in] hProcess
* @param[out] moduleName
*/
void GetModuleName(HANDLE hProcess, char *moduleName) {
  DWORD ReturnSize;
  HMODULE hModule;
  BOOL bResult;

  // �v���Z�X�̃��C�����W���[���̃n���h�����擾
  bResult = ::EnumProcessModules(hProcess, &hModule, sizeof(HMODULE), &ReturnSize);

  if (bResult) {
    // ���C�����W���[���̖��O��\��
    ::GetModuleBaseName(hProcess, hModule, moduleName, 1000);
  }
}

/*!
* �ȈՃt�@�C������
* @param[out] fileList �ǉ�����t�@�C�����X�g
* @param[in] dir �Ώۃf�B���N�g��
* @param[in] mask �}�X�N
* @param[in] excludeDirMask �ΏۊO�f�B���N�g���}�X�N
* @param[in] excludeFileMask �ΏۊO�t�@�C���}�X�N
*/
void DoFind(StringList *fileList, const mx::string &dir, const mx::string &mask,
            const mx::string &excludeDirMask, const mx::string &excludeFileMask) {
  HANDLE hFind;
  WIN32_FIND_DATA fd;

  mx::string dir_ = dir;

  // �ŏ��̃t�@�C������
  //dir_ = dir;
  dir_ = dir_.replace("/", "\\");  // �Z�p���[�^�̒u��

  if (dir_.at(dir_.length() - 1) != '\\') {
    dir_ += "\\";
  }

  dir_ += "*.*";
  hFind = ::FindFirstFile(dir_.c_str(), &fd);

  // �������s�H
  if (hFind == INVALID_HANDLE_VALUE) {
    //printf("error\n");
    return;  /// �G���[�I�� ///
  }

  do {
    // "." ".." �͖���
    if (mx::strcmp(fd.cFileName, ".") != 0 && mx::strcmp(fd.cFileName, "..") != 0) {
      // �f�B���N�g��
      if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        dir_ = dir;

        if (dir_.at(dir_.length() - 1) != '\\') {
          dir_ += "\\";
        }
        dir_ += fd.cFileName;

        // �ċN����
        if (mx::sjis::CheckMasks(fd.cFileName, excludeDirMask.c_str())) {
        } else {
          DoFind(fileList, dir_.c_str(), mask, excludeDirMask, excludeFileMask);
        }

      } else {  // �t�@�C��
        if (mx::sjis::CheckMasks(fd.cFileName, excludeFileMask.c_str())) {
        } else {
          if (mx::sjis::CheckFileMasks(fd.cFileName, mask.c_str())) {
            dir_ = dir;

            if (dir_.at(dir_.length() - 1) != '\\') {
              dir_ += "\\";
            }

            dir_ += fd.cFileName;

            // �t�@�C�����ǉ�
            mx::string &s = fileList->add(dir_.c_str());
            s.tag(0, fd.ftLastWriteTime.dwLowDateTime);
            s.tag(1, fd.ftLastWriteTime.dwHighDateTime);
          }
        }
      }
    }
  } while (::FindNextFile(hFind, &fd));  //���̃t�@�C��������

  // �����I��
  ::FindClose(hFind);
}

/*!
* �t�@�C������
* @param[in] fileList �ǉ�����t�@�C�����X�g
* @param[in] dir �Ώۃf�B���N�g��
* @param[in] prefix �v���t�B�N�X
* @param[in] ext �g���q
* @param[in] subLevel �f�B���N�g���[�x
*/
void DoFind2(StringList *fileList, const mx::string &dir, const mx::string &prefix,
             const mx::string &ext, int subLevel) {
  HANDLE hFind;
  WIN32_FIND_DATA fd;

  mx::string t_dir = dir;

  // �ŏ��̃t�@�C������
  t_dir = dir;
  t_dir += "*.*";
  hFind = ::FindFirstFile(t_dir.c_str(), &fd);

  // �������s�H
  if (hFind == INVALID_HANDLE_VALUE) {
    printf("error\n");
    return;  /// �G���[�I�� ///
  }

  do {
    // "." ".." �͖���
    if (mx::strcmp(fd.cFileName, ".") != 0 && mx::strcmp(fd.cFileName, "..") != 0) {
      // �f�B���N�g��
      if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        if (subLevel > 0) {
          t_dir = dir;
          t_dir = t_dir + fd.cFileName + "\\";

          // �ċN����
          DoFind2(fileList, t_dir.c_str(), prefix, ext, subLevel - 1);
        }

      } else {  // �t�@�C��
        // �w��O
        if (prefix.c_str()[0] == '!') {
          mx::string prefix1 = fd.cFileName;
          mx::string prefix2 = prefix.c_str() + 1;
          mx::string ext1 = mx::string(fd.cFileName).asFileExt();
          mx::string ext2 = ext;

          if (!mx::sjis::SearchString(prefix1.upCase().c_str(), prefix2.upCase().c_str()) &&
              ext1.upCase() == ext2.upCase()) {
            t_dir = dir;
            t_dir = t_dir + fd.cFileName;

            // �t�@�C�����ǉ�
            fileList->add(t_dir.c_str());
          }

        } else {  // �w��
          mx::string prefix1 = fd.cFileName;
          mx::string prefix2 = prefix.c_str() + 1;
          mx::string ext1 = mx::string(fd.cFileName).asFileExt();
          mx::string ext2 = ext;

          if (mx::sjis::SearchString(prefix1.upCase().c_str(), prefix2.upCase().c_str()) &&
              ext1.upCase() == ext2.upCase()) {
            t_dir = dir;
            t_dir = t_dir + fd.cFileName;

            // �t�@�C�����ǉ�
            fileList->add(t_dir.c_str());
          }
        }
      }
    }
  } while (::FindNextFile(hFind, &fd));  //���̃t�@�C��������

  // �����I��
  ::FindClose(hFind);
}

/*!
* ����t�H���_�̃p�X�擾
* @param[in] hWnd �ΏۃE�B���h�E�n���h��
* @param[in] nFolder �t�H���_ID
* @param[out] path �t�H���_
* @return BOOL
* 
* @note
* �֐� SHGetSpecialFolderPath() ���g�p�ł��Ȃ����œ���t�H���_�̃p�X���擾����B<br>
* ������ SHGetSpecialFolderPath() �Ɠ����B<br>
*/
BOOL GetSpecialPath(HWND hWnd, int nFolder, char *path) {
  IMalloc *pMalloc;
  ITEMIDLIST *pidl;

  if (NOERROR == ::SHGetMalloc(&pMalloc)) {
    if (NOERROR == ::SHGetSpecialFolderLocation(hWnd, nFolder, &pidl)) {
      if (::SHGetPathFromIDList(pidl, path)) {
        pMalloc->Free(pidl);
        pMalloc->Release();
        return TRUE;
      }

      pMalloc->Free(pidl);
    }

    pMalloc->Release();
  }

  return FALSE;
}

void SpecialPathToPath(mx::string &str) {
  for (int i = 0; i < s_folderMax; i++) {
    if (str.find(s_folderId[i].Name) >= 0) {
      char path[MAX_PATH];

      // �擾�ł�����u������
      if (GetSpecialPath(NULL, s_folderId[i].Id, path)) {
        str = str.replace(s_folderId[i].Name, path);
        break;
      }
    }
  }
}

void PathToSpecialPath(mx::string &str) {
  for (int i = 0; i < s_folderMax; i++) {
    char path[MAX_PATH];

    if (GetSpecialPath(NULL, s_folderId[i].Id, path)) {
      if (str.find(path) >= 0) {
        str = str.replace(path, s_folderId[i].Name);
        break;
      }
    }
  }
}

/*!
* �V���[�g�J�b�g�̏��擾
* @param[in] link �����N�p�X
* @param[out] path �p�X
* @param[out] work ��ƃf�B���N�g��
* @param[out] param ����
* @param[out] icon �Q�Ƃ���A�C�R���̃p�X
* @return HRESULT
* 
* @note
* GetArguments        �����N��́A�R�}���h���C���������擾����B
* GetDescription      �u�R�����g�v���擾����B
* GetHotkey           �u�V���[�g�J�b�g�L�[�v���擾����B
* GetIconLocation     �\�������A�C�R���̃p�X�{���\�[�XID���擾����B
* GetIDList           �u�����N��v�̃p�X���AITEMIDLIST�œ���B
* GetPath             �u�����N��v�̃p�X�𕶎���œ���B
* GetShowCmd          �u���s���̑傫���v(SW_*)�̐ݒ�𓾂�B
* GetWorkingDirectory �u��ƃt�H���_�v�̃p�X�𓾂�B
* Resolve             �����N�悪�L�邩�ǂ��������B������΃_�C�A���O��\�����ĒT���B�i�󒍁F�H�j
* SetArguments        �����N��̃R�}���h���C��������ݒ�B
* SetDescrition       �u�R�����g�v��ݒ肷��B
* SetHotkey           �u�V���[�g�J�b�g�L�[�v��ݒ肷��B
* SetIconLocation     �\�������A�C�R���̏ꏊ�ƃA�C�R���̃C���f�b�N�X��ݒ肷��B
* SetIDList           �u�����N��v�̃p�X���AITEMIDLIST�Ŏw�肷��B
* SetPath             �u�����N��v�̃p�X���A�ݒ肷��B
* SetRelativePath     �����N��̃p�X���A���΃p�X�Ŏw�肷��B
* SetShowCmd          �u���s���̑傫���v(SW_*)��ݒ肷��B
* SetWorkingDirectory �u��ƃt�H���_�v��ݒ肷��B
*/
HRESULT GetShortcutInfo(LPCSTR link, LPSTR path, LPSTR work, LPSTR param, LPSTR icon) {
  HRESULT hres;
  IShellLink *pShLink;

  WIN32_FIND_DATA wfd;  //�t�@�C����񂪕K�v�ȏꍇ�͊O�ɏo��

  hres = ::CoInitialize(NULL);

  if (SUCCEEDED(hres)) {
    // IShellLink �I�u�W�F�N�g���쐬���|�C���^���擾����
    hres = ::CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink,
                              (void **)&pShLink);

    if (SUCCEEDED(hres)) {
      IPersistFile *ppf;

      // IPersistFile �C���^�[�t�F�C�X�̖₢���킹�������Ȃ�
      hres = pShLink->QueryInterface(IID_IPersistFile, (void **)&ppf);

      if (SUCCEEDED(hres)) {
        wchar_t wsz[MAX_PATH];  // Unicode ������ւ̃o�b�t�@

        // ������ANSI�����ō\�������悤�ɂ���
        MultiByteToWideChar(CP_ACP, 0, link, -1, wsz, MAX_PATH);

        // �V���[�g�J�b�g�����[�h����
        hres = ppf->Load(wsz, STGM_READ);

        if (SUCCEEDED(hres)) {
          //�u�����N��v�̃p�X�𕶎���œ���B
          if (path) {
            hres = pShLink->GetPath(path, MAX_PATH, &wfd, SLGP_UNCPRIORITY);
          }

          //�u��ƃt�H���_�v�̃p�X�𓾂�B
          if (work) {
            hres = pShLink->GetWorkingDirectory(work, MAX_PATH);
          }

          //�����N��́A�R�}���h���C���������擾����B
          if (param) {
            hres = pShLink->GetArguments(param, MAX_PATH);
          }

          //�u���s���̑傫���v(SW_*)�̐ݒ�𓾂�B
          //hres = pShLink->GetShowCmd(int  *piShowCmd);

          //�\�������A�C�R���̃p�X�{���\�[�XID���擾����B
          if (icon) {
            int iconNo;
            hres = pShLink->GetIconLocation(icon, MAX_PATH, &iconNo);
          }

        } else {
          printf("ppf->Load failed.\n");
        }

        // IPersistFile �ւ̃|�C���^���J������
        ppf->Release();

      } else {
        printf("pShLink->QueryInterface failed.\n");
      }

      // IShellLink�ւ̃|�C���^���J������
      pShLink->Release();

    } else {
      printf("CoCreateInstance failed.\n");
    }

    ::CoUninitialize();

  } else {
    printf("CoInitialize failed.\n");
  }
  return hres;
}

void GetShortcutInfoArray(LPCSTR link, ShortcutCommandQue *a, int max) {
  HRESULT hres;
  IShellLink *pShLink;

  WIN32_FIND_DATA wfd;  //�t�@�C����񂪕K�v�ȏꍇ�͊O�ɏo��

  hres = ::CoInitialize(NULL);

  if (SUCCEEDED(hres)) {
    // IShellLink �I�u�W�F�N�g���쐬���|�C���^���擾����
    hres = ::CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink,
                              (void **)&pShLink);

    if (SUCCEEDED(hres)) {
      IPersistFile *ppf;

      // IPersistFile �C���^�[�t�F�C�X�̖₢���킹�������Ȃ�
      hres = pShLink->QueryInterface(IID_IPersistFile, (void **)&ppf);

      if (SUCCEEDED(hres)) {
        wchar_t wsz[MAX_PATH];  // Unicode ������ւ̃o�b�t�@

        // ������ANSI�����ō\�������悤�ɂ���
        MultiByteToWideChar(CP_ACP, 0, link, -1, wsz, MAX_PATH);

        // �V���[�g�J�b�g�����[�h����
        hres = ppf->Load(wsz, STGM_READ);

        if (SUCCEEDED(hres)) {
          for (int i = 0; i < max; i++) {
            ShortcutCommandQue *data = &a[i];

            switch (data->cmd) {
            //�u�����N��v�̃p�X�𕶎���œ���B
            case kShortcutPath:
              hres = pShLink->GetPath(data->buf, data->len, &wfd, SLGP_UNCPRIORITY);
              break;
            //�u��ƃt�H���_�v�̃p�X�𓾂�B
            case kShortcutWorkingDirectory:
              hres = pShLink->GetWorkingDirectory(data->buf, data->len);
              break;
            //�����N��́A�R�}���h���C���������擾����B
            case kShortcutArguments:
              hres = pShLink->GetArguments(data->buf, data->len);
              break;
            //�\�������A�C�R���̃p�X�{���\�[�XID���擾����B
            case kShortcutIcon: {
              int iconNo;
              hres = pShLink->GetIconLocation(data->buf, data->len, &iconNo);
            } break;
            //�u�R�����g�v���擾����B
            case kShortcutDescription:
              hres = pShLink->GetDescription(data->buf, data->len);
              break;
            }
          }
        } else {
          printf("ppf->Load failed.\n");
        }

        // IPersistFile �ւ̃|�C���^���J������
        ppf->Release();
      } else {
        printf("pShLink->QueryInterface failed.\n");
      }

      // IShellLink�ւ̃|�C���^���J������
      pShLink->Release();
    } else {
      printf("CoCreateInstance failed.\n");
    }

    ::CoUninitialize();
  } else {
    printf("CoInitialize failed.\n");
  }
  (void)hres;
}

/*!
* �t�H���_�̑I���_�C�A���O���J���i�R�[���o�b�N�j
* @param[in] hWnd
* @param[in] uMsg
* @param[in] lParam
* @param[in] lpData
* @return int
*/
static int CALLBACK SHBrowseProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData) {
  (void)lParam;

  if (uMsg == BFFM_INITIALIZED && lpData) {
    //	�f�t�H���g�őI��������p�X�̎w��
    ::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, lpData);
  }

  return 0;
}

/*!
* �t�H���_�̑I���_�C�A���O���J��
* @param[in] Handle �n���h��
* @param[out] dirPath �I�������f�B���N�g���p�X
* @param[in] title �^�C�g��
* @param[in] defaultPath �f�t�H���g�p�X
* @return char*
*/
char *GetFolder(HWND Handle, char *dirPath, const char *title, const char *defaultPath) {
  BROWSEINFO BI;
  //	char dirPath[MAX_PATH];
  char FName[MAX_PATH];
  LPITEMIDLIST ItemID;

  mx::Memory::Zero(&BI, sizeof(BROWSEINFO));
  //	mx::Memory::Zero(dirPath, MAX_PATH);
  mx::Memory::Zero(FName, MAX_PATH);

  BI.hwndOwner = Handle;              // �I�[�i�[�E�B���h�E�̃n���h��
  BI.pidlRoot = NULL;                 // ���[�g�t�H���_��ID���X�g
  BI.pszDisplayName = FName;          // �I���t�H���_�̃p�X
  BI.lpszTitle = title;               //"�t�H���_��I�����ĉ�����";	 // �\������
  BI.ulFlags = BIF_RETURNONLYFSDIRS;  // �t�H���_�̂ݑI��
  BI.lpfn = SHBrowseProc;             //nil;			// �R�[���o�b�N�֐�
  BI.lParam = (LPARAM)defaultPath;    // �R�[���o�b�N�֐��ɓn�����l
  BI.iImage = 0;                      // �I���t�H���_�̃C���[�W���X�g�C���f�b�N�X

  ItemID = ::SHBrowseForFolder(&BI);
  ::SHGetPathFromIDList(ItemID, dirPath);
  GlobalFreePtr(ItemID);

  return dirPath;
}

/*!
* �V���[�g�J�b�g�̍쐬
* @param[in] name �����N��
* @param[in] dir �����N�쐬�f�B���N�g��
* @param[in] fileName �����N��t�@�C���p�X
* @param[in] param �p�����[�^
* @return bool
*/
bool CreateShortcut(const char *name, const char *dir, const char *fileName, const char *param) {
  mx::string sTarget;    //�N�����������A�v���P�[�V�����̃t�@�C����
  mx::string sCmdParam;  //�A�v���P�[�V�����ɓn���p�����[�^
  mx::string sSCFName;   //�V���[�g�J�b�g���̂��̂̃t�@�C����
  wchar_t wszSCFName[MAX_PATH];
  long hRes;
  IPersistFile *xPF;
  IShellLink *xSL;

  sTarget = fileName;
  sCmdParam = param;  //�R�}���h���C���p�����[�^

  mx::string s = dir;
  SpecialPathToPath(s);
  s += "\\";  //�V���[�g�J�b�g���̂��̂̃t�@�C����
  s += name;
  s += ".lnk";
  sSCFName = s;
  ::CoInitialize(NULL);

  if (SUCCEEDED(::CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink,
                                   (void **)&xSL))) {
    hRes = xSL->QueryInterface(IID_IPersistFile, (void **)&xPF);

    if (SUCCEEDED(hRes)) {
      xSL->SetPath(sTarget.c_str());
      xSL->SetArguments(sCmdParam.c_str());
      xSL->SetWorkingDirectory(sTarget.asFileDir());
      xSL->SetDescription("" /*�����ɃR�����g�������*/);
      MultiByteToWideChar(CP_ACP, 0, sSCFName.c_str(), -1, wszSCFName, MAX_PATH);
      xPF->Save(wszSCFName, TRUE);
      xPF->Release();
    }

    xSL->Release();
  }

  ::CoUninitialize();

  return true;
}

/*!
* �t�@�C���̑��݊m�F
* @param[in] fileName �t�@�C���p�X
* @return bool
*/
bool FileExists(const char *fileName) {
  FILE *fp;

  if ((fp = fopen(fileName, "rb")) != NULL) {
    fclose(fp);
    return true;
  }

  return false;
}

/*!
* �g���q�Ɋ֘A�t����ꂽ���s�t�@�C���擾
* @param[in] strExtension �g���q
* @return strCommand ���s�t�@�C����
* 
* @note
* strExtension��".txt"�̂悤�ɂ���i"txt"�ł��\��Ȃ��j
* ".bat"���擾�����"%1"�ɂȂ�Ȃǎ��ۂ̃R�}���h�ƂȂ�Ȃ����Ƃ�����I
*/
mx::string GetExecutable(const mx::string &strExtension) {
  DWORD dwOut;
  HRESULT hr;
  TCHAR fname[MAX_PATH + 10];
  mx::string ext;

  mx::string strCommand = "";

  if (strExtension.empty()) {
    return strCommand;
  }

  ext = strExtension;

  if (ext.at(0) != '.') {
    ext = mx::string(".") + ext;
  }

  if (ext.length() == 1) {
    return strCommand;
  }

  dwOut = MAX_PATH + 10;
  hr = ::AssocQueryString(ASSOCF_NOTRUNCATE, ASSOCSTR_EXECUTABLE, ext.c_str(), "open", fname,
                          &dwOut);

  if (FAILED(hr) || dwOut == 0) {
    return strCommand;
  }

  strCommand = fname;

  return strCommand;
}

}  //win
}  //mx
