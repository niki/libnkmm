
#ifndef SPECIAL_PATH_NAME_H
#define SPECIAL_PATH_NAME_H

/*
http://technet.microsoft.com/ja-jp/library/cc749104%28v=ws.10%29.aspx

オペレーティング システムに対して、および各ユーザーのコンテキストで処理される変数
ALLUSERSAPPDATA					CSIDL_COMMON_APPDATA と同じです。
ALLUSERSPROFILE					%PROFILESFOLDER%\Public または %PROFILESFOLDER%\all users を参照します。
COMMONPROGRAMFILES				CSIDL_PROGRAM_FILES_COMMON と同じです。
COMMONPROGRAMFILES(X86)			64 ビット システムの C:\Program Files (x86)\Common Files フォルダを参照します。
CSIDL_COMMON_ADMINTOOLS			Version 5.0。コンピュータのすべてのユーザーの管理ツールを含むファイル システム ディレクトリ。
CSIDL_COMMON_ALTSTARTUP			すべてのユーザーのローカライズされていないスタートアップ プログラム グループに対応するファイル システム ディレクトリ。
CSIDL_COMMON_APPDATA			Version 5.0。すべてのユーザーのアプリケーション データを含むファイル システム ディレクトリ。一般的なパスは、C:\Documents and Settings\All Users\Application Data です。
CSIDL_COMMON_DESKTOPDIRECTORY	すべてのユーザーのデスクトップに表示されるファイルとフォルダを含むファイル システム ディレクトリ。一般的なパスは、C:\Documents and Settings\All Users\Desktop です。
CSIDL_COMMON_DOCUMENTS			すべてのユーザーに共通のドキュメントを含むファイル システム ディレクトリ。一般的なパスは、C:\Documents and Settings\All Users\Documents です。
CSIDL_COMMON_FAVORITES			すべてのユーザーに共通のお気に入りの項目の一般的なレポジトリとして機能するファイル システム ディレクトリ。
CSIDL_COMMON_MUSIC				Version 6.0。すべてのユーザーに共通の音楽ファイルのレポジトリとして機能するファイル システム ディレクトリ。一般的なパスは、C:\Documents and Settings\All Users\Documents\My Music です。
CSIDL_COMMON_PICTURES			Version 6.0。すべてのユーザーに共通のイメージ ファイルのレポジトリとして機能するファイル システム ディレクトリ。一般的なパスは、C:\Documents and Settings\All Users\Documents\My Pictures です。
CSIDL_COMMON_PROGRAMS			すべてのユーザーの [スタート] メニューに表示される共通のプログラム グループのディレクトリを含むファイル システム ディレクトリ。一般的なパスは、C:\Documents and Settings\All Users\Start Menu\Programs です。
CSIDL_COMMON_STARTMENU			すべてのユーザーの [スタート] メニューに表示されるプログラムとフォルダを含むファイル システム ディレクトリ。一般的なパスは、C:\Documents and Settings\All Users\Start Menu です。
CSIDL_COMMON_STARTUP			すべてのユーザーのスタートアップ フォルダに表示されるプログラムを含むファイル システム ディレクトリ。一般的なパスは、C:\Documents and Settings\All Users\Start Menu\Programs\Startup です。
CSIDL_COMMON_TEMPLATES			すべてのユーザーが使用できるテンプレートを含むファイル システム ディレクトリ。一般的なパスは、C:\Documents and Settings\All Users\Templates です。
CSIDL_COMMON_VIDEO				Version 6.0。すべてのユーザーに共通のビデオ ファイルのレポジトリとして機能するファイル システム ディレクトリ。一般的なパスは、C:\Documents and Settings\All Users\Documents\My Videos です。
CSIDL_DEFAULT_APPDATA			%DEFAULTUSERPROFILE% 内の Appdata フォルダを参照します。
CSIDL_DEFAULT_LOCAL_APPDATA		%DEFAULTUSERPROFILE% 内のローカル Appdata フォルダを参照します。
CSIDL_DEFAULT_COOKIES			%DEFAULTUSERPROFILE% 内の Cookies フォルダを参照します。
CSIDL_DEFAULT_CONTACTS			%DEFAULTUSERPROFILE% 内のアドレス帳フォルダを参照します。
CSIDL_DEFAULT_DESKTOP			%DEFAULTUSERPROFILE% 内のデスクトップ フォルダを参照します。
CSIDL_DEFAULT_DOWNLOADS			%DEFAULTUSERPROFILE% 内のダウンロード フォルダを参照します。
CSIDL_DEFAULT_FAVORITES			%DEFAULTUSERPROFILE% 内のお気に入りフォルダを参照します。
CSIDL_DEFAULT_HISTORY			%DEFAULTUSERPROFILE% 内の履歴フォルダを参照します。
CSIDL_DEFAULT_INTERNET_CACHE	%DEFAULTUSERPROFILE% 内のインターネット キャッシュ フォルダを参照します。
CSIDL_DEFAULT_PERSONAL			%DEFAULTUSERPROFILE% 内の個人用フォルダを参照します。
CSIDL_DEFAULT_MYDOCUMENTS		%DEFAULTUSERPROFILE% 内のマイ ドキュメント フォルダを参照します。
CSIDL_DEFAULT_MYPICTURES		%DEFAULTUSERPROFILE% 内のマイ ピクチャ フォルダを参照します。
CSIDL_DEFAULT_MYMUSIC			%DEFAULTUSERPROFILE% 内のマイ ミュージック フォルダを参照します。
CSIDL_DEFAULT_MYVIDEO			%DEFAULTUSERPROFILE% 内のマイ ビデオ フォルダを参照します。
CSIDL_DEFAULT_RECENT			%DEFAULTUSERPROFILE% 内の Recent フォルダを参照します。
CSIDL_DEFAULT_SENDTO			%DEFAULTUSERPROFILE% 内の送るフォルダを参照します。
CSIDL_DEFAULT_STARTMENU			%DEFAULTUSERPROFILE% 内のスタート メニュー フォルダを参照します。
CSIDL_DEFAULT_PROGRAMS			%DEFAULTUSERPROFILE% 内のプログラム フォルダを参照します。
CSIDL_DEFAULT_STARTUP			%DEFAULTUSERPROFILE% 内のスタートアップ フォルダを参照します。
CSIDL_DEFAULT_TEMPLATES			%DEFAULTUSERPROFILE% 内のテンプレート フォルダを参照します。
CSIDL_DEFAULT_QUICKLAUNCH		%DEFAULTUSERPROFILE% 内のクイック起動フォルダを参照します。
CSIDL_FONTS						フォントを含む仮想フォルダ。一般的なパスは、C:\Windows\Fonts です。
CSIDL_PROGRAM_FILESX86			64 ビット システム上の Program Files フォルダ。一般的なパスは、C:\Program Files(86) です。
CSIDL_PROGRAM_FILES_COMMONX86	64 ビット システムのアプリケーションで共有されるコンポーネントのフォルダ。一般的なパスは、C:\Program Files(86)\Common です。
CSIDL_PROGRAM_FILES				Version 5.0。Program Files フォルダ。一般的なパスは、C:\Program Files です。
CSIDL_PROGRAM_FILES_COMMON		Version 5.0。アプリケーションで共有されるコンポーネントのフォルダ。一般的なパスは、C:\Program Files\Common です。Windows NT、Windows 2000、および Windows XP が実行されているコンピュータにのみ有効です。Windows Millennium Edition には有効でありません。
CSIDL_RESOURCES					Windows Vista が実行されているコンピュータの、リソース データを含むファイル システム ディレクトリ。一般的なパスは、C:\Windows\Resources です。
CSIDL_SYSTEM					Version 5.0。Windows のシステム フォルダ。一般的なパスは、C:\Windows\System32 です。
CSIDL_WINDOWS					Version 5.0。Windows ディレクトリまたは SYSROOT。これは、環境変数 %WINDIR% または %SYSTEMROOT% に対応します。一般的なパスは、C:\Windows です。
DEFAULTUSERPROFILE				HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\ProfileList [DefaultUserProfile] 内の値を参照します。
PROFILESFOLDER					HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\ProfileList [ProfilesDirectory] 内の値を参照します。
PROGRAMFILES					CSIDL_PROGRAM_FILES と同じです。
PROGRAMFILES(X86)				64 ビット システムの C:\Program Files (x86) フォルダを参照します。
SYSTEM							%WINDIR%\system32 を参照します。
SYSTEM16						%WINDIR%\system を参照します。
SYSTEM32						%WINDIR%\system32 を参照します。
SYSTEMPROFILE					HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\ProfileList\\S-1-5-18 [ProfileImagePath] 内の値を参照します。
SYSTEMROOT						システム ドライブのルートを参照します。
WINDIR							システム ドライブにある Windows フォルダを参照します。

ユーザー コンテキストでのみ認識される変数
APPDATA							CSIDL_APPDATA と同じです。
CSIDL_ADMINTOOLS				個々のユーザーの管理ツールを格納するために使用されるファイル システム ディレクトリ。Microsoft 管理コンソール (MMC) では、カスタマイズされたコンソールがこのディレクトリに保存され、ユーザーと共に移動できるようになります。
CSIDL_ALTSTARTUP				ユーザーのローカライズされていないスタートアップ プログラム グループに対応するファイル システム ディレクトリ。
CSIDL_APPDATA					アプリケーション固有のデータの共通レポジトリとして機能するファイル システム ディレクトリ。一般的なパスは、C:\Documents and Settings\username\Application Data です。この CSIDL は、Microsoft Internet Explorer 4.0 が統合されたシェルがインストールされていないシステムに対し、再配布可能な Shfolder.dll によってサポートされます。
CSIDL_BITBUCKET					ユーザーのごみ箱のオブジェクトを含む仮想フォルダ。
CSIDL_CDBURN_AREA				CD への書き込みを待機しているファイルのステージング領域として機能するファイル システム ディレクトリ。一般的なパスは、C:\Documents and Settings\username\Local Settings\Application Data\Microsoft\CD Burning です。
CSIDL_CONNECTIONS				ネットワークとダイヤルアップ接続を含むネットワーク接続を示す仮想フォルダ。
CSIDL_CONTACTS					Windows Vista が実行されているコンピュータ上で、%CSIDL_PROFILE% のアドレス帳フォルダを参照します。
CSIDL_CONTROLS					コントロール パネル アプリケーションのアイコンを含む仮想フォルダ。
CSIDL_COOKIES					インターネット Cookie の共通レポジトリとして機能するファイル システム ディレクトリ。一般的なパスは、C:\Documents and Settings\username\Cookies です。
CSIDL_DESKTOP					Windows デスクトップ、名前空間のルートを示す仮想フォルダ。
CSIDL_DESKTOPDIRECTORY			デスクトップ上のファイル オブジェクトを物理的に格納するために使用されるファイル システム ディレクトリ (デスクトップ フォルダ自体と混同しないでください)。一般的なパスは、C:\Documents and Settings\username\Desktop です。
CSIDL_DRIVES					記憶装置、プリンタ、コントロール パネルなど、ローカル コンピュータ上のすべてを含むマイ コンピュータを示す仮想フォルダ。フォルダには、マップされたネットワーク ドライブも含まれている場合があります。
CSIDL_FAVORITES					ユーザーのお気に入りの項目の共通レポジトリとして機能するファイル システム ディレクトリ。一般的なパスは、C:\Documents and Settings\username\Favorites です。
CSIDL_HISTORY					インターネット履歴項目の共通レポジトリとして機能するファイル システム ディレクトリ。
CSIDL_INTERNET					Internet Explorer の仮想フォルダ (デスクトップ上のアイコン)。
CSIDL_INTERNET_CACHE			一時インターネット ファイルの共通レポジトリとして機能するファイル システム ディレクトリ。一般的なパスは、C:\Documents and Settings\username\Local Settings\Temporary Internet Files です。
CSIDL_LOCAL_APPDATA				ローカル (移動しない) アプリケーションのデータ レポジトリとして機能するファイル システム ディレクトリ。一般的なパスは、C:\Documents and Settings\username\Local Settings\Application Data です。
CSIDL_MYDOCUMENTS				マイ ドキュメント デスクトップ項目を示す仮想フォルダ。
CSIDL_MYMUSIC					音楽ファイルの共通レポジトリとして機能するファイル システム ディレクトリ。一般的なパスは、C:\Documents and Settings\User\My Documents\My Music です。
CSIDL_MYPICTURES				イメージ ファイルの共通レポジトリとして機能するファイル システム ディレクトリ。一般的なパスは、C:\Documents and Settings\username\My Documents\My Pictures です。
CSIDL_MYVIDEO					ビデオ ファイルの共通レポジトリとして機能するファイル システム ディレクトリ。一般的なパスは、C:\Documents and Settings\username\My Documents\My Videos です。
CSIDL_NETHOOD					マイ ネットワーク仮想フォルダに存在する可能性のあるリンク オブジェクトを含むファイル システム ディレクトリ。ネットワーク名前空間のルートを示す CSIDL_NETWORK とは異なります。一般的なパスは、C:\Documents and Settings\username\NetHood です。
CSIDL_NETWORK					マイ ネットワーク、ネットワーク名前空間階層のルートを示す仮想フォルダ。
CSIDL_PERSONAL					Version 6.0。マイ ドキュメント デスクトップ項目を示す仮想フォルダ。これは CSIDL_MYDOCUMENTS と同じです。
CSIDL_PLAYLISTS					Windows Vista が実行されているコンピュータの、再生アルバムを格納するために使用される仮想フォルダ。通常は、username\My Music\Playlists です。
CSIDL_PRINTERS					インストールしたプリンタを含む仮想フォルダ。
CSIDL_PRINTHOOD					プリンタ仮想フォルダに存在する可能性のあるリンク オブジェクトを含むファイル システム ディレクトリ。一般的なパスは、C:\Documents and Settings\username\PrintHood です。
CSIDL_PROFILE					ユーザーのプロファイル フォルダ。一般的なパスは、C:\Documents and Settings\username です。アプリケーションによってファイルやフォルダがこのレベルで作成されるべきではありません。アプリケーションでは、CSIDL_APPDATA または CSIDL_LOCAL_APPDATA によって参照される場所の下にデータが保存される必要があります。
CSIDL_PROGRAMS					ユーザーのプログラム グループ (それら自体がファイル システム ディレクトリ) を含むファイル システム ディレクトリ。一般的なパスは、C:\Documents and Settings\username\Start Menu\Programs です。
CSIDL_RECENT					ユーザーの最も最近使ったファイルへのショートカットを含むファイル システム ディレクトリ。一般的なパスは、C:\Documents and Settings\username\My Recent Documents です。このフォルダでショートカットを作成するには、SHAddToRecentDocs を使用します。この関数では、ショートカットが作成されるほか、シェルの最近使ったファイルの一覧が更新され、ショートカットが [スタート] メニューの [最近使ったファイル] サブメニューに追加されます。
CSIDL_SENDTO					[送る] メニュー項目を含むファイル システム ディレクトリ。一般的なパスは、C:\Documents and Settings\username\SendTo です。
CSIDL_STARTMENU					[スタート] メニュー項目を含むファイル システム ディレクトリ。一般的なパスは、C:\Documents and Settings\username\Start Menu です。
CSIDL_STARTUP					ユーザーのスタートアップ プログラム グループに対応するファイル システム ディレクトリ。一般的なパスは、C:\Documents and Settings\username\Start Menu\Programs\Startup です。
CSIDL_TEMPLATES					ドキュメント テンプレートの共通レポジトリとして機能するファイル システム ディレクトリ。一般的なパスは、C:\Documents and Settings\username\Templates です。
HOMEPATH						%USERPROFILE% と同じです。
TEMP							コンピュータ上の一時フォルダ。Windows XP の場合、一般的なパスは %USERPROFILE%\Local Settings\Temp です。Windows Vista の場合、一般的なパスは %USERPROFILE%\AppData\Local\Temp です。
TMP								コンピュータ上の一時フォルダ。Windows XP の場合、一般的なパスは %USERPROFILE%\Local Settings\Temp です。Windows Vista の場合、一般的なパスは %USERPROFILE%\AppData\Local\Temp です。
USERPROFILE						CSIDL_PROFILE と同じです。
USERSID							現在のユーザー アカウント セキュリティ識別子 (SID) を示します。たとえば、S-1-5-21-1714567821-1326601894-715345443-1026 など。
*/

