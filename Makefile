.POSIX:

PREFIX = /usr/local

all: unixtoiso8601

unixtoiso8601:
	c99 unixtoiso8601.c -o $@

install: unixtoiso8601
	chmod 755 unixtoiso8601
	cp unixtoiso8601 ${PREFIX}/bin

clean:
	rm -f unixtoiso8601
