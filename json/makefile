CC=g++
VAR=relase
LIB=libjson.a 
ifeq ($(VAR),debug)
	CFLAG= -c -g
else 
	CFLAG= -c
endif
all:$(LIB)
$(LIB):MyJson.o Node.o
	ar crs $(LIB) MyJson.o Node.o
MyJson.o:MyJson.h MyJson.cpp
	$(CC) $(CFLAG) MyJson.cpp MyJson.h
Node.o:Node.h Node.cpp
	$(CC) $(CFLAG) Node.h Node.cpp
clean:
	rm -rf *.o
