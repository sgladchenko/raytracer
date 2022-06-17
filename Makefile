 CC = gcc
 
.PHONY:
test: build/test

build/raytracercore.so:
	$(CC) -shared -o build/raytracercore.so crossproducts.c intersect.c normalize.c

build/test: test/test.c raytracercore/raytracercore.h
	$(CC) -o "build/test" "test/test.c" -I"raytracercore/"
