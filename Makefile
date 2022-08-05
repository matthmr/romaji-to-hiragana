default: romaji

CC?=gcc
CFLAGS?=

romaji.o: romaji.c romaji.h
	${CC} ${CFLAGS} -c $< -o $@

romaji: romaji.o
	${CC} ${CFLAGS} $? -o $@

clean:
	rm -rfv romaji romaji.o
