// -*- mode:c++; coding:utf-8-ws -*-
/*
参考：
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
#include "nkmm_memory.h"
#include "nkmm_basis.h"
#include "nkmm_math.h"
#include "nkmm_strlib.h"
#include "nkmm_string_list.h"
#include "nkmm_utils.h"

#include "nkmm_w32.h"
#include "special_path_name.h"

#define FILE_ATTRIBUTE_NONE 0x00000000
#define FILE_ATTRIBUTE_ALL 0x0000ffff

namespace nkmm {
namespace win {

//! ファイル列挙情報構造体
struct EnumFilesDescription {
  WIN32_FIND_DATA fd;  //!< 列挙情報取得用
  StringList *files;   //!< ファイル名登録用リスト
  const char *dir;     //!< 検索対象フォルダ
  const char *pat;     //!< 検索対象パターン
};

#define NEXT_TOKEN(p) (p + nkmm::strlen(p) + 1)

static int local_SearchFolders(nkmm::string &path, const nkmm::string &patIn,
                               const nkmm::string &patEx, const nkmm::string &dirIn,
                               const nkmm::string &dirEx);
static int fnDefaultCallback(const nkmm::string &fname, void *data);

static int (*s_fnCallback)(const nkmm::string &, void *) = fnDefaultCallback;
static void *s_data = 0;

/*!
* デフォルトコールバック
* @param[in] fname ファイル名
* @param[in] data データ
* @return 0
*/
static int fnDefaultCallback(const nkmm::string &fname, void *data) {
  (void)fname;
  (void)data;
  return 0;
}

/*!
* フォルダの列挙関数
* @param[in] fname ファイル名
* @param[in] data データ
* @return 0 とりあえず０を返す
*/
static int fnEnumFoldersCallback(const nkmm::string &fname, void *data) {
  EnumFilesDescription *ef = (EnumFilesDescription *)data;

  // ファイル名をつなげてフォルダ名を構成
  nkmm::string folder;

  folder = ef->dir;
  folder += "\\";
  folder += fname;

  // リストにフォルダを追加
  nkmm::string &item = ef->files->basic_Add(fname);
  item.tag(0, SubFolderExists(folder));
  item.tag(1, ef->fd.dwFileAttributes);  // ファイル属性

  return 0;
}

/*!
* ファイルの列挙関数
* @param[in] fname ファイル名
* @param[in] data データ
* @return 0  とりあえず０を返す
*/
static int fnEnumFilesCallback(const nkmm::string &fname, void *data) {
  EnumFilesDescription *ef = reinterpret_cast<EnumFilesDescription *>(data);

  // リストにファイルを追加
  nkmm::string &item = ef->files->basic_Add(fname);
  item.tag(0, ef->fd.nFileSizeLow);      // ファイルサイズ
  item.tag(1, ef->fd.dwFileAttributes);  // ファイル属性

  return 0;
}

/*!
* ファイルの列挙し、リストを作成する
* @param[in] func 実行関数
* @param[out] files リストを作成する StringList クラスのポインタ
* @param[in] dir 検索フォルダ
* @param[in] pat 検索パターン
* @param[in] reqattr 要求された取得情報
* @param[in] unreqattr 要求しない取得情報
* @return 一致した個数
* 
* @note
* FILE_ATTRIBUTE_ARCHIVE		// アーカイブファイル<br>
* FILE_ATTRIBUTE_COMPRESSED	// 圧縮ファイル<br>
* FILE_ATTRIBUTE_DIRECTORY		// ディレクトリ<br>
* FILE_ATTRIBUTE_NORMAL		// 他の属性は設定されていません<br>
* FILE_ATTRIBUTE_HIDDEN		// 隠しファイル<br>
* FILE_ATTRIBUTE_READONLY		// 上書き禁止<br>
* FILE_ATTRIBUTE_SYSTEM		// システムファイル<br>
* FILE_ATTRIBUTE_TEMPORARY		// 一時記憶ファイル<br>
*/
static int local_EnumFilesToList(const u_long reqattr, const u_long unreqattr) {
  HANDLE hFind;  // 検索用のハンドル
  int nFileCount = 0;
  char t_path[256];

  EnumFilesDescription *ef = reinterpret_cast<EnumFilesDescription *>(s_data);

  nkmm::strcpy(t_path, ef->dir);
  nkmm::strcat(t_path, "\\");
  nkmm::strcat(t_path, ef->pat);

  // 最初のファイルを検索
  hFind = ::FindFirstFile(t_path, &ef->fd);
  if (hFind == INVALID_HANDLE_VALUE) {
    return 0;  // ファイルは１個も存在しない(実際は "." ".." があるので実行されないはず)
  }

  // 最後まで
  do {
    // "." ".." は無視
    if (nkmm::strcmp(ef->fd.cFileName, ".") && nkmm::strcmp(ef->fd.cFileName, "..")) {
      u_long attr = ef->fd.dwFileAttributes;

      // 要求された属性のファイルだった
      // 要求しない属性のファイルではない
      if ((attr & reqattr) && !(attr & unreqattr)) {
        // 要求された属性だったのでファイルカウンタを増やす
        nFileCount++;

        // コールバック関数（返り値が 1 だったら終了コードなので途中でも止める）
        if (s_fnCallback(ef->fd.cFileName, s_data) == 1) {
          break;
        }
      }
    }
  } while (::FindNextFile(hFind, &ef->fd) != NULL);  // ファイルが無くなるまで続ける

  ::FindClose(hFind);  //ハンドルを閉じる

  // 一致した個数を返す
  return nFileCount;
}

