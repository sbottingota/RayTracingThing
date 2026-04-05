.PHONY: all image jpg clean

CXXFLAGS=-Wall -Werror
FFMPEG_FLAGS=-hide_banner -loglevel error

EXE=build/render
OBJS=build/main.o build/vec3.o build/color.o

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXE)

build/main.o: src/main.cc
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c src/main.cc -o build/main.o

build/%.o: src/%.h src/%.cc
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c src/$*.cc -o build/$*.o

image: all
	$(EXE) > img.ppm

jpg: image
	ffmpeg $(FFMPEG_FLAGS) -i img.ppm img.jpg

clean:
	@rm -f build/* *.ppm *.png *.jpg

