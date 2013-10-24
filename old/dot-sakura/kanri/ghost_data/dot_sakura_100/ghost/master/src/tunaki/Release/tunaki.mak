#-------------------------------------------------------
# BCC Developer 1.2.18
# Copyright (C) 2003 jun_miura@hi-ho.ne.jp
#-------------------------------------------------------
.autodepend
CC=bcc32
RC=brc32
ILIB=implib
CFLAG=-WD  -6 -O2 -w- -AT -pc -H- -k -b  
OUTDIR=-nRelease
CINCS=
TARGET=Release\tunaki.dll
LIB=Release\tunaki.lib
SRC1=D:\cvsSV\home\myWeb\httpd\html_nanika\dot-sakura\kanri\ghost_data\dot_sakura_100\ghost\master\src\tunaki\shiori.c
OBJ1=Release\shiori.obj
SRC2=D:\cvsSV\home\myWeb\httpd\html_nanika\dot-sakura\kanri\ghost_data\dot_sakura_100\ghost\master\src\tunaki\tunaki_heap.c
OBJ2=Release\tunaki_heap.obj
SRC3=D:\cvsSV\home\myWeb\httpd\html_nanika\dot-sakura\kanri\ghost_data\dot_sakura_100\ghost\master\src\tunaki\tunaki_string.c
OBJ3=Release\tunaki_string.obj
SRC4=D:\cvsSV\home\myWeb\httpd\html_nanika\dot-sakura\kanri\ghost_data\dot_sakura_100\ghost\master\src\tunaki\tunaki_init.c
OBJ4=Release\tunaki_init.obj
SRC5=D:\cvsSV\home\myWeb\httpd\html_nanika\dot-sakura\kanri\ghost_data\dot_sakura_100\ghost\master\src\tunaki\tunaki_log.c
OBJ5=Release\tunaki_log.obj
SRC6=D:\cvsSV\home\myWeb\httpd\html_nanika\dot-sakura\kanri\ghost_data\dot_sakura_100\ghost\master\src\tunaki\pipedprocess.c
OBJ6=Release\pipedprocess.obj
SRC7=D:\cvsSV\home\myWeb\httpd\html_nanika\dot-sakura\kanri\ghost_data\dot_sakura_100\ghost\master\src\tunaki\tunaki_pipe.c
OBJ7=Release\tunaki_pipe.obj
SRC8=D:\cvsSV\home\myWeb\httpd\html_nanika\dot-sakura\kanri\ghost_data\dot_sakura_100\ghost\master\src\tunaki\tunaki_comm.c
OBJ8=Release\tunaki_comm.obj

TARGET: $(TARGET)

$(TARGET): $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) $(OBJ6) $(OBJ7) $(OBJ8)
    $(CC) $(CFLAG) -e$(TARGET) $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) $(OBJ6) $(OBJ7) $(OBJ8)
    $(ILIB) $(LIB) $(TARGET)

$(OBJ1): $(SRC1)
    $(CC) $(CFLAG) $(OUTDIR) $(CINCS) -c $(SRC1)

$(OBJ2): $(SRC2)
    $(CC) $(CFLAG) $(OUTDIR) $(CINCS) -c $(SRC2)

$(OBJ3): $(SRC3)
    $(CC) $(CFLAG) $(OUTDIR) $(CINCS) -c $(SRC3)

$(OBJ4): $(SRC4)
    $(CC) $(CFLAG) $(OUTDIR) $(CINCS) -c $(SRC4)

$(OBJ5): $(SRC5)
    $(CC) $(CFLAG) $(OUTDIR) $(CINCS) -c $(SRC5)

$(OBJ6): $(SRC6)
    $(CC) $(CFLAG) $(OUTDIR) $(CINCS) -c $(SRC6)

$(OBJ7): $(SRC7)
    $(CC) $(CFLAG) $(OUTDIR) $(CINCS) -c $(SRC7)

$(OBJ8): $(SRC8)
    $(CC) $(CFLAG) $(OUTDIR) $(CINCS) -c $(SRC8)

TARGET_AFTER:
    copy Release\tunaki.dll ..\..\