/*!
* コールバック関数の設定
* @param[in] fnCallback コールバック関数
* @param[in] data コールバック関数に渡すデータ
*/
void SetCallback(int (*fnCallback)(const nkmm::string &, void *), void *data) {
  if (fnCallback) {
    s_fnCallback = fnCallback;
    s_data = data;

  } else {
    s_fnCallback = fnDefaultCallback;
    s_data = NULL;
  }
}

/*!
* サブフォルダの存在チェック(内部処理)
* @param[in] dir 検索フォルダ
* @return 0:なし, 1:あり
*/
int SubFolderExists(const nkmm::string &dir) {
  int sub = 0;
  HANDLE hFind;        // 検索用のハンドル
  WIN32_FIND_DATA fd;  // 列挙情報取得用
  char t_path[256];

  nkmm::strcpy(t_path, dir.c_str());
  nkmm::strcat(t_path, "\\*.*");

  // 最初のファイルを検索
  hFind = ::FindFirstFile(t_path, &fd);

  if (hFind == INVALID_HANDLE_VALUE) {
    return 0;  // ファイルは１個も存在しない(実際は "." ".." があるので実行されないはず)
  }

  // 最後まで
  do {
    // "." ".." は無視
    if (nkmm::strcmp(fd.cFileName, ".") && reireirei::strcmp(fd.cFileName, "..")) {
      if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        sub = 1;
        break;
      }
    }
  } while (::FindNextFile(hFind, &fd) != NULL);  // ファイルが無くなるまで続ける

  ::FindClose(hFind);  //ハンドルを閉じる

  return sub;
}

/*!
* フォルダの列挙
* @param[in] dir 検索フォルダ
* @return 登録した StringList クラスのポインタ
* 
* @note
* 使い終わったら必ず delete してください。<br>
* 
* @code
* StringList *list = EnumFolders( "c:\" );
* 	/// 処理 ///
* delete list;
* @endcode
*/
StringList *EnumFolders(const nkmm::string &dir) {
  EnumFilesDescription ef;

  // ファイル列挙情報構造体設定
  ef.files = SAFE_NEW(__SOURCE__, StringList);  // 登録用 StringList クラス
  ef.dir = dir.c_str();
  ef.pat = "*";

  // コールバック設定
  SetCallback(fnEnumFoldersCallback, &ef);

  // ファイルの列挙
  local_EnumFilesToList(FILE_ATTRIBUTE_DIRECTORY, FILE_ATTRIBUTE_NONE);

  return ef.files;
}

/*!
* ファイルの列挙
* @param[in] dir 検索フォルダ
* @param[in] mask 検索パターン
* @return 登録した StringList クラスのポインタ
* 
* @note
* 使い終わったら必ず delete してください。<br>
* 
* @code
* StringList *list = EnumFiles( "c:\" );
* 	/// 処理 ///
* delete list;
* @endcode
*/
StringList *EnumFiles(const nkmm::string &dir, const nkmm::string &mask) {
  int tok_cnt;
  char t_mask[256];
  char *maskP = t_mask;

  EnumFilesDescription ef;

  // ファイル列挙情報構造体設定
  ef.files = SAFE_NEW(__SOURCE__, StringList);  // 登録用 StringList クラス
  ef.dir = dir.c_str();
  ef.pat = "*";

  // コールバック設定
  SetCallback(fnEnumFilesCallback, &ef);

  // トークンの抜き出し
  tok_cnt = nkmm::sjis::ExtractTokenWithoutEmpty(t_mask, mask.c_str(), " ,;",
                                                "");  // トークンは ' ' ',' ';'

  // ファイルの列挙
  for (int i = 0; i < tok_cnt; i++) {
    ef.pat = maskP;
    maskP = NEXT_TOKEN(maskP);

    local_EnumFilesToList(FILE_ATTRIBUTE_ALL, FILE_ATTRIBUTE_DIRECTORY);
  }

  return ef.files;
}

