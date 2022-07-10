LIBS = $(shell pkg-config --libs curlpp jsoncpp)

ROOT = .

SRC_PATH = $(ROOT)/src
BUILD_PATH = $(ROOT)/build
OBJS_PATH = $(BUILD_PATH)/objs

objects = post.o connection.o

.PHONY: program

program: directories $(objects)
	g++ -o $(BUILD_PATH)/post $(OBJS_PATH)/post.o \
		$(OBJS_PATH)/connection.o \
		$(LIBS)

directories:
	mkdir -p $(BUILD_PATH) $(OBJS_PATH)

connection.o:
	g++ -c $(SRC_PATH)/connection/connection.cpp \
		-o $(OBJS_PATH)/connection.o \
		$(LIBS)

post.o:
	g++ -c $(SRC_PATH)/post.cpp \
		-o $(OBJS_PATH)/post.o \
		$(LIBS)

