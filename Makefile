.PHONY: all image jpg clean

CXXFLAGS=-Wall -Wextra -Wpedantic -Wno-unused-parameter
FFMPEG_FLAGS=-hide_banner -loglevel error

SRCS=$(wildcard src/*.cc)
OBJS=$(SRCS:src/%.cc=build/%.o)
DEPS=$(OBJS:.o=.d)
EXE=build/render

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXE)

build/%.o: src/%.cc
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

image: all
	$(EXE) > img.ppm

jpg: image
	ffmpeg $(FFMPEG_FLAGS) -i img.ppm img.jpg

png: image
	ffmpeg $(FFMPEG_FLAGS) -i img.ppm img.png

clean:
	@rm -f build/* *.ppm *.png *.jpg

-include $(DEPS)

