
CC = clang
CXX = clang++

prebuild:
	mkdir -p tmp
	(cd tmp && export CC=$(CC); export CXX=$(CXX); cmake ../)

build:
	make -C tmp