/*!
* ファイルの検索
* @param[in] path 検索パス
* @param[in] patIn 検索パターン
* @param[in] patEx 検索除外パターン
* @param[in] dirIn 検索フォルダ（nil のときはサブフォルダを検索しません）
* @param[in] dirEx 検索除外フォルダ
* @return ヒットした数
* 
* @note
* dirIn が nil のときは、サブフォルダの検索を行いません。<br>
* dirEx を指定する場合は dirIn には "*" または nil 以外の文字列を入れてください。<br>
*/
static int local_SearchFiles(nkmm::string &path, const nkmm::string &patIn, const nkmm::string &patEx,
                             const nkmm::string &dirIn, const nkmm::string &dirEx) {
  int nCount = 0;

  // はじめにフォルダチェック
  if (dirIn.length() > 0) {
    nCount += local_SearchFolders(path, patIn, patEx, dirIn, dirEx);
  }

  WIN32_FIND_DATA fd;  // 列挙情報取得用
  HANDLE hFind;        // 検索用のハンドル

  // パターン抽出
  char t_mask[256];
  char *maskP = t_mask;
  int tok_cnt = nkmm::sjis::ExtractTokenWithoutEmpty(t_mask, patIn.c_str(), " ,;", "");

  for (int i = 0; i < tok_cnt; i++) {
    // パターン名作成
    nkmm::string searchPath = path + "\\" + maskP;

    maskP = NEXT_TOKEN(maskP);

    // 最初のファイルを検索
    hFind = ::FindFirstFile(searchPath.c_str(), &fd);

    if (hFind != INVALID_HANDLE_VALUE) {
      // ファイルが無くなるまで続ける
      do {
        // . と .. は無視する
        if (nkmm::strcmp(fd.cFileName, ".") != 0 && nkmm::strcmp(fd.cFileName, "..") != 0) {
          // フォルダ属性か調べる
          if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {

          } else {
            int nCheck = 1;

            // 除外パターンのチェック
            if (patEx.length() > 0) {
              if (nkmm::sjis::CheckMasks(fd.cFileName, patEx.c_str())) {
                nCheck = 0;
              }
            }

            if (nCheck == 1) {
              // ファイル名を加える
              nkmm::string s = path + "\\" + fd.cFileName;

              nCount++;

              // コールバック関数（返り値が 1 だったら終了コードなので途中でも止める）
              if (s_fnCallback(s.c_str(), s_data) == 1) {
                break;
              }
            }
          }
        }
      } while (::FindNextFile(hFind, &fd));

      // ハンドルを閉じる
      ::FindClose(hFind);
    }
  }

  return nCount;
}

/*!
* フォルダの検索
* @param[in] path 検索パス
* @param[in] patIn 検索パターン
* @param[in] patEx 検索除外パターン
* @param[in] dirIn 検索フォルダ（nil のときはサブフォルダを検索しません）
* @param[in] dirEx 検索除外フォルダ
* @return ヒットした数
*/
static int local_SearchFolders(nkmm::string &path, const nkmm::string &patIn,
                               const nkmm::string &patEx, const nkmm::string &dirIn,
                               const nkmm::string &dirEx) {
  int nCount = 0;

  WIN32_FIND_DATA fd;  // 列挙情報取得用
  HANDLE hFind;        // 検索用のハンドル

  // パターン抽出
  char t_mask[256];
  char *maskP = t_mask;
  int tok_cnt = nkmm::sjis::ExtractTokenWithoutEmpty(t_mask, dirIn.c_str(), " ,;", "");

  for (int i = 0; i < tok_cnt; i++) {
    // パターン名作成
    nkmm::string searchPath = path + "\\" + maskP;

    maskP = NEXT_TOKEN(maskP);

    // 最初のファイルを検索
    hFind = ::FindFirstFile(searchPath.c_str(), &fd);

    if (hFind != INVALID_HANDLE_VALUE) {
      // ファイルが無くなるまで続ける
      do {
        // . と .. は無視する
        if (nkmm::strcmp(fd.cFileName, ".") != 0 && nkmm::strcmp(fd.cFileName, "..") != 0) {
          // フォルダ属性か調べる
          if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            int nCheck = 1;

            // 除外パターンのチェック
            if (dirEx.length() > 0) {
              if (nkmm::sjis::CheckMasks(fd.cFileName, dirEx.c_str())) {
                nCheck = 0;
              }
            }

            if (nCheck == 1) {
              // ファイル名作成
              nkmm::string folderPath = path + "\\" + fd.cFileName;

              // サブフォルダを検索する
              nCount += local_SearchFiles(folderPath, patIn, patEx, dirIn, dirEx);
            }
          }
        }
      } while (::FindNextFile(hFind, &fd));

      // ハンドルを閉じる
      ::FindClose(hFind);
    }
  }

  return nCount;
}