static const struct {
  char *Name;
  int Id;
} s_folderId[] = {
    {"%Desktop%", CSIDL_DESKTOP},                     //デスクトップ
    {"%Program%", CSIDL_PROGRAMS},                    //スタートメニュー\プログラム
    {"%ControlPanel%", CSIDL_CONTROLS},               //コントロール パネル
    {"%MyDocment%", CSIDL_PERSONAL},                  //マイドキュメント
    {"%Favorite%", CSIDL_FAVORITES},                  //お気に入り
    {"%Startup%", CSIDL_STARTUP},                     //スタートアップ
    {"%Recent%", CSIDL_RECENT},                       //最近使ったファイル
    {"%SendTo%", CSIDL_SENDTO},                       //送る
    {"%BitBucket%", CSIDL_BITBUCKET},                 //ごみ箱
    {"%StartMenu%", CSIDL_STARTMENU},                 //スタートメニュー
    {"%MyComputer%", CSIDL_DRIVES},                   //マイ コンピュータ
    {"%MyNetwork%", CSIDL_NETWORK},                   //マイネットワーク
    {"%Network%", CSIDL_NETHOOD},                     //ネットワーク
    {"%Font%", CSIDL_FONTS},                          //フォント
    {"%ApplicationData%", CSIDL_APPDATA},             //アプリケーションデータ
    {"%LocalApplicationData%", CSIDL_LOCAL_APPDATA},  //ローカルアプリケーションデータ
    {"%Windows%", CSIDL_WINDOWS},                     //Windowsディレクトリ
    {"%System%", CSIDL_SYSTEM},                       //Systemディレクトリ
    {"%ProgramFiles%", CSIDL_PROGRAM_FILES},          //C:\Program Files
    {"%MyPicture%", CSIDL_MYPICTURES},                //マイピクチャ

    {"%AllUsers StartMenu%", CSIDL_COMMON_STARTMENU},  //C:\WINDOWS\All Users\スタート メニュー
    {"%AllUsers Program%",
     CSIDL_COMMON_PROGRAMS},  //C:\WINDOWS\All Users\スタート メニュー\プログラム
    {"%AllUsers Startup%",
     CSIDL_COMMON_STARTUP},  //C:\WINDOWS\All Users\スタート メニュー\プログラム\スタートアップ
    {"%AllUsers Favorite%", CSIDL_COMMON_FAVORITES},  //C:\WINDOWS\All Users\Favorites
};

static const int s_folderMax = sizeof(s_folderId) / sizeof(s_folderId[0]);

#if 0
inline static int getSpecialFolderID(const char *name)
{
	for( int i = 0; i < s_folderMax; i++ ) {
		if ( strcmp( s_folderId[ i ].Name, name ) == 0 ) {
			return s_folderId[ i ].Id;
		}
	}
	return -1;
}
#endif

#endif  //SPECIAL_PATH_NAME_H
