LIBS = $(shell pkg-config --cflags --libs curlpp jsoncpp opencv4) -lpthread -lwiringPi

ROOT = .

SRC_PATH = $(ROOT)/src
BUILD_PATH = $(ROOT)/build
OBJS_PATH = $(BUILD_PATH)/objs

# objects = main.o connection.o frame.o serial.o
objects = main.o serial.o

# .PHONY: program

main: directories $(objects)
	g++ -o $(BUILD_PATH)/main $(OBJS_PATH)/main.o \
		$(OBJS_PATH)/connection.o $(OBJS_PATH)/frame.o $(OBJS_PATH)/serial.o \
		$(LIBS)

directories:
	mkdir -p $(BUILD_PATH) $(OBJS_PATH)

main.o:
	g++ -c $(SRC_PATH)/main.cpp \
		-o $(OBJS_PATH)/main.o \
		$(LIBS)

connection.o:
	g++ -c $(SRC_PATH)/connection/connection.cpp \
		-o $(OBJS_PATH)/connection.o \
		$(LIBS)

frame.o:
	g++ -c $(SRC_PATH)/frame/frame.cpp \
		-o $(OBJS_PATH)/frame.o \
		$(LIBS)

serial.o:
	g++ -c $(SRC_PATH)/serial/serial.cpp \
		-o $(OBJS_PATH)/serial.o \
		$(LIBS)
