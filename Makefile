 CC = gcc
 CXX = g++
 BUILD = build
 INCLUDE = -I rtcore/ -I render/
 
######################
# Main build targets #
######################

$(BUILD):
	mkdir $(BUILD)

.PHONY: rtcore
rtcore: $(BUILD) $(BUILD)/rtcore.o

# Main shared library of raytracer
$(BUILD)/raytracer.so: $(BUILD)/rtcore.o render/render.hpp $(BUILD)/pixmap.o $(BUILD)/render.o $(BUILD)/rgb.o
	$(CXX) 

# A part of the raytracer that contains all the geometry computations
$(BUILD)/rtcore.o: $(BUILD) rtcore/rtcore.h rtcore/internal.h rtcore/rtcore.c
	$(CC) $(INCLUDE) -c rtcore/rtcore.c -o $(BUILD)/rtcore.o -lm

$(BUILD)/pixmap.o: render/render.hpp render/pixmap.cpp
	$(CXX) $(INCLUDE) -I lib/libpng/include -fPIC -c render/pixmap.cpp -L ./lib/libpng/lib -lpng -o $(BUILD)/pixmap.o

.PHONY: clean
clean:
	rm build/*

##################################
# Some smaller files for testing #
##################################

.PHONY: test
test: $(BUILD) $(BUILD)/test $(BUILD)/cpptest $(BUILD)/pngtest

$(BUILD)/test: $(BUILD) test/test.c rtcore/internal.h
	$(CC) $(INCLUDE) test/test.c -o $(BUILD)/test

$(BUILD)/cpptest: $(BUILD) test/cpptest.cpp rtcore/internal.h
	$(CXX) $(INCLUDE) test/cpptest.cpp -o $(BUILD)/cpptest

.PHONY: $(BUILD)/pngtest
$(BUILD)/pngtest: test/pngtest.c
	$(CC) -I lib/libpng/include test/pngtest.c -L ./lib/libpng/lib -lpng16 -o $(BUILD)/pngtest

.PHONY: $(BUILD)/pixmap_save_test
$(BUILD)/pixmap_save_test: render/render.hpp \
						   render/pixmap.cpp \
						   render/rgb.cpp \
						   test/pixmap_save_test.cpp
	$(CXX) $(INCLUDE) -I lib/libpng/include \
		   render/pixmap.cpp \
		   render/rgb.cpp \
		   test/pixmap_save_test.cpp \
		   -L ./lib/libpng/lib -lpng16 -lz  \
		   -o $(BUILD)/pixmap_save_test