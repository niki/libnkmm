### makefile ###


# �����t�@�C��(�g���q�͊܂߂Ȃ�)
TARGET	=	rei


# �����N���C�u����
LIBS	=	\


# �ǉ��̃C���N���[�h�p�X
INCLUDE		= include


# �����N�I�u�W�F�N�g
OBJS	=	$(OBJDIR)\rei_basic.obj \
			$(OBJDIR)\rei_base64.obj \
			$(OBJDIR)\rei_buffer.obj \
			$(OBJDIR)\rei_calc.obj \
			$(OBJDIR)\rei_eval.obj \
			$(OBJDIR)\rei_file.obj \
			$(OBJDIR)\rei_ini.obj \
			$(OBJDIR)\rei_math.obj \
			$(OBJDIR)\rei_memory.obj \
			$(OBJDIR)\rei_scramble.obj \
			$(OBJDIR)\rei_sort.obj \
			$(OBJDIR)\rei_strlib.obj \
			$(OBJDIR)\rei_strlib_sjis.obj \
			$(OBJDIR)\rei_strlib_unicode.obj \
			$(OBJDIR)\rei_stringlist.obj \
			$(OBJDIR)\rei_utils.obj \
			$(OBJDIR)\rei_w32.obj \


# �t���O
DEFINE		=	


!include	../builtins.mak


