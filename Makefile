 CC = gcc
 
.PHONY:
test: build/test

build/rtcore.so: rtcore/rtcore.h
	$(CC) -shared -o build/rtcore.so cache.c intersect.c normalize.c

build/test: test/test.c rtcore/rtcore_internal.h
	$(CC) -o "build/test" "test/test.c" -I "rtcore/"
