CC=clang
LDFLAGS=-lcunit

FORCE: ;

test: FORCE chacha20.c chacha20_test.c
	$(CC) $(CFLAGS) chacha20.c chacha20_test.c -o $@ $(LDFLAGS)
	./test

