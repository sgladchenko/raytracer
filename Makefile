 CC = gcc
 CXX = g++
 BUILD = build
 INCLUDE = -I rtcore/ -I render/ -I lib/
 
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

.PHONY: $(BUILD)/render_test
$(BUILD)/test_sample: render/render.hpp \
					  render/render.cpp \
					  render/pixmap.cpp \
					  render/rgb.cpp \
					  $(BUILD)/rtcore.o \
					  test/test_sample.cpp
	$(CXX) $(INCLUDE) -I lib/libpng/include \
		   render/render.cpp \
		   render/pixmap.cpp \
		   render/rgb.cpp \
		   test/test_sample.cpp \
		   $(BUILD)/rtcore.o \
		   -L ./lib/libpng/lib -lpng16 -lz  \
		   -o $(BUILD)/test_sample