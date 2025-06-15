# cmake macros
SHELL = /bin/bash
.PHONY: all build dbuild run drun venv

all: run

build:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
	cmake --build build

dbuild:
	cmake -S . -B dbuild -DCMAKE_BUILD_TYPE=Debug
	cmake --build dbuild

run: build
	./build/Release/game

drun: dbuild
	gdb ./dbuild/Debug/game

test:
	