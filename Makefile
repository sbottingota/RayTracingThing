.PHONY: all image jpg clean

CXXFLAGS=-Wall -Werror
FFMPEG_FLAGS=-hide_banner -loglevel error

all: main vec3 color 
	$(CXX) $(CXXFLAGS) build/*.o -o build/main

main: src/main.cc
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c src/main.cc -o build/main.o

%: src/%.h src/%.cc
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c src/$*.cc -o build/$*.o

image: all
	build/main > img.ppm

jpg: image
	ffmpeg $(FFMPEG_FLAGS) -i img.ppm img.jpg

clean:
	@rm -f build/* *.ppm *.png *.jpg

