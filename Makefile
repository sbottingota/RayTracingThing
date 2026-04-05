.PHONY: all image jpg clean

FFMPEG_FLAGS=-hide_banner -loglevel error

all: main.cc
	$(CXX) main.cc -o main

image: all
	./main > img.ppm

jpg: image
	ffmpeg $(FFMPEG_FLAGS) -i img.ppm img.jpg

clean:
	@rm -f main *.ppm *.jpg *.png

