CC=clang
LDFLAGS=-lcunit

UNAME_S := $(shell uname -s)
ARCH := $(shell arch)

ifeq ($(UNAME_S),Darwin)
	CFLAGS += -I$(shell brew --prefix cunit)/include
	LDFLAGS = -L$(shell brew --prefix cunit)/lib -lcunit
endif

FORCE: ;

test: FORCE chacha20.c chacha20_test.c
	$(CC) $(CFLAGS) chacha20.c chacha20_test.c -o $@ $(LDFLAGS)
	./test

