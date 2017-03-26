
CC=gcc
RM=rm -rf

#OUTPUT=../objs
#RELEASE=../release

OBJS=main.o

CFLAGS=-g -O0
LDFLAGS=-lbase

INCLUDE_DIR=-I../include
LD_DIR=-L../lib

CFLAGS+=$(INCLUDE_DIR)
LDFLAGS+= $(LD_DIR)

main: $(OBJS)

clean: 
	$(RM) ./main *.o