/*!
* ファイルの検索
* @param[in] path 検索パス
* @param[in] patIn 検索パターン
* @param[in] patEx 検索除外パターン
* @param[in] dirIn 検索フォルダ（nil のときはサブフォルダを検索しません）
* @param[in] dirEx 検索除外フォルダ
* @return ヒットした数
*/
int SearchFiles(const nkmm::string &path, const nkmm::string &patIn, const nkmm::string &patEx,
                const nkmm::string &dirIn, const nkmm::string &dirEx) {
  nkmm::string path_ = path;
  return local_SearchFiles(path_, patIn, patEx, dirIn, dirEx);
}

/*!
* 表示されているウィンドウのクラス一覧作成
* @param[in] visibleOnly
* @return StringList*
* 
* @note
* WS_OVERLAPPED           0x00000000      オーバーラップウィンドウを作成します。オーバーラップウィンドウはタイトルと枠を持ちます。
* WS_TILED
* WS_POPUP                0x80000000      ポップアップウィンドウを作成します。このスタイルは、WS_CHILD スタイルと一緒には使えません。
* WS_CHILD                0x40000000      子ウィンドウを作成します。このスタイルは、WS_POPUP スタイルと一緒には使えません。
* WS_CHILDWINDOW
* WS_MINIMIZE             0x20000000      ウィンドウを最小化の状態で作成します。
* WS_ICONIC
* WS_VISIBLE              0x10000000      可視状態のウィンドウを作成します。
* WS_DISABLED             0x08000000      無効 (使用不能) なウィンドウを作成します。無効なウィンドウは、有効にするまで、ユーザーからの入力を受け取りません。
* WS_CLIPSIBLINGS         0x04000000      兄弟関係にある子ウィンドウをクリップします。
* WS_CLIPCHILDREN         0x02000000      親ウィンドウ内部を描画するときに、子ウィンドウが占める領域を除外します。このスタイルは、親ウィンドウを作成するときに使います。
* WS_CAPTION              0x00C00000      タイトルバーを持つウィンドウを作成します。（WS_BORDER | WS_DLGFRAME）
* WS_BORDER               0x00800000      境界線を持つウィンドウを作成します。
* WS_DLGFRAME             0x00400000      ダイアログボックスで一般的に使われるスタイルの境界を持つウィンドウを作成します。
* WS_VSCROLL              0x00200000      垂直スクロールバーを持つウィンドウを作成します。
* WS_HSCROLL              0x00100000      水平スクロールバーを持つウィンドウを作成します。
* WS_SYSMENU              0x00080000      タイトルバー上にウィンドウメニューボックスを持つウィンドウを作成します。
* WS_THICKFRAME           0x00040000      サイズ変更境界を持つウィンドウを作成します。
* WS_SIZEBOX
* WS_GROUP                0x00020000      コントロールグループの最初のコントロールを指定します。このコントロールから、次に WS_GROUP スタイルのコントロールがくるまでに定義されたコントロールが、コントロールグループになります。
* WS_TABSTOP              0x00010000      ユーザーが［Tab］キーを押すと入力フォーカスを受け取るコントロールを指定します。［Tab］キーを押すと、WS_TABSTOP スタイルを持つ次のコントロールに、入力フォーカスが移動します。
* WS_MINIMIZEBOX          0x00020000      最小化ボタンを持つウィンドウを作成します。 WS_SYSMENU スタイルも指定する必要があります。拡張スタイルに WS_EX_CONTEXTHELP を指定することはできません。
* WS_MAXIMIZEBOX          0x00010000      最大化ボタンを持つウィンドウを作成します。 WS_SYSMENU スタイルも指定する必要があります。拡張スタイルに WS_EX_CONTEXTHELP を指定することはできません。
* WS_OVERLAPPEDWINDOW     0x00CF0000      WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX に等しい。
* WS_POPUPWINDOW          0x80880000      WS_POPUP | WS_BORDER | WS_SYSMENU に等しい。
*/
StringList *EnumWindow(bool visibleOnly) {
  u_long nStyle;

  char szCaption[256];
  char szClassName[256];

  StringList *list = SAFE_NEW(__SOURCE__, StringList);  // new List

  HWND hWnd = ::GetTopWindow(::GetDesktopWindow());

  for (0; hWnd; hWnd = ::GetNextWindow(hWnd, GW_HWNDNEXT)) {
    nStyle = ::GetWindowLong(hWnd, GWL_STYLE);

    // ポップアップウィンドウ・子ウィンドウはダメ
    if (nStyle & (WS_POPUP | WS_CHILD)) {
      continue;
    }

    // 不可視チェック
    if (visibleOnly && !::IsWindowVisible(hWnd)) {
      continue;
    }

    ::GetWindowText(hWnd, szCaption, 256);
    ::GetClassName(hWnd, szClassName, 256);

    // リストに同じものが登録されているかチェック
    for (int i = list->count() - 1; i >= 0; i--) {
      if (list->Strings[i].tag(0) == (u_long)hWnd) {
        goto _Lnext;
      }
    }

    {
      //nkmm::string &s = list->Add( "%s@%s,%.8x", szCaption, szClassName, ::GetWindowLong( hWnd, GWL_STYLE ));
      nkmm::string &s = list->add("%s@%s", szCaption, szClassName);
      s.tag(0, (u_long)hWnd);
      s.tag(1, nStyle);
    }

  _Lnext:;
  }

  return list;
}

