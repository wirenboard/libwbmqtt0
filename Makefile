DESTDIR ?= /

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
LDFLAGS= -lmosquittopp -lcurl -ljsoncpp

COMMON_DIR=common
COMMON_H=$(COMMON_DIR)/utils.h $(COMMON_DIR)/mqtt_wrapper.h $(COMMON_DIR)/http_helper.h $(COMMON_DIR)/mqttrpc.h
COMMON_O=$(COMMON_DIR)/mqtt_wrapper.o $(COMMON_DIR)/utils.o $(COMMON_DIR)/http_helper.o $(COMMON_DIR)/mqttrpc.o
NAME=libwbmqtt
INCLUDE_DIR=wbmqtt
MAJOR=0
MINOR=1
VERSION=$(MAJOR).$(MINOR)
LIBRARY_NAME=$(NAME).so.$(VERSION)

.PHONY: all clean

lib : $(COMMON_DIR)/$(NAME).so.$(VERSION)

$(COMMON_DIR)/$(LIBRARY_NAME) : $(COMMON_O)
	${CXX} -shared -Wl,-soname,$(NAME).so.$(MAJOR)  -o $(COMMON_DIR)/$(LIBRARY_NAME) $(COMMON_O) $(LDFLAGS)

$(COMMON_DIR)/utils.o : $(COMMON_DIR)/utils.cpp $(COMMON_H)
	${CXX} -c $< -o $@ ${CFLAGS}

$(COMMON_DIR)/mqttrpc.o : $(COMMON_DIR)/mqttrpc.cpp $(COMMON_H)
	${CXX} -c $< -o $@ ${CFLAGS}

$(COMMON_DIR)/mqtt_wrapper.o : $(COMMON_DIR)/mqtt_wrapper.cpp $(COMMON_H)
	${CXX} -c $< -o $@ ${CFLAGS}

$(COMMON_DIR)/http_helper.o : $(COMMON_DIR)/http_helper.cpp $(COMMON_H)
	${CXX} -c $< -o $@ ${CFLAGS}

clean :
	-rm -f $(COMMON_DIR)/*.o
	-rm -f $(COMMON_DIR)/*.so


install: lib
	install -d $(DESTDIR)
	install -d $(DESTDIR)/usr/include
	install -d $(DESTDIR)/usr/include/$(INCLUDE_DIR)
	install -d $(DESTDIR)/usr/lib

	install -m 0755  $(COMMON_DIR)/$(LIBRARY_NAME) $(DESTDIR)/usr/lib/$(LIBRARY_NAME)
	install -m 0755  $(COMMON_DIR)/mqtt_wrapper.h $(DESTDIR)/usr/include/$(INCLUDE_DIR)/mqtt_wrapper.h
	install -m 0755  $(COMMON_DIR)/utils.h $(DESTDIR)/usr/include/$(INCLUDE_DIR)/utils.h
	install -m 0755  $(COMMON_DIR)/http_helper.h $(DESTDIR)/usr/include/$(INCLUDE_DIR)/http_helper.h
	install -m 0755  $(COMMON_DIR)/mqttrpc.h $(DESTDIR)/usr/include/$(INCLUDE_DIR)/mqttrpc.h
