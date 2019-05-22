CC = cc
CFLAGS = --std=c99

cat:
	${CC} ${CFLAGS} cat.c -o bin/cat
