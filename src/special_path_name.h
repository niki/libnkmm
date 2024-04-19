
#ifndef SPECIAL_PATH_NAME_H
#define SPECIAL_PATH_NAME_H

/*
http://technet.microsoft.com/ja-jp/library/cc749104%28v=ws.10%29.aspx

�I�y���[�e�B���O �V�X�e���ɑ΂��āA����ъe���[�U�[�̃R���e�L�X�g�ŏ��������ϐ�
ALLUSERSAPPDATA					CSIDL_COMMON_APPDATA �Ɠ����ł��B
ALLUSERSPROFILE					%PROFILESFOLDER%\Public �܂��� %PROFILESFOLDER%\all users ���Q�Ƃ��܂��B
COMMONPROGRAMFILES				CSIDL_PROGRAM_FILES_COMMON �Ɠ����ł��B
COMMONPROGRAMFILES(X86)			64 �r�b�g �V�X�e���� C:\Program Files (x86)\Common Files �t�H���_���Q�Ƃ��܂��B
CSIDL_COMMON_ADMINTOOLS			Version 5.0�B�R���s���[�^�̂��ׂẴ��[�U�[�̊Ǘ��c�[�����܂ރt�@�C�� �V�X�e�� �f�B���N�g���B
CSIDL_COMMON_ALTSTARTUP			���ׂẴ��[�U�[�̃��[�J���C�Y����Ă��Ȃ��X�^�[�g�A�b�v �v���O���� �O���[�v�ɑΉ�����t�@�C�� �V�X�e�� �f�B���N�g���B
CSIDL_COMMON_APPDATA			Version 5.0�B���ׂẴ��[�U�[�̃A�v���P�[�V���� �f�[�^���܂ރt�@�C�� �V�X�e�� �f�B���N�g���B��ʓI�ȃp�X�́AC:\Documents and Settings\All Users\Application Data �ł��B
CSIDL_COMMON_DESKTOPDIRECTORY	���ׂẴ��[�U�[�̃f�X�N�g�b�v�ɕ\�������t�@�C���ƃt�H���_���܂ރt�@�C�� �V�X�e�� �f�B���N�g���B��ʓI�ȃp�X�́AC:\Documents and Settings\All Users\Desktop �ł��B
CSIDL_COMMON_DOCUMENTS			���ׂẴ��[�U�[�ɋ��ʂ̃h�L�������g���܂ރt�@�C�� �V�X�e�� �f�B���N�g���B��ʓI�ȃp�X�́AC:\Documents and Settings\All Users\Documents �ł��B
CSIDL_COMMON_FAVORITES			���ׂẴ��[�U�[�ɋ��ʂ̂��C�ɓ���̍��ڂ̈�ʓI�ȃ��|�W�g���Ƃ��ċ@�\����t�@�C�� �V�X�e�� �f�B���N�g���B
CSIDL_COMMON_MUSIC				Version 6.0�B���ׂẴ��[�U�[�ɋ��ʂ̉��y�t�@�C���̃��|�W�g���Ƃ��ċ@�\����t�@�C�� �V�X�e�� �f�B���N�g���B��ʓI�ȃp�X�́AC:\Documents and Settings\All Users\Documents\My Music �ł��B
CSIDL_COMMON_PICTURES			Version 6.0�B���ׂẴ��[�U�[�ɋ��ʂ̃C���[�W �t�@�C���̃��|�W�g���Ƃ��ċ@�\����t�@�C�� �V�X�e�� �f�B���N�g���B��ʓI�ȃp�X�́AC:\Documents and Settings\All Users\Documents\My Pictures �ł��B
CSIDL_COMMON_PROGRAMS			���ׂẴ��[�U�[�� [�X�^�[�g] ���j���[�ɕ\������鋤�ʂ̃v���O���� �O���[�v�̃f�B���N�g�����܂ރt�@�C�� �V�X�e�� �f�B���N�g���B��ʓI�ȃp�X�́AC:\Documents and Settings\All Users\Start Menu\Programs �ł��B
CSIDL_COMMON_STARTMENU			���ׂẴ��[�U�[�� [�X�^�[�g] ���j���[�ɕ\�������v���O�����ƃt�H���_���܂ރt�@�C�� �V�X�e�� �f�B���N�g���B��ʓI�ȃp�X�́AC:\Documents and Settings\All Users\Start Menu �ł��B
CSIDL_COMMON_STARTUP			���ׂẴ��[�U�[�̃X�^�[�g�A�b�v �t�H���_�ɕ\�������v���O�������܂ރt�@�C�� �V�X�e�� �f�B���N�g���B��ʓI�ȃp�X�́AC:\Documents and Settings\All Users\Start Menu\Programs\Startup �ł��B
CSIDL_COMMON_TEMPLATES			���ׂẴ��[�U�[���g�p�ł���e���v���[�g���܂ރt�@�C�� �V�X�e�� �f�B���N�g���B��ʓI�ȃp�X�́AC:\Documents and Settings\All Users\Templates �ł��B
CSIDL_COMMON_VIDEO				Version 6.0�B���ׂẴ��[�U�[�ɋ��ʂ̃r�f�I �t�@�C���̃��|�W�g���Ƃ��ċ@�\����t�@�C�� �V�X�e�� �f�B���N�g���B��ʓI�ȃp�X�́AC:\Documents and Settings\All Users\Documents\My Videos �ł��B
CSIDL_DEFAULT_APPDATA			%DEFAULTUSERPROFILE% ���� Appdata �t�H���_���Q�Ƃ��܂��B
CSIDL_DEFAULT_LOCAL_APPDATA		%DEFAULTUSERPROFILE% ���̃��[�J�� Appdata �t�H���_���Q�Ƃ��܂��B
CSIDL_DEFAULT_COOKIES			%DEFAULTUSERPROFILE% ���� Cookies �t�H���_���Q�Ƃ��܂��B
CSIDL_DEFAULT_CONTACTS			%DEFAULTUSERPROFILE% ���̃A�h���X���t�H���_���Q�Ƃ��܂��B
CSIDL_DEFAULT_DESKTOP			%DEFAULTUSERPROFILE% ���̃f�X�N�g�b�v �t�H���_���Q�Ƃ��܂��B
CSIDL_DEFAULT_DOWNLOADS			%DEFAULTUSERPROFILE% ���̃_�E�����[�h �t�H���_���Q�Ƃ��܂��B
CSIDL_DEFAULT_FAVORITES			%DEFAULTUSERPROFILE% ���̂��C�ɓ���t�H���_���Q�Ƃ��܂��B
CSIDL_DEFAULT_HISTORY			%DEFAULTUSERPROFILE% ���̗����t�H���_���Q�Ƃ��܂��B
CSIDL_DEFAULT_INTERNET_CACHE	%DEFAULTUSERPROFILE% ���̃C���^�[�l�b�g �L���b�V�� �t�H���_���Q�Ƃ��܂��B
CSIDL_DEFAULT_PERSONAL			%DEFAULTUSERPROFILE% ���̌l�p�t�H���_���Q�Ƃ��܂��B
CSIDL_DEFAULT_MYDOCUMENTS		%DEFAULTUSERPROFILE% ���̃}�C �h�L�������g �t�H���_���Q�Ƃ��܂��B
CSIDL_DEFAULT_MYPICTURES		%DEFAULTUSERPROFILE% ���̃}�C �s�N�`�� �t�H���_���Q�Ƃ��܂��B
CSIDL_DEFAULT_MYMUSIC			%DEFAULTUSERPROFILE% ���̃}�C �~���[�W�b�N �t�H���_���Q�Ƃ��܂��B
CSIDL_DEFAULT_MYVIDEO			%DEFAULTUSERPROFILE% ���̃}�C �r�f�I �t�H���_���Q�Ƃ��܂��B
CSIDL_DEFAULT_RECENT			%DEFAULTUSERPROFILE% ���� Recent �t�H���_���Q�Ƃ��܂��B
CSIDL_DEFAULT_SENDTO			%DEFAULTUSERPROFILE% ���̑���t�H���_���Q�Ƃ��܂��B
CSIDL_DEFAULT_STARTMENU			%DEFAULTUSERPROFILE% ���̃X�^�[�g ���j���[ �t�H���_���Q�Ƃ��܂��B
CSIDL_DEFAULT_PROGRAMS			%DEFAULTUSERPROFILE% ���̃v���O���� �t�H���_���Q�Ƃ��܂��B
CSIDL_DEFAULT_STARTUP			%DEFAULTUSERPROFILE% ���̃X�^�[�g�A�b�v �t�H���_���Q�Ƃ��܂��B
CSIDL_DEFAULT_TEMPLATES			%DEFAULTUSERPROFILE% ���̃e���v���[�g �t�H���_���Q�Ƃ��܂��B
CSIDL_DEFAULT_QUICKLAUNCH		%DEFAULTUSERPROFILE% ���̃N�C�b�N�N���t�H���_���Q�Ƃ��܂��B
CSIDL_FONTS						�t�H���g���܂މ��z�t�H���_�B��ʓI�ȃp�X�́AC:\Windows\Fonts �ł��B
CSIDL_PROGRAM_FILESX86			64 �r�b�g �V�X�e����� Program Files �t�H���_�B��ʓI�ȃp�X�́AC:\Program Files(86) �ł��B
CSIDL_PROGRAM_FILES_COMMONX86	64 �r�b�g �V�X�e���̃A�v���P�[�V�����ŋ��L�����R���|�[�l���g�̃t�H���_�B��ʓI�ȃp�X�́AC:\Program Files(86)\Common �ł��B
CSIDL_PROGRAM_FILES				Version 5.0�BProgram Files �t�H���_�B��ʓI�ȃp�X�́AC:\Program Files �ł��B
CSIDL_PROGRAM_FILES_COMMON		Version 5.0�B�A�v���P�[�V�����ŋ��L�����R���|�[�l���g�̃t�H���_�B��ʓI�ȃp�X�́AC:\Program Files\Common �ł��BWindows NT�AWindows 2000�A����� Windows XP �����s����Ă���R���s���[�^�ɂ̂ݗL���ł��BWindows Millennium Edition �ɂ͗L���ł���܂���B
CSIDL_RESOURCES					Windows Vista �����s����Ă���R���s���[�^�́A���\�[�X �f�[�^���܂ރt�@�C�� �V�X�e�� �f�B���N�g���B��ʓI�ȃp�X�́AC:\Windows\Resources �ł��B
CSIDL_SYSTEM					Version 5.0�BWindows �̃V�X�e�� �t�H���_�B��ʓI�ȃp�X�́AC:\Windows\System32 �ł��B
CSIDL_WINDOWS					Version 5.0�BWindows �f�B���N�g���܂��� SYSROOT�B����́A���ϐ� %WINDIR% �܂��� %SYSTEMROOT% �ɑΉ����܂��B��ʓI�ȃp�X�́AC:\Windows �ł��B
DEFAULTUSERPROFILE				HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\ProfileList [DefaultUserProfile] ���̒l���Q�Ƃ��܂��B
PROFILESFOLDER					HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\ProfileList [ProfilesDirectory] ���̒l���Q�Ƃ��܂��B
PROGRAMFILES					CSIDL_PROGRAM_FILES �Ɠ����ł��B
PROGRAMFILES(X86)				64 �r�b�g �V�X�e���� C:\Program Files (x86) �t�H���_���Q�Ƃ��܂��B
SYSTEM							%WINDIR%\system32 ���Q�Ƃ��܂��B
SYSTEM16						%WINDIR%\system ���Q�Ƃ��܂��B
SYSTEM32						%WINDIR%\system32 ���Q�Ƃ��܂��B
SYSTEMPROFILE					HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\ProfileList\\S-1-5-18 [ProfileImagePath] ���̒l���Q�Ƃ��܂��B
SYSTEMROOT						�V�X�e�� �h���C�u�̃��[�g���Q�Ƃ��܂��B
WINDIR							�V�X�e�� �h���C�u�ɂ��� Windows �t�H���_���Q�Ƃ��܂��B

���[�U�[ �R���e�L�X�g�ł̂ݔF�������ϐ�
APPDATA							CSIDL_APPDATA �Ɠ����ł��B
CSIDL_ADMINTOOLS				�X�̃��[�U�[�̊Ǘ��c�[�����i�[���邽�߂Ɏg�p�����t�@�C�� �V�X�e�� �f�B���N�g���BMicrosoft �Ǘ��R���\�[�� (MMC) �ł́A�J�X�^�}�C�Y���ꂽ�R���\�[�������̃f�B���N�g���ɕۑ�����A���[�U�[�Ƌ��Ɉړ��ł���悤�ɂȂ�܂��B
CSIDL_ALTSTARTUP				���[�U�[�̃��[�J���C�Y����Ă��Ȃ��X�^�[�g�A�b�v �v���O���� �O���[�v�ɑΉ�����t�@�C�� �V�X�e�� �f�B���N�g���B
CSIDL_APPDATA					�A�v���P�[�V�����ŗL�̃f�[�^�̋��ʃ��|�W�g���Ƃ��ċ@�\����t�@�C�� �V�X�e�� �f�B���N�g���B��ʓI�ȃp�X�́AC:\Documents and Settings\username\Application Data �ł��B���� CSIDL �́AMicrosoft Internet Explorer 4.0 ���������ꂽ�V�F�����C���X�g�[������Ă��Ȃ��V�X�e���ɑ΂��A�Ĕz�z�\�� Shfolder.dll �ɂ���ăT�|�[�g����܂��B
CSIDL_BITBUCKET					���[�U�[�̂��ݔ��̃I�u�W�F�N�g���܂މ��z�t�H���_�B
CSIDL_CDBURN_AREA				CD �ւ̏������݂�ҋ@���Ă���t�@�C���̃X�e�[�W���O�̈�Ƃ��ċ@�\����t�@�C�� �V�X�e�� �f�B���N�g���B��ʓI�ȃp�X�́AC:\Documents and Settings\username\Local Settings\Application Data\Microsoft\CD Burning �ł��B
CSIDL_CONNECTIONS				�l�b�g���[�N�ƃ_�C�����A�b�v�ڑ����܂ރl�b�g���[�N�ڑ����������z�t�H���_�B
CSIDL_CONTACTS					Windows Vista �����s����Ă���R���s���[�^��ŁA%CSIDL_PROFILE% �̃A�h���X���t�H���_���Q�Ƃ��܂��B
CSIDL_CONTROLS					�R���g���[�� �p�l�� �A�v���P�[�V�����̃A�C�R�����܂މ��z�t�H���_�B
CSIDL_COOKIES					�C���^�[�l�b�g Cookie �̋��ʃ��|�W�g���Ƃ��ċ@�\����t�@�C�� �V�X�e�� �f�B���N�g���B��ʓI�ȃp�X�́AC:\Documents and Settings\username\Cookies �ł��B
CSIDL_DESKTOP					Windows �f�X�N�g�b�v�A���O��Ԃ̃��[�g���������z�t�H���_�B
CSIDL_DESKTOPDIRECTORY			�f�X�N�g�b�v��̃t�@�C�� �I�u�W�F�N�g�𕨗��I�Ɋi�[���邽�߂Ɏg�p�����t�@�C�� �V�X�e�� �f�B���N�g�� (�f�X�N�g�b�v �t�H���_���̂ƍ������Ȃ��ł�������)�B��ʓI�ȃp�X�́AC:\Documents and Settings\username\Desktop �ł��B
CSIDL_DRIVES					�L�����u�A�v�����^�A�R���g���[�� �p�l���ȂǁA���[�J�� �R���s���[�^��̂��ׂĂ��܂ރ}�C �R���s���[�^���������z�t�H���_�B�t�H���_�ɂ́A�}�b�v���ꂽ�l�b�g���[�N �h���C�u���܂܂�Ă���ꍇ������܂��B
CSIDL_FAVORITES					���[�U�[�̂��C�ɓ���̍��ڂ̋��ʃ��|�W�g���Ƃ��ċ@�\����t�@�C�� �V�X�e�� �f�B���N�g���B��ʓI�ȃp�X�́AC:\Documents and Settings\username\Favorites �ł��B
CSIDL_HISTORY					�C���^�[�l�b�g�������ڂ̋��ʃ��|�W�g���Ƃ��ċ@�\����t�@�C�� �V�X�e�� �f�B���N�g���B
CSIDL_INTERNET					Internet Explorer �̉��z�t�H���_ (�f�X�N�g�b�v��̃A�C�R��)�B
CSIDL_INTERNET_CACHE			�ꎞ�C���^�[�l�b�g �t�@�C���̋��ʃ��|�W�g���Ƃ��ċ@�\����t�@�C�� �V�X�e�� �f�B���N�g���B��ʓI�ȃp�X�́AC:\Documents and Settings\username\Local Settings\Temporary Internet Files �ł��B
CSIDL_LOCAL_APPDATA				���[�J�� (�ړ����Ȃ�) �A�v���P�[�V�����̃f�[�^ ���|�W�g���Ƃ��ċ@�\����t�@�C�� �V�X�e�� �f�B���N�g���B��ʓI�ȃp�X�́AC:\Documents and Settings\username\Local Settings\Application Data �ł��B
CSIDL_MYDOCUMENTS				�}�C �h�L�������g �f�X�N�g�b�v���ڂ��������z�t�H���_�B
CSIDL_MYMUSIC					���y�t�@�C���̋��ʃ��|�W�g���Ƃ��ċ@�\����t�@�C�� �V�X�e�� �f�B���N�g���B��ʓI�ȃp�X�́AC:\Documents and Settings\User\My Documents\My Music �ł��B
CSIDL_MYPICTURES				�C���[�W �t�@�C���̋��ʃ��|�W�g���Ƃ��ċ@�\����t�@�C�� �V�X�e�� �f�B���N�g���B��ʓI�ȃp�X�́AC:\Documents and Settings\username\My Documents\My Pictures �ł��B
CSIDL_MYVIDEO					�r�f�I �t�@�C���̋��ʃ��|�W�g���Ƃ��ċ@�\����t�@�C�� �V�X�e�� �f�B���N�g���B��ʓI�ȃp�X�́AC:\Documents and Settings\username\My Documents\My Videos �ł��B
CSIDL_NETHOOD					�}�C �l�b�g���[�N���z�t�H���_�ɑ��݂���\���̂��郊���N �I�u�W�F�N�g���܂ރt�@�C�� �V�X�e�� �f�B���N�g���B�l�b�g���[�N���O��Ԃ̃��[�g������ CSIDL_NETWORK �Ƃ͈قȂ�܂��B��ʓI�ȃp�X�́AC:\Documents and Settings\username\NetHood �ł��B
CSIDL_NETWORK					�}�C �l�b�g���[�N�A�l�b�g���[�N���O��ԊK�w�̃��[�g���������z�t�H���_�B
CSIDL_PERSONAL					Version 6.0�B�}�C �h�L�������g �f�X�N�g�b�v���ڂ��������z�t�H���_�B����� CSIDL_MYDOCUMENTS �Ɠ����ł��B
CSIDL_PLAYLISTS					Windows Vista �����s����Ă���R���s���[�^�́A�Đ��A���o�����i�[���邽�߂Ɏg�p����鉼�z�t�H���_�B�ʏ�́Ausername\My Music\Playlists �ł��B
CSIDL_PRINTERS					�C���X�g�[�������v�����^���܂މ��z�t�H���_�B
CSIDL_PRINTHOOD					�v�����^���z�t�H���_�ɑ��݂���\���̂��郊���N �I�u�W�F�N�g���܂ރt�@�C�� �V�X�e�� �f�B���N�g���B��ʓI�ȃp�X�́AC:\Documents and Settings\username\PrintHood �ł��B
CSIDL_PROFILE					���[�U�[�̃v���t�@�C�� �t�H���_�B��ʓI�ȃp�X�́AC:\Documents and Settings\username �ł��B�A�v���P�[�V�����ɂ���ăt�@�C����t�H���_�����̃��x���ō쐬�����ׂ��ł͂���܂���B�A�v���P�[�V�����ł́ACSIDL_APPDATA �܂��� CSIDL_LOCAL_APPDATA �ɂ���ĎQ�Ƃ����ꏊ�̉��Ƀf�[�^���ۑ������K�v������܂��B
CSIDL_PROGRAMS					���[�U�[�̃v���O���� �O���[�v (����玩�̂��t�@�C�� �V�X�e�� �f�B���N�g��) ���܂ރt�@�C�� �V�X�e�� �f�B���N�g���B��ʓI�ȃp�X�́AC:\Documents and Settings\username\Start Menu\Programs �ł��B
CSIDL_RECENT					���[�U�[�̍ł��ŋߎg�����t�@�C���ւ̃V���[�g�J�b�g���܂ރt�@�C�� �V�X�e�� �f�B���N�g���B��ʓI�ȃp�X�́AC:\Documents and Settings\username\My Recent Documents �ł��B���̃t�H���_�ŃV���[�g�J�b�g���쐬����ɂ́ASHAddToRecentDocs ���g�p���܂��B���̊֐��ł́A�V���[�g�J�b�g���쐬�����ق��A�V�F���̍ŋߎg�����t�@�C���̈ꗗ���X�V����A�V���[�g�J�b�g�� [�X�^�[�g] ���j���[�� [�ŋߎg�����t�@�C��] �T�u���j���[�ɒǉ�����܂��B
CSIDL_SENDTO					[����] ���j���[���ڂ��܂ރt�@�C�� �V�X�e�� �f�B���N�g���B��ʓI�ȃp�X�́AC:\Documents and Settings\username\SendTo �ł��B
CSIDL_STARTMENU					[�X�^�[�g] ���j���[���ڂ��܂ރt�@�C�� �V�X�e�� �f�B���N�g���B��ʓI�ȃp�X�́AC:\Documents and Settings\username\Start Menu �ł��B
CSIDL_STARTUP					���[�U�[�̃X�^�[�g�A�b�v �v���O���� �O���[�v�ɑΉ�����t�@�C�� �V�X�e�� �f�B���N�g���B��ʓI�ȃp�X�́AC:\Documents and Settings\username\Start Menu\Programs\Startup �ł��B
CSIDL_TEMPLATES					�h�L�������g �e���v���[�g�̋��ʃ��|�W�g���Ƃ��ċ@�\����t�@�C�� �V�X�e�� �f�B���N�g���B��ʓI�ȃp�X�́AC:\Documents and Settings\username\Templates �ł��B
HOMEPATH						%USERPROFILE% �Ɠ����ł��B
TEMP							�R���s���[�^��̈ꎞ�t�H���_�BWindows XP �̏ꍇ�A��ʓI�ȃp�X�� %USERPROFILE%\Local Settings\Temp �ł��BWindows Vista �̏ꍇ�A��ʓI�ȃp�X�� %USERPROFILE%\AppData\Local\Temp �ł��B
TMP								�R���s���[�^��̈ꎞ�t�H���_�BWindows XP �̏ꍇ�A��ʓI�ȃp�X�� %USERPROFILE%\Local Settings\Temp �ł��BWindows Vista �̏ꍇ�A��ʓI�ȃp�X�� %USERPROFILE%\AppData\Local\Temp �ł��B
USERPROFILE						CSIDL_PROFILE �Ɠ����ł��B
USERSID							���݂̃��[�U�[ �A�J�E���g �Z�L�����e�B���ʎq (SID) �������܂��B���Ƃ��΁AS-1-5-21-1714567821-1326601894-715345443-1026 �ȂǁB
*/

