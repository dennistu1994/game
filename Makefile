# cmake macros
all: prep
	cmake --build build

prep:
	cmake -S . -B build

debug:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug

run:
	./build/game