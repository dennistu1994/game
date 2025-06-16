# cmake macros
SHELL = /bin/bash
.PHONY: all build dbuild run drun venv test

all: run

build:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
	cmake --build build

dbuild:
	cmake -S . -B dbuild -DCMAKE_BUILD_TYPE=Debug
	cmake --build dbuild

run: build
	./build/game

drun: dbuild
	gdb ./dbuild/game

test: dbuild
	$(MAKE) -C dbuild test ARGS="--output-on-failure"

valgrind: build
	valgrind ./build/game --leak-check=full -s