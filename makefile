build_DIR=build
source_DIR=src
target=libchroma.so

all: $(wildcard $(source_DIR)/*.cpp)
	g++ -fPIC -shared $(source_DIR)/*.cpp -std=c++14 -Wall -o $(build_DIR)/$(target)