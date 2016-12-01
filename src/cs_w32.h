//-----------------------------------------------------------------------------
/*!
* @file		mx_w32.h
* @brief	Windows 関連
* @author	koma
* @version	$Id$
* 
* @note
* ・更新履歴  
*/
//-----------------------------------------------------------------------------

#ifndef MX_W32_H
#define MX_W32_H

/*
SetDwrod に設定されるモノ

フォルダ
0:サブフォルダの有無
1:属性

ファイル
0:ファイルサイズ
1:属性
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
// でフォルダ・ファイル単位で呼ばれるコールバック関数を設定
void SetCallback(int (*fnCallback)(const mx::string&, void*), void* data);

/*!
* サブフォルダの存在チェック(内部処理)
* @param[in] dir 検索フォルダ
* @return 0:なし, 1:あり
*/
int SubFolderExists(const mx::string& dir);

/*!
* フォルダの列挙
* @param[in] dir 検索フォルダ
* @return 登録した StringList クラスのポインタ
*/
StringList* EnumFolders(const mx::string& dir);

/*!
* ファイルの列挙
* @param[in] dir 検索フォルダ
* @param[in] mask 検索パターン
* @return 登録した StringList クラスのポインタ
*/
StringList* EnumFiles(const mx::string& dir, const mx::string& mask);

/*!
* ファイルの検索
* @param[in] path 検索パス
* @param[in] patIn 検索パターン
* @param[in] patEx 検索除外パターン
* @param[in] dirIn 検索フォルダ（nil のときはサブフォルダを検索しません）
* @param[in] dirEx 検索除外フォルダ
* @return ヒットした数
*/
int SearchFiles(const mx::string& path, const mx::string& patIn, const mx::string& patEx,
                const mx::string& dirIn, const mx::string& dirEx);

/*!
* 表示されているウィンドウのクラス一覧作成
* @param[in] visibleOnly
* @return StringList*
*/
StringList* EnumWindow(bool visibleOnly);

/*!
* プロセスIDからウィンドウハンドルを取得する
* @param[in] targetID プロセスID
* @param[in] caption 含まれるタイトル
* @return HWND
*/
HWND GetWindowHandleFromProcess(const DWORD targetID, const char* caption);

/*!
* プロセスリストの取得
* @param[out] processID プロセスIDを格納する配列
* @param[in] size 配列サイズ
* @return int プロセス数
*/
int EnumProcess(DWORD* processID, int size);

/*!
* プロセスハンドルからモジュール名を取得
* @param[in] hProcess
* @param[out] moduleName
*/
void GetModuleName(HANDLE hProcess, char* moduleName);

/*!
* 簡易ファイル検索
* @param[out] fileList 追加するファイルリスト
* @param[in] dir 対象ディレクトリ
* @param[in] mask マスク
* @param[in] excludeDirMask 対象外ディレクトリマスク
* @param[in] excludeFileMask 対象外ファイルマスク
*/
void DoFind(StringList* fileList, const mx::string& dir, const mx::string& mask,
            const mx::string& excludeDirMask, const mx::string& excludeFileMask);

/*!
* ファイル検索
* @param[in] fileList 追加するファイルリスト
* @param[in] dir 対象ディレクトリ
* @param[in] prefix プレフィクス
* @param[in] ext 拡張子
* @param[in] subLevel ディレクトリ深度
*/
void DoFind2(StringList* fileList, const mx::string& dir, const mx::string& prefix,
             const mx::string& ext, int subLevel);

/*!
* 特殊フォルダのパス取得
* @param[in] hWnd 対象ウィンドウハンドル
* @param[in] nFolder フォルダID
* @param[out] path フォルダ
* @return BOOL
*/
BOOL GetSpecialPath(HWND hWnd, int nFolder, char* Path);
void SpecialPathToPath(mx::string& str);
void PathToSpecialPath(mx::string& str);

/*!
* ショートカットの情報取得
* @param[in] link リンクパス
* @param[out] path パス
* @param[out] work 作業ディレクトリ
* @param[out] param 引数
* @param[out] icon 参照するアイコンのパス
* @return HRESULT
*/
HRESULT GetShortcutInfo(LPCSTR link, LPSTR path, LPSTR work, LPSTR param, LPSTR icon);
void GetShortcutInfoArray(LPCSTR link, ShortcutCommandQue* a, int max);

/*!
* フォルダの選択ダイアログを開く
* @param[in] Handle ハンドル
* @param[out] dirPath 選択したディレクトリパス
* @param[in] title タイトル
* @param[in] defaultPath デフォルトパス
* @return char*
*/
char* GetFolder(HWND Handle, char* dirPath, const char* title, const char* defaultPath = 0);

/*!
* ショートカットの作成
* @param[in] name リンク名
* @param[in] dir リンク作成ディレクトリ
* @param[in] fileName リンク先ファイルパス
* @param[in] param パラメータ
* @return bool
*/
bool CreateShortcut(const char* name, const char* dir, const char* fileName, const char* param);

/*!
* ファイルの存在確認
* @param[in] fileName ファイルパス
* @return bool
*/
bool FileExists(const char* fileName);

/*!
* 拡張子に関連付けられた実行ファイル取得
* @param[in] strExtension 拡張子
* @return strCommand 実行ファイル名
*/
mx::string GetExecutable(const mx::string& strExtension);

}  //win
}  //mx

#endif  //MX_W32_H
