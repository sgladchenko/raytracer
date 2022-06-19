 CC = gcc
 
.PHONY:
test: build/test

build/raytracercore.so: raytracercore/raytracercore.h
	$(CC) -shared -o build/raytracercore.so cache.c intersect.c normalize.c

build/test: test/test.c raytracercore/raytracercore.h
	$(CC) -o "build/test" "test/test.c" -I"raytracercore/"
