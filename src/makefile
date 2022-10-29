SRC_FILES=$(wildcard *.cpp)
OBJ_FILES=$(addprefix $(BUILD_DIR), $(SRC_FILES:cpp=o))

$(OBJ_FILES): $(SRC_FILES)
	$(CC) -c $?
	mv *.o ../build/