/*!
* プロセスIDからウィンドウハンドルを取得する
* @param[in] targetID プロセスID
* @param[in] caption 含まれるタイトル
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

      if (nkmm::sjis::SearchString(szCaption, caption)) {
        return hWnd;
      }
    }
  } while ((hWnd = ::GetNextWindow(hWnd, GW_HWNDNEXT)) != NULL);

  return NULL;
}

/*!
* プロセスリストの取得
* @param[out] processID プロセスIDを格納する配列
* @param[in] size 配列サイズ
* @return int プロセス数
*/
int EnumProcess(DWORD *processID, int size) {
  DWORD ProcessNum;
  DWORD ReturnSize;
  //	PROCESS_MEMORY_COUNTERS MemInfo;

  // プロセスIDの一覧と数を取得
  ::EnumProcesses(processID, size * sizeof(DWORD), &ReturnSize);
  ProcessNum = ReturnSize / sizeof(DWORD);

  return ProcessNum;
}

/*!
* プロセスハンドルからモジュール名を取得
* @param[in] hProcess
* @param[out] moduleName
*/
void GetModuleName(HANDLE hProcess, char *moduleName) {
  DWORD ReturnSize;
  HMODULE hModule;
  BOOL bResult;

  // プロセスのメインモジュールのハンドルを取得
  bResult = ::EnumProcessModules(hProcess, &hModule, sizeof(HMODULE), &ReturnSize);

  if (bResult) {
    // メインモジュールの名前を表示
    ::GetModuleBaseName(hProcess, hModule, moduleName, 1000);
  }
}

/*!
* 簡易ファイル検索
* @param[out] fileList 追加するファイルリスト
* @param[in] dir 対象ディレクトリ
* @param[in] mask マスク
* @param[in] excludeDirMask 対象外ディレクトリマスク
* @param[in] excludeFileMask 対象外ファイルマスク
*/
void DoFind(StringList *fileList, const nkmm::string &dir, const nkmm::string &mask,
            const nkmm::string &excludeDirMask, const nkmm::string &excludeFileMask) {
  HANDLE hFind;
  WIN32_FIND_DATA fd;

  nkmm::string dir_ = dir;

  // 最初のファイル検索
  //dir_ = dir;
  dir_ = dir_.replace("/", "\\");  // セパレータの置換

  if (dir_.at(dir_.length() - 1) != '\\') {
    dir_ += "\\";
  }

  dir_ += "*.*";
  hFind = ::FindFirstFile(dir_.c_str(), &fd);

  // 検索失敗？
  if (hFind == INVALID_HANDLE_VALUE) {
    //printf("error\n");
    return;  /// エラー終了 ///
  }

  do {
    // "." ".." は無視
    if (nkmm::strcmp(fd.cFileName, ".") != 0 && nkmm::strcmp(fd.cFileName, "..") != 0) {
      // ディレクトリ
      if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        dir_ = dir;

        if (dir_.at(dir_.length() - 1) != '\\') {
          dir_ += "\\";
        }
        dir_ += fd.cFileName;

        // 再起検索
        if (nkmm::sjis::CheckMasks(fd.cFileName, excludeDirMask.c_str())) {
        } else {
          DoFind(fileList, dir_.c_str(), mask, excludeDirMask, excludeFileMask);
        }

      } else {  // ファイル
        if (nkmm::sjis::CheckMasks(fd.cFileName, excludeFileMask.c_str())) {
        } else {
          if (nkmm::sjis::CheckFileMasks(fd.cFileName, mask.c_str())) {
            dir_ = dir;

            if (dir_.at(dir_.length() - 1) != '\\') {
              dir_ += "\\";
            }

            dir_ += fd.cFileName;

            // ファイル名追加
            nkmm::string &s = fileList->add(dir_.c_str());
            s.tag(0, fd.ftLastWriteTime.dwLowDateTime);
            s.tag(1, fd.ftLastWriteTime.dwHighDateTime);
          }
        }
      }
    }
  } while (::FindNextFile(hFind, &fd));  //次のファイルを検索

  // 検索終了
  ::FindClose(hFind);
}

