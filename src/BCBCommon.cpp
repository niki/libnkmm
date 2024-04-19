//---------------------------------------------------------------------------

//#define ShlObjHPP
#include <vcl.h>
#include <windowsx.h>
//#include <shlobj.h>
//#include <StrUtils.hpp>
#pragma hdrstop

#include "BCBCommon.h"

//#include "special_path_name.h"

//#include <io>
//#include <dos>
//#include <stack>
//#include <memory>

#include <psapi.h>
#include <tlhelp32.h>


#include "stack.h"

using namespace std;

static const char *ErrOpenProcess	   = "Error OpenProcessToken";
static const char *ErrLookupPrivilege = "Error LookupPrivilegeValue";
static const char *ErrAdjustToken	   = "Error AdjustTokenPrivileges";
static const char *ErrExitWindowsEx   = "Error ExitWindowEx";


//---------------------------------------------------------------------------
/*!
 権限の設定
 @param[in] Prvlg
 @param[in] ServerName
 @param[in] Mode
*/
static void AccessPrivilege( char *Prvlg, String ServerName, bool Mode ) throw(Exception)
{
	HANDLE htoken;
	TOKEN_PRIVILEGES	tknPrvlgs;
	
	bool success = OpenProcessToken( GetCurrentProcess(),
							 TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &htoken );
	if ( !success )
	{
		throw Exception( ErrOpenProcess );
	}
	
	success = LookupPrivilegeValue( ServerName.c_str(), Prvlg, &(tknPrvlgs.Privileges[0].Luid) );
	if ( !success )
	{
		throw Exception( ErrLookupPrivilege );
	}

	tknPrvlgs.PrivilegeCount = 1;

	if ( Mode )
	{
		tknPrvlgs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	}
	else
	{
		tknPrvlgs.Privileges[0].Attributes = 0;
	}


	AdjustTokenPrivileges( htoken, false, &tknPrvlgs,
								   sizeof(TOKEN_PRIVILEGES), 0, 0 );
	//AdjustTokenPrivileges は常に true を返すのが仕様らしい？
	//SDK の Help のサンプルにも GetLastError で記述されている
	if ( GetLastError() != ERROR_SUCCESS )
	{
		throw Exception( ErrAdjustToken );
	}

}
//---------------------------------------------------------------------------
/*!
 システムシャットダウン
 @param[in] type
*/
void		systemShutdown(const int type)
{
	//EWX_POWEROFF/*EWX_SHUTDOWN*/ | EWX_FORCE
	//EWX_REBOOT
	//EWX_LOGOFF
	
	OSVERSIONINFO	osvi;
	osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
//	bool bVer = (GetVersion() < 0x80000000) ? 1 : 0; // Win32s or WinNT
	GetVersionEx( &osvi );
	
	////////////////////////////////////////////////////
	// NT だったら権限変更してシャットダウン
	// 電源オフ機能をサポートしていることが前提
	////////////////////////////////////////////////////
	if ( osvi.dwPlatformId == VER_PLATFORM_WIN32_NT )
	{
		try
		{
			AccessPrivilege( SE_SHUTDOWN_NAME, "", true );
		}
		catch( Exception &Ex )
		{
//			ErrMsg( Ex.Message );
			return;
		}

		if ( !ExitWindowsEx( type, 0 ) )
		{
//			ErrMsg( ErrExitWindowsEx );
		}

		try
		{
			AccessPrivilege( SE_SHUTDOWN_NAME, "", false );
		}
		catch( Exception &Ex )
		{
//			ErrMsg( Ex.Message );
			return;
		}
	}
	////////////////////////////////////////////////////
	// Win9xシャットダウン
	////////////////////////////////////////////////////
	else
	{
		if ( !ExitWindowsEx( type, 0 ) )
		{
//			ErrMsg( ErrExitWindowsEx );
		}
	}
}
//---------------------------------------------------------------------------
/*!
 文字列の送信
 @param[in] hWnd 対象ウィンドウハンドル
 @param[in] str 送信文字列
*/
void SendString(HWND hWnd, const char *str)
{
	for ( int i = 0; str[i] != '\0'; i++ )
	{
		::SendMessage( hWnd, WM_CHAR, str[i] & 0xff, 0 );
	}
}
//---------------------------------------------------------------------------
/*!
 フォルダ単位で消去
 @param[in] DirName
*/
void		DelDir(AnsiString DirName)
{
	// 指定フォルダ以下のファイル、フォルダをすべて削除
#if 0
	TSearchRec	sr;
	int			Ret;
	AnsiString	Work;
	
	Ret = FindFirst( DirName + "\\*.*", faAnyFile, sr );
	while ( Ret == 0 )
	{
		if ( sr.Name != "." && sr.Name != ".." )
		{
			if ( sr.Attr == faDirectory )
			{
				// 再帰呼び出し
				DelDir( DirName + "\\" + sr.Name );
				RemoveDir( DirName + "\\" + sr.Name );
			}
			else
			{
				Work = DirName + "\\" + sr.Name;
				_rtl_chmod( Work.c_str(), 1, FA_ARCH );
				DeleteFile( Work );
			}
		}
		Ret = FindNext( sr );
	}
	FindClose( sr );
#else
	AnsiString from = IncludeTrailingPathDelimiter( DirName ) + "\\" + "*.*";
	SHFILEOPSTRUCT fo =
	{
		Application->Handle, FO_DELETE, from.c_str(), "", /*FOF_ALLOWUNDO |*/ FOF_NOCONFIRMATION
	};
	
	SHFileOperation( &fo );
#endif
}
//---------------------------------------------------------------------------
/*!
 ファイルオペレーション(TStringList)
 FO_MOVE				ファイルを移動
 FO_COPY				ファイルをコピー
 FO_DELETE				ファイルを削除
 FO_RENAME				ファイル名を変更
 FOF_ARROWUNDO			可能であれば、取り消し情報を記憶する
 FOF_FILESONLY			最ワイルドカード指定の場合、フォルダは処理をしない。
 FOF_MULTIDESTFILES		pTOに全ファイル・フォルダをコピー
 FOF_NOCONFIREMATION	確認ダイアログに「すべてのファイル（フォルダ）～」ボタンを表示する
 FOF_NOCONFIRMKDIR		新しいディレクトリの作成時に確認をしない
 FOF_RENAMEONCOLLISION	pTOのファイル・フォルダが既存している場合、新しい名前を定義
 FOF_SILENT				進行状況ダイアログを表示しない
 FOF_SIMPLEPROGRESS		進行状況ダイアログにファイル名を表示しない
 FOF_WANTMAPPINGHANDLE	hNameMappingsを使用する
*/
DWORD		fileOperationFromList(HWND Handle, TStringList *list, long wFunc, long fFlags, const char *To)
{
	int		i;
	DWORD	ret;
	
	char *Pointer;
	char *FileName;
	
	FileName = new char[ list->Text.Length() ];
	ZeroMemory( FileName,list->Text.Length() );
	Pointer = FileName;
	
	for ( i = 0; i < list->Count; i++ )
	{
		StrCopy( Pointer, list->Strings[i].c_str() );
		Pointer = Pointer + list->Strings[i].Length() + 1;
	}
	
	SHFILEOPSTRUCT op;
	op.hwnd		= Handle;		// ハンドル
	op.wFunc	= wFunc;		// ごみ箱に捨てる
	op.pFrom	= FileName;		// ファイルのポインタ
	op.pTo		= To;
	op.fFlags	= fFlags;		// Undo可
	op.fAnyOperationsAborted	= true;	  // キャンセル可
	op.hNameMappings			= NULL;
	op.lpszProgressTitle		= NULL;
	ret = ::SHFileOperation( &op );
	
	delete[] FileName;
	
/*	ファイルをごみ箱に捨てるには、SHFileOperation()を使用します。
	SHFILEOPSTRUCT構造体を宣言し、その中で色々設定します。
	ごみ箱に捨てるファイルは複数指定でき、ファイル名を "ファイル名１\0ファイル名２\0ファイル名\0\0" のように
	'\0（NULL）' で区切って最後が '\0\0（NULLNULL）' で終わるようにするのですが、
	AnsiString型では \0 で区切る事が出来ません。（ \0 で終わりと判断されてしまう。）
	以下のサンプルではchar型でファイル名を指定しています。
	
	char *FileName ="C:\\Windows\\ﾃﾞｽｸﾄｯﾌﾟ\\1.txt\0\0";	  // ごみ箱に捨てるファイル
	SHFILEOPSTRUCT op;
	op.hwnd	 = Handle;				   // ハンドル
	op.wFunc = FO_DELETE;			   // ごみ箱に捨てる
	op.pFrom = FileName;			   // ファイルのポインタ
	op.pTo	 = NULL
	op.fFlags= FOF_ALLOWUNDO;		   // Undo可
	op.fAnyOperationsAborted = true;   // キャンセル可
	op.hNameMappings = NULL;
	op.lpszProgressTitle = NULL;
	::SHFileOperation(&op);
*/

	return ret;
}
//---------------------------------------------------------------------------
/*!
 ウィンドウハンドルから、そのモジュールのフルパスを取得します。
 @param[in] hWnd 調べるウィンドウのハンドル
 @param[in] lpszFileName モジュールの完全修飾パスを受け取るバッファへのポインタ
 @param[in] nSize 取得したい文字の最大の長さ
 @return モジュールのフルパス名が取得できたら TRUE が返ります。それ以外は FALSE が返ります。
*/
BOOL		GetFileNameFromHwnd(HWND hWnd, LPTSTR lpszFileName, DWORD nSize)
{
	BOOL bResult = FALSE;
	// ウィンドウを作成したプロセスの ID を取得
	DWORD dwProcessId;
	GetWindowThreadProcessId( hWnd , &dwProcessId );
	
	// 現在実行している OS のバージョン情報を取得
	OSVERSIONINFO osverinfo;
	osverinfo.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
	if ( !GetVersionEx( &osverinfo ) )
	{
		return FALSE;
	}
	
	// プラットフォームが Windows NT/2000 の場合
	if ( osverinfo.dwPlatformId == VER_PLATFORM_WIN32_NT )
	{
		// PSAPI 関数のポインタ
		BOOL (WINAPI *lpfEnumProcessModules)
							(HANDLE, HMODULE*, DWORD, LPDWORD);
		DWORD (WINAPI *lpfGetModuleFileNameEx)
							(HANDLE, HMODULE, LPTSTR, DWORD);
		
		// PSAPI.DLL ライブラリをロード
		HINSTANCE hInstLib = LoadLibrary( "PSAPI.DLL" );
		if ( hInstLib == NULL )
		{
			return FALSE ;
		}
		
		// プロシージャのアドレスを取得
		lpfEnumProcessModules = (BOOL(WINAPI *)
			(HANDLE, HMODULE *, DWORD, LPDWORD))GetProcAddress(
						hInstLib, "EnumProcessModules");
		lpfGetModuleFileNameEx =(DWORD (WINAPI *)
			(HANDLE, HMODULE, LPTSTR, DWORD))GetProcAddress(
						hInstLib, "GetModuleFileNameExA");
		
		if ( lpfEnumProcessModules && lpfGetModuleFileNameEx )
		{
			// プロセスオブジェクトのハンドルを取得
			HANDLE hProcess;
			hProcess = OpenProcess(
					PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
					FALSE, dwProcessId);
			if ( hProcess )
			{
				// .EXE モジュールのハンドルを取得
				HMODULE hModule;
				DWORD dwNeed;
				if ( lpfEnumProcessModules( hProcess, &hModule, sizeof( hModule ), &dwNeed ) )
				{
					// モジュールハンドルからフルパス名を取得
					if ( lpfGetModuleFileNameEx( hProcess, hModule, lpszFileName, nSize ) )
					{
						bResult = TRUE;
					}
				}
				// プロセスオブジェクトのハンドルをクローズ
				CloseHandle( hProcess ) ;
			}
		}
		// PSAPI.DLL ライブラリを開放
		FreeLibrary( hInstLib ) ;
	}
	// プラットフォームが Windows 9x の場合
	else if ( osverinfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
	{
		// ToolHelp 関数ポインタ
		HANDLE (WINAPI *lpfCreateSnapshot)(DWORD, DWORD);
		BOOL (WINAPI *lpfProcess32First)(HANDLE, LPPROCESSENTRY32);
		BOOL (WINAPI *lpfProcess32Next)(HANDLE, LPPROCESSENTRY32);
		
		// プロシージャのアドレスを取得
		lpfCreateSnapshot =
			(HANDLE(WINAPI*)(DWORD,DWORD))GetProcAddress(
								GetModuleHandle("kernel32.dll"),
								"CreateToolhelp32Snapshot" );
		lpfProcess32First=
			(BOOL(WINAPI*)(HANDLE,LPPROCESSENTRY32))GetProcAddress(
								GetModuleHandle("kernel32.dll"),
								"Process32First" );
		lpfProcess32Next=
			(BOOL(WINAPI*)(HANDLE,LPPROCESSENTRY32))GetProcAddress(
								GetModuleHandle("kernel32.dll"),
								"Process32Next" );
		if ( !lpfCreateSnapshot || !lpfProcess32First || !lpfProcess32Next )
		{
			return FALSE;
		}
		
		// システム プロセスの Toolhelp スナップショットを作成
		HANDLE hSnapshot;
		hSnapshot = lpfCreateSnapshot( TH32CS_SNAPPROCESS , 0 );
		if ( hSnapshot != (HANDLE)-1 )
		{
			// 最初のプロセスに関する情報を取得
			PROCESSENTRY32 pe;
			pe.dwSize = sizeof( PROCESSENTRY32 );
			if ( lpfProcess32First( hSnapshot, &pe ) )
			{
				do
				{
					// 同じプロセスID であれば、ファイル名を取得
					if ( pe.th32ProcessID == dwProcessId )
					{
						lstrcpy( lpszFileName, pe.szExeFile );
						bResult = TRUE;
						break;
					}
				} while ( lpfProcess32Next( hSnapshot, &pe ) );
			}
			// スナップショットを破棄
			CloseHandle( hSnapshot );
		}
	}
	else
	{
		return FALSE;
	}
	
	return bResult;
}
//---------------------------------------------------------------------------
/*!
 コンボボックスへの履歴に追加管理
 @param[in] cmb 対象のコンボボックス
 @return 0:未追加 1:追加
*/
int		SetComboBoxHistory(TComboBox *cmb)
{
	// 空は対象外
	if ( !cmb->Text.IsEmpty() )
	{
		AnsiString	cmbText	= cmb->Text;
		
		// 同じものがあったら移動する
		for ( int i = 0; i < cmb->Items->Count; i++ )
		{
			if ( !cmb->Items->Strings[i].AnsiCompare( cmbText ) )
			{
				cmb->Items->Move( i, 0 );
				
				// テキスト設定
				cmb->ItemIndex = 0;
				return 1;	// もどりー
			}
		}
		
		// 頭に挿入
		cmb->Items->Insert( 0, cmbText );
		
		// 最大値制限
		if ( Conbo_box_history_max < cmb->Items->Count )
		{
			cmb->Items->Delete( cmb->Items->Count - 1 );
		}
		
		// テキスト設定
		cmb->ItemIndex = 0;
		
		return 1;
	}
	
	return 0;
}
//---------------------------------------------------------------------------
/*!
 コンボボックスの二重登録のチェック
 @param[in] cmb 対象のコンボボックス
 @param[in] text チェックしたい文字列
 @return 0:すでにあります 1:新規です
*/
int		ComboBoxExistCheck(TComboBox *cmb, AnsiString text)
{
	if ( text.IsEmpty() )
	{
		return 0;
	}
	
	for ( int i = 0; i < cmb->Items->Count; i++ )
	{
		if ( !cmb->Items->Strings[i].AnsiCompare( text ) )
		{
			return 0;
		}
	}
	
	return 1;
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
