CXX=g++
CXX_PATH := $(shell which g++-4.7)

CC=gcc
CC_PATH := $(shell which gcc-4.7)

ifneq ($(CXX_PATH),)
	CXX=g++-4.7
endif

ifneq ($(CC_PATH),)
	CC=gcc-4.7
endif

#CFLAGS=-Wall -ggdb -std=c++0x -O0 -I.
CFLAGS=-Wall -std=c++0x -Os -I. -fPIC -g
LDFLAGS= -lmosquittopp -lmosquitto -ljsoncpp

COMMON_DIR=common
COMMON_H=$(COMMON_DIR)/utils.h $(COMMON_DIR)/mqtt_wrapper.h
COMMON_O=$(COMMON_DIR)/mqtt_wrapper.o $(COMMON_DIR)/utils.o
SONAME=libwbmqtt.so.1

.PHONY: all clean

lib : $(COMMON_DIR)/libwbmqtt.so.1.0.1

$(COMMON_DIR)/libwbmqtt.so.1.0.1 : $(COMMON_O)
	${CXX} -shared -Wl,-soname,$(SONAME) -o $(COMMON_DIR)/libwbmqtt.so.1.0.1 $(COMMON_O) 

$(COMMON_DIR)/utils.o : $(COMMON_DIR)/utils.cpp $(COMMON_H)
	${CXX} -c $< -o $@ ${CFLAGS}

$(COMMON_DIR)/mqtt_wrapper.o : $(COMMON_DIR)/mqtt_wrapper.cpp $(COMMON_H)
	${CXX} -c $< -o $@ ${CFLAGS}

clean :
	-rm -f $(COMMON_DIR)/*.o
	-rm -f $(COMMON_DIR)/*.so


install: all
	install -d $(DESTDIR)
	install -d $(DESTDIR)/etc
	install -d $(DESTDIR)/usr/bin
	install -d $(DESTDIR)/usr/lib

	install -m 0755  $(GPIO_DIR)/$(GPIO_BIN) $(DESTDIR)/usr/bin/$(GPIO_BIN)
	install -m 0755  $(NINJABRIDGE_DIR)/$(NINJABRIDGE_BIN) $(DESTDIR)/usr/bin/$(NINJABRIDGE_BIN)