/*!
* ファイル検索
* @param[in] fileList 追加するファイルリスト
* @param[in] dir 対象ディレクトリ
* @param[in] prefix プレフィクス
* @param[in] ext 拡張子
* @param[in] subLevel ディレクトリ深度
*/
void DoFind2(StringList *fileList, const nkmm::string &dir, const nkmm::string &prefix,
             const nkmm::string &ext, int subLevel) {
  HANDLE hFind;
  WIN32_FIND_DATA fd;

  nkmm::string t_dir = dir;

  // 最初のファイル検索
  t_dir = dir;
  t_dir += "*.*";
  hFind = ::FindFirstFile(t_dir.c_str(), &fd);

  // 検索失敗？
  if (hFind == INVALID_HANDLE_VALUE) {
    printf("error\n");
    return;  /// エラー終了 ///
  }

  do {
    // "." ".." は無視
    if (nkmm::strcmp(fd.cFileName, ".") != 0 && nkmm::strcmp(fd.cFileName, "..") != 0) {
      // ディレクトリ
      if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        if (subLevel > 0) {
          t_dir = dir;
          t_dir = t_dir + fd.cFileName + "\\";

          // 再起検索
          DoFind2(fileList, t_dir.c_str(), prefix, ext, subLevel - 1);
        }

      } else {  // ファイル
        // 指定外
        if (prefix.c_str()[0] == '!') {
          nkmm::string prefix1 = fd.cFileName;
          nkmm::string prefix2 = prefix.c_str() + 1;
          nkmm::string ext1 = nkmm::string(fd.cFileName).asFileExt();
          nkmm::string ext2 = ext;

          if (!nkmm::sjis::SearchString(prefix1.upCase().c_str(), prefix2.upCase().c_str()) &&
              ext1.upCase() == ext2.upCase()) {
            t_dir = dir;
            t_dir = t_dir + fd.cFileName;

            // ファイル名追加
            fileList->add(t_dir.c_str());
          }

        } else {  // 指定
          nkmm::string prefix1 = fd.cFileName;
          nkmm::string prefix2 = prefix.c_str() + 1;
          nkmm::string ext1 = nkmm::string(fd.cFileName).asFileExt();
          nkmm::string ext2 = ext;

          if (nkmm::sjis::SearchString(prefix1.upCase().c_str(), prefix2.upCase().c_str()) &&
              ext1.upCase() == ext2.upCase()) {
            t_dir = dir;
            t_dir = t_dir + fd.cFileName;

            // ファイル名追加
            fileList->add(t_dir.c_str());
          }
        }
      }
    }
  } while (::FindNextFile(hFind, &fd));  //次のファイルを検索

  // 検索終了
  ::FindClose(hFind);
}

