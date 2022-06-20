 CC = gcc
 CXX = g++
 BUILD = build
 
.PHONY: test
test: $(BUILD) $(BUILD)/test $(BUILD)/cpptest $(BUILD)/pngtest

.PHONY: rtcore
rtcore: $(BUILD) $(BUILD)/rtcore.so

$(BUILD):
	mkdir $(BUILD)

$(BUILD)/rtcore.so: $(BUILD) rtcore/rtcore.h rtcore/internal.h rtcore/rtcore.c
	$(CC) -c -fPIC -I rtcore/ rtcore/rtcore.c -o $(BUILD)/rtcore.o -lm
	$(CC) -shared $(BUILD)/rtcore.o -o $(BUILD)/rtcore.so

$(BUILD)/test: $(BUILD) test/test.c rtcore/internal.h
	$(CC) -I rtcore/ test/test.c -o $(BUILD)/test

$(BUILD)/cpptest: $(BUILD) test/cpptest.cpp rtcore/internal.h
	$(CXX) -I rtcore/ test/cpptest.cpp -o $(BUILD)/cpptest

$(BUILD)/pngtest:
	$(CXX) -I lib/libpng/include -L lib/libpng/lib/ test/pngtest.c -lpng -o $(BUILD)/pngtest -Wl,-rpath=lib/libpng/lib/