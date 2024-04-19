//---------------------------------------------------------------------------

#ifndef _BCBCOMMON_H_ 
#define _BCBCOMMON_H_

//---------------------------------------------------------------------------

static const int Conbo_box_history_max = 32;

//---------------------------------------------------------------------------

void	systemShutdown(const int type);
static void	SystemShutdown() { systemShutdown( EWX_POWEROFF/*EWX_SHUTDOWN*/ | EWX_FORCE ); }
static void	SystemReboot() { systemShutdown( EWX_REBOOT ); }
static void	SystemLogoff() { systemShutdown( EWX_LOGOFF ); }

void	SendString(HWND hWnd, const char *str);
void	DelDir(AnsiString DirName);
DWORD	fileOperationFromList(HWND Handle, TStringList *list, long wFunc, long fFlags, const char *To);
BOOL	GetFileNameFromHwnd(HWND hWnd, LPTSTR lpszFileName, DWORD nSize);
int		SetComboBoxHistory(TComboBox *cmb);
int		ComboBoxExistCheck(TComboBox *cmb, AnsiString text);


//-----------------------------------------------------------------------------
/*!
 ファイルの削除(TStringList)
 @param[in] Handle
 @param[out] list
 @param[in] undo できる限りアンドゥ情報を保持するようにします
*/
static __inline DWORD DeleteFileFromList(HWND Handle, TStringList *list, bool undo)
{
	long fFlags;
	
	if ( undo ) { fFlags = FOF_ALLOWUNDO; }
	else        { fFlags = NULL;          }
	
	return fileOperationFromList( Handle, list, FO_DELETE, fFlags, NULL );
}
//-----------------------------------------------------------------------------
/*!
 ファイルのコピー(TStringList)
 @param[in] Handle
 @param[out] list
 @param[in] ToCopy
 @param[in] newName 指定したファイル名がすでに存在していた場合には、操作対象のファイルに新しい名前を付けます。
*/
static __inline DWORD CopyFileFromList(HWND Handle, TStringList *list, AnsiString ToCopy, bool newName)
{
	long fFlags;
	
	if ( newName ) { fFlags = FOF_ALLOWUNDO | FOF_RENAMEONCOLLISION; }
	else           { fFlags = FOF_ALLOWUNDO;                         }
	
	return fileOperationFromList( Handle, list, FO_COPY, fFlags, ToCopy.c_str() );
}
//-----------------------------------------------------------------------------
/*!
 ファイルの移動(TStringList)
 @param[in] Handle
 @param[out] list
 @param[in] ToMove
*/
static __inline DWORD MoveFileFromList(HWND Handle, TStringList *list, AnsiString ToMove)
{
	return fileOperationFromList( Handle, list, FO_MOVE, FOF_ALLOWUNDO, ToMove.c_str() );
}
//-----------------------------------------------------------------------------
/*!
 名前の変更(TStringList)
 @param[in] Handle
 @param[out] list
 @param[in] ToName
*/
static __inline DWORD RenameFileFromList(HWND Handle, TStringList *list, AnsiString ToName)
{
	return fileOperationFromList( Handle, list, FO_RENAME, FOF_ALLOWUNDO, ToName.c_str() );
}

//---------------------------------------------------------------------------
#endif // _BCBCOMMON_H_
