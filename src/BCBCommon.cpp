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
 �����̐ݒ�
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
	//AdjustTokenPrivileges �͏�� true ��Ԃ��̂��d�l�炵���H
	//SDK �� Help �̃T���v���ɂ� GetLastError �ŋL�q����Ă���
	if ( GetLastError() != ERROR_SUCCESS )
	{
		throw Exception( ErrAdjustToken );
	}

}
//---------------------------------------------------------------------------
/*!
 �V�X�e���V���b�g�_�E��
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
	// NT �������猠���ύX���ăV���b�g�_�E��
	// �d���I�t�@�\���T�|�[�g���Ă��邱�Ƃ��O��
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
	// Win9x�V���b�g�_�E��
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
 ������̑��M
 @param[in] hWnd �ΏۃE�B���h�E�n���h��
 @param[in] str ���M������
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
 �t�H���_�P�ʂŏ���
 @param[in] DirName
*/
void		DelDir(AnsiString DirName)
{
	// �w��t�H���_�ȉ��̃t�@�C���A�t�H���_�����ׂč폜
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
				// �ċA�Ăяo��
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
 �t�@�C���I�y���[�V����(TStringList)
 FO_MOVE				�t�@�C�����ړ�
 FO_COPY				�t�@�C�����R�s�[
 FO_DELETE				�t�@�C�����폜
 FO_RENAME				�t�@�C������ύX
 FOF_ARROWUNDO			�\�ł���΁A�����������L������
 FOF_FILESONLY			�Ń��C���h�J�[�h�w��̏ꍇ�A�t�H���_�͏��������Ȃ��B
 FOF_MULTIDESTFILES		pTO�ɑS�t�@�C���E�t�H���_���R�s�[
 FOF_NOCONFIREMATION	�m�F�_�C�A���O�Ɂu���ׂẴt�@�C���i�t�H���_�j�`�v�{�^����\������
 FOF_NOCONFIRMKDIR		�V�����f�B���N�g���̍쐬���Ɋm�F�����Ȃ�
 FOF_RENAMEONCOLLISION	pTO�̃t�@�C���E�t�H���_���������Ă���ꍇ�A�V�������O���`
 FOF_SILENT				�i�s�󋵃_�C�A���O��\�����Ȃ�
 FOF_SIMPLEPROGRESS		�i�s�󋵃_�C�A���O�Ƀt�@�C������\�����Ȃ�
 FOF_WANTMAPPINGHANDLE	hNameMappings���g�p����
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
	op.hwnd		= Handle;		// �n���h��
	op.wFunc	= wFunc;		// ���ݔ��Ɏ̂Ă�
	op.pFrom	= FileName;		// �t�@�C���̃|�C���^
	op.pTo		= To;
	op.fFlags	= fFlags;		// Undo��
	op.fAnyOperationsAborted	= true;	  // �L�����Z����
	op.hNameMappings			= NULL;
	op.lpszProgressTitle		= NULL;
	ret = ::SHFileOperation( &op );
	
	delete[] FileName;
	
/*	�t�@�C�������ݔ��Ɏ̂Ă�ɂ́ASHFileOperation()���g�p���܂��B
	SHFILEOPSTRUCT�\���̂�錾���A���̒��ŐF�X�ݒ肵�܂��B
	���ݔ��Ɏ̂Ă�t�@�C���͕����w��ł��A�t�@�C������ "�t�@�C�����P\0�t�@�C�����Q\0�t�@�C����\0\0" �̂悤��
	'\0�iNULL�j' �ŋ�؂��čŌオ '\0\0�iNULLNULL�j' �ŏI���悤�ɂ���̂ł����A
	AnsiString�^�ł� \0 �ŋ�؂鎖���o���܂���B�i \0 �ŏI���Ɣ��f����Ă��܂��B�j
	�ȉ��̃T���v���ł�char�^�Ńt�@�C�������w�肵�Ă��܂��B
	
	char *FileName ="C:\\Windows\\�޽�į��\\1.txt\0\0";	  // ���ݔ��Ɏ̂Ă�t�@�C��
	SHFILEOPSTRUCT op;
	op.hwnd	 = Handle;				   // �n���h��
	op.wFunc = FO_DELETE;			   // ���ݔ��Ɏ̂Ă�
	op.pFrom = FileName;			   // �t�@�C���̃|�C���^
	op.pTo	 = NULL
	op.fFlags= FOF_ALLOWUNDO;		   // Undo��
	op.fAnyOperationsAborted = true;   // �L�����Z����
	op.hNameMappings = NULL;
	op.lpszProgressTitle = NULL;
	::SHFileOperation(&op);
*/

	return ret;
}
//---------------------------------------------------------------------------
/*!
 �E�B���h�E�n���h������A���̃��W���[���̃t���p�X���擾���܂��B
 @param[in] hWnd ���ׂ�E�B���h�E�̃n���h��
 @param[in] lpszFileName ���W���[���̊��S�C���p�X���󂯎��o�b�t�@�ւ̃|�C���^
 @param[in] nSize �擾�����������̍ő�̒���
 @return ���W���[���̃t���p�X�����擾�ł����� TRUE ���Ԃ�܂��B����ȊO�� FALSE ���Ԃ�܂��B
*/
BOOL		GetFileNameFromHwnd(HWND hWnd, LPTSTR lpszFileName, DWORD nSize)
{
	BOOL bResult = FALSE;
	// �E�B���h�E���쐬�����v���Z�X�� ID ���擾
	DWORD dwProcessId;
	GetWindowThreadProcessId( hWnd , &dwProcessId );
	
	// ���ݎ��s���Ă��� OS �̃o�[�W���������擾
	OSVERSIONINFO osverinfo;
	osverinfo.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
	if ( !GetVersionEx( &osverinfo ) )
	{
		return FALSE;
	}
	
	// �v���b�g�t�H�[���� Windows NT/2000 �̏ꍇ
	if ( osverinfo.dwPlatformId == VER_PLATFORM_WIN32_NT )
	{
		// PSAPI �֐��̃|�C���^
		BOOL (WINAPI *lpfEnumProcessModules)
							(HANDLE, HMODULE*, DWORD, LPDWORD);
		DWORD (WINAPI *lpfGetModuleFileNameEx)
							(HANDLE, HMODULE, LPTSTR, DWORD);
		
		// PSAPI.DLL ���C�u���������[�h
		HINSTANCE hInstLib = LoadLibrary( "PSAPI.DLL" );
		if ( hInstLib == NULL )
		{
			return FALSE ;
		}
		
		// �v���V�[�W���̃A�h���X���擾
		lpfEnumProcessModules = (BOOL(WINAPI *)
			(HANDLE, HMODULE *, DWORD, LPDWORD))GetProcAddress(
						hInstLib, "EnumProcessModules");
		lpfGetModuleFileNameEx =(DWORD (WINAPI *)
			(HANDLE, HMODULE, LPTSTR, DWORD))GetProcAddress(
						hInstLib, "GetModuleFileNameExA");
		
		if ( lpfEnumProcessModules && lpfGetModuleFileNameEx )
		{
			// �v���Z�X�I�u�W�F�N�g�̃n���h�����擾
			HANDLE hProcess;
			hProcess = OpenProcess(
					PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
					FALSE, dwProcessId);
			if ( hProcess )
			{
				// .EXE ���W���[���̃n���h�����擾
				HMODULE hModule;
				DWORD dwNeed;
				if ( lpfEnumProcessModules( hProcess, &hModule, sizeof( hModule ), &dwNeed ) )
				{
					// ���W���[���n���h������t���p�X�����擾
					if ( lpfGetModuleFileNameEx( hProcess, hModule, lpszFileName, nSize ) )
					{
						bResult = TRUE;
					}
				}
				// �v���Z�X�I�u�W�F�N�g�̃n���h�����N���[�Y
				CloseHandle( hProcess ) ;
			}
		}
		// PSAPI.DLL ���C�u�������J��
		FreeLibrary( hInstLib ) ;
	}
	// �v���b�g�t�H�[���� Windows 9x �̏ꍇ
	else if ( osverinfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
	{
		// ToolHelp �֐��|�C���^
		HANDLE (WINAPI *lpfCreateSnapshot)(DWORD, DWORD);
		BOOL (WINAPI *lpfProcess32First)(HANDLE, LPPROCESSENTRY32);
		BOOL (WINAPI *lpfProcess32Next)(HANDLE, LPPROCESSENTRY32);
		
		// �v���V�[�W���̃A�h���X���擾
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
		
		// �V�X�e�� �v���Z�X�� Toolhelp �X�i�b�v�V���b�g���쐬
		HANDLE hSnapshot;
		hSnapshot = lpfCreateSnapshot( TH32CS_SNAPPROCESS , 0 );
		if ( hSnapshot != (HANDLE)-1 )
		{
			// �ŏ��̃v���Z�X�Ɋւ�������擾
			PROCESSENTRY32 pe;
			pe.dwSize = sizeof( PROCESSENTRY32 );
			if ( lpfProcess32First( hSnapshot, &pe ) )
			{
				do
				{
					// �����v���Z�XID �ł���΁A�t�@�C�������擾
					if ( pe.th32ProcessID == dwProcessId )
					{
						lstrcpy( lpszFileName, pe.szExeFile );
						bResult = TRUE;
						break;
					}
				} while ( lpfProcess32Next( hSnapshot, &pe ) );
			}
			// �X�i�b�v�V���b�g��j��
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
 �R���{�{�b�N�X�ւ̗����ɒǉ��Ǘ�
 @param[in] cmb �Ώۂ̃R���{�{�b�N�X
 @return 0:���ǉ� 1:�ǉ�
*/
int		SetComboBoxHistory(TComboBox *cmb)
{
	// ��͑ΏۊO
	if ( !cmb->Text.IsEmpty() )
	{
		AnsiString	cmbText	= cmb->Text;
		
		// �������̂���������ړ�����
		for ( int i = 0; i < cmb->Items->Count; i++ )
		{
			if ( !cmb->Items->Strings[i].AnsiCompare( cmbText ) )
			{
				cmb->Items->Move( i, 0 );
				
				// �e�L�X�g�ݒ�
				cmb->ItemIndex = 0;
				return 1;	// ���ǂ�[
			}
		}
		
		// ���ɑ}��
		cmb->Items->Insert( 0, cmbText );
		
		// �ő�l����
		if ( Conbo_box_history_max < cmb->Items->Count )
		{
			cmb->Items->Delete( cmb->Items->Count - 1 );
		}
		
		// �e�L�X�g�ݒ�
		cmb->ItemIndex = 0;
		
		return 1;
	}
	
	return 0;
}
//---------------------------------------------------------------------------
/*!
 �R���{�{�b�N�X�̓�d�o�^�̃`�F�b�N
 @param[in] cmb �Ώۂ̃R���{�{�b�N�X
 @param[in] text �`�F�b�N������������
 @return 0:���łɂ���܂� 1:�V�K�ł�
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
