#
# Possible options
# DEBUG=1                                          # default is DEBUG=0
#

# compiler
CC=g++

# platform
PLATFORM := $(shell uname -s)
$(warning platform=$(PLATFORM))

# file extensions
ifeq ($(PLATFORM), Linux)
  OBJ=.o
  EXE=
  DLL=.so
  LIB=.a
else
# cygwin or mingw
  OBJ=.obj
  EXE=.exe
  DLL=.dll
  LIB=.lib
endif

TARGETS=$(patsubst %, %$(EXE), main)

CFLAGS=-std=c++14 -Iinclude
LFLAGS=

ifdef DEBUG
   CFLAGS += -g -DDEBUG
else
   CFLAGS += -O3
endif

HEADERS=$(wildcard *.h)
SRC=$(basename $(notdir $(wildcard *.cpp)))

OBJS += $(patsubst %, %$(OBJ), $(SRC))

all : $(TARGETS)

%$(OBJ) : %.cpp $(HEADERS) Makefile
	$(CC) -c $(CFLAGS) $< -o $@

%.i : %.cpp $(HEADERS) Makefile
	$(CC) -E -c $(CFLAGS) $< -o $@

%.asm : %.cpp $(HEADERS) Makefile
	$(CC) -c -g -Wa,-adhln -masm=intel $(CFLAGS) $< > $@


$(TARGETS) : $(OBJS)
	$(CC) $(LFLAGS) -o $@ $(OBJS)

# clean
.PHONY: clean
clean:
	rm -f *.o *.i *.ii *.exe *.asm *.stackdump $(TARGETS)
