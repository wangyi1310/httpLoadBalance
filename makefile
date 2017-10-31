CC=g++
VAR=relase
TEST=main
LIB=log/liblog.a 
ifeq ($(VAR),debug)
	CFLAG= -c -g
else 
	CFLAG= -c
endif
all:$(TEST)
$(TEST):main.o $(LIB)
	$(CC) -o $(TEST) main.o -Llog/ -llog 
main.o: main.cpp
	$(CC) $(CFLAG) main.cpp
$(LIB): log
	make -C log
run: $(TEST)
	./$(TEST)
clean:
	rm -rf *.o $(TEST)
