#-------------------------------------------------------
# BCC Developer 1.2.18
# Copyright (C) 2003 jun_miura@hi-ho.ne.jp
#-------------------------------------------------------
.autodepend
CC=bcc32
RC=brc32
CFLAG=-WC  -3 -Od -w- -AT -pc -H- -k -b -v -y  -DDEBUG
OUTDIR=-nDebug
CINCS=
TARGET=Debug\echo.exe
SRC1=D:\cvsSV\home\myWeb\httpd\html_nanika\dot-sakura\kanri\ghost_data\dot_sakura_100\ghost\master\src\tunaki\test\echo.c
OBJ1=Debug\echo.obj

TARGET: $(TARGET)

$(TARGET): $(OBJ1)
    $(CC) $(CFLAG) -e$(TARGET) $(OBJ1)

$(OBJ1): $(SRC1)
    $(CC) $(CFLAG) $(OUTDIR) $(CINCS) -c $(SRC1)
