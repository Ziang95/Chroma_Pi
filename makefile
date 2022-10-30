export PROJ_DIR=$(shell pwd)
export BUILD_DIR=$(PROJ_DIR)/build
export CC=g++

INSTALL_DIR=/usr/local/

TARGET=libchroma.so
SRC_DIR=src
HDR_DIR=headers
SRC_NAMES=$(shell ls $(SRC_DIR)| sed -n 's/\.cpp//p')

export SRC_FILES=$(wildcard $(PROJ_DIR)/$(SRC_DIR)/*.cpp)
export HDR_FILES=$(wildcard $(PROJ_DIR)/$(HDR_DIR)/*.h)
export OBJ_FILES=$(addprefix $(BUILD_DIR)/, $(SRC_NAMES:=.o))

$(BUILD_DIR)/$(TARGET): $(BUILD_DIR) $(SRC_FILES) $(HDR_FILES)
	make SRC
	$(CC) -fPIC -shared $(OBJ_FILES) -std=c++14 -Wall -o $@

SRC: $(SRC_FILES) $(HDR_FILES)
	make -C $(SRC_DIR)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

install:
	install -d $(INSTALL_DIR)/lib/
	install -m 644 $(BUILD_DIR)/$(TARGET) $(INSTALL_DIR)/lib/
	install -d $(INSTALL_DIR)/include/chroma-pi/
	install -m 644 $(HDR_FILES) $(INSTALL_DIR)/include/chroma-pi/