static const struct {
  char *Name;
  int Id;
} s_folderId[] = {
    {"%Desktop%", CSIDL_DESKTOP},                     //�f�X�N�g�b�v
    {"%Program%", CSIDL_PROGRAMS},                    //�X�^�[�g���j���[\�v���O����
    {"%ControlPanel%", CSIDL_CONTROLS},               //�R���g���[�� �p�l��
    {"%MyDocment%", CSIDL_PERSONAL},                  //�}�C�h�L�������g
    {"%Favorite%", CSIDL_FAVORITES},                  //���C�ɓ���
    {"%Startup%", CSIDL_STARTUP},                     //�X�^�[�g�A�b�v
    {"%Recent%", CSIDL_RECENT},                       //�ŋߎg�����t�@�C��
    {"%SendTo%", CSIDL_SENDTO},                       //����
    {"%BitBucket%", CSIDL_BITBUCKET},                 //���ݔ�
    {"%StartMenu%", CSIDL_STARTMENU},                 //�X�^�[�g���j���[
    {"%MyComputer%", CSIDL_DRIVES},                   //�}�C �R���s���[�^
    {"%MyNetwork%", CSIDL_NETWORK},                   //�}�C�l�b�g���[�N
    {"%Network%", CSIDL_NETHOOD},                     //�l�b�g���[�N
    {"%Font%", CSIDL_FONTS},                          //�t�H���g
    {"%ApplicationData%", CSIDL_APPDATA},             //�A�v���P�[�V�����f�[�^
    {"%LocalApplicationData%", CSIDL_LOCAL_APPDATA},  //���[�J���A�v���P�[�V�����f�[�^
    {"%Windows%", CSIDL_WINDOWS},                     //Windows�f�B���N�g��
    {"%System%", CSIDL_SYSTEM},                       //System�f�B���N�g��
    {"%ProgramFiles%", CSIDL_PROGRAM_FILES},          //C:\Program Files
    {"%MyPicture%", CSIDL_MYPICTURES},                //�}�C�s�N�`��

    {"%AllUsers StartMenu%", CSIDL_COMMON_STARTMENU},  //C:\WINDOWS\All Users\�X�^�[�g ���j���[
    {"%AllUsers Program%",
     CSIDL_COMMON_PROGRAMS},  //C:\WINDOWS\All Users\�X�^�[�g ���j���[\�v���O����
    {"%AllUsers Startup%",
     CSIDL_COMMON_STARTUP},  //C:\WINDOWS\All Users\�X�^�[�g ���j���[\�v���O����\�X�^�[�g�A�b�v
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