/*!
* 特殊フォルダのパス取得
* @param[in] hWnd 対象ウィンドウハンドル
* @param[in] nFolder フォルダID
* @param[out] path フォルダ
* @return BOOL
* 
* @note
* 関数 SHGetSpecialFolderPath() が使用できない環境で特殊フォルダのパスを取得する。<br>
* 引数は SHGetSpecialFolderPath() と同じ。<br>
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

void SpecialPathToPath(nkmm::string &str) {
  for (int i = 0; i < s_folderMax; i++) {
    if (str.find(s_folderId[i].Name) >= 0) {
      char path[MAX_PATH];

      // 取得できたら置換する
      if (GetSpecialPath(NULL, s_folderId[i].Id, path)) {
        str = str.replace(s_folderId[i].Name, path);
        break;
      }
    }
  }
}

void PathToSpecialPath(nkmm::string &str) {
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
* ショートカットの情報取得
* @param[in] link リンクパス
* @param[out] path パス
* @param[out] work 作業ディレクトリ
* @param[out] param 引数
* @param[out] icon 参照するアイコンのパス
* @return HRESULT
* 
* @note
* GetArguments        リンク先の、コマンドライン引数を取得する。
* GetDescription      「コメント」を取得する。
* GetHotkey           「ショートカットキー」を取得する。
* GetIconLocation     表示されるアイコンのパス＋リソースIDを取得する。
* GetIDList           「リンク先」のパスを、ITEMIDLISTで得る。
* GetPath             「リンク先」のパスを文字列で得る。
* GetShowCmd          「実行時の大きさ」(SW_*)の設定を得る。
* GetWorkingDirectory 「作業フォルダ」のパスを得る。
* Resolve             リンク先が有るかどうか試す。無ければダイアログを表示して探す。（訳注：？）
* SetArguments        リンク先のコマンドライン引数を設定。
* SetDescrition       「コメント」を設定する。
* SetHotkey           「ショートカットキー」を設定する。
* SetIconLocation     表示されるアイコンの場所とアイコンのインデックスを設定する。
* SetIDList           「リンク先」のパスを、ITEMIDLISTで指定する。
* SetPath             「リンク先」のパスを、設定する。
* SetRelativePath     リンク先のパスを、相対パスで指定する。
* SetShowCmd          「実行時の大きさ」(SW_*)を設定する。
* SetWorkingDirectory 「作業フォルダ」を設定する。
*/
HRESULT GetShortcutInfo(LPCSTR link, LPSTR path, LPSTR work, LPSTR param, LPSTR icon) {
  HRESULT hres;
  IShellLink *pShLink;

  WIN32_FIND_DATA wfd;  //ファイル情報が必要な場合は外に出す

  hres = ::CoInitialize(NULL);

  if (SUCCEEDED(hres)) {
    // IShellLink オブジェクトを作成しポインタを取得する
    hres = ::CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink,
                              (void **)&pShLink);

    if (SUCCEEDED(hres)) {
      IPersistFile *ppf;

      // IPersistFile インターフェイスの問い合わせをおこなう
      hres = pShLink->QueryInterface(IID_IPersistFile, (void **)&ppf);

      if (SUCCEEDED(hres)) {
        wchar_t wsz[MAX_PATH];  // Unicode 文字列へのバッファ

        // 文字列がANSI文字で構成されるようにする
        MultiByteToWideChar(CP_ACP, 0, link, -1, wsz, MAX_PATH);

        // ショートカットをロードする
        hres = ppf->Load(wsz, STGM_READ);

        if (SUCCEEDED(hres)) {
          //「リンク先」のパスを文字列で得る。
          if (path) {
            hres = pShLink->GetPath(path, MAX_PATH, &wfd, SLGP_UNCPRIORITY);
          }

          //「作業フォルダ」のパスを得る。
          if (work) {
            hres = pShLink->GetWorkingDirectory(work, MAX_PATH);
          }

          //リンク先の、コマンドライン引数を取得する。
          if (param) {
            hres = pShLink->GetArguments(param, MAX_PATH);
          }

          //「実行時の大きさ」(SW_*)の設定を得る。
          //hres = pShLink->GetShowCmd(int  *piShowCmd);

          //表示されるアイコンのパス＋リソースIDを取得する。
          if (icon) {
            int iconNo;
            hres = pShLink->GetIconLocation(icon, MAX_PATH, &iconNo);
          }

        } else {
          printf("ppf->Load failed.\n");
        }

        // IPersistFile へのポインタを開放する
        ppf->Release();

      } else {
        printf("pShLink->QueryInterface failed.\n");
      }

      // IShellLinkへのポインタを開放する
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

  WIN32_FIND_DATA wfd;  //ファイル情報が必要な場合は外に出す

  hres = ::CoInitialize(NULL);

  if (SUCCEEDED(hres)) {
    // IShellLink オブジェクトを作成しポインタを取得する
    hres = ::CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink,
                              (void **)&pShLink);

    if (SUCCEEDED(hres)) {
      IPersistFile *ppf;

      // IPersistFile インターフェイスの問い合わせをおこなう
      hres = pShLink->QueryInterface(IID_IPersistFile, (void **)&ppf);

      if (SUCCEEDED(hres)) {
        wchar_t wsz[MAX_PATH];  // Unicode 文字列へのバッファ

        // 文字列がANSI文字で構成されるようにする
        MultiByteToWideChar(CP_ACP, 0, link, -1, wsz, MAX_PATH);

        // ショートカットをロードする
        hres = ppf->Load(wsz, STGM_READ);

        if (SUCCEEDED(hres)) {
          for (int i = 0; i < max; i++) {
            ShortcutCommandQue *data = &a[i];

            switch (data->cmd) {
            //「リンク先」のパスを文字列で得る。
            case kShortcutPath:
              hres = pShLink->GetPath(data->buf, data->len, &wfd, SLGP_UNCPRIORITY);
              break;
            //「作業フォルダ」のパスを得る。
            case kShortcutWorkingDirectory:
              hres = pShLink->GetWorkingDirectory(data->buf, data->len);
              break;
            //リンク先の、コマンドライン引数を取得する。
            case kShortcutArguments:
              hres = pShLink->GetArguments(data->buf, data->len);
              break;
            //表示されるアイコンのパス＋リソースIDを取得する。
            case kShortcutIcon: {
              int iconNo;
              hres = pShLink->GetIconLocation(data->buf, data->len, &iconNo);
            } break;
            //「コメント」を取得する。
            case kShortcutDescription:
              hres = pShLink->GetDescription(data->buf, data->len);
              break;
            }
          }
        } else {
          printf("ppf->Load failed.\n");
        }

        // IPersistFile へのポインタを開放する
        ppf->Release();
      } else {
        printf("pShLink->QueryInterface failed.\n");
      }

      // IShellLinkへのポインタを開放する
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
* フォルダの選択ダイアログを開く（コールバック）
* @param[in] hWnd
* @param[in] uMsg
* @param[in] lParam
* @param[in] lpData
* @return int
*/
static int CALLBACK SHBrowseProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData) {
  (void)lParam;

  if (uMsg == BFFM_INITIALIZED && lpData) {
    //	デフォルトで選択させるパスの指定
    ::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, lpData);
  }

  return 0;
}

/*!
* フォルダの選択ダイアログを開く
* @param[in] Handle ハンドル
* @param[out] dirPath 選択したディレクトリパス
* @param[in] title タイトル
* @param[in] defaultPath デフォルトパス
* @return char*
*/
char *GetFolder(HWND Handle, char *dirPath, const char *title, const char *defaultPath) {
  BROWSEINFO BI;
  //	char dirPath[MAX_PATH];
  char FName[MAX_PATH];
  LPITEMIDLIST ItemID;

  nkmm::Memory::Zero(&BI, sizeof(BROWSEINFO));
  //	nkmm::Memory::Zero(dirPath, MAX_PATH);
  nkmm::Memory::Zero(FName, MAX_PATH);

  BI.hwndOwner = Handle;              // オーナーウィンドウのハンドル
  BI.pidlRoot = NULL;                 // ルートフォルダのIDリスト
  BI.pszDisplayName = FName;          // 選択フォルダのパス
  BI.lpszTitle = title;               //"フォルダを選択して下さい";	 // 表示文字
  BI.ulFlags = BIF_RETURNONLYFSDIRS;  // フォルダのみ選択
  BI.lpfn = SHBrowseProc;             //nil;			// コールバック関数
  BI.lParam = (LPARAM)defaultPath;    // コールバック関数に渡される値
  BI.iImage = 0;  // 選択フォルダのイメージリストインデックス

  ItemID = ::SHBrowseForFolder(&BI);
  ::SHGetPathFromIDList(ItemID, dirPath);
  GlobalFreePtr(ItemID);

  return dirPath;
}

/*!
* ショートカットの作成
* @param[in] name リンク名
* @param[in] dir リンク作成ディレクトリ
* @param[in] fileName リンク先ファイルパス
* @param[in] param パラメータ
* @return bool
*/
bool CreateShortcut(const char *name, const char *dir, const char *fileName, const char *param) {
  nkmm::string sTarget;    //起動させたいアプリケーションのファイル名
  nkmm::string sCmdParam;  //アプリケーションに渡すパラメータ
  nkmm::string sSCFName;   //ショートカットそのもののファイル名
  wchar_t wszSCFName[MAX_PATH];
  long hRes;
  IPersistFile *xPF;
  IShellLink *xSL;

  sTarget = fileName;
  sCmdParam = param;  //コマンドラインパラメータ

  nkmm::string s = dir;
  SpecialPathToPath(s);
  s += "\\";  //ショートカットそのもののファイル名
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
      xSL->SetDescription("" /*ここにコメントをいれる*/);
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
* ファイルの存在確認
* @param[in] fileName ファイルパス
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
* 拡張子に関連付けられた実行ファイル取得
* @param[in] strExtension 拡張子
* @return strCommand 実行ファイル名
* 
* @note
* strExtensionは".txt"のようにする（"txt"でも構わない）
* ".bat"を取得すると"%1"になるなど実際のコマンドとならないことがある！
*/
nkmm::string GetExecutable(const nkmm::string &strExtension) {
  DWORD dwOut;
  HRESULT hr;
  TCHAR fname[MAX_PATH + 10];
  nkmm::string ext;

  nkmm::string strCommand = "";

  if (strExtension.empty()) {
    return strCommand;
  }

  ext = strExtension;

  if (ext.at(0) != '.') {
    ext = nkmm::string(".") + ext;
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
}  //nkmm
