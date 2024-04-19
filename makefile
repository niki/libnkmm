### makefile ###


# 生成ファイル(拡張子は含めない)
TARGET	=	nkmm


# リンクライブラリ
LIBS	=	\


# 追加のインクルードパス
INCLUDE		= include


# リンクオブジェクト
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


# フラグ
DEFINE		=	


!include	../builtins.mak


