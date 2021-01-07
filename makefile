build_DIR=build
source_DIR=src
target=libchroma.so

build/libchroma.so: $(wildcard $(source_DIR)/*.cpp) build
	g++ -fPIC -shared $(source_DIR)/*.cpp -std=c++14 -Wall -o $(build_DIR)/$(target)

build :
	mkdir build