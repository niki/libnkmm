### makefile ###


# �����t�@�C��(�g���q�͊܂߂Ȃ�)
TARGET	=	ul


# �����N���C�u����
LIBS	=	\


# �ǉ��̃C���N���[�h�p�X
INCLUDE		= include


# �����N�I�u�W�F�N�g
OBJS	=	$(OBJDIR)\ul_basic.obj \
			$(OBJDIR)\ul_base64.obj \
			$(OBJDIR)\ul_buffer.obj \
			$(OBJDIR)\ul_calc.obj \
			$(OBJDIR)\ul_eval.obj \
			$(OBJDIR)\ul_file.obj \
			$(OBJDIR)\ul_ini.obj \
			$(OBJDIR)\ul_math.obj \
			$(OBJDIR)\ul_memory.obj \
			$(OBJDIR)\ul_scramble.obj \
			$(OBJDIR)\ul_sort.obj \
			$(OBJDIR)\ul_strlib.obj \
			$(OBJDIR)\ul_strlib_sjis.obj \
			$(OBJDIR)\ul_strlib_unicode.obj \
			$(OBJDIR)\ul_stringlist.obj \
			$(OBJDIR)\ul_utils.obj \
			$(OBJDIR)\ul_w32.obj \


# �t���O
DEFINE		=	


!include	../builtins.mak


