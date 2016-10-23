DESTDIR ?= /

HEADERS=utils.h mqtt_wrapper.h http_helper.h mqttrpc.h version.h
OBJS=mqtt_wrapper.o utils.o http_helper.o mqttrpc.o


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
LDFLAGS= -lmosquittopp -lcurl -ljsoncpp -lmosquitto

COMMON_DIR=common
COMMON_H=$(patsubst %.h,$(COMMON_DIR)/%.h,$(HEADERS))
COMMON_O=$(patsubst %.o,$(COMMON_DIR)/%.o,$(OBJS))
NAME=libwbmqtt
INCLUDE_DIR=wbmqtt
MAJOR=0
MINOR=1
VERSION=$(MAJOR).$(MINOR)
LIBRARY_NAME=$(NAME).so.$(VERSION)

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


INSTALL_HEADERS=$(patsubst %.h,%.install-header,$(HEADERS))

install-headers: $(INSTALL_HEADERS)

install-headers-dir:
	install -d $(DESTDIR)
	install -d $(DESTDIR)/usr/include
	install -d $(DESTDIR)/usr/include/$(INCLUDE_DIR)
	
%.install-header: $(COMMON_DIR)/%.h install-headers-dir
	install -m 0755 $< $(DESTDIR)/usr/include/$(INCLUDE_DIR)/$(patsubst $(COMMON_DIR)/%.h,%.h,$<)

install: lib install-headers
	install -d $(DESTDIR)
	install -d $(DESTDIR)/usr/lib
	install -m 0755  $(COMMON_DIR)/$(LIBRARY_NAME) $(DESTDIR)/usr/lib/$(LIBRARY_NAME)

.PHONY: all lib clean install install-headers
