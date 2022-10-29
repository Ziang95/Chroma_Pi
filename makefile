export PROJ_DIR=$(shell pwd)
export BUILD_DIR=$(PROJ_DIR)/build
export CC=g++

TARGET=libchroma.so
SRC_DIR=src
SRC_NAMES=$(shell ls $(SRC_DIR)| sed -n 's/\.cpp//p')
SRC_FILES=$(wildcard $(SRC_DIR)/*.cpp)

OBJ_FILES=$(addprefix $(BUILD_DIR)/, $(SRC_NAMES:=.o))

$(BUILD_DIR)/$(TARGET): $(BUILD_DIR) $(OBJ_FILES)
	$(CC) -fPIC -shared $(OBJ_FILES) -std=c++14 -Wall -o $@

$(OBJ_FILES):
	make -C $(SRC_DIR)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)