### makefile ###


# �����t�@�C��(�g���q�͊܂߂Ȃ�)
TARGET	=	nkmm


# �����N���C�u����
LIBS	=	\


# �ǉ��̃C���N���[�h�p�X
INCLUDE		= include


# �����N�I�u�W�F�N�g
OBJS	=	$(OBJDIR)\nkmm_basic.obj \
			$(OBJDIR)\nkmm_base64.obj \
			$(OBJDIR)\nkmm_buffer.obj \
			$(OBJDIR)\nkmm_calc.obj \
			$(OBJDIR)\nkmm_eval.obj \
			$(OBJDIR)\nkmm_file.obj \
			$(OBJDIR)\nkmm_ini.obj \
			$(OBJDIR)\nkmm_math.obj \
			$(OBJDIR)\nkmm_memory.obj \
			$(OBJDIR)\nkmm_scramble.obj \
			$(OBJDIR)\nkmm_sort.obj \
			$(OBJDIR)\nkmm_strlib.obj \
			$(OBJDIR)\nkmm_strlib_sjis.obj \
			$(OBJDIR)\nkmm_strlib_unicode.obj \
			$(OBJDIR)\nkmm_stringlist.obj \
			$(OBJDIR)\nkmm_utils.obj \
			$(OBJDIR)\nkmm_w32.obj \


# �t���O
DEFINE		=	


!include	../builtins.mak


