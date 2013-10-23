#-------------------------------------------------------
# BCC Developer 1.2.18
# Copyright (C) 2003 jun_miura@hi-ho.ne.jp
#-------------------------------------------------------
.autodepend
CC=bcc32
RC=brc32
CFLAG=-WC  -3 -Od -w- -AT -pc -H- -k -b -v -y  -DDEBUG
OUTDIR=-nDebug
CINCS=-I..\..\..\..\..\..\..\..\..\..\..\..\lib\CUnit\CUnit\Headers
TARGET=Debug\test.exe
SRC1=D:\cvsSV\home\myWeb\httpd\html_nanika\dot-sakura\kanri\ghost_data\dot_sakura_100\ghost\master\src\tunaki\test\test.c
OBJ1=Debug\test.obj
SRC2=D:\cvsSV\home\myWeb\httpd\html_nanika\dot-sakura\kanri\ghost_data\dot_sakura_100\ghost\master\src\tunaki\test\test_heap.c
OBJ2=Debug\test_heap.obj
SRC3=D:\cvsSV\home\myWeb\httpd\html_nanika\dot-sakura\kanri\ghost_data\dot_sakura_100\ghost\master\src\tunaki\pipedprocess.c
OBJ3=Debug\pipedprocess.obj
SRC4=D:\cvsSV\home\myWeb\httpd\html_nanika\dot-sakura\kanri\ghost_data\dot_sakura_100\ghost\master\src\tunaki\tunaki_heap.c
OBJ4=Debug\tunaki_heap.obj
SRC5=D:\cvsSV\home\myWeb\httpd\html_nanika\dot-sakura\kanri\ghost_data\dot_sakura_100\ghost\master\src\tunaki\tunaki_init.c
OBJ5=Debug\tunaki_init.obj
SRC6=D:\cvsSV\home\myWeb\httpd\html_nanika\dot-sakura\kanri\ghost_data\dot_sakura_100\ghost\master\src\tunaki\tunaki_log.c
OBJ6=Debug\tunaki_log.obj
SRC7=D:\cvsSV\home\myWeb\httpd\html_nanika\dot-sakura\kanri\ghost_data\dot_sakura_100\ghost\master\src\tunaki\tunaki_string.c
OBJ7=Debug\tunaki_string.obj
SRC8=D:\cvsSV\home\myWeb\httpd\html_nanika\dot-sakura\kanri\ghost_data\dot_sakura_100\ghost\master\src\tunaki\tunaki_pipe.c
OBJ8=Debug\tunaki_pipe.obj
SRC9=D:\cvsSV\home\myWeb\httpd\html_nanika\dot-sakura\kanri\ghost_data\dot_sakura_100\ghost\master\src\tunaki\tunaki_comm.c
OBJ9=Debug\tunaki_comm.obj
LIB1=..\..\..\..\..\..\..\..\..\..\..\..\lib\CUnit\CUnit\Debug\CUnit.lib

TARGET: $(TARGET)

$(TARGET): $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) $(OBJ6) $(OBJ7) $(OBJ8) $(OBJ9)
    $(CC) $(CFLAG) -e$(TARGET) $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) $(OBJ6) $(OBJ7) $(OBJ8) $(OBJ9) $(LIB1)

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

$(OBJ9): $(SRC9)
    $(CC) $(CFLAG) $(OUTDIR) $(CINCS) -c $(SRC9)

TARGET_AFTER:
    touch Debug\tunaki.log
    del Debug\tunaki.log
