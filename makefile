# -----------------------------------------------------------------------------
#        (c) ADIT - Advanced Driver Information Technology JV
# -----------------------------------------------------------------------------
# Title:       Subcomponent makefile
#
# Description: Makefile for the subcomponent
# -----------------------------------------------------------------------------

SRC=src/backmonitor.c
OBJ=backmonitor.o
OUT=backmonitor
CFLAG=-g -DEVENT

INCLUDE=-I$(PKG_CONFIG_SYSROOT_DIR)/usr/include/ilm 

LIBDIR=-L$(PKG_CONFIG_SYSROOT_DIR)/usr/lib
LIBS=-lilmClient -lilmCommon -lilmControl


all: $(OBJ) 
	$(CC) $(CFLAG) -o $(OUT) $(LIBDIR) $(LIBS) $(OBJ)

$(OBJ): $(SRC)
	$(CC) $(CFLAG) $(INCLUDE) -o $(OBJ) -c $(SRC)

.PHONY: clean
clean:
	$(RM) $(OUT) $(OBJ)
